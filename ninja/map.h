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
    "0                                      0",
    "0                                      0",
    "0              s    00                 0",
    "0           0000000000                 0",
    "0           0000000000                 0",
    "0   f    h  0000000000                 0",
    "0000000000000000000000000000000000000000",
};
int countStone = 6; //количество камней

void randomMapGenerate(int countStone){//рандомно расставляем камни
 
    int randomElementX = 0;//случайный элемент по горизонтали
    int randomElementY = 0;//случ эл-т по вертикали
    long srand(time(0));//рандом
    
    while (countStone>0){
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширина карты-1, чтобы не получать числа бордюра карты
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по игреку так же
        
        if (TileMap[randomElementY][randomElementX] == ' ')   {//если встретили символ пробел,
            TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
            //std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
            countStone--;
        }
    }
}

#endif /* map_h */
