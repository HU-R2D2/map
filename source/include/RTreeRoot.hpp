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
		using RTreeBranch<MIN, MAX, T>::children;
		using RTreeBranch<MIN, MAX, T>::num_children;

	public:
		RTreeRoot():
				RTreeRoot::RTreeBranch{{}} {
		}

		virtual vector<shared_ptr<RTree<MIN, MAX, T>>> search(r2d2::Box box,
				shared_ptr<RTree<MIN, MAX, T>> this_ptr = {nullptr}) const override {
			return RTreeBranch<MIN, MAX, T>::search(box, this_ptr);
		}

		// O(log(N))
		virtual std::shared_ptr<RTree<MIN, MAX, T>> insert(std::shared_ptr<RTree<MIN, MAX, T>> &node, int max_depth = -1) override {
			if (num_children == 0) {
				children[num_children++] = node;
				node->set_parent(this);
				bounds = node->get_bounds();
			} else {
				auto overflow = RTreeBranch<MIN, MAX, T>::insert(node, max_depth);
				if (overflow != nullptr) {
					children[0] = std::shared_ptr<RTree<MIN, MAX, T>>{
							new RTreeBranch<MIN, MAX, T>{
									std::vector<std::shared_ptr<RTree<MIN, MAX, T>>>{
											children.begin(),
											children.begin() + num_children
									} // TODO something with pointers
							}
					};
					children[0]->set_parent(this);
					children[1] = overflow;
					children[1]->set_parent(this);
					num_children = 2;
				} else {
					bounds = bounds.get_union_box(node->get_bounds());
				}
			}
			return {};
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
			/*if (num_children == 1 && ) { // here should be something with type detection and stuff
				// inherit all the child's children as the node is superfluous
				auto child = children[0];
				children = child->children;
				num_children = child->num_children;
				for (int i = 0; i < num_children; ++i) {
					children[i]->set_parent(this);
				}
			}*/
			// otherwise do nothing, as the root node is allowed to underflow
		}

		template<long unsigned int S>
		RTreeRoot(std::array<std::shared_ptr<RTree<MIN, MAX, T>>, S> children) :
				RTreeRoot::RTreeBranch(children) {
		}

	};

}

#endif //MAP_RTREEROOT_HPP
