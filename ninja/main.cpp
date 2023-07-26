#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h" //map
#include "view.h" //camera
#include "mission.h" //mission
using namespace sf;
using namespace std;


class Entity {
public:
    float dx, dy, x, y, speed, moveTimer;
    int w1, h1, w2, h2, health;
    bool life, onGround, isMove;
    Texture texture;
    Sprite sprite;
    String name;
    Entity(Image &image, float X, float Y, int W, int H, String Name) {
        x = X; y = Y; name = Name; moveTimer = 0;
        speed = 0; health = 100; dx = 0; dy = 0;
        //w1 = W1; h1 = H1; w2 = W2; h2 = H2;
        life = true; onGround = false;  isMove = false;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w2 / 2, h2 / 2);
    }
};

class Player :public Entity {
public:
int countStone;
int dir;
int playerScore;//эта переменная может быть только у игрока
 
Player(Image &image, float X, float Y, int W2, int H2,String Name):Entity(image,X,Y,W2,H2,Name) {
    playerScore = 0; dir = 0;
           if (name == "Player1") {
               sprite.setTextureRect(IntRect(128, 768, w2, h2));
           }
       }
/*Player(String F, int X, int Y, float W1, float H1, float W2, float H2) {
    // Конструктор класса Player, инициализирует переменные и загружает текстуры
    dir = 0; playerScore = 0; health = 100; dx = 0; dy = 0;
    life = true; isMove = false; onGround = false;
    File = F;
    w1 = W1;
    h1 = H1;
    w2 = W2;
    h2 = H2;
    x = X; y = Y;
    image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/" + File);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(w1, h1, w2, h2));
    sprite.setOrigin(w2/2, h2/2);
}*/
    
void update(float time) {
    // Метод обновления положения игрока
    switch (dir) {
        case 0:
            dx = speed; break;
        case 1:
            dx = -speed; break;
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
    sprite.setPosition(x+w2/2, y+h2/2);
    if(health <= 0) { life = false; }
    if(!isMove) { speed = 0; }
    if (dy == 0 && onGround) {
            dy = 0; // Сбрасываем значение dy на ноль, если персонаж находится на земле и не прыгает
        }
    if(!onGround) { checkCollisionWithMap(0, dy); dy = dy + 0.00002*time;  }
    if(life) { setPlayerCoordinateForView(x, y); }
}

float getPlayerCoordinateX() {
    return x;
}
float getPlayerCoordinateY() {
    return y;
}

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = (y + Dy) / 32; i < (y + h2 + Dy) / 32; i++) {
            for (int j = (x + Dx) / 32; j < (x + w2 + Dx) / 32; j++) {
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        y = i * 32 - h2 - 72;
                        dy = 0;
                        onGround = true;
                    }
                    if (Dy < 0) {
                        y = i * 32 + 32;
                        dy = 0;
                    }
                    if (Dx > 0) {
                        x = j * 32 - w2;
                    }
                    if (Dx < 0) {
                        x = j * 32 + 32;
                    }
                } else {
                    onGround = false;
                }
            }
        }
    }

};

class Enemy :public Entity{
public:
    Enemy(Image &image, float X, float Y,int W,int H,String Name):Entity(image,X,Y,W,H,Name){
        if (name == "EasyEnemy"){
            sprite.setTextureRect(IntRect(0, 0, w2, h2));
            dx = 0.01;//даем скорость.этот объект всегда двигается
        }
    }
 
    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h2) / 32; i++)//проходимся по элементам карты
        for (int j = x / 32; j<(x + w2) / 32; j++)
        {
            if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
            {
                if (Dy>0){ y = i * 32 - h2; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
                if (Dy<0){ y = i * 32 + 32; }//столкновение с верхними краями карты(может и не пригодиться)
                if (Dx>0){ x = j * 32 - w2; dx = -0.1; sprite.scale(-1, 1); }//с правым краем карты
                if (Dx<0){ x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }// с левым краем карты
            }
        }
    }
 
    void update(float time)
    {
        if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
            
            //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
            checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
            x += dx*time;
            sprite.setPosition(x + w2 / 2, y + h2 / 2); //задаем позицию спрайта в место его центра
            if (health <= 0){ life = false; }
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
Sprite s_map;
s_map.setTexture(map);

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
Image quest_image;
quest_image.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/missionbg.jpg");
quest_image.createMaskFromColor(Color(0, 0, 0));
Texture quest_texture;
quest_texture.loadFromImage(quest_image);
Sprite s_quest;
s_quest.setTexture(quest_texture);
s_quest.setTextureRect(IntRect(0, 0, 340, 510));
s_quest.setScale(0.6f, 0.6f);

Image heroImage;
heroImage.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/anima.png");
    
Image easyEnemyImage;
easyEnemyImage.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/shamaich.png");
easyEnemyImage.createMaskFromColor(Color(255, 0, 0));
    
// для виндовс
//Image icon;
//icon.loadFromFile("image/ninja.png");
//sprite.setTextureRect(IntRect(0, 540, 128, 128));
//sprite.setPosition(30, 30);
//window.setIcon(32, 32, icon.getPixelsPtr());

Player p(heroImage, 200, 500, 128, 128, "Player1");
Enemy easyEnemy(easyEnemyImage, 850, 671,200,97,"EasyEnemy");//простой враг, объект класса врага
    
bool showMissionText = true; bool isMove = false;
bool keys[4] = {false, false, false, false};
    
Clock clock;
float Frame = 0;

float dX = 0; float dY = 0;

while (window.isOpen())
{
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/100;
    
    if(!p.life) { view.zoom(0.99992); }
    
    Vector2i pixelPos = Mouse::getPosition(window);
    Vector2f pos = window.mapPixelToCoords(pixelPos);
    cout << pixelPos.x << "\n";
    cout << pos.x << "\n";
    
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
        
        // Обработка событий клавиатуры
        if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Right)
                        keys[0] = true;
                    else if (event.key.code == Keyboard::Left)
                        keys[1] = true;
                    else if (event.key.code == Keyboard::Up)
                        keys[2] = true;
                    else if (event.key.code == Keyboard::Down)
                        keys[3] = true;
                }
                else if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Right)
                        keys[0] = false;
                    else if (event.key.code == Keyboard::Left)
                        keys[1] = false;
                    else if (event.key.code == Keyboard::Up)
                        keys[2] = false;
                    else if (event.key.code == Keyboard::Down)
                        keys[3] = false;
                }
        
        
        // мышка
        /*if (event.type == Event::MouseButtonPressed) {
            if(Mouse::isButtonPressed(Mouse::Left)) {
                if (p1.sprite.getGlobalBounds().contains(pos.x, pos.y)) {
                    cout << "\nclick";
                    dX = pos.x - p1.sprite.getPosition().x;
                    dY = pos.y - p1.sprite.getPosition().y;
                    isMove = true;
                }
            }
        }
        if (event.type == Event::MouseButtonReleased) { //если отпустили клавишу
            if (event.key.code == Mouse::Left) //а именно левую
                isMove = false; //то не можем двигать спрайт
            p1.sprite.setColor(Color::White);//и даем ему прежний цвет
        }
        if (isMove) {
            p1.sprite.setColor(Color::Red);
            p1.x = pos.x - dX;
            p1.y = pos.y - dY;
        }*/
        
        
        // клавиатура
        if (p.life == true) {
            if(keys[0]) {
                p.dir = 0; p.speed = 0.1;
                Frame += 0.003*time;
                std::cout << "\nRight";
                if(Frame > 7) {
                    Frame -= 3;
                }
                p.sprite.setTextureRect(IntRect(128*int(Frame), 512, 128, 128));
                setPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
                //sprite.move(speed*time, 0);
            }
            
            if(event.key.code == Keyboard::Tab) {
                switch (showMissionText) {
                    case true: {
                        ostringstream task;
                        task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));
                        text.setString(task.str());
                        text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
                        s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
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
            
            if(keys[1]) {
                p.dir = 1; p.speed = 0.1;
                Frame += 0.003*time;
                if(Frame > 7) {
                    Frame -= 3;
                    std::cout << "\nLeft";
                }
                p.sprite.setTextureRect(IntRect(128*int(Frame), 0, 128, 128));
                setPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
                //sprite.move(-speed*time, 0);
            }
            if(keys[2] && p.onGround) {
                cout << "up";
                p.dir = 3; p.speed = 0.1; p.dy = -0.08; p.onGround = false;
                Frame += 0.003*time;
                if(Frame > 7) {
                    std::cout << "\nUp";
                    Frame -= 3;
                }
                p.sprite.setTextureRect(IntRect(128*int(Frame), 256, 128, 128));
                setPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
                //sprite.move(0, -speed*time);
            }
            if(keys[3]) {
                p.dir = 2; p.speed = 0.1;
                Frame += 0.003*time;
                if(Frame > 7) {
                    std::cout << "\nDown";
                    Frame -= 3;
                }
                p.sprite.setTextureRect(IntRect(128*int(Frame), 768, 128, 128));
                setPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
                //sprite.move(0, speed*time);
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
    }
    
    
    p.update(time);
    easyEnemy.update(time);
    
    window.setView(view);
    window.clear();
    
    
    // RISUNOK MAP
    for(int i = 0; i < HEIGHT_MAP; i++) {
        for (int j = 0; j < WIDTH_MAP; j++) {
            if (TileMap[i][j] == ' ') {
                s_map.setTextureRect(IntRect(0, 0, 32, 32));
            }
            if (TileMap[i][j] == 's') {
                s_map.setTextureRect(IntRect(32, 0, 32, 32));
            }
            if (TileMap[i][j] == '0') {
                s_map.setTextureRect(IntRect(64, 0, 32, 32));
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
            s_map.setPosition(j*32, i*32);
            
            window.draw(s_map);
        }
    }
    
    if (!showMissionText) {
        text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
        s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
        window.draw(s_quest); window.draw(text);
    }
    
    // text
    ostringstream playerScoreString;
    ostringstream playerHealthString;
    playerScoreString << p.playerScore;
    playerHealthString << p.health;
    textPoints.setString("Points: " + playerScoreString.str());
    textPoints.setPosition(view.getCenter().x-300, view.getCenter().y-220);
    textHP.setString("Health: " + playerHealthString.str());
    textHP.setPosition(view.getCenter().x-300, view.getCenter().y-190);
    
    window.draw(text);
    window.draw(textPoints);
    window.draw(textHP);
    window.draw(easyEnemy.sprite);
    window.draw(p.sprite);
    window.display();
}

return 0;
}
