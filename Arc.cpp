#include "Header.h"
sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
class Game{
public:
    vector <Brick> Bricks;
    Ball _ball;
    Platform plat;
    int random;
    vector <Bonuses> Bon;
    void Play();
    void handleEvents();
    void Creation();
    void Drawing();
    void FindColisions();
};

void Game::FindColisions() {
    FindColision(Bon, plat, _ball);
    FindColision(_ball, plat);
    FindColision(_ball, Bricks);
}

void Game::Drawing() {
    for (int i = 0; i < Bricks.size(); i++)
        window.draw(Bricks[i].brick);
    for (int i = 0; i < Bon.size(); i++) {
        window.draw(Bon[i].Bonus);
        Bon[i].Update();
    }
    window.draw(_ball.ball);
    window.draw(plat.platform);
    window.display();
    window.clear();

}

void Game::Creation() {
    _ball.Create();
    plat.Create(200);
    Bricks.resize(50);
    for (int i = 0; i < 50; i++) {
        Bricks[i].Fill(i);
    }

}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                if (plat.x1_pos >= 40) {
                    plat.platform.move(-40, 0);
                    plat.x1_pos -= 40;
                    plat.x2_pos -= 40;
                }

            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (plat.x2_pos <= window_x - 40) {
                    plat.platform.move(40, 0);
                    plat.x1_pos += 40;
                    plat.x2_pos += 40;
                }

            }
        }
    }
}

void Game::Play() {
    window.setFramerateLimit(300);
    Creation();
    while (window.isOpen()) {
        Drawing();
        handleEvents();
        _ball.Update();
        FindColisions();
        if (_ball.Life == 0)
            window.close();
        for (int i = 0; i < Bricks.size(); i++) {
            if (Bricks[i].brick_life == 0) {
                random = rand() % 10;
                if (random > 6) {
                    Bonuses temp_bon;
                    temp_bon.Ins(Bricks[i]);
                    Bon.push_back(temp_bon);
                }
                Bricks.erase(Bricks.cbegin() + i);
                break;
            }
        }
    }

}

int main() {
    srand(time(0));
    Game PlayGame;
    PlayGame.Play();

}
