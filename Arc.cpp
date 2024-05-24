#include <SFML/Graphics.hpp>

sf::RenderWindow  window(sf::VideoMode(800, 600), "Arkanoid");

using namespace std;

class Moving {
public:
    double x_speed=0.1, y_speed=-0.3;
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
    void Create();
};

void Platform::Create() {
    platform.setSize(sf::Vector2f(200, 20));
    platform.setPosition(window.getSize().x / 2 - platform.getSize().x / 2, window.getSize().y - 50);
    x1_pos = window.getSize().x / 2 - platform.getSize().x / 2;
    x2_pos = x1_pos + platform.getSize().x;
    y1_pos = window.getSize().y - 50;
    y2_pos = y1_pos;
    platform.setFillColor(sf::Color::White);
}

class Brick :public Figures, public Moving {
public: 
    sf::RectangleShape brick;  // обычные , неразрушимые, увел скор, размер платформы, прилипание, сайф , траектория
    int brick_type, brick_life; 
    bool IsMoving;
    //0-Неразрушаемые. От них мяч просто отскакивает.
    //1-Блоки могут быть со спрятанными бонусами : при попадании вертикально вниз падает бонус
    //2-Блоки, увеличивающие скорость шарика при столкновении
    //3-Блоки имеют уровень здоровья = число попаданий, чтобы блок исчез.За каждое попадание + 1 очко игроку, -1 очко здоровья блоку.
    //4_Бонусы : меняют размер каретки, меняют скорость шарика, меняют прилипание шарика к каретке.
    //5-Бонус : одноразовое дно для шарика – можно один раз пропустить, дно сработает каккаретка, и исчезнет.
    //6=Бонус : шарик в произвольный момент меняет траекторию.
    void Fill(int number);
};

void Brick::Fill(int number) {
    IsMoving = false;
    int i = number % 10;
    int j = number / 10;
            brick_type = rand() % 7;
            brick_life = rand() % 3 + 1;
            brick.setSize(sf::Vector2f(80, 20));
            brick.setPosition(i * 80, j * 20);
            x1_pos = i* brick.getSize().x;
            x2_pos = x1_pos + brick.getSize().x;
            y1_pos = j* brick.getSize().y;
            y2_pos = y1_pos + brick.getSize().y;
;            brick.setFillColor(sf::Color(255 - 50 * brick_type, 255 - 50 * brick_type, 255));
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
}

void CheckColision(Ball& _ball, vector<Brick>& _Bricks, Platform _plat) {
    if (_ball.y2_pos >= window.getSize().y) {
        window.close();
        return;
    }
    if (_ball.y1_pos <= 0) {
        _ball.y_speed *= -1;
        return;
    }
    if (_ball.x2_pos == window.getSize().x) {
        _ball.x_speed *= -1;
    }
    if ((_ball.x1_pos <= 0) || (_ball.x2_pos >= window.getSize().x)) {
        _ball.x_speed *= -1;
        return;
    }
    if ((((_ball.x1_pos >= _plat.x1_pos) && (_ball.x1_pos <= _plat.x2_pos))||((_ball.x2_pos >= _plat.x1_pos) && (_ball.x2_pos <= _plat.x2_pos))) && (_ball.y2_pos >= _plat.y1_pos)) {
        _ball.y_speed *= -1;
        return;
    }
    for (int i = 0; i < _Bricks.size(); i++) {
        if ((((_ball.x1_pos >= _Bricks[i].x1_pos) && (_ball.x1_pos <= _Bricks[i].x2_pos)) || ((_ball.x2_pos >= _Bricks[i].x1_pos) && (_ball.x2_pos <= _Bricks[i].x2_pos))) && (((_ball.y1_pos >= _Bricks[i].y1_pos) && (_ball.y1_pos <= _Bricks[i].y2_pos)) || ((_ball.y2_pos >= _Bricks[i].y1_pos) && (_ball.y2_pos <= _Bricks[i].y2_pos)))) {
            if ((((_ball.x1_pos >= _Bricks[i].x1_pos) && (_ball.x1_pos <= _Bricks[i].x2_pos)) && ((_ball.x2_pos >= _Bricks[i].x1_pos) && (_ball.x2_pos <= _Bricks[i].x2_pos))))
                _ball.y_speed *= -1;
            else
                _ball.x_speed *= -1;
            
            if (_Bricks[i].brick_type != 1) {
                _Bricks[i].brick_life--;
                if (_Bricks[i].brick_life == 0) {
                    _Bricks.erase(_Bricks.cbegin() + i);
                }
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

int main() {
    vector <Brick> Bricks;
    Bricks.resize(50);
    for (int i = 0; i < 50; i++) {
        Bricks[i].Fill(i);
    }
    Ball _ball;
    _ball.Create();
    Platform plat;
    plat.Create();
    sf::sleep(sf::seconds(1));
    while (window.isOpen()) {
        handleEvents(plat);
        for (int i = 0; i < Bricks.size(); i++)
            window.draw(Bricks[i].brick);
        window.draw(_ball.ball);
        window.draw(plat.platform);
        window.display();
        _ball.Update();
        CheckColision(_ball, Bricks, plat);
        window.clear();
    }

}
