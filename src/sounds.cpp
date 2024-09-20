#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

class Sounds {
public:
    sf::SoundBuffer hitBuffer;
    sf::Sound hitSound;

    sf::SoundBuffer coinBuffer;
    sf::Sound coinSound;

    sf::SoundBuffer powerupBuffer;
    sf::Sound powerupSound;

    Sounds() {
        if (!hitBuffer.loadFromFile("assets/sounds/shoot_sound.wav")) {
            std::cerr << "Error loading sound" << std::endl;
        }
        hitSound.setBuffer(hitBuffer);
        hitSound.setVolume(25);

        if (!coinBuffer.loadFromFile("assets/sounds/coin_sound.wav")) {
            std::cerr << "Error loading sound" << std::endl;
        }
        coinSound.setBuffer(coinBuffer);
        coinSound.setVolume(15);

        if (!powerupBuffer.loadFromFile("assets/sounds/powerup_sound.wav")) {
            std::cerr << "Error loading sound" << std::endl;
        }
        powerupSound.setBuffer(powerupBuffer);
        powerupSound.setVolume(20);
    }

    void playHitSound() {
        hitSound.play();
    }

    void playCoinSound() {
        coinSound.play();
    }

    void playPowerupSound() {
        powerupSound.play();
    }

};