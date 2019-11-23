#include "LivingEntity.hpp"
#include "Application.hpp"

//INITIALISATION ATTRIBUT DE CLASSE
std::unordered_map<std::string, unsigned int> LivingEntity::nbLivingEntities = {{s::WOLVES, 0}, {s::SHEEP, 0}, {s::GRASS, 0}, {s::WATER, 0}};

//CONSTRUCTEUR
LivingEntity::LivingEntity(Vec2d pos, double nivEnergie)
: Obstacle(), position(pos), energyLevel(nivEnergie),
 lifeTime(sf::Time::Zero), tracked(false)
{}

LivingEntity::~LivingEntity() {
  if(tracked) getAppEnv().stopTrackingAnyEntity();
}

/**************************************************************************************************************/
//GETTERS
Vec2d LivingEntity::getPosition() const {
    return position;
}

double LivingEntity::getEnergyLevel() const {
  return energyLevel;
}

sf::Time LivingEntity::getLifeTime() const {
  return lifeTime;
}

Vec2d LivingEntity::getCenter() const {
    return getPosition();
}

unsigned int LivingEntity::getNbLivingEntities(std::string entite) {
  return LivingEntity::nbLivingEntities[entite];
}

bool LivingEntity::isTracked() const {
  return tracked;
}

/**************************************************************************************************************/
//SETTERS
void LivingEntity::setEnergyLevel(double nrj) {
  energyLevel = nrj;
}

void LivingEntity::setPosition() {
    //dépassement à droite de la fenêtre
    if(position.x >= getAppConfig().simulation_world_size) {
        position.x = 0.0;
    }
    //dépassement à gauche de la fenêtre
    if(position.x < 0.0) {
        position.x = getAppConfig().simulation_world_size;
    }
    //dépassement en bas de la fenêtre
    if(position.y >= getAppConfig().simulation_world_size) {
        position.y = 0.0;
    }
    //dépassement en haut de la fenêtre
    if(position.y < 0.0) {
        position.y = getAppConfig().simulation_world_size;
    }
}

void LivingEntity::setTrack(bool value) {
  tracked = value;
}

/**************************************************************************************************************/
//MÉTHODE DE MISE À JOUR
void LivingEntity::update(sf::Time dt) {
  setPosition(); //mise à jour position de l'animal si dépasse du monde
  lifeTime += dt; //augmente le temps de vie de dt
}
