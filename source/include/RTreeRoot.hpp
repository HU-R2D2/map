//
// Created by chiel on 18/05/16.
//

#ifndef MAP_RTREEROOT_HPP
#define MAP_RTREEROOT_HPP

#include "RTreeBranch.hpp"

namespace r2d2 {

	template<int MIN, int MAX, typename T>
	class RTreeRoot : public RTreeBranch<MIN, MAX, T> {
		using RTree<MIN, MAX, T>::bounds;
		using RTreeBranch<MIN, MAX, T>::has_edge;
		using RTreeBranch<MIN, MAX, T>::split;
		using RTreeBranch<MIN, MAX, T>::children;
		using RTreeBranch<MIN, MAX, T>::num_children;

	public:
		RTreeRoot():
				RTreeRoot::RTreeBranch{{}} {
		}

		virtual shared_ptr<RTree<MIN, MAX, T>> find_leaf(shared_ptr<RTree<MIN, MAX, T>> node,
		                                                 shared_ptr<RTree<MIN, MAX, T>> this_ptr,
		                                                 int max_depth = -1) override {
			if (num_children > 0 || max_depth == 0) {
				return RTreeBranch<MIN, MAX, T>::find_leaf(node, this_ptr, max_depth);
			} else {
				return this_ptr;
			}
		}

		// O(log(N))
		virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) override {
			children[num_children++] = node;
			node->set_parent(this);
			if (num_children <= 1) {
				bounds = node->get_bounds();
			} else {
				if (num_children >= MAX) {
					auto splitNode = split();
					// make the root two new nodes
					children[0] = std::shared_ptr<RTree<MIN, MAX, T>>{
							new RTreeBranch<MIN, MAX, T>{
									std::vector<std::shared_ptr<RTree<MIN, MAX, T>>>{
											children.begin(),
											children.begin() + num_children
									}
							}
					};
					children[0]->set_parent(this);
					children[1] = splitNode;
					children[1]->set_parent(this);
					num_children = 2;
					bounds = children[0]->get_bounds().get_union_box(children[1]->get_bounds());
				} else {
					// add bounds as the inner children could not have been removed
					bounds = bounds.get_union_box(node->get_bounds());
				}
			}
		}

//		virtual void remove(const RTree<MIN, MAX, T> *node, std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> raise_nodes = {}) override {
//			std::cout << "test" << std::endl;
//			RTreeBranch<MIN, MAX, T>::remove(node, raise_nodes);
//		}

		virtual void remove() override {
			// clear the entire root, as this root is supposed to be removed
			children = {};
			num_children = 0;
		}

	private:
		virtual void underflow_treatment() override {
			if (num_children == 0) {
				bounds = {};

				// the case of num_children == 1 doesn't have to be handled, as
				// the one child will always comply to the min/max rules of the rtree
			}
			// otherwise do nothing, as the root node is allowed to underflow
		}

		template<long unsigned int S>
		RTreeRoot(std::array<std::shared_ptr<RTree<MIN, MAX, T>>, S> children) :
				RTreeRoot::RTreeBranch(children) {
		}

	protected:
		virtual void add_bound(const Box box) {
			if (has_edge(box)) {
				bounds = bounds.get_union_box(box);
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
			}
		}

		virtual void recompute_bounds() {
			bounds = children[0]->get_bounds();
			for (int i = 1; i < num_children; ++i) {
				bounds = bounds.get_union_box(children[i]->get_bounds());
			}
		}

	};

}

#endif //MAP_RTREEROOT_HPP
