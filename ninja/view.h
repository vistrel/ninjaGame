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
    
    float tempX = x; float tempY = y;
    if(x < 320) {
        tempX = 320;
    }
    if(x > 960) {
        tempX = 960;
    }
    if(y < 240) {
        tempY = 240;
    }
    if(y > 554) {
        tempY = 554;
    }
    view.setCenter(tempX, tempY);
    
    return view;
}

#endif /* view_h */
