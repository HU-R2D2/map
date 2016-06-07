//
// Created by chiel on 11/05/16.
//

#include "../include/RStarMap.hpp"
#include "../include/RTreeLeaf.hpp"

namespace r2d2 {

	RStarMap::RStarMap():
			map{} {
	}
	
	const BoxInfo RStarMap::get_box_info(const Box box) {
		bool temp_has_obstacle = false;
		bool temp_has_unknown = false;
		bool temp_has_navigatable = false;

		for (auto &known_box : map.search(box)){
			auto info = known_box->get_data();

			temp_has_obstacle = (
					temp_has_obstacle ||
					info->get_has_obstacle()
			);

			temp_has_unknown = (
					temp_has_unknown ||
					info->get_has_unknown()
			);

			temp_has_navigatable = (
					temp_has_navigatable ||
					info->get_has_navigatable()
			);

			if (temp_has_obstacle && temp_has_unknown && temp_has_navigatable) {
				break;
			}
		}

		return BoxInfo{
				temp_has_obstacle,
				temp_has_unknown,
				temp_has_navigatable
		};
	}

	const Box RStarMap::get_map_bounding_box() {
		return map.get_bounds();
	}

	void RStarMap::set_box_info(const Box box, const BoxInfo box_info) {
		std::vector<std::pair<Box, BoxInfo>> new_boxes;

		std::vector<std::shared_ptr<RTree<MIN_NODES, MAX_NODES, const BoxInfo>>> found{map.search(box)};
		for (auto leaf : found) {
			auto data = leaf->get_data();
			Box bounds = leaf->get_bounds();
			// the box has edges that are outside the new box

			// temporary objects are removed as they diminish performance

			if (bounds.get_bottom_left().get_x()
			    < box.get_bottom_left().get_x()) {
				// the current box extends the new box in the -x direction

				// insert the extension
				new_boxes.push_back(
						{Box{bounds.get_bottom_left(), Coordinate{
								box.get_bottom_left().get_x(),
								bounds.get_top_right().get_y(),
								bounds.get_top_right().get_z()
						}},
						 *data
						}
				);

				// resize the current box as to not include the newly made box
				bounds = Box{Coordinate{
						box.get_bottom_left().get_x(),
						bounds.get_bottom_left().get_y(),
						bounds.get_bottom_left().get_z()
				}, bounds.get_top_right()};
			}

			if (bounds.get_top_right().get_x()
			    > box.get_top_right().get_x()) {

				//Make new box
				new_boxes.push_back(
						{Box{Coordinate{
								box.get_top_right().get_x(),
								bounds.get_bottom_left().get_y(),
								bounds.get_bottom_left().get_z()
						}, bounds.get_top_right()},
						 *data
						});

				//Cut-away old
				bounds = Box{bounds.get_bottom_left(), Coordinate{
						box.get_top_right().get_x(),
						bounds.get_top_right().get_y(),
						bounds.get_top_right().get_z()
				}};
			}

			if (bounds.get_bottom_left().get_y()
			    < box.get_bottom_left().get_y()) {
				new_boxes.push_back(
						{Box{bounds.get_bottom_left(), Coordinate{
								bounds.get_top_right().get_x(),
								box.get_bottom_left().get_y(),
								bounds.get_top_right().get_z()
						}},
						 *data
						});

				bounds = Box{Coordinate{
						bounds.get_bottom_left().get_x(),
						box.get_bottom_left().get_y(),
						bounds.get_bottom_left().get_z()
				}, bounds.get_top_right()};
			}

			if (bounds.get_top_right().get_y()
			    > box.get_top_right().get_y()) {
				new_boxes.push_back(
						{Box{Coordinate{
								bounds.get_bottom_left().get_x(),
								box.get_top_right().get_y(),
								bounds.get_bottom_left().get_z()
						}, bounds.get_top_right()},
						 *data
						});

				bounds = Box{bounds.get_bottom_left(), Coordinate{
						bounds.get_top_right().get_x(),
						box.get_top_right().get_y(),
						bounds.get_top_right().get_z()
				}};
			}

			if (bounds.get_top_right().get_z()
			    > box.get_top_right().get_z()) {
				new_boxes.push_back(
						{Box{Coordinate{
								bounds.get_bottom_left().get_x(),
								bounds.get_bottom_left().get_y(),
								box.get_top_right().get_z()
						}, bounds.get_top_right()},
						 *data
						});

				// last box cutaway removed
			}

			if (bounds.get_bottom_left().get_z()
			    < box.get_bottom_left().get_z()) {
				new_boxes.push_back(
						{Box{bounds.get_bottom_left(), Coordinate{
								bounds.get_top_right().get_x(),
								bounds.get_top_right().get_y(),
								box.get_bottom_left().get_z()
						}},
						 *data
						});

				// last box cutaway removed
			}
			leaf->remove();
		}
		// shrink to fit decreases performance substantially
		// map.shrink_to_fit();

		//Add newly cut boxes
		for (std::pair<Box, BoxInfo> box_cut : new_boxes){
			std::shared_ptr<RTree<MIN_NODES, MAX_NODES, const BoxInfo>> newLeaf{
					new RTreeLeaf<MIN_NODES, MAX_NODES, const BoxInfo>{
							box_cut.first, std::make_shared<const BoxInfo>(box_cut.second)
					}
			};
			map.insert(newLeaf);
		}

		//Add the actual new box
		std::shared_ptr<RTree<MIN_NODES, MAX_NODES, const BoxInfo>> origLeaf{
				new RTreeLeaf<MIN_NODES, MAX_NODES, const BoxInfo>{
						box, std::make_shared<const BoxInfo>(box_info)
				}
		};
		map.insert(origLeaf);
	}

	void RStarMap::save(std::string filename) {
		// TODO
	}

	void RStarMap::load(std::string filename) {
		// TODO
	}

	int RStarMap::get_map_size() {
		return int(map.search(map.get_bounds()).size());
	}
}