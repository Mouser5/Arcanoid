#include <SFML/Graphics.hpp>

sf::RenderWindow  window(sf::VideoMode(800, 600), "Arkanoid");
int Life = 3;
using namespace std;

class Moving {
public:
    double x_speed=1, y_speed=-1;
    bool SpeedUp = false;
};

class Place {
public:
	double x1_pos,x2_pos, y1_pos, y2_pos;
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

void Platform::Create(int i) {
    platform.setSize(sf::Vector2f(i, 20));
    platform.setPosition(window.getSize().x / 2 - platform.getSize().x / 2, window.getSize().y - 50);
    x1_pos = window.getSize().x / 2 - platform.getSize().x / 2;
    x2_pos = x1_pos + platform.getSize().x;
    y1_pos = window.getSize().y - 50;
    y2_pos = y1_pos;
    platform.setFillColor(sf::Color::White);
}

class Brick :public Figures, public Moving {
public: 
    sf::RectangleShape brick;  
    int brick_type, brick_life;   // 0- обычный 9- неуз 
    void ColorConventer();
    void Fill(int number);
};

void Brick::ColorConventer() {
    if (brick_type == 9) {
        brick.setFillColor(sf::Color(128,128,128));
        return;
    }
    if (brick_life == 3) {
        brick.setFillColor(sf::Color::Green);
    }
    else
        if (brick_life == 2) {
            brick.setFillColor(sf::Color::Yellow);
        }
        else
            brick.setFillColor(sf::Color::Red);

}

void Brick::Fill(int number) {
    int i = number % 10;
    int j = number / 10;
            brick_type = rand() % 10;
            brick_life = rand() % 3 + 1;
            brick.setSize(sf::Vector2f(80, 20));
            brick.setPosition(i * 80, j * 20);
            x1_pos = i* brick.getSize().x;
            x2_pos = x1_pos + brick.getSize().x;
            y1_pos = j* brick.getSize().y;
            y2_pos = y1_pos + brick.getSize().y;
            ColorConventer();
}

class Ball : public Figures, public Moving {
public:
    sf::CircleShape ball;
    void Create();
    void Update();
};

void Ball::Create() {
    ball.setRadius(10);
    ball.setPosition(window.getSize().x / 2 - ball.getRadius(), window.getSize().y / 2 - ball.getRadius());
    x1_pos = window.getSize().x / 2 - ball.getRadius();
    x2_pos = window.getSize().x / 2 + ball.getRadius();
    y1_pos = window.getSize().y / 2 - ball.getRadius();
    y2_pos = window.getSize().y / 2 + ball.getRadius();
    ball.setFillColor(sf::Color::White);
}

void Ball::Update() {
    ball.move(x_speed, y_speed);
    x1_pos += x_speed;
    x2_pos += x_speed;
    y1_pos += y_speed;
    y2_pos += y_speed;
    if (y2_pos >= window.getSize().y) {
        Life--;
        if (Life == 0)
            window.close();
        else
            y_speed *= -1;
        return;
    }
    if (y1_pos <= 0) {
        y_speed *= -1;
        return;
    }
    if ((x1_pos <= 0) || (x2_pos >= window.getSize().x)) {
        x_speed *= -1;
        return;
    }
}

class Bonuses : public Figures , public Moving{
public:
    sf::CircleShape Bonus;
    int type;
    void Update();
    void Ins(Brick _brick);
    void ActBonus(Platform&,Ball&);
    void ColorConvert();

};

void ChangeSpeed(Ball& _ball) {
    if (!_ball.SpeedUp) {
        _ball.x_speed *= 1.25;
        _ball.y_speed *= 1.25;
        _ball.SpeedUp = true;
    }
    else
    {
        _ball.x_speed /= 1.25;
        _ball.y_speed /= 1.25;
    }

}

void ChangeSize(Platform& _plat) {
    if (_plat.platform.getSize().x == 200) {
        _plat.Create(300);
    }
    else
        _plat.Create(200);
}

void RandomBall(Ball& _ball) {
    double angle = (rand() % 360) * 3.14159 / 180;
    _ball.x_speed = _ball.x_speed * cos(angle);
    _ball.y_speed = _ball.y_speed * sin(angle);
}

void Bonuses::ActBonus(Platform& _plat,Ball& _ball) {
    switch (type)
    {
    case 0: ChangeSize(_plat);
        break;
    case 1: ChangeSpeed(_ball);
        break;
    case 2: Life++;
        break;
    case 3: RandomBall(_ball);
    }
}

void Bonuses::ColorConvert() {
    switch (type)
    {
    case 0: Bonus.setFillColor(sf::Color::Blue);
        break;
    case 1:     Bonus.setFillColor(sf::Color::Yellow);
        break;
    case 2:     Bonus.setFillColor(sf::Color::Magenta);
        break;
    case 3:     Bonus.setFillColor(sf::Color::Cyan);
    }
}

void Bonuses::Ins(Brick _brick) {
    type = rand() % 4;
    x1_pos = _brick.x1_pos;
    x2_pos = _brick.x2_pos;
    y1_pos = _brick.y1_pos;
    y2_pos = _brick.y2_pos;
    Bonus.setRadius(10);
    Bonus.setPosition(x1_pos+abs(x1_pos-x2_pos)/2,y1_pos+abs(y1_pos-y2_pos)/2);
    ColorConvert();
}

void Bonuses::Update() {
    Bonus.move(0, 0.5);
    y1_pos += 0.5;
    y2_pos += 0.5;
}

bool isColision(Figures & A, Figures& B)
{
    return A.x2_pos >= B.x1_pos && A.x1_pos <= B.x2_pos &&
        A.y2_pos >= B.y1_pos && A.y1_pos <= B.y2_pos;
}

void FindColision(vector<Bonuses> &_bonus, Platform & _plat, Ball& _ball) {
    for (int i = 0; i < _bonus.size(); i++) {
        if (isColision(_bonus[i], _plat)) {
            _bonus[i].ActBonus(_plat, _ball);
            _bonus.erase(_bonus.cbegin() + i);
        }
        else
        if (_bonus[i].y1_pos > _plat.y2_pos)
            _bonus.erase(_bonus.cbegin() + i);
    }
}

void FindColision(Ball &_ball, Platform _plat) {
    if (!isColision(_ball, _plat))
        return;
    double ball_center = _ball.x1_pos + _ball.ball.getRadius(),
        plat_center = _plat.x1_pos + _plat.platform.getSize().x / 2;
    _ball.y_speed *= -1;
    if (ball_center < plat_center) {
        _ball.x_speed = -abs(_ball.x_speed);
    }
    else
        _ball.x_speed = abs(_ball.x_speed);
}

void FindColision(Ball& _ball, vector <Brick>& _bricks) {
    double min_x, min_y;
    for (int i = 0; i < _bricks.size(); i++) {
        if (isColision(_ball, _bricks[i])) {
            double IntersectsLeft{ _ball.x2_pos - _bricks[i].x1_pos };
            double  IntersectsRight{ _bricks[i].x2_pos - _ball.x1_pos };
            double IntersectsTop{ _ball.y2_pos - _bricks[i].y1_pos };
            double IntersectsBottom{ _bricks[i].y2_pos - _ball.y1_pos };
            bool DirectionRight(abs(IntersectsLeft) > abs(IntersectsRight));
            bool DirectionTop(abs(IntersectsTop) < abs(IntersectsBottom));
            if (DirectionRight) {
                min_x = IntersectsRight;
            }
            else
                min_x = IntersectsLeft;
            if (DirectionTop) {
                min_y = IntersectsTop;
            }
            else
                min_y = IntersectsBottom;
            if (abs(min_x) < abs(min_y))
                if (DirectionRight)
                    _ball.x_speed = abs(_ball.x_speed);
                else
                    _ball.x_speed = -abs(_ball.x_speed);
            else
                if (DirectionTop)
                    _ball.y_speed = -abs(_ball.y_speed);
                else
                    _ball.y_speed = abs(_ball.y_speed);
            if (_bricks[i].brick_type != 9) {
                _bricks[i].brick_life--;
                _bricks[i].ColorConventer();
            }
            return;
        }
    }


}

void handleEvents(Platform & _plat) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                if (_plat.x1_pos >=40) {
                _plat.platform.move(-40, 0);
                _plat.x1_pos -= 40;
                _plat.x2_pos -= 40;
                }

            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (_plat.x2_pos <= window.getSize().x - 40) {
                _plat.platform.move(40, 0);
                _plat.x1_pos += 40;
                _plat.x2_pos += 40;
                }

            }
        }
    }
}

void Game(vector <Brick> Bricks, Ball _ball, Platform plat) {
    int random = rand() % 10;
    vector <Bonuses> Bon;
    Bricks.resize(50);
    for (int i = 0; i < 50; i++) {
        Bricks[i].Fill(i);
    }
 while (window.isOpen()) {
        handleEvents(plat);
        for (int i = 0; i < Bricks.size(); i++)
            window.draw(Bricks[i].brick);
        for (int i = 0; i < Bon.size(); i++) {
            window.draw(Bon[i].Bonus);
            Bon[i].Update();
        }
        FindColision(Bon, plat,_ball);
        window.draw(_ball.ball);
        window.draw(plat.platform);
        window.display();
        _ball.Update();
        FindColision(_ball, plat);
        FindColision(_ball, Bricks);
        for (int i = 0; i < Bricks.size(); i++) {
            if (Bricks[i].brick_life == 0) {
                if (random>6){
                    Bonuses temp_bon;
                    temp_bon.Ins(Bricks[i]);
                    Bon.push_back(temp_bon);               
                }
                Bricks.erase(Bricks.cbegin() + i);
                break;
            }
        }
        window.clear();
    }
}

int main() {
    vector <Brick> Bricks;
    srand(time(0));
    window.setFramerateLimit(300);
    Ball _ball;
    _ball.Create();
    Platform plat;
    plat.Create(200);
    Game(Bricks, _ball, plat);

}
