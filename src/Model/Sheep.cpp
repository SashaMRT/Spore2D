#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"
#include <cstdlib>
#include <cmath>
#include <ctime>

// Mise à jour du mouton (consomme de l’énergie, gère la mort et l’opacité)
void Sheep::update(float dt) {
    if (!alive) return;

    energy -= hunger * dt;             // Perte d’énergie progressive
    if (energy <= 0) alive = false;    // Meurt s’il n’a plus d’énergie

    // Couleur dépendante de l’énergie (blanc → transparent en mourant)
    unsigned char alpha = static_cast<unsigned char>(energy / maxEnergy * 255);
    shape.setFillColor(sf::Color(255, 255, 255, alpha));
}

// Mange l’herbe proche pour regagner de l’énergie
void Sheep::eat(std::vector<Grass>& grass) {
    for (auto& g : grass) {
        if (g.alive && dist(g.pos) < 25) {                 // Si une herbe est proche
            float e = std::min(30.f, g.energy);            // Montant d’énergie pris
            energy = std::min(maxEnergy, energy + e);      // Recharge l’énergie
            g.energy -= e;                                 // L’herbe perd de l’énergie
            if (g.energy <= 0) g.alive = false;            // Herbe épuisée
            break;
        }
    }
}
