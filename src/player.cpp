#pragma once

#include <SFML/Graphics.hpp>

class Player {
public:
    sf::RectangleShape shape;
    float speed;

    Player(float width, float height) {
        shape.setSize(sf::Vector2f(25, 25));
        shape.setFillColor(sf::Color::Blue);
        speed = 400.f;

        shape.setPosition((width / 2) - (shape.getSize().x / 2), height - shape.getSize().y - 20);
    }

    void move(float deltaTime, const sf::RenderWindow& window) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            shape.move(-speed * deltaTime, 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            shape.move(speed * deltaTime, 0.f);
        }

        // Prevent the player from moving out of bounds
        if (shape.getPosition().x < 0) {
            shape.setPosition(0, shape.getPosition().y);
        }

        if (shape.getPosition().x + shape.getSize().x > window.getSize().x) {
            shape.setPosition(window.getSize().x - shape.getSize().x, shape.getPosition().y);
        }
    }

    void reset(float width, float height) {
        shape.setPosition((width / 2) - (shape.getSize().x / 2), height - shape.getSize().y - 20);
        speed = 400.f;
    }

};