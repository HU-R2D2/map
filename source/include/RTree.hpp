//
// Created by chiel on 9-5-16.
//

#ifndef MAP_RTREE_HPP
#define MAP_RTREE_HPP

#include <Box.hpp>
#include <vector>
#include <memory>

template<typename T>
class RTree<T> {
public:
	const r2d2::Box &get_bounds() = 0;

	const std::vector<RTree<T>> search(r2d2::Box box) = 0;

	virtual void insert(RTree<T> node) = 0;

	virtual const std::shared_ptr<T> get_data() = 0;

};

#endif //MAP_RTREE_HPP
