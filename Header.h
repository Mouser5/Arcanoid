#pragma once
#include <SFML/Graphics.hpp>
#define window_x 800
#define window_y 600
using namespace std;

class Moving {
public:
    double x_speed = 1, y_speed = -1;
    bool SpeedUp = false;
};

class Place {
public:
    double x1_pos, x2_pos, y1_pos, y2_pos;
};

class Figures :public Place {

public:
    void Fill();
    void Create();

};

class Platform :public Figures {
public:
    sf::RectangleShape platform;
    void Create(int);
};

class Brick :public Figures, public Moving {
public:
    sf::RectangleShape brick;
    int brick_type, brick_life;   // 0- обычный 9- неуз 
    void ColorConventer();
    void Fill(int number);
};

class Ball : public Figures, public Moving {
public:
    int Life = 3;
    sf::CircleShape ball;
    void Create();
    void Update();
};

class Bonuses : public Figures, public Moving {
public:
    sf::CircleShape Bonus;
    int type;
    void Update();
    void Ins(Brick _brick);
    void ActBonus(Platform&, Ball&);
    void ColorConvert();
};

void FindColision(vector<Bonuses>& _bonus, Platform& _plat, Ball& _ball);
void FindColision(Ball& _ball, Platform _plat);
void FindColision(Ball& _ball, vector <Brick>& _bricks);

