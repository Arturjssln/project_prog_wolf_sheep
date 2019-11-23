#include "ImmuneSystem.hpp"
#include "Application.hpp"
#include <Random/Random.hpp>


//INITIALISATION ATTRIBUT DE CLASSE
unsigned int ImmuneSystem::nbInfected = 0;

//CONSTRUCTEUR
ImmuneSystem::ImmuneSystem(Animal* host)
: healthState(getAppConfig().immune_health_max),
  immuneProfile({{0}}),
  activationLevel(getAppConfig().immune_adaptive_baseline),
  host(host), virus(nullptr), adaScore(0.0), globalScore(0.0),
  isFighting(false)
{}

//DESTRUCTEUR
ImmuneSystem::~ImmuneSystem()
{
  //Si l'animal est infecté et meurt, on décrémente le nombre d'animaux infectés
  if (isInfected()) --nbInfected;
}

//GETTERS
double ImmuneSystem::getHealthState() const {
  return healthState;
}

double ImmuneSystem::getAdaScore() const {
  return adaScore;
}

double ImmuneSystem::getGlobalScore() const {
  return globalScore;
}

double ImmuneSystem::getActivationLevel() const {
  return activationLevel;
}

double ImmuneSystem::getQuantityVirus() const {
  if (virus == nullptr) return 0.0;
  return virus->getQuantityInfectiousAgents();
}

Virus* ImmuneSystem::getVirus() const {
  return virus;
}

unsigned int ImmuneSystem::getNbInfected() {
  return ImmuneSystem::nbInfected;
}

//SETTERS
void ImmuneSystem::setVirus(Virus* v) {
  virus = new Virus(*v);
  //On incrémente le nombre d'animaux infectés
  ++nbInfected;
}

void ImmuneSystem::increaseHealthState(double health) {
  healthState += health;
}

//MÉTHODES
bool ImmuneSystem::isInfected() const {
  return (virus != nullptr);
}

void ImmuneSystem::update(sf::Time dt) {
  calculScore(); //S'il n'y a pas de virus, les scores sont nuls

  if (isInfected()) {
    //Évolution du niveau activation
    activationLevel = activationLevel * (1 + dt.asSeconds() * (0.5 * (1 - pow(activationLevel, 2)/16)));

    //Évolution du virus
    virus->evolve(dt);

    if(virus->isAbleToInfect() or (isFighting)) {
    /*Le système immunitaire combat le virus quand celui-ci peut l'infecter,
    une fois infecté, le système immuniataire doit combattre le virus jusqu'à son extinction
    même si le virus n'est plus considéré comme "capable d'infecter"*/
      if (!isFighting) isFighting = true;
      //Adaptation au virus
      if (virus->isAbleToInfect()) {
        std::array<double,10> virulence(virus->getVirulenceProfile());
        for (size_t i(0); i < immuneProfile.size(); ++ i) {
          immuneProfile[i] += virulence[i] * dt.asSeconds() * 0.1;
        }
      }

      //Combat
      double virusQuantity(virus->getQuantityInfectiousAgents());
      healthState -= getAppConfig().immune_health_penalty * virusQuantity * dt.asSeconds();
      virus->setQuantityInfectiousAgents(virusQuantity - globalScore * dt.asSeconds());

      //Si le virus est mort on le supprime du système immunitaire
      if (virus->isDead()) {
        delete virus;
        virus = nullptr;
        isFighting = false;
        //On décrémente le nombre d'animaux infectés
        --nbInfected;
      }
    }
  } else {
    if (activationLevel > getAppConfig().immune_adaptive_baseline) {
        const double decreaseFactor(0.995);
        activationLevel *= decreaseFactor;
    }
    if (healthState < getAppConfig().immune_health_max) {
      healthState += getAppConfig().immune_health_recovery * dt.asSeconds();
    }
  }
}

void ImmuneSystem::calculAdaScore() {
  std::array<double,10> virulence(virus->getVirulenceProfile());
  double adaptatifScore(0.0);
  for (size_t i(0); i < immuneProfile.size(); ++ i) {
    adaptatifScore += immuneProfile[i] * virulence[i];
  }
  adaptatifScore *= getAppConfig().immune_defense_effectiveness;
  adaScore = adaptatifScore;
}

void ImmuneSystem::calculScore() {
  if (!isInfected()) {
    globalScore = 0;
    adaScore = 0;
  } else {
    std::array<double,10> virulence(virus->getVirulenceProfile());

    calculAdaScore();
    double score(adaScore);

    for (size_t i(0); i < immuneProfile.size(); ++ i) {
      double variability = uniform(-getAppConfig().immune_defense_random_variability,
                                    getAppConfig().immune_defense_random_variability);
      score += ((getAppConfig().immune_defense_effectiveness) * (host->getGenome().getImmuneGenes(i) * virulence[i] + variability));
    }
    globalScore = pow(score, 2) * activationLevel;
  }
}
