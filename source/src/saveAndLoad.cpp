//
// Created by MaikelB on 4/5/2016.
//

#include "saveAndLoad.hpp"
#include "../include/tml.hpp"
#include <iostream>

saveAndLoad::saveAndLoad() { }

std::string saveAndLoad::load(std::string name) {
    TmlDoc *doc = TmlDoc::parseString(name);
    TmlNode root = doc->getRoot();

    TmlNode positionNode = root;

    return positionNode[1].toString();
}
