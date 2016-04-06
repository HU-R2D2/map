#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"

class saveAndLoad {
public:
    void openFile() {
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
};