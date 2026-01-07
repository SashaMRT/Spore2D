/**
 * @file Simulation.cpp
 * @brief Gestion du moteur physique et des limites du monde.
 */

// AUCUN INCLUDE (PCH)

static float g_xMin = 0.f, g_xMax = 1000.f;
static float g_yMin = 0.f, g_yMax = 1000.f;
static float g_simulationTime = 0.f;

static std::list<Grass> ecosystem_grass; 
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

static int g_deadSheep = 0, g_deadWolves = 0;
static int g_bornSheep = 0, g_bornWolves = 0;

/**
 * @brief Définit les limites strictes du terrain de jeu.
 */
void setWorldBounds(float xMin, float xMax, float yMin, float yMax) {
    g_xMin = xMin;
    g_xMax = xMax;
    g_yMin = yMin;
    g_yMax = yMax;
}

/**
 * @brief Génère une position aléatoire strictement à l'intérieur des bordures.
 */
static sf::Vector2f randomPos() {
    float margin = 30.f;
    float rangeX = std::max(1.f, g_xMax - g_xMin - (margin * 2));
    float rangeY = std::max(1.f, g_yMax - g_yMin - (margin * 2));
    
    float x = g_xMin + margin + (rand() % (int)rangeX);
    float y = g_yMin + margin + (rand() % (int)rangeY);
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
    check(ecosystem_sheeps);
    check(ecosystem_wolves);
}

void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();
    g_simulationTime = 0.f;
    g_deadSheep = 0; g_deadWolves = 0;
    g_bornSheep = 0; g_bornWolves = 0;

    for (int i = 0; i < 50; i++) ecosystem_grass.emplace_back(randomPos());
    for (int i = 0; i < 20; i++) ecosystem_sheeps.emplace_back(randomPos());
    for (int i = 0; i < 3;  i++) ecosystem_wolves.emplace_back(randomPos());
}

void ecosystemUpdate(float dt) {
    static bool _init = [](){ srand(time(NULL)); initEcosystem(); return true; }();
    g_simulationTime += dt;

    // 1. HERBE
    if ((rand() % 100) < 5) ecosystem_grass.emplace_back(randomPos());
    for (auto it = ecosystem_grass.begin(); it != ecosystem_grass.end(); ) {
        if (it->pos.x < g_xMin || it->pos.x > g_xMax || it->pos.y < g_yMin || it->pos.y > g_yMax) {
            it = ecosystem_grass.erase(it); 
        } else { ++it; }
    }

    // 2. LOUPS
    std::vector<Wolf> babyWolves;
    for (size_t i = 0; i < ecosystem_wolves.size(); ++i) {
        Wolf& w = ecosystem_wolves[i];
        if (!w.alive) continue;
        w.update(dt);
        w.moveAI(dt, ecosystem_sheeps, g_simulationTime);
        w.eat(ecosystem_sheeps); 
        w.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax); // CRITIQUE

        if (w.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_wolves.size(); ++j) {
                Wolf& partner = ecosystem_wolves[j];
                if (partner.alive && partner.canReproduce() && w.dist(partner.pos) < 40.f) {
                    babyWolves.emplace_back(w.pos);
                    w.resetReproduction();
                    partner.resetReproduction();
                    g_bornWolves++;
                    break;
                }
            }
        }
    }

    // 3. MOUTONS
    std::vector<Sheep> babySheeps;
    for (size_t i = 0; i < ecosystem_sheeps.size(); ++i) {
        Sheep& s = ecosystem_sheeps[i];
        if (!s.alive) continue;
        s.update(dt);
        s.moveAI(dt, ecosystem_wolves, ecosystem_grass, g_simulationTime);
        for (auto& g : ecosystem_grass) {
            if (s.dist(g.pos) < 15.f) { g.alive = false; s.eatGrass(); break; }
        }
        s.checkBounds(g_xMin, g_xMax, g_yMin, g_yMax); // CRITIQUE

        if (s.canReproduce()) {
            for (size_t j = i + 1; j < ecosystem_sheeps.size(); ++j) {
                Sheep& partner = ecosystem_sheeps[j];
                if (partner.alive && partner.canReproduce() && s.dist(partner.pos) < 30.f) {
                    babySheeps.emplace_back(s.pos);
                    s.resetReproduction();
                    partner.resetReproduction();
                    g_bornSheep++;
                    break;
                }
            }
        }
    }

    solveCollisions();
    ecosystem_sheeps.insert(ecosystem_sheeps.end(), babySheeps.begin(), babySheeps.end());
    ecosystem_wolves.insert(ecosystem_wolves.end(), babyWolves.begin(), babyWolves.end());

    auto cleanDead = [](auto& vec, int& counter) {
        vec.erase(std::remove_if(vec.begin(), vec.end(), [&](auto& e){
            if (!e.alive) { counter++; return true; }
            return false;
        }), vec.end());
    };
    cleanDead(ecosystem_sheeps, g_deadSheep);
    cleanDead(ecosystem_wolves, g_deadWolves);
    ecosystem_grass.remove_if([](const Grass& g){ return !g.alive; });
}

EcosystemStats getEcosystemStats() {
    int lambsCount = 0;
    for (const auto& s : ecosystem_sheeps) if (s.getLevel() == 1) lambsCount++;
    int pupsCount = 0;
    for (const auto& w : ecosystem_wolves) if (w.getLevel() == 1) pupsCount++;

    return { (int)ecosystem_grass.size(), (int)ecosystem_sheeps.size(), lambsCount,
             (int)ecosystem_wolves.size(), pupsCount, g_deadSheep, g_deadWolves, 
             g_bornSheep, g_bornWolves, g_simulationTime };
}

void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) g.draw(window);
    for (auto& s : ecosystem_sheeps) if (s.alive) s.draw(window);
    for (auto& w : ecosystem_wolves) if (w.alive) w.draw(window);
}