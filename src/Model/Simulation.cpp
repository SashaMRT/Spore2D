/**
 * @file Simulation.cpp
 * @brief Moteur gérant la chaîne alimentaire : Plante -> Bactérie -> Poisson -> Requin.
 */

// AUCUN INCLUDE ICI (Géré par CMake)

static float g_xMin = 0.f, g_xMax = 1000.f;
static float g_yMin = 0.f, g_yMax = 1000.f;
static float g_simulationTime = 0.f;

static std::list<Grass> ecosystem_plants; 
static std::vector<Sheep> ecosystem_prey; // Bactéries et Poissons
static std::vector<Wolf> ecosystem_sharks;

static int g_deadPrey = 0, g_deadSharks = 0;
static int g_bornPrey = 0, g_bornSharks = 0;

void setWorldBounds(float xMin, float xMax, float yMin, float yMax) {
    g_xMin = xMin; g_xMax = xMax;
    g_yMin = yMin; g_yMax = yMax;
}

static sf::Vector2f randomPos() {
    float margin = 30.f;
    float x = g_xMin + margin + (rand() % (int)std::max(1.f, g_xMax - g_xMin - margin * 2));
    float y = g_yMin + margin + (rand() % (int)std::max(1.f, g_yMax - g_yMin - margin * 2));
    return {x, y};
}

void solveCollisions() {
    auto check = [](auto& entities) {
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                entities[i].resolveCollision(entities[j]);
            }
        }
    };
    check(ecosystem_prey);
    check(ecosystem_sharks);
}

void initEcosystem() {
    ecosystem_plants.clear();
    ecosystem_prey.clear();
    ecosystem_sharks.clear();
    g_simulationTime = 0.f;
    g_deadPrey = 0; g_deadSharks = 0;
    g_bornPrey = 0; g_bornSharks = 0;

    for (int i = 0; i < 60; i++) ecosystem_plants.emplace_back(randomPos());
    for (int i = 0; i < 25; i++) ecosystem_prey.emplace_back(randomPos());
    for (int i = 0; i < 2;  i++) ecosystem_sharks.emplace_back(randomPos());
}

void ecosystemUpdate(float dt) {
    static bool _init = [](){ srand(time(NULL)); initEcosystem(); return true; }();
    g_simulationTime += dt;

    // 1. PLANTES (Apparition)
    if ((rand() % 100) < 7) ecosystem_plants.emplace_back(randomPos());

    // 2. REQUINS (Mangent les poissons)
    std::vector<Wolf> babySharks;
    for (size_t i = 0; i < ecosystem_sharks.size(); ++i) {
        Wolf& w = ecosystem_sharks[i];
        if (!w.alive) continue;
        w.update(dt);
        w.moveAI(dt, ecosystem_prey, g_simulationTime);
        w.eat(ecosystem_prey); // Mange uniquement les poissons (Level 2)
        w.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

        if (w.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sharks.size(); ++j) {
                if (ecosystem_sharks[j].alive && ecosystem_sharks[j].canReproduce() && w.dist(ecosystem_sharks[j].pos) < 40.f) {
                    babySharks.emplace_back(w.pos);
                    w.resetReproduction(); ecosystem_sharks[j].resetReproduction();
                    g_bornSharks++; break;
                }
            }
        }
    }

    // 3. PROIES (Bactéries & Poissons mangent les plantes)
    std::vector<Sheep> babyPrey;
    std::vector<Wolf> newSharksFromEvolution;

    for (size_t i = 0; i < ecosystem_prey.size(); ++i) {
        Sheep& s = ecosystem_prey[i];
        if (!s.alive) continue;
        s.update(dt);
        s.moveAI(dt, ecosystem_sharks, ecosystem_plants, g_simulationTime);
        
        for (auto& p : ecosystem_plants) {
            if (s.dist(p.pos) < 15.f) { 
                p.alive = false; 
                s.eatGrass(); // Gère l'évolution interne (Niveau 1 -> 2)
                break; 
            }
        }

        // EVOLUTION ULTIME : Poisson (Niveau 2) -> Requin (Niveau 3)
        if (s.getLevel() == 3) {
            newSharksFromEvolution.emplace_back(s.pos);
            s.alive = false; // Le poisson "disparaît" pour devenir un requin
            continue;
        }

        s.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax);

        if (s.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_prey.size(); ++j) {
                if (ecosystem_prey[j].alive && ecosystem_prey[j].canReproduce() && s.dist(ecosystem_prey[j].pos) < 30.f) {
                    babyPrey.emplace_back(s.pos);
                    s.resetReproduction(); ecosystem_prey[j].resetReproduction();
                    g_bornPrey++; break;
                }
            }
        }
    }

    solveCollisions();

    // Intégration des nouveaux-nés et évolutions
    ecosystem_prey.insert(ecosystem_prey.end(), babyPrey.begin(), babyPrey.end());
    ecosystem_sharks.insert(ecosystem_sharks.end(), babySharks.begin(), babySharks.end());
    ecosystem_sharks.insert(ecosystem_sharks.end(), newSharksFromEvolution.begin(), newSharksFromEvolution.end());

    // Nettoyage
    auto clean = [](auto& vec, int& counter) {
        vec.erase(std::remove_if(vec.begin(), vec.end(), [&](auto& e){
            if (!e.alive) { counter++; return true; } return false;
        }), vec.end());
    };
    clean(ecosystem_prey, g_deadPrey);
    clean(ecosystem_sharks, g_deadSharks);
    ecosystem_plants.remove_if([](const Grass& p){ return !p.alive; });
}

EcosystemStats getEcosystemStats() {
    int bac = 0;
    int fish = 0;
    
    // On compte les niveaux dans le vecteur des proies
    for (const auto& s : ecosystem_prey) {
        if (s.getLevel() == 1) bac++; 
        else if (s.getLevel() == 2) fish++;
    }

    // Retourne la structure en respectant l'ordre défini dans Simulation.hpp
    return { 
        (int)ecosystem_plants.size(), 
        (int)ecosystem_prey.size(), 
        bac, 
        fish, 
        (int)ecosystem_sharks.size(), 
        g_deadPrey, 
        g_deadSharks, 
        g_bornPrey, 
        g_bornSharks, 
        g_simulationTime 
    };
}
void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& p : ecosystem_plants) p.draw(window);
    for (auto& s : ecosystem_prey) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_sharks) if (w.alive) w.draw(window);
}