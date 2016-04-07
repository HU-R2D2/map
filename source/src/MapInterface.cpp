
#include "../../../adt/source/include/Box.hpp"
#include "../include/MapInterface.hpp"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"

namespace r2d2
{
    BoxInfo::BoxInfo(bool has_obstacle, bool has_unknown, bool has_navigatable) :
        has_obstacle{ has_obstacle },
        has_unknown{ has_unknown },
        has_navigatable{ has_navigatable }
    {}

    bool BoxInfo::operator==(BoxInfo & rhs)
    {
        return has_obstacle == rhs.has_obstacle && has_unknown == rhs.has_unknown && has_navigatable == rhs.has_navigatable;
    }

    bool BoxInfo::operator!=(BoxInfo & rhs)
    {
        return !operator==(rhs);
    }

    bool BoxInfo::get_has_obstacle(){
        return BoxInfo::has_obstacle;
    }
    bool BoxInfo::get_has_unknown(){
        return BoxInfo::has_unknown;
    }
    bool BoxInfo::get_has_navigatable(){
        return BoxInfo::has_navigatable;
    }

    void SaveLoadMap::Save(std::string filename) {

    }

    void SaveLoadMap::Load(std::string filename) {
        FILE* pFILE = fopen("test.json" , "rb");
        char buff[65536];
        rapidjson::FileReadStream is(pFILE, buff, sizeof(buff));
        // 1. Parse a JSON string into DOM
        rapidjson::Document d;
        d.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

        Box::Box pBox = Box(Coordinate(d["box"]["leftCoordinate"]["x"].GetDouble() * Length::METER, d["box"]["leftCoordinate"]["y"].GetDouble() * Length::METER, d["box"]["leftCoordinate"]["z"].GetDouble() * Length::METER), Coordinate::Coordinate(d["box"]["rightCoordinate"]["x"].GetDouble() * Length::METER, d["box"]["rightCoordinate"]["y"].GetDouble() * Length::METER, d["box"]["rightCoordinate"]["z"].GetDouble() * Length::METER));
        BoxInfo::Boxinfo pBoxInfo = BoxInfo::BoxInfo(d["box"]["boxInfo"]["has_obstacle"].GetBool(),d["box"]["boxInfo"]["has_unknown"].GetBool(),d["box"]["boxInfo"]["has_navigatable"].GetBool());
        map.set_box_info(pBox, pBoxInfo);
    }

}

