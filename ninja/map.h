//
//  map.h
//  ghgh
//
//  Copyright © 2023 Devilish. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef map_h
#define map_h

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

sf::String TileMap[HEIGHT_MAP] = {
    "0000000000000000000000000000000000000000",
    "0              f              f        0",
    "0   s                  s               0",
    "0            h        s    ff          0",
    "0    fff             sss           h   0",
    "0        sss        f                  0",
    "0                      s               0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0000000000000000000000000000000000000000",
};

#endif /* map_h */
