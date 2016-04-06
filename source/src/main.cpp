#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"

using namespace rapidjson;

int main() {
    FILE* pFILE = fopen("test.json" , "rb");
    char buff[65536];
    FileReadStream is(pFILE, buff, sizeof(buff));
    // 1. Parse a JSON string into DOM
    Document d;
    d.ParseStream<0, UTF8<>, FileReadStream>(is);

    return 0;
}