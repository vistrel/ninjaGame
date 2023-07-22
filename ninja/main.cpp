#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h" //map
#include "view.h" //camera
#include "mission.h" //mission
using namespace sf;
using namespace std;

class Player {
public:
float x, y = 0;
float speed = 0, w1, h1, w2, h2, dx, dy;
int countStone;
int dir; int playerScore; int health;
    bool life, isMove, onGround;
String File;
Image image;
Texture mTexture;
Sprite mSprite;
Player(String F, int X, int Y, float W1, float H1, float W2, float H2) {
    // Конструктор класса Player, инициализирует переменные и загружает текстуры
    dir = 0; playerScore = 0; health = 100;
    life = true; isMove = false; onGround = false;
    File = F;
    w1 = W1;
    h1 = H1;
    w2 = W2;
    h2 = H2;
    image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/" + File);
    mTexture.loadFromImage(image);
    mSprite.setTexture(mTexture);
    x = X; y = Y;
    mSprite.setTextureRect(IntRect(w1, h1, w2, h2));
}
void update(float time) {
    // Метод обновления положения игрока
    switch (dir) {
        case 0:
            dx = speed; dy = 0; break;
        case 1:
            dx = -speed; dy = 0; break;
        case 2:
            dx = 0; break;
        case 3:
            break;
        default:
            break;
    }
    x += dx*time;
    checkCollisionWithMap(dx, 0);
    y += dy*time;
    checkCollisionWithMap(0, dy);
    if (dy == 0) { // Приземлился
            onGround = true;
        }
    mSprite.setPosition(x+w2/2, y+h2/2);
    if(health <= 0) { life = false; }
    if(!isMove) { speed = 0; }
    if(isMove) { speed = 0.1; }
    if(!onGround) { dy = dy + 0.0002*time; }
    if(life) { getPlayerCoordinateForView(x, y); }
}

float getPlayerCoordinateX() {
    return x;
}
float getPlayerCoordinateY() {
    return y;
}

    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
        {
            for (int i = y / 32; i < (y + h2) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w2) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
                {
                    if (Dy>0){ y = i * 32 - h2;  dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
                    if (Dy<0){y = i * 32 + 32;  dy = 0;}//столкновение с верхними краями карты(может и не пригодиться)
                    if (Dx>0){x = j * 32 - w2;}//с правым краем карты
                    if (Dx<0){x = j * 32 + 32;}// с левым краем карты
                } else {onGround=false;}
            }
        }
};

int main()
{
RenderWindow window(VideoMode(1280, 720), L"Беговой Бибиджон!");
view.reset(FloatRect(0, 0, 640, 480));

randomMapGenerate(countStone);

// text1
Font font;
font.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/Zhizn.otf");
Text text("", font, 20);
text.setFillColor(Color::White);
text.setOutlineColor(Color::Black);
text.setOutlineThickness(2);
text.setStyle(Text::Bold);

// text2
Text textHP("", font, 20);
textHP.setFillColor(Color::White);
textHP.setOutlineColor(Color::Black);
textHP.setOutlineThickness(2);
textHP.setStyle(Text::Bold);

// text textPoints
Text textPoints("", font, 20);
textPoints.setFillColor(Color::White);
textPoints.setOutlineColor(Color::Black);
textPoints.setOutlineThickness(2);
textPoints.setStyle(Text::Bold);

// map
Image map_image;
map_image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/map.png");
Texture map;
map.loadFromImage(map_image);
Sprite sMap;
sMap.setTexture(map);

// health
Image health_image;
health_image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/health.png");
Texture hp;
hp.loadFromImage(health_image);
Sprite sHp;
sHp.setTexture(hp);

// stone
Image stone_image;
stone_image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/stone.png");
Texture tStone;
tStone.loadFromImage(stone_image);
Sprite sStone;
sStone.setTexture(tStone);

// mission
Image mission_image;
mission_image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/missionbg.jpg");
mission_image.createMaskFromColor(Color(0, 0, 0));
Texture tMission;
tMission.loadFromImage(mission_image);
Sprite sMission;
sMission.setTexture(tMission);
sMission.setTextureRect(IntRect(0, 0, 340, 510));
sMission.setScale(0.6f, 0.6f);

//Image icon;
//icon.loadFromFile("image/ninja.png");
//mSprite.setTextureRect(IntRect(0, 540, 128, 128));
//mSprite.setPosition(30, 30);
//window.setIcon(32, 32, icon.getPixelsPtr());

Player p1("anima.png", 200, 340, 128, 768, 128, 128);

bool showMissionText = true;

Clock clock;
float Frame = 0;

bool isMove = false;
float dX = 0; float dY = 0;

while (window.isOpen())
{
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/100;
    
    if(!p1.life) { view.zoom(0.99992); }
    
    Vector2i pixelPos = Mouse::getPosition(window);
    Vector2f pos = window.mapPixelToCoords(pixelPos);
    cout << pixelPos.x << "\n";
    cout << pos.x << "\n";
    
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
        
        // мышка
        if (event.type == Event::MouseButtonPressed) {
            if(Mouse::isButtonPressed(Mouse::Left)) {
                if (p1.mSprite.getGlobalBounds().contains(pos.x, pos.y)) {
                    cout << "\nclick";
                    dX = pos.x - p1.mSprite.getPosition().x;
                    dY = pos.y - p1.mSprite.getPosition().y;
                    isMove = true;
                }
            }
        }
        if (event.type == Event::MouseButtonReleased) { //если отпустили клавишу
            if (event.key.code == Mouse::Left) //а именно левую
                isMove = false; //то не можем двигать спрайт
            p1.mSprite.setColor(Color::White);//и даем ему прежний цвет
        }
        if (isMove) {
            p1.mSprite.setColor(Color::Red);
            p1.x = pos.x - dX;
            p1.y = pos.y - dY;
        }
        
        
        // клавиатура
        if (event.type == Event::KeyPressed && p1.life == true) {
            if (event.key.code == Keyboard::Right) {
                    p1.dir = 0;
                p1.isMove = true;
                    p1.speed = 1;
                    Frame += 0.031 * time;
                    if (Frame > 7) {
                        std::cout << "\nRight";
                        Frame -= 3;
                    }
                    p1.mSprite.setTextureRect(IntRect(128 * int(Frame), 512, 128, 128));
                    //getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY()); // Не уверен, зачем это здесь
                    //mSprite.move(speed * time, 0); // Не уверен, зачем это здесь
                }
            if(event.key.code == Keyboard::Tab) {
                switch (showMissionText) {
                    case true: {
                        ostringstream task;
                        task << getTextMission(getCurrentMission(p1.getPlayerCoordinateX()));
                        text.setString(task.str());
                        text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
                        sMission.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
                        showMissionText = false;
                        break;
                    }
                    case false: {
                        text.setString("");
                        showMissionText = true;
                        break;
                    }
                }
                
            }
            
            if(event.key.code == Keyboard::Left) {
                p1.dir = 1; p1.speed = 1;
                Frame += 0.031*time;
                if(Frame > 7) {
                    Frame -= 3;
                    std::cout << "\nLeft";
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 0, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(-speed*time, 0);
            }
            if((event.key.code == Keyboard::Up) && (p1.onGround)) {
                cout << "up";
                p1.dir = 3; p1.speed = 1; p1.dy = -0.3; p1.onGround = false;
                /*Frame += 0.031*time;
                if(Frame > 7) {
                    std::cout << "\nUp";
                    Frame -= 3;
                }*/
                //p1.mSprite.setTextureRect(IntRect(128*int(Frame), 256, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(0, -speed*time);
            }
            if(event.key.code == Keyboard::Down) {
                p1.dir = 2; p1.speed = 1;
                Frame += 0.031*time;
                if(Frame > 7) {
                    std::cout << "\nDown";
                    Frame -= 3;
                }
                p1.mSprite.setTextureRect(IntRect(128*int(Frame), 768, 128, 128));
                getPlayerCoordinateForView(p1.getPlayerCoordinateX(), p1.getPlayerCoordinateY());
                //mSprite.move(0, speed*time);
            }
            if(event.key.code == Keyboard::LAlt) {
                view.zoom(1.0900f);
                std::cout << "\n-zoom";
            }
            if(event.key.code == Keyboard::LControl) {
                view.zoom(0.9100f);
                std::cout << "\n+Zoom";
            }
        }
        if (event.type == Event::KeyReleased && p1.life == true) {
            if (event.key.code == Keyboard::Right || event.key.code == Keyboard::Left) {
                p1.isMove = false; // Сбрасываем флаг движения при отпускании клавиш
                // Остальной код без изменений
            }
        }
    }
    
    
    p1.update(time);
    
    window.setView(view);
    window.clear();
    
    
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
            if (TileMap[i][j] == 'f') {
                sStone.setTextureRect(IntRect(0, 0, 32, 32));
                window.draw(sStone);
            }
            if (TileMap[i][j] == 'h') {
                sHp.setTextureRect(IntRect(0, 0, 32, 32));
                window.draw(sHp);
            }
            
            sStone.setPosition(j*32, i*32);
            sHp.setPosition(j*32, i*32);
            sMap.setPosition(j*32, i*32);
            
            window.draw(sMap);
        }
    }
    
    if (!showMissionText) {
        text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
        sMission.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
        window.draw(sMission); window.draw(text);
    }
    
    // text
    ostringstream playerScoreString;
    ostringstream playerHealthString;
    playerScoreString << p1.playerScore;
    playerHealthString << p1.health;
    textPoints.setString("Points: " + playerScoreString.str());
    textPoints.setPosition(view.getCenter().x-300, view.getCenter().y-220);
    textHP.setString("Health: " + playerHealthString.str());
    textHP.setPosition(view.getCenter().x-300, view.getCenter().y-190);
    
    window.draw(text);
    window.draw(textPoints);
    window.draw(textHP);
    window.draw(p1.mSprite);
    window.display();
}

return 0;
}
