/**
 * @file Grass.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation optimisée de la classe Grass (Compatible SFML 3).
 * @version 0.6
 * @date 2026-01-06
 */


// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Grass::Grass(sf::Vector2f position) : pos(position), alive(true) {
    // Configuration de la tige principale
    shape.setRadius(4.f);
    
    // CORRECTION SFML 3 : Il faut des accolades {} pour créer un Vector2f
    shape.setOrigin({4.f, 4.f}); 
    
    shape.setFillColor(sf::Color(50, 200, 50)); 
    shape.setPosition(pos);
}

// -------------------------------------------------------------------------
// MISE À JOUR
// -------------------------------------------------------------------------

void Grass::update(float dt) {
    // Vide car statique.
}

// -------------------------------------------------------------------------
// AFFICHAGE (OPTIMISÉ)
// -------------------------------------------------------------------------

void Grass::draw(sf::RenderWindow& window) {
    if (!alive) return;

    // Dessin de la tige principale
    window.draw(shape);

    // OPTIMISATION : Forme statique (créée une seule fois)
    static sf::CircleShape leaf(3.f);
    static bool initialized = false;

    if (!initialized) {
        // CORRECTION SFML 3 : Accolades {}
        leaf.setOrigin({3.f, 3.f});
        initialized = true;
    }

    // Feuille Gauche
    // CORRECTION SFML 3 : Accolades {} autour des coordonnées
    leaf.setPosition({pos.x - 4.f, pos.y + 2.f});
    leaf.setFillColor(sf::Color(30, 180, 30)); 
    window.draw(leaf);

    // Feuille Droite
    // CORRECTION SFML 3 : Accolades {} autour des coordonnées
    leaf.setPosition({pos.x + 4.f, pos.y + 2.f});
    leaf.setFillColor(sf::Color(70, 220, 70)); 
    window.draw(leaf);
}