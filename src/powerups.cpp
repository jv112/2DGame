#pragma once

#include <SFML/Graphics.hpp>
#include "player.cpp"
#include "enemy.cpp"
#include <iostream>

class Powerups {
public:
    enum Type {
        SPEEDBOOST,
        SCOREMULTIPLIER,
        LARGERCROSSHAIR,
        SCREENCLEAR
    };

    Type type;
    sf::CircleShape shape;
    float speed;
    float duration;
    float timer;

    Powerups(float width, float height) {
        int randomType = rand() % 4;

        switch (randomType) {
            case 0:
                type = SPEEDBOOST;
                break;
            case 1:
                type = SCOREMULTIPLIER;
                break;
            case 2:
                type = LARGERCROSSHAIR;
                break;
            case 3:
                type = SCREENCLEAR;
                break;
        }
        shape.setRadius(10);
        shape.setFillColor(sf::Color::Green);
        speed = 200.f;
        timer = 0.f;
        duration = 10.f;

        shape.setPosition(rand() % int(width - shape.getPoint(1).x), 0);
    }

    void move(float deltaTime) {
        shape.move(0.f, speed * deltaTime);
    }

    bool isAtBottom(float height) {
        return shape.getPosition().y > height;
    }

    void update(float deltaTime) {
        timer += deltaTime;
    }

    bool isExpired() const {
        return timer >= duration;
    }

    bool isColliding(const sf::RectangleShape& other) {
        return shape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

    void applyEffect(Player& player, std::vector<Enemy>& enemies, int& scoreMultiplier, int& crosshairMultiplier) {
        switch (type) {
            case SPEEDBOOST:
                player.speed = 600.f;
                break;
            case SCOREMULTIPLIER:
                scoreMultiplier = 10;
                break;
            case LARGERCROSSHAIR:
                crosshairMultiplier = 2;
                break;
            case SCREENCLEAR:
                enemies.clear();
                break;
        }
    }

    void removeEffect(Player& player, int& scoreMultiplier, int& crosshairMultiplier) {
        switch (type) {
            case SPEEDBOOST:
                player.speed = 400.f;
                break;
            case SCOREMULTIPLIER:
                scoreMultiplier = 1;
                break;
            case LARGERCROSSHAIR:
                crosshairMultiplier = 1;
                break;
        }
    }

    bool hasNewerEffect(std::vector<Powerups>& powerups) {
        for (size_t i = 1; i < powerups.size(); i++) {
            if (powerups[i].type == type) {
                return true;
            }
        }
        return false;
    }

    void printType() {
        switch (type) {
            case SPEEDBOOST:
                std::cout << "Speedboost" << std::endl;
                break;
            case SCOREMULTIPLIER:
                std::cout << "Score Multiplier" << std::endl;
                break;
            case LARGERCROSSHAIR:
                std::cout << "Larger Crosshair" << std::endl;
                break;
            case SCREENCLEAR:
                std::cout << "Screen Clear" << std::endl;
                break;
        }
    }

};