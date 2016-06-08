//
// Created by chiel on 10/05/16.
//

#ifndef MAP_RTREELEAF_HPP
#define MAP_RTREELEAF_HPP

#include "RTree.hpp"

namespace r2d2 {

	template<int MIN, int MAX, typename T>
	class RTreeLeaf : public RTree<MIN, MAX, T> {
		using RTree<MIN, MAX, T>::bounds;
		using RTree<MIN, MAX, T>::parent;
	public:
		RTreeLeaf(Box bounds, std::shared_ptr<T> data) :
				RTree<MIN, MAX, T>{bounds},
				data(data) {
		}

		virtual std::vector<std::shared_ptr<r2d2::RTree<MIN, MAX, T>>> search(Box box, std::shared_ptr<r2d2::RTree<MIN, MAX, T>> this_ptr) const override {
			if (box.intersects(bounds)) {
				return std::vector<std::shared_ptr<RTree<MIN, MAX, T>>>{this_ptr};
			}
			return {};
		}

		virtual shared_ptr<r2d2::RTree<MIN, MAX, T>> find_leaf(shared_ptr<r2d2::RTree<MIN, MAX, T>> node,
		                                                       shared_ptr<r2d2::RTree<MIN, MAX, T>> this_ptr,
		                                                       int max_depth = -1) override {
			return this_ptr;
		}

		virtual void insert(std::shared_ptr<r2d2::RTree<MIN, MAX, T>> node) override {
//			std::cout << "  leaf insert" << std::endl;
			parent->insert(node); // a leaf cannot insert into itself
		}

		virtual std::shared_ptr<T> get_data() const override {
			return data;
		}

		virtual std::ostream &print(std::ostream &rhs, int level) const override {
			for (int i = 0; i < level; ++i) {
				rhs << ((i == level - 1) ? "+#" : "| ");
			}
			return rhs << this->bounds << std::endl;
		}

	private:
		std::shared_ptr<T> data;

	};

}

#endif //MAP_RTREELEAF_HPP
