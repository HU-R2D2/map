//
// Created by chiel on 8-5-16.
//

#ifndef MAP_RSTARMAP_HPP
#define MAP_RSTARMAP_HPP

#include "MapInterface.hpp"
#include "RTree.hpp"

namespace r2d2
{
	class BoxMap : public SaveLoadMap
	{

	public:
		virtual const BoxInfo get_box_info(const Box box) override;

		virtual const Box get_map_bounding_box() override;

		virtual void set_box_info(const Box box, const BoxInfo box_info) override;

		virtual void save(std::string filename) override;

		virtual void load(std::string filename) override;

		//! @brief  gets the size of map
		//!
		//! @return int BoxMap::map.size()
		int get_map_size();


	private:
		RTree<BoxInfo> map; // a map in which boxes and boxinfos are stored in as a pairs

	};
}

#endif //MAP_RSTARMAP_HPP
