//
// Created by chiel on 8-5-16.
//

#ifndef MAP_RSTARMAP_HPP
#define MAP_RSTARMAP_HPP

#include "MapInterface.hpp"
#include "RTreeRoot.hpp"
#include "BoxMap.hpp"

namespace r2d2 {
	#define MIN_NODES 2
	#define MAX_NODES 4
	typedef RTreeRoot<MIN_NODES, MAX_NODES, const BoxInfo> root_type;
	typedef RTree<MIN_NODES, MAX_NODES, const BoxInfo> node_type;

	class RStarMap : public BoxMap {

	public:
		RStarMap();

		virtual const BoxInfo get_box_info(const Box box) override;

		virtual const Box get_map_bounding_box() override;

		virtual void set_box_info(const Box box, const BoxInfo box_info) override;

		virtual void save(std::string filename) override;

		virtual void load(std::string filename) override;

		virtual int get_map_size() const override;

		virtual std::vector<std::pair<Box, BoxInfo>> get_intersecting(const Box &bounds) const override;

		virtual std::ostream &print(std::ostream &lhs) {
			return lhs << *map;
		}

	private:
		void insert(std::shared_ptr<node_type> node);

		std::shared_ptr<root_type> map;

	};
}

#endif //MAP_RSTARMAP_HPP
