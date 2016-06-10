//
// Created by chiel on 11/05/16.
//

#include "../include/RStarMap.hpp"
#include "../include/RTreeLeaf.hpp"

namespace r2d2 {

	RStarMap::RStarMap():
			map{new root_type{}} {
	}

	const Box RStarMap::get_map_bounding_box() {
		return map->get_bounds();
	}

	void RStarMap::set_box_info(const Box box, const BoxInfo box_info) {
		std::vector<std::pair<Box, BoxInfo>> new_boxes;

		r2d2::Translation axis_size{box.get_axis_size()};
		bool has_x = axis_size.get_x() > 0 * r2d2::Length::METER,
				has_y = axis_size.get_y() > 0 * r2d2::Length::METER,
				has_z = axis_size.get_z() > 0 * r2d2::Length::METER;

		std::vector<std::shared_ptr<node_type>> found{map->search(box, map)};
		for (auto leaf : found) {
			auto data = leaf->get_data();
			Box bounds = leaf->get_bounds();

			// at least two of the three axes must actually intersect for proper areas to be formed
			// use the old version of the intersection detection, as otherwise there will be boxes of length zero
			// do some wizzardry with length zero detection and stuff to circumvent some bugs
			if ((   (bounds.get_top_right().get_x() > box.get_bottom_left().get_x() &&
			         bounds.get_bottom_left().get_x() < box.get_top_right().get_x())
			        || (!has_x &&
			            (bounds.get_bottom_left().get_x() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_x() / r2d2::Length::METER))
			    )
			    &&
			    (   (bounds.get_top_right().get_y() > box.get_bottom_left().get_y() &&
			         bounds.get_bottom_left().get_y() < box.get_top_right().get_y())
			        || (!has_y &&
			            (bounds.get_bottom_left().get_y() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_y() / r2d2::Length::METER))
			    )
			    &&
			    (   (bounds.get_top_right().get_z() > box.get_bottom_left().get_z() &&
			         bounds.get_bottom_left().get_z() < box.get_top_right().get_z())
			        || (!has_z &&
			            (bounds.get_bottom_left().get_z() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_z() / r2d2::Length::METER)))) {

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
		}
		// shrink to fit decreases performance substantially
		// map.shrink_to_fit();

		//Add newly cut boxes
		for (std::pair<Box, BoxInfo> box_cut : new_boxes) {
			add_box(box_cut.first, box_cut.second);
		}

		// if the new box has neither navigable or an obstacle
		// then don't bother adding it as the empty area is unknown by default
		if (box_info.get_has_navigatable() || box_info.get_has_obstacle()) {
			//Add the actual new box
			add_box(box, box_info);
		}
	}

	int RStarMap::get_map_size() const {
		return int(map->search(map->get_bounds(), map).size());
	}

	std::vector<std::pair<Box, BoxInfo>> RStarMap::get_intersecting(const Box &bounds) const {
		std::vector<std::shared_ptr<r2d2::node_type>> found{map->search(bounds, map)};
		std::vector<std::pair<Box, BoxInfo>> pair_vector{};
		for (auto item : found) {
			pair_vector.push_back({item->get_bounds(), *item->get_data()});
		}
		return pair_vector;
	}

	void RStarMap::add_box(Box box, BoxInfo info) {
		std::shared_ptr<node_type> newLeaf{
				new RTreeLeaf<MIN_NODES, MAX_NODES, const BoxInfo>{
						box, std::make_shared<const BoxInfo>(info)
				}
		};
		insert(newLeaf);
	}

	void RStarMap::insert(std::shared_ptr<node_type> node) {
		// first find the correct leaf to insert to, then insert it into that node
		map->find_leaf(node, map)->insert(node);
//		std::cout << "insert" << std::endl << *node << *map;
	}

}