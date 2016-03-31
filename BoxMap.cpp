#include "BoxMap.hpp"

namespace r2d2{
	namespace map{

		BoxInfo & BoxMap::get_box_info(const adt::Box & box){
			return BoxInfo();
		}

		adt::Box & BoxMap::get_map_bounding_box(){
          return adt::Box();
		}

		std::shared_ptr<BoxInfo**> BoxMap::get_map_area_2d(const adt::Box & area, const adt::Box & pixel_size){
          std::shared_ptr <BoxInfo**> bs;
          return bs;
		}

		void BoxMap::set_box_info(const adt::Box & box, const BoxInfo & box_info){

		}

		void BoxMap::save(const std::string filename){

		}

		void BoxMap::load(const std::string filename){

		}
	}
}