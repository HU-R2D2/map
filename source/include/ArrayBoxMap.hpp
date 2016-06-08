//
// Created by chiel on 07/06/16.
//

#ifndef MAP_ARRAYBOXMAP_HPP
#define MAP_ARRAYBOXMAP_HPP

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/filewritestream.h"
#include "BoxMap.hpp"
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>

namespace r2d2 {

	class ArrayBoxMap : public BoxMap {
	public:

		virtual const BoxInfo get_box_info(const Box box) override;

		virtual const Box get_map_bounding_box() override;

		virtual void set_box_info(const Box box, const BoxInfo box_info) override;

		virtual void save(std::string filename) override;

		virtual void load(std::string filename) override;

		//! @brief  gets the size of map
		//!
		//! @return int BoxMap::map.size()
		virtual int get_map_size() const override;

		virtual std::vector<std::pair<Box, BoxInfo>> get_intersecting(const Box &bounds) const override;

		virtual std::ostream &print(std::ostream &rhs) {
			return rhs;
		}

	private:
		std::vector<std::pair<Box, BoxInfo>> map; // a map in which boxes and boxinfos are stored in as a pairs

	};

}

#endif //MAP_ARRAYBOXMAP_HPP
