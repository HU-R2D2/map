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
	class RTree : public std::enable_shared_from_this<RTree<MIN, MAX, T>> {
	public:
		RTree(r2d2::Box bounds) :
				parent{nullptr},
				bounds{bounds} {
		}

		const r2d2::Box &get_bounds() const {
			return bounds;
		}

		virtual void search(const Box &box, std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> &add_to) = 0;

		virtual std::shared_ptr<RTree<MIN, MAX, T>> find_leaf(
				const std::shared_ptr<const RTree<MIN, MAX, T>> node,
				int max_depth = -1) = 0;

		virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) = 0;

		virtual T *get_data() const = 0;

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
