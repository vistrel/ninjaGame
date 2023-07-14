#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h" //map
#include "view.h" //camera
using namespace sf;

class Player {
private: float x, y = 0;
public:
    float speed = 0, w1, h1, w2, h2, dx, dy;
    int dir = 0;
    String File;
    Image image;
    Texture mTexture;
    Sprite mSprite;
    Player(String F, int X, int Y, float W1, float H1, float W2, float H2) {
        File = F;
        w1 = W1;
        h1 = H1;
        w2 = W2;
        h2 = H2;
        image.loadFromFile("/Users/vladislav/Desktop/ghgh/ghgh/image/" + File);
        mTexture.loadFromImage(image);
        mSprite.setTexture(mTexture);
        x = X; y = Y;
        mSprite.setTextureRect(IntRect(w1, h1, w2, h2));
    }
    void update(float time) {
        switch (dir) {
            case 0:
                dx = speed;
                dy = 0;
                break;
            case 1:
                dx = -speed;
                dy = 0;
                break;
            case 2:
                dx = 0;
                dy = speed;
                break;
            case 3:
                dx = 0;
                dy = -speed;
                break;
                
            default:
                break;
        }
        x += dx*time;
        y += dy*time;
        speed = 0;
        mSprite.setPosition(x, y);
        interactionWithMap();
    }
    
    float getPlayerCoordinateX() {
        return x;
    }
    float getPlayerCoordinateY() {
        return y;
    }
    
    void interactionWithMap()
        {
     
                for (int i = y / 32; i < (y + h2) / 32; i++)
                for (int j = x / 32; j<(x + w2) / 32; j++)
                {
                    if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
                    {
                        if (dy>0)//если мы шли вниз,
                        {
                            y = i * 32 - h2;
                        }
                        if (dy<0)
                        {
                            y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
                        }
                        if (dx>0)
                        {
                            x = j * 32 - w2;
                        }
                        if (dx < 0)
                        {
                            x = j * 32 + 32;//аналогично идем влево
                        }
                    }
     
                    if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
                        //какое то действие... например телепортация героя
                        TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
                    }
                }
        }
};

int main()
{
    RenderWindow window(VideoMode(1280, 720), L"Беговой Бибиджон!");
    view.reset(FloatRect(0, 0, 640, 480));
    
    // map
    Image map_image;
    map_image.loadFromFile("/Users/vladislav/Desktop/ghgh/ghgh/image/map.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite sMap;
    sMap.setTexture(map);
    
    //Image icon;
    //icon.loadFromFile("image/ninja.png");
    //mSprite.setTextureRect(IntRect(0, 540, 128, 128));
    //mSprite.setPosition(30, 30);
    //window.setIcon(32, 32, icon.getPixelsPtr());
    
    Player p1("anima.png", 200, 340, 128, 768, 128, 128);
    
    Clock clock;
    float Frame = 0;
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/60;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        // клавиатура
        if (event.type == Event::KeyPressed)
        {
            
            if(event.key.code == Keyboard::Right) {
                p1.dir = 0; p1.speed = 0.5;
                std::cout << "\nRight";
                Frame += 0.031*time;
                if(Frame > 7) {
                    Frame -= 3;
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 512, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(speed*time, 0);
            }
            if(event.key.code == Keyboard::Left) {
                p1.dir = 1; p1.speed = 0.5;
                std::cout << "\nLeft";
                Frame += 0.031*time;
                if(Frame > 7) {
                    Frame -= 3;
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 0, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(-speed*time, 0);
            }
            if(event.key.code == Keyboard::Up) {
                p1.dir = 3; p1.speed = 0.5;
                std::cout << "\nUp";
                Frame += 0.031*time;
                if(Frame > 7) {
                    Frame -= 3;
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 256, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(0, -speed*time);
            }
            if(event.key.code == Keyboard::Down) {
                p1.dir = 2; p1.speed = 0.5;
                std::cout << "\nDown";
                Frame += 0.031*time;
                if(Frame > 7) {
                    Frame -= 3;
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 768, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(0, speed*time);
            }
            if(event.key.code == Keyboard::LAlt) {
                view.zoom(1.0400f);
            }
            if(event.key.code == Keyboard::LControl) {
                view.zoom(0.9600f);
            }
        }
        p1.update(time);
        
        // мышка
        //if(Mouse::isButtonPressed(Mouse::Right)) {
          //  mSprite.setColor(Color::Red);
           // mSprite.setPosition(111, 122);
       // }
        

        window.clear(Color(128, 106, 89));
        
        // RISUNOK MAP
        for(int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') {
                    sMap.setTextureRect(IntRect(0, 0, 32, 32));
                }
                if (TileMap[i][j] == 's') {
                    sMap.setTextureRect(IntRect(32, 0, 32, 32));
                }
                if (TileMap[i][j] == '0') {
                    sMap.setTextureRect(IntRect(64, 0, 32, 32));
                }
                
                sMap.setPosition(j*32, i*32);
                
                window.draw(sMap);
            }
        }
        
        window.draw(p1.mSprite);
        window.setView(view);
        window.display();
    }

    return 0;
}
