//
// Created by chiel on 8-5-16.
//

#ifndef MAP_RSTARMAP_HPP
#define MAP_RSTARMAP_HPP

#include "MapInterface.hpp"
#include "RTreeRoot.hpp"

#define MIN_NODES 3
#define MAX_NODES 6

namespace r2d2
{
	class RStarMap : public SaveLoadMap
	{

	public:
		RStarMap();

		virtual const BoxInfo get_box_info(const Box box) override;

		virtual const Box get_map_bounding_box() override;

		virtual void set_box_info(const Box box, const BoxInfo box_info) override;

		virtual void save(std::string filename) override;

		virtual void load(std::string filename) override;

		//! @brief  gets the size of map
		//!
		//! @return int BoxMap::map.size()
		int get_map_size();

		friend std::ostream &operator<<(std::ostream &lhs, const RStarMap &rhs) {
			return lhs << rhs.map;
		}

	private:
		RTreeRoot<MIN_NODES, MAX_NODES, const BoxInfo> map;

	};
}

#endif //MAP_RSTARMAP_HPP
