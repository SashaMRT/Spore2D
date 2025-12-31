#ifndef GRASS_HPP
#define GRASS_HPP

#include "Entity.hpp"

// Représente une touffe d’herbe (source d’énergie pour les moutons)
struct Grass : public Entity {
    float regen = 30.f;  // Vitesse de régénération de l’énergie

    // Constructeur : position, énergie max, couleur, taille
    Grass(sf::Vector2f p) : Entity(p, 100, sf::Color::Green, 8.f) {}

    // Mise à jour : régénère l’énergie et ajuste la transparence
    void update(float dt) {
        if (!alive) return;
        energy = std::min(maxEnergy, energy + regen * dt);
        unsigned char alpha = static_cast<unsigned char>(energy / maxEnergy * 255);
        shape.setFillColor(sf::Color(0, 255, 0, alpha));
    }
};

#endif
