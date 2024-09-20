#pragma once

#include <SFML/Graphics.hpp>

class Coins {
public:
    sf::CircleShape shape;
    float speed;

    Coins(float width, float height) {
        shape.setRadius(10);
        shape.setFillColor(sf::Color::Yellow);
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

    // Check for collision with the player
    bool isCollidingWithPlayer(const sf::RectangleShape& other) {
        return shape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

};