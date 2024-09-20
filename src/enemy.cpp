#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
public:
    sf::ConvexShape shape;
    float speed;

    Enemy(float width, float height) {
        shape.setPointCount(3);
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(35, 0));
        shape.setPoint(2, sf::Vector2f(17.5, 35));
        shape.setFillColor(sf::Color::Red);
        speed = 200.f;

        // Position randomly at top of window
        shape.setPosition(rand() % int(width - shape.getPoint(1).x), 0);
    }

    void move(float deltaTime) {
        shape.move(0.f, speed * deltaTime);
    }

    bool isAtBottom(float height) {
        return shape.getPosition().y > height;
    }

    bool isCollidingWithPlayer(const sf::RectangleShape& other) {
        return shape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

    bool isOnTarget(const sf::Sprite& other) {
        return shape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

};