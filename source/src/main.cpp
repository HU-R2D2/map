//
// Created by MaikelB on 4/5/2016.
//

#include "saveAndLoad.hpp"
#include <iostream>

int main(void) {
    std::string ld ("[opengl model \n"
                            "    [mode | indexed triangles]\n"
                            "\n"
                            "    [buffer vertex |\n"
                            "        [attrib position | [layout interleaved] [type float3]]\n"
                            "        [attrib uv | [layout interleaved] [type float2]]\n"
                            "\n"
                            "        [data position | [0 1 0] [-1 0 -1] [1 0 -1] [-1 0 1] [1 0 1]]\n"
                            "        [data uv | [0.5 0.0] [0 1] [1 1] [0 1] [1 1]]\n"
                            "    ]\n"
                            "\n"
                            "    [buffer element16 |\n"
                            "        [data | 0 1 2  0 2 3  0 3 4  0 4 1 ]\n"
                            "    ]\n"
                            "\n"
                            "    [texture 0 |\n"
                            "        [file | media/rock.png]\n"
                            "        [filter min | nearest]\n"
                            "        [filter mag | bilinear]\n"
                            "    ]\n"
                            "\n"
                            "    [program fragment | media/rocky.frag]\n"
                            "    [program vertex | media/rocky.vert]\n"
                            "]");

    saveAndLoad* SAL = new saveAndLoad();
    std::string position;
    position = SAL->load(ld);
    std::cout << position;
    return 0;
}
