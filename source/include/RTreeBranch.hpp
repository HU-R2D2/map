//
// Created by chiel on 10/05/16.
//

#ifndef MAP_RTREEBRANCH_HPP
#define MAP_RTREEBRANCH_HPP

#include <limits>
#include <algorithm>
#include "RTree.hpp"

namespace r2d2 {

	template<int MIN, int MAX, typename T>
	class RTreeBranch : public RTree<MIN, MAX, T> {
	protected:
		using RTree<MIN, MAX, T>::set_parent;
		using RTree<MIN, MAX, T>::remove;
		using RTree<MIN, MAX, T>::parent;
		using RTree<MIN, MAX, T>::bounds;

	public:
		RTreeBranch(std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> children) :
				RTreeBranch::RTree({}) {
			static_assert(MIN >= 2, "MIN must be >= 2");
			static_assert(MAX >= MIN * 2, "MAX must be >= MIN*2");
			assert(children.size() <= MAX);
			for (unsigned int i = 0; i < children.size(); ++i) {
				this->children[i] = children[i];
				children[i]->set_parent(this);
				bounds = i == 0 ? children[0]->get_bounds() : bounds.get_union_box(children[i]->get_bounds());
			}
			num_children = int(children.size());
		}

		template<long unsigned int S>
		RTreeBranch(std::array<std::shared_ptr<RTree<MIN, MAX, T>>, S> children) :
				RTreeBranch::RTree({}) {
			static_assert(MIN >= 2, "MIN must be >= 2");
			static_assert(MAX >= MIN * 2, "MAX must be >= MIN*2");
			static_assert(0 < S <= MAX, "children cannot have more elements than MAX");
			bounds = children[0]->get_bounds();
			for (int i = 0; i < S; ++i) {
				this->children[i] = children[i];
				children[i]->set_parent(this);
				bounds = bounds.get_union_box(children[i]->get_bounds());
			}
			num_children = int(S);
		}

		virtual std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> search(r2d2::Box box, std::shared_ptr<RTree<MIN, MAX, T>> this_ptr) const override {
			std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> found;
			if (bounds.intersects(box)) {
				for (int i = 0; i < num_children; ++i) {
					std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> childFound{children[i]->search(box, children[i])};
					found.insert(found.cend(), childFound.cbegin(), childFound.cend());
				}
			}
			return found;
		}

		// search for the leaf that is best suited for insertion of 'node'
		virtual std::shared_ptr<RTree<MIN, MAX, T>> find_leaf(std::shared_ptr<RTree<MIN, MAX, T>> node,
		                                                      std::shared_ptr<RTree<MIN, MAX, T>> this_ptr,
		                                                      int max_depth = -1) override {
			// search for the best match to insert
			if (max_depth == 0) {
				return this_ptr;
			} else {
				unsigned long bestIndex = 0;
				std::tuple<r2d2::Box, double> bestBox{
						get_union_score(children[0]->get_bounds(), node->get_bounds())
				};
				for (int i = 1; i < num_children; ++i) {
					// find the node with the least area plus enlargement
					std::tuple<r2d2::Box, double> newBox = get_union_score(
							children[i]->get_bounds(), node->get_bounds());
					if (std::get<1>(newBox) < std::get<1>(bestBox)) {
						// this box takes less area and enlargement, therefore it is more efficient
						bestIndex = i;
						bestBox = newBox;
					}
				}
				return children[bestIndex]->find_leaf(node, children[bestIndex], max_depth - 1);
			}
		};

		// worst case O(log(n) * (MAX + MAX^2))
		// amortized O(log(n))
		// best case O(log(n))
		virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) override {
			children[num_children++] = node;
			node->set_parent(this);
			if (num_children >= MAX) {
				// split the node into equal parts
				auto splitNode = split();

				// propagate split recursively upward
				parent->insert(splitNode);
			} else {
				// spit stopped propagating
				// add bounds as the inner children could not have been removed
				add_bound(node->get_bounds());
			}

			// TODO if this is not the root level and this is the first call of OverflowTreatment
			// in the given level during the insertion of one given rectangle then invoke ReInsert
			// - the r* paper
			// reinsertion might be somewhat good for the performance, but adds too much complexity to the current program
		}

		virtual std::shared_ptr<T> get_data() const override {
			return nullptr; // branches do not store any data; call the default constructor just so we have some
		}

		// O(log(n)*MAX) worst case
		virtual void remove(const RTree<MIN, MAX, T> *node, const std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> raise_nodes = {}) {
			std::shared_ptr<RTree<MIN, MAX, T>> found{};
			for (auto child : children) {
				if (child.get() == node) {
					found = child;
				}
			}
			if (found != nullptr) {
				(*std::remove(children.begin(), children.begin() + num_children, found)) = nullptr;
				--num_children;

				// do the node insertion after the node removal to prevent splits,
				// but do it before node underflow te prevent removal
				for (auto raise_node : raise_nodes) {
					// try inserting the raised node into this node
					auto leaf = find_leaf(raise_node, nullptr, 1);
					// just use a nullptr here as find_leaf can always recurse from a branch
					leaf->insert(raise_node);
				}

				if (num_children < MIN) {
					// underfull node, add all the children to the parent
					underflow_treatment();
				} else {
					remove_bound(node->get_bounds());
				}
			} else {
				// the node was not found, insert the raised ones anyways
				for (auto raise_node : raise_nodes) {
					insert(raise_node);
				}
			}
		}

		virtual std::ostream &print(std::ostream &rhs, int level) const override {
			RTree<MIN, MAX, T>::print(rhs, level) << std::endl;
			for (int i = 0; i < num_children; ++i) {
				children[i]->print(rhs, level + 1);
			}
			return rhs;
		}

	private:
		// O(1)
		static const r2d2::Length get_axis(r2d2::Coordinate coord, int axis) {
			switch (axis) {
				case 0:
					return coord.get_x();
				case 1:
					return coord.get_y();
				case 2:
					return coord.get_z();
				default:
					// what no stop
					return {};
			}
		}

		// O(n log(n))
		void sort(int axis) {
			std::sort(children.begin(), children.begin() + num_children,
			          [axis](const std::shared_ptr<RTree<MIN, MAX, T>> &e1,
			                 const std::shared_ptr<RTree<MIN, MAX, T>> &e2) {
				          const r2d2::Box &e1Bounds{e1->get_bounds()}, &e2Bounds{e2->get_bounds()};
				          const r2d2::Length &minAxis1 = get_axis(e1Bounds.get_bottom_left(), axis),
						          &minAxis2 = get_axis(e2Bounds.get_bottom_left(), axis);
				          if (minAxis1 < minAxis2) {
					          return -1;
				          } else if (minAxis2 < minAxis1) {
					          return 1;
				          } else {
					          const r2d2::Length &maxAxis1 = get_axis(e1Bounds.get_top_right(), axis),
							          &maxAxis2 = get_axis(e2Bounds.get_top_right(), axis);
					          if (maxAxis1 < maxAxis2) {
						          return -1;
					          } else if (maxAxis2 < maxAxis1) {
						          return 1;
					          } else {
						          return 0;
					          }
				          }
			          });
		}

		virtual void underflow_treatment() {
			// double removal/insertion to avoid object delete problems
			parent->remove(this, {children.begin(), children.begin() + num_children});
		}

		// O(1)
		static std::tuple<r2d2::Box, double> get_union_score(const r2d2::Box &b1, const r2d2::Box &b2) {
			r2d2::Box union_box{b1.get_union_box(b2)};
			return std::tuple<r2d2::Box, double>{
					union_box,
					(get_box_margin(union_box) * 2.0 - get_box_margin(b2)) / Length::METER
					// box margin is used here instead of the usual box size as it is not feasable in 3d
			};
		}

		// O(1)
		static r2d2::Length get_box_margin(const r2d2::Box &box) {
			const r2d2::Translation &axis_size{box.get_axis_size()};
			return axis_size.get_x() + axis_size.get_y() + axis_size.get_z();
		}

		// O(1)
		static double get_box_size(const r2d2::Box &box) {
			r2d2::Translation &axis_size{box.get_axis_size()};
			return axis_size.get_x() / Length::METER
			       * axis_size.get_y() / Length::METER
			       * axis_size.get_z() / Length::METER;
		}

	protected:
		#define NUM_AXES 3
		// O((n log(n) + (n^2)) * NUM_AXES+1) = O(n^2)
		// best case O(n^2)
		std::shared_ptr<RTree<MIN, MAX, T>> split() {
			// find the most efficient split between the nodes currently in the set
			int bestAxis = 0, bestSpilt = 0;
			r2d2::Box bestSplitBounds{};
			double bestSplitScore = std::numeric_limits<double>::infinity();
			for (int axis = 0; axis < NUM_AXES; ++axis) {
				// do a sort as it the most efficient way to find minimal overlap
				sort(axis);
				r2d2::Box frontMargin{};
				// update the front margin to reflect the minimum amount of children
				for (int i = 0; i < MIN; ++i) {
					frontMargin = frontMargin.get_union_box(children[i]->get_bounds());
				}
				for (int i = MIN - 1; i < num_children - MIN; ++i) {
					// the back margin has to be recomputed every box, as it cannot be dynamically updated.
					// this takes the complexity of this function up to O(n^2), but it is worth it for small numbers of MAX
					r2d2::Box backMargin{};
					for (int j = i + 1; j < num_children; ++j) {
						backMargin = backMargin.get_union_box(children[j]->get_bounds());
					}

					double firstScore = get_box_margin(frontMargin) / Length::METER,
							secondScore = get_box_margin(backMargin) / Length::METER,
							overlap =
							get_axis(children[i + 1]->get_bounds().get_bottom_left(), axis) / Length::METER
							- get_axis(children[i]->get_bounds().get_top_right(), axis) / Length::METER,
					// overlap also includes seperation from the two boxes
							totalScore = firstScore * firstScore + secondScore * secondScore + overlap;
					// taking the square of the score does two things:
					// 1. it tries to minimise the difference between the box sizes
					// 2. it tries to maximise the squareness of the boxes
					if (totalScore < bestSplitScore) {
						bestSplitScore = totalScore;
						bestSpilt = i;
						bestAxis = axis;
						bestSplitBounds = frontMargin;
					}
				}
			}

			// the best spilt has been found, now extract if from the array
			if (bestAxis != NUM_AXES - 1) {
				// the last axis sorted can be skipped
				sort(bestAxis);
			}
			std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> newTree;
			for (int i = bestSpilt + 1; i < num_children; ++i) {
				newTree.push_back(children[i]);
				children[i] = nullptr;
			}
			num_children = bestSpilt + 1;
			// update the bounds of the parent(s)
			recompute_bounds();
			// bounds = bestSplitBounds;
			// we already know the bounds here, but it's not very elegant to use them

			return std::shared_ptr<RTree<MIN, MAX, T>>{new RTreeBranch<MIN, MAX, T>{newTree}};
		}

		// checks wether a certain box has one of it's edges on the edge of this rtree node
		// this can be used for things like checking whether to propagate bounds growing/schrinking
		bool has_edge(const Box box) {
			return !bounds.contains(box);
		}

		virtual void add_bound(const Box box) {
			if (has_edge(box)) {
				bounds = bounds.get_union_box(box);

				parent->add_bound(box);
			}
		}

		virtual void remove_bound(const Box box) {
			// if the removed node made up part of this node's edges
			if (has_edge(box)) {
				// shrink the bounds to fit the leftover nodes
				bounds = children[0]->get_bounds();
				for (int i = 1; i < num_children; ++i) {
					bounds = bounds.get_union_box(children[i]->get_bounds());
				}

				parent->remove_bound(box);
			}
		}

		virtual void recompute_bounds() {
			bounds = children[0]->get_bounds();
			for (int i = 1; i < num_children; ++i) {
				bounds = bounds.get_union_box(children[i]->get_bounds());
			}

			parent->recompute_bounds();
		}

		std::array<std::shared_ptr<RTree<MIN, MAX, T>>, MAX> children;
		int num_children;

	};

}

#endif //MAP_RTREEBRANCH_HPP
