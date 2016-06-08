//
// Created by chiel on 9-5-16.
//

#ifndef MAP_RTREE_HPP
#define MAP_RTREE_HPP

#include <vector>
#include <memory>
#include <Box.hpp>

namespace r2d2 {

	template<int MIN, int MAX, typename T>
	class RTreeBranch;

	template<int MIN, int MAX, typename T>
	class RTree {
	public:
		RTree(r2d2::Box bounds) :
				bounds{bounds},
				parent{nullptr} {
//			std::cout << "rtree create" << this << std::endl;
		}

		virtual ~RTree() {
//			std::cout << "rtree destruct " << this << std::endl;
		}

		const r2d2::Box &get_bounds() const {
			return bounds;
		}

		virtual std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> search(
				r2d2::Box box, std::shared_ptr<RTree<MIN, MAX, T>> this_ptr) const = 0;

		virtual std::shared_ptr<RTree<MIN, MAX, T>> find_leaf(std::shared_ptr<RTree<MIN, MAX, T>> node,
		                                                      std::shared_ptr<RTree<MIN, MAX, T>> this_ptr,
		                                                      int max_depth = -1) = 0;

		virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) = 0;

		virtual std::shared_ptr<T> get_data() const = 0;

		// be sure that when you call this function the node is not
		// disconnected from any tree, otherwise you will segfault
		// also note that you should not do anything within this node after
		// calling this function without holding the shared_ptr to it,
		// which at this point is impossible
		virtual void remove() {
			parent->remove(this);
		}

		// this method has to be public to allow for correct template
		// resolution, please don't use it outside of this class
		void set_parent(RTreeBranch<MIN, MAX, T> *parent) {
			this->parent = parent;
		}

		virtual std::ostream &print(std::ostream &rhs, int level) const {
			for (int i = 0; i < level; ++i) {
				rhs << ((i == level - 1) ? "+-" : "| ");
			}
			return rhs << bounds;
		}

		friend std::ostream &operator<<(std::ostream &rhs, const RTree<MIN, MAX, T> &lhs) {
			return lhs.print(rhs, 0);
		}

	protected:
		// A normal pointer is used here as the parent should not be kept alive by this child,
		// and we are unable to use weak_ptr because the inability to get a weak pointer to the set parent caller.
		// In theory this pointer should never have an deleted parent, otherwise the leaf would no longer
		// be in a existing tree, and any functions that call the parent would not be called in such a case
		RTreeBranch<MIN, MAX, T>* parent;
		r2d2::Box bounds;

	};

}

#endif //MAP_RTREE_HPP
