/**
 * @file Grass.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de l'entité Herbe : source d'énergie régénérative
 * @details Héritage d'Entity, ajoute la régénération automatique d'énergie
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */

// Empêche d'inclure ce fichier deux fois.
#pragma once

#ifndef GRASS_HPP
#define GRASS_HPP

// Inclut la classe de base Entity, dont Grass hérite.
// On y récupère les champs : pos, energy, maxEnergy, alive, shape, etc.
#include "Entity.hpp"

// Représente une touffe d’herbe dans l’écosystème.
// Utilise l'héritage de Entity pour profiter de la position, de l'énergie et de l'affichage.
struct Grass : public Entity {
    // Vitesse à laquelle l’herbe régénère son énergie au cours du temps.
    float regen = 30.f;

    // Constructeur : initialise une herbe à la position p,
    // avec les paramètres par défaut définis dans Grass.cpp (énergie max, couleur, rayon...).
    Grass(sf::Vector2f p);

    // Met à jour l’état de l’herbe.
    // Typiquement : régénération de l’énergie et mise à jour de la couleur/transparence.
    void update(float dt);
};

#endif
