//
// Created by chiel on 10/05/16.
//

#ifndef MAP_RTREEBRANCH_HPP
#define MAP_RTREEBRANCH_HPP

template<typename T>
class RTreeBranch<T> {
public:
	const r2d2::Box &get_bounds() = 0;

	const std::vector<RTree<T>> search(r2d2::Box box) = 0;

	virtual void insert(RTree<T> node) = 0;

};

#endif //MAP_RTREEBRANCH_HPP
