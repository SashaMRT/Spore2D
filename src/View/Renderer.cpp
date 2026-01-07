/**
 * @file Renderer.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gestion de l'affichage du terrain de jeu.
 * @version 0.4
 * @date 2026-01-05
 */


// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Renderer::Renderer() {
    // Rien à faire ici
}

// -------------------------------------------------------------------------
// INITIALISATION
// -------------------------------------------------------------------------

void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    // Aléatoire global
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Style du terrain (Noir avec bordure grise)
    m_gameArea.setFillColor(sf::Color(0, 105, 148)); // Bleu océan
    m_gameArea.setOutlineThickness(-2.f);
    m_gameArea.setOutlineColor(sf::Color(127, 255, 212)); // Aquamarine pour l'écume
    // Calcul de la taille et lancement de la simulation
    onResize(windowSize, hudWidth);
    initEcosystem();
}

// -------------------------------------------------------------------------
// RENDU (DRAW)
// -------------------------------------------------------------------------

void Renderer::draw(sf::RenderWindow& window) {
    window.draw(m_gameArea); // 1. Fond
    ecosystemDraw(window);   // 2. Animaux
}

// -------------------------------------------------------------------------
// GESTION DU REDIMENSIONNEMENT
// -------------------------------------------------------------------------

void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    float gap = 5.f; // Marge standard

    // Calcul de la position (Juste après le menu)
    float posX = hudWidth + gap;
    float posY = gap;

    // Calcul des dimensions avec sécurité (std::max évite les nombres négatifs)
    // Largeur = Fenêtre - Menu - Marges
    float w = std::max(0.f, static_cast<float>(newSize.x) - posX - gap);
    // Hauteur = Fenêtre - Marge Haut - Marge Bas (10.f)
    float h = std::max(0.f, static_cast<float>(newSize.y) - posY - 10.f);

    m_gameArea.setPosition(sf::Vector2f(posX, posY));
    m_gameArea.setSize(sf::Vector2f(w, h));
}