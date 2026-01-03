/**
 * @file Grass.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de l'herbe : régénération d'énergie et affichage dynamique
 * @details 
 *  - Touffe d'herbe comestible par les moutons
 *  - Régénère automatiquement son énergie
 *  - Transparence visuelle proportionnelle à l'énergie disponible
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */


#include "../../include/Model/Grass.hpp"
#include <algorithm>  // Pour std::min

// Constructeur de Grass : initialise  l'herbe à la position donnée.
// Appelle le constructeur Entity avec :
// - Position p
// - Énergie max = 100
// - Couleur verte
// - Rayon = 8 pixels (petite taille)
Grass::Grass(sf::Vector2f p) : Entity(p, 100, sf::Color::Green, 8.f) {}

// Met à jour l'état de l'herbe à chaque frame.
// - Régénère l'énergie progressivement (limite à maxEnergy)
// - Ajuste la transparence selon le niveau d'énergie (pleine = opaque, vide = transparent)
void Grass::update(float dt) {
    if (!alive) return;  // Entité morte : rien à faire
    
    // Régénération d'énergie : energy += regen * temps écoulé
    energy = std::min(maxEnergy, energy + regen * dt);
    
    // Calcul de la transparence : 0% énergie = transparent, 100% = opaque
    unsigned char alpha = static_cast<unsigned char>(energy / maxEnergy * 255);
    
    // Met à jour la couleur : vert vif + transparence variable
    shape.setFillColor(sf::Color(0, 255, 0, alpha));
}
