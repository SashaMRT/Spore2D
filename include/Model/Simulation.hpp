/**
 * @file Simulation.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Interface publique de l'écosystème : init, update, draw
 * @details Déclare les 3 fonctions principales pour contrôler la simulation complète
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */

// Empêche d'inclure ce fichier deux fois.
#pragma once

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

// Inclut SFML Graphics pour les références sf::RenderWindow et les graphiques.
#include <SFML/Graphics.hpp>

// ===== INTERFACE DE L'ÉCOSYSTÈME =====
// Fonctions centrales pour gérer la simulation complète (herbe, moutons, loups).

// Initialise l'écosystème : crée les premières herbes, moutons et loups à des positions aléatoires.
// À appeler une seule fois au démarrage de la simulation.
void initEcosystem();

// Met à jour toute la logique de l'écosystème :
// - Mouvements, IA (recherche d'herbe, fuite des loups)
// - Régénération, chasse, alimentation
// - Suppression des entités mortes et respawn automatique.
void ecosystemUpdate(float dt);

// Dessine toutes les entités vivantes (herbes, moutons, loups) dans la fenêtre.
// Positionne automatiquement les formes SFML aux bonnes coordonnées.
void ecosystemDraw(sf::RenderWindow& window);

// Structure pour transporter les stats facilement
struct EcosystemStats {
    int grass;
    int sheep;
    int wolves;
};

// Fonction pour récupérer les statistiques actuelles
EcosystemStats getEcosystemStats();

#endif // SIMULATION_HPP
