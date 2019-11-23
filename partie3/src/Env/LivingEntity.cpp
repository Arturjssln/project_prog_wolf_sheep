#include "LivingEntity.hpp"
#include "Application.hpp"

//CONSTRUCTEUR
LivingEntity::LivingEntity(Vec2d pos, double nivEnergie)
: Obstacle(), position(pos), niveauEnergie(nivEnergie), tempsDeVie(sf::Time::Zero)
{}

/**************************************************************************************************************/
//METHODES GETTERS

Vec2d LivingEntity::getPosition() const {
    return position;
}

double LivingEntity::getNiveauEnergie() const {
  return niveauEnergie;
}

Vec2d LivingEntity::getCenter() const {
    return getPosition();
}

/**************************************************************************************************************/
//METHODES SETTERS


void LivingEntity::setNiveauEnergie(double nrj) {
  niveauEnergie = nrj;
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

/**************************************************************************************************************/
//METHODES

bool LivingEntity::isAlive() const {
  if(niveauEnergie < getAppConfig().animal_min_energy or tempsDeVie > getLongevity()) return false;
  return true;
}

void LivingEntity::update(sf::Time dt) {
  setPosition(); //mise à jour position de l'animal si dépasse du monde
  tempsDeVie += dt; //augmente le temps de vie de dt
}
