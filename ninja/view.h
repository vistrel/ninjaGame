//
//  view.h
//  ghgh
//
//  Copyright © 2023 Devilish. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#ifndef view_h
#define view_h

sf::View view;

View getPlayerCoordinateForView(float x, float y) {
    view.setCenter(x+100, y);
    return view;
}

#endif /* view_h */
