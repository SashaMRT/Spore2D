/**
 * @file Application.cpp
 * @author sasha.marieterehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Renderer
 */

#include "../../include/View/Renderer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "../../include/Model/Simulation.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR (CORRECTION 1 : Initialisation de sf::Text)
// -------------------------------------------------------------------------
Renderer::Renderer() : m_statsText(m_font) {
    // Vide
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Initialise les ressources graphiques et la géométrie de la zone de jeu.
 * @details Cette fonction prépare le terrain avant le lancement de la boucle de jeu :
 * 1. Initialise le générateur de nombres aléatoires.
 * 2. Charge la police d'écriture pour le texte.
 * 3. Calcule la taille exacte du rectangle noir pour qu'il rentre dans la fenêtre sans toucher le menu.
 * 4. Lance la création initiale des animaux (initEcosystem).
 * * @param windowSize La taille totale de la fenêtre (Largeur, Hauteur).
 * @param hudWidth La largeur du menu latéral (pour décaler le jeu vers la droite).
 */
void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    
    // -----------------------------------------------------------
    // 1. INITIALISATION DE L'ALÉATOIRE
    // -----------------------------------------------------------
    // On utilise l'heure actuelle (time) comme "graine".
    // Cela garantit que chaque lancement du jeu aura des positions d'animaux différentes.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    // -----------------------------------------------------------
    // 2. CHARGEMENT DE LA POLICE (Ressources)
    // -----------------------------------------------------------
    // On charge le fichier "arial.ttf" depuis le disque.
    // SFML 3 utilise "openFromFile". Si le fichier est absent, on affiche une erreur.
    if (!m_font.openFromFile("arial.ttf")) {
        std::cerr << "ERREUR CRITIQUE : Impossible de charger arial.ttf !" << std::endl;
        // Le programme continuera, mais aucun texte ne s'affichera.
    }

    // Configuration de l'objet texte pour les stats
    m_statsText.setFont(m_font);                // On lie la police chargée
    m_statsText.setCharacterSize(16);           // Taille des lettres en pixels
    m_statsText.setFillColor(sf::Color::White); // Couleur blanche
    
    // Positionnement du texte (En haut à gauche, avec une petite marge de 10px)
    // SFML 3 demande un sf::Vector2f pour la position.
    m_statsText.setPosition(sf::Vector2f(10.f, 10.f)); 


    // -----------------------------------------------------------
    // 3. CALCUL DE LA GÉOMÉTRIE (Map)
    // -----------------------------------------------------------
    // Définition des marges (espace vide autour du rectangle noir)
    float gapLeft = 5.f;   
    float gapRight = 5.f;  
    float gapTop = 5.f;    
    float gapBottom = 10.f;

    // Calcul de la position de départ (Coin Haut-Gauche du rectangle)
    // On se décale de la largeur du menu + la petite marge.
    float posX = hudWidth + gapLeft;
    float posY = gapTop;

    // Calcul de la taille disponible
    // Largeur = Tout l'écran - (Menu + Marge Gauche) - Marge Droite
    float rectWidth = static_cast<float>(windowSize.x) - posX - gapRight;
    // Hauteur = Tout l'écran - Marge Haut - Marge Bas
    float rectHeight = static_cast<float>(windowSize.y) - gapTop - gapBottom;

    // Sécurité : On évite des tailles négatives si la fenêtre est trop petite
    if (rectWidth < 0) rectWidth = 100.f;
    if (rectHeight < 0) rectHeight = 100.f;


    // -----------------------------------------------------------
    // 4. APPLICATION DU STYLE
    // -----------------------------------------------------------
    m_gameArea.setSize(sf::Vector2f(rectWidth, rectHeight));
    m_gameArea.setPosition(sf::Vector2f(posX, posY));
    
    m_gameArea.setFillColor(sf::Color::Black); // Fond noir           
    
    // Bordure "Intérieure" : Une valeur négative dessine vers l'intérieur du rectangle,
    // ce qui évite que la bordure ne dépasse de la zone calculée.
    m_gameArea.setOutlineThickness(-2.f); 
    m_gameArea.setOutlineColor(sf::Color(100, 100, 100)); // Gris clair


    // -----------------------------------------------------------
    // 5. LANCEMENT DE LA SIMULATION
    // -----------------------------------------------------------
    // On appelle la fonction du Modèle (Simulation.cpp) pour créer
    // les premiers moutons, loups et herbes.
    initEcosystem();

    // Debug : On confirme dans la console que tout est prêt
    std::cout << "Renderer initialise : Zone " << rectWidth << "x" << rectHeight << std::endl;
}

/**
 * @brief Boucle de rendu et de mise à jour de la zone de jeu.
 * @details Cette fonction est le cœur visuel du Renderer. Elle effectue 4 actions :
 * 1. Dessine le fond noir.
 * 2. Calcule le temps écoulé (dt) pour fluidifier les mouvements.
 * 3. Met à jour et dessine les entités (via Simulation.cpp).
 * 4. Affiche les statistiques textuelles par-dessus le tout.
 * * @param window La fenêtre SFML cible pour le dessin.
 */
void Renderer::draw(sf::RenderWindow& window) {
    
    // -----------------------------------------------------------
    // 1. DESSIN DU FOND
    // -----------------------------------------------------------
    // On dessine d'abord le rectangle noir (la zone de jeu)
    // pour qu'il soit en arrière-plan (derrière les animaux).
    window.draw(m_gameArea);


    // -----------------------------------------------------------
    // 2. GESTION DU TEMPS (Delta Time)
    // -----------------------------------------------------------
    // "static" signifie que cette horloge n'est pas recréée à chaque appel de la fonction.
    // Elle continue de tourner entre chaque image.
    static sf::Clock ecoClock;
    
    // On redémarre l'horloge et on récupère le temps écoulé depuis la dernière image (en secondes).
    // Cela permet aux animaux de bouger à la même vitesse, peu importe la puissance du PC.
    float dt = ecoClock.restart().asSeconds();


    // -----------------------------------------------------------
    // 3. LOGIQUE ET DESSIN DES ENTITÉS
    // -----------------------------------------------------------
    // On met à jour les positions et la vie des animaux (Simulation.cpp).
    ecosystemUpdate(dt);
    
    // On dessine les sprites des animaux et de l'herbe (Simulation.cpp).
    ecosystemDraw(window);


    // -----------------------------------------------------------
    // 4. MISE À JOUR DU HUD (Overlay)
    // -----------------------------------------------------------
    // On récupère les derniers chiffres de la simulation.
    EcosystemStats stats = getEcosystemStats();

    // On construit le texte à afficher.
    std::string info = "=== VIVANTS ===\n";
    info += "Herbe   : " + std::to_string(stats.grass) + "\n";
    info += "Moutons : " + std::to_string(stats.sheep) + "\n";
    info += "Loups   : " + std::to_string(stats.wolves) + "\n\n";

    info += "=== CIMETIERE ===\n";
    info += "Moutons : " + std::to_string(stats.deadSheep) + "\n";
    info += "Loups   : " + std::to_string(stats.deadWolves);

    // On applique le texte et on le dessine en dernier (pour qu'il soit au premier plan).
    m_statsText.setString(info);
    window.draw(m_statsText);
}

/**
 * @brief Recalcule la géométrie de la zone de jeu lors du redimensionnement de la fenêtre.
 * @details Cette méthode est appelée automatiquement (via Application.cpp) quand l'utilisateur change la taille de la fenêtre.
 * Elle adapte les dimensions du rectangle noir pour qu'il remplisse toujours l'espace restant à droite du menu.
 * * @param newSize La nouvelle taille de la fenêtre (Largeur, Hauteur).
 * @param hudWidth La largeur actuelle du menu (HUD) pour savoir où commencer le dessin.
 */
void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    
    // -----------------------------------------------------------
    // 1. DÉFINITION DES MARGES (Padding)
    // -----------------------------------------------------------
    // On redéfinit les espaces de sécurité pour éviter que le jeu ne colle aux bords
    // même après un redimensionnement.
    float gapLeft = 5.f;   
    float gapRight = 5.f;  
    float gapTop = 5.f;    
    float gapBottom = 10.f;

    // -----------------------------------------------------------
    // 2. CALCUL DES NOUVELLES DIMENSIONS
    // -----------------------------------------------------------
    
    // Le début de la zone de jeu (X) est après le menu + la marge gauche.
    float posX = hudWidth + gapLeft;

    // La largeur disponible = Largeur Fenêtre - (Menu + Marge Gauche) - Marge Droite.
    // On convertit newSize.x (unsigned int) en float pour le calcul.
    float rectWidth = static_cast<float>(newSize.x) - posX - gapRight;
    
    // La hauteur disponible = Hauteur Fenêtre - Marge Haut - Marge Bas.
    float rectHeight = static_cast<float>(newSize.y) - gapTop - gapBottom;

    // -----------------------------------------------------------
    // 3. APPLICATION DES CHANGEMENTS
    // -----------------------------------------------------------
    // On applique la nouvelle taille au rectangle noir.
    // Note : On ne change pas la position (setPosition) car le menu est ancré à gauche,
    // donc le point de départ (Haut-Gauche) reste le même.
    m_gameArea.setSize(sf::Vector2f(rectWidth, rectHeight));
}