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
    }
};