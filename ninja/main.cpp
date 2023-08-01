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
    int w, h, health;
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
        sprite.setOrigin(w / 2, h / 2);
    }
};

class Player :public Entity {
public:
int countStone;
int dir;
int playerScore;//эта переменная может быть только у игрока
 
Player(Image &image, float X, float Y, int W, int H,String Name):Entity(image,X,Y,W,H,Name) {
    playerScore = 0; dir = 0;
           if (name == "Player1") {
               sprite.setTextureRect(IntRect(4, 19, 40, 30));
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
    
    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = (y + Dy) / 32; i < (y + h + Dy) / 32; i++) {
            for (int j = (x + Dx) / 32; j < (x + w + Dx) / 32; j++) {
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        onGround = true;
                        y = i * 32 - h - 32;
                        dy = 0;
                    }
                    if (Dy < 0) {
                        y = i * 32 + 32;
                        dy = 0;
                    }
                    if (Dx > 0) {
                        x = j * 32 - w;
                    }
                    if (Dx < 0) {
                        x = j * 32 + 32;
                    }
                } //else { onGround = false; }
            }
        }
    }
    
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
    sprite.setPosition(x+w/2, y+h/2);
    if(health <= 0) { life = false; }
    if(!isMove) { speed = 0; }
    if (dy == 0 && onGround) {
            dy = 0; // Сбрасываем значение dy на ноль, если персонаж находится на земле и не прыгает
        }
    if (!onGround) {
            // Если персонаж не на земле, то увеличиваем его скорость падения (гравитацию)
            dy = dy + 0.00002 * time;
        } else {
            // Если персонаж на земле, то обнуляем скорость по оси Y
            dy = 0;
        }
    if(life) { setPlayerCoordinateForView(x, y); }
}

float getPlayerCoordinateX() {
    return x;
}
float getPlayerCoordinateY() {
    return y;
}
};

class Enemy :public Entity{
public:
    Enemy(Image &image, float X, float Y,int W, int H, String Name):Entity(image,X,Y,W,H,Name){
        if (name == "EasyEnemy"){
            sprite.setTextureRect(IntRect(0, 0, W, H));
            dx = 0.01;//даем скорость.этот объект всегда двигается
        }
    }
 
    void checkCollisionWithMap(float Dx, float Dy) {
        int startX = std::max(static_cast<int>(x + Dx) / 32, 0);
        int startY = std::max(static_cast<int>(y + Dy) / 32, 0);
        int endX = std::min(static_cast<int>((x + w + Dx) / 32), WIDTH_MAP - 1);
        int endY = std::min(static_cast<int>((y + h + Dy) / 32), HEIGHT_MAP - 1);

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        y = i * 32 - h;
                    } else if (Dy < 0) {
                        y = i * 32 + 32;
                    }
                    if (Dx > 0) {
                        x = j * 32 - w;
                        dx = -0.05; // Изменяем направление движения влево
                        sprite.scale(-1, 1); // Переворачиваем спрайт влево
                    } else if (Dx < 0) {
                        x = j * 32 + 32;
                        dx = 0.05; // Изменяем направление движения вправо
                        sprite.scale(-1, 1); // Переворачиваем спрайт вправо
                    }
                }
            }
        }
    }

 
    void update(float time)
    {
        if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
            
            //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
            checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
            
            x += dx*time;
            

            sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
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

    //игрок
Image heroImage;
heroImage.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/MilesTailsPrower.gif");
    
    //враг
Image easyEnemyImage;
easyEnemyImage.loadFromFile("/Users/vladislav/Desktop/ninja/ninja/image/shamaich.png");
easyEnemyImage.createMaskFromColor(Color(255, 0, 0));
    
// для виндовс
//Image icon;
//icon.loadFromFile("image/ninja.png");
//sprite.setTextureRect(IntRect(0, 540, 128, 128));
//sprite.setPosition(30, 30);
//window.setIcon(32, 32, icon.getPixelsPtr());

Player p(heroImage, 900, 500, 40, 30, "Player1");
Enemy easyEnemy(easyEnemyImage, 400, 500, 200, 97, "EasyEnemy");//простой враг, объект класса врага
    
bool showMissionText = true; bool isMove = false;
bool keys[4] = {false, false, false, false};
    
Clock clock;
float Frame = 0;

float dX = 0; float dY = 0;

while (window.isOpen())
{
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time/120;
    
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
            // Перемещение персонажа
    
                if (keys[0]) {
                    p.dir = 0; p.speed = 0.5;
                }
                else if (keys[1]) {
                    p.dir = 1; p.speed = 0.5;
                }
                else if (keys[2] && p.onGround) {
                    p.dir = 3; p.speed = 0.5; p.dy = -0.08; p.onGround = false;
                }
                else if (keys[3]) {
                    p.dir = 2; p.speed = 0.5;
                }
                else {
                    // Если не нажата ни одна клавиша, останавливаем персонажа
                    p.speed = 0;
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
