#include "../../include/Model/Wolf.hpp"
#include "../../include/Model/Sheep.hpp"
#include <cmath>

// Mise à jour du loup (énergie, vie, apparence)
void Wolf::update(float dt) {
    if (!alive) return;

    energy -= hunger * dt;            // Consomme de l’énergie
    if (energy <= 0) alive = false;   // Meurt s’il n’en a plus

    // Transparence selon l’énergie restante
    unsigned char alpha = static_cast<unsigned char>(energy / maxEnergy * 255);
    shape.setFillColor(sf::Color(139, 69, 19, alpha));
}

// Recherche du mouton le plus proche dans la portée "view"
void Wolf::hunt(std::vector<Sheep>& sheeps) {
    Sheep* target = nullptr;
    float minDist = view;

    for (auto& s : sheeps) {
        if (s.alive) {
            float d = dist(s.pos);
            if (d < minDist) { minDist = d; target = &s; }
        }
    }

    // Avance vers la cible si trouvée
    if (target) {
        sf::Vector2f dir = target->pos - pos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) pos += (dir / len) * speed * 0.016f;
    }
}

// Mange un mouton proche et regagne de l’énergie
void Wolf::eat(std::vector<Sheep>& sheeps) {
    for (auto& s : sheeps) {
        if (s.alive && dist(s.pos) < 30) {
            energy = std::min(maxEnergy, energy + 90);
            s.alive = false;
            break;
        }
    }
}
