#include "Grass.hpp"
#include "Environment.hpp"
#include <Utility/Utility.hpp>
#include <iostream>

//CONSTRUCTEUR
Grass::Grass(Vec2d position, double energyLevel)
: Consommable(position, energyLevel)
{ ++nbLivingEntities[s::GRASS]; }

//DESTRUCTEUR
Grass::~Grass()
{ --nbLivingEntities[s::GRASS]; }

/**************************************************************************************************************/
//GETTERS
std::string Grass::getTexture() const {
  return getAppConfig().grass_texture;
}

std::string Grass::getStatLabel() const {
  return s::GRASS_INDIVIDUAL;
}

double Grass::getMaxRadius() const {
  return getAppConfig().grass_max_energy/2;
}

/**************************************************************************************************************/
//SETTER
void Grass::setPosition() {
  //dépassement à droite de la fenêtre
  if(position.x >= getAppConfig().simulation_world_size - getAppConfig().grass_max_energy/2) {
      position.x = getAppConfig().simulation_world_size - getAppConfig().grass_max_energy/2;
  }
  //dépassement à gauche de la fenêtre
  if(position.x < getAppConfig().grass_max_energy/2) {
      position.x = getAppConfig().grass_max_energy/2;
  }
  //dépassement en bas de la fenêtre
  if(position.y >= getAppConfig().simulation_world_size - getAppConfig().grass_max_energy/2) {
      position.y = getAppConfig().simulation_world_size - getAppConfig().grass_max_energy/2;
  }
  //dépassement en haut de la fenêtre
  if(position.y < getAppConfig().grass_max_energy/2) {
      position.y = getAppConfig().grass_max_energy/2;
  }
}

/**************************************************************************************************************/
//MÉTHODE DE MISE À JOUR
void Grass::update(sf::Time dt) {
  switch (getAppEnv().getSeason()) {
    case WINTER: //On ne fait rien -> l'herbre ne pousse pas en hiver
      break;
    case SPRING: if ((energyLevel < getAppConfig().grass_max_energy) and (canGrow())) {
      energyLevel += getAppConfig().grass_growth_factor * 2 * dt.asSeconds(); }
      //On multiplie le growth facteur par 2 -> l'herbe pousse 2 fois plus vite au printemps
      break;
    default: if ((energyLevel < getAppConfig().grass_max_energy) and (canGrow())) {
      energyLevel += getAppConfig().grass_growth_factor * dt.asSeconds(); }
  }
  LivingEntity::update(dt);
}

/**************************************************************************************************************/
//AUTRES MÉTHODES
//GESTION DE LA NOURRITURE
bool Grass::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Grass::eatableBy(Wolf const*) const {
	return false;
}

bool Grass::eatableBy(Sheep const*) const {
	return true;
}

bool Grass::eatableBy(Grass const*) const {
	return false;
}

bool Grass::eatableBy(Water const*) const {
	return false;
}

//GESTION DE LA BOISSON
bool Grass::drinkable(LivingEntity const* entity) const {
	return entity->drinkableBy(this);
}

bool Grass::drinkableBy(Wolf const*) const {
	return false;
}

bool Grass::drinkableBy(Sheep const*) const {
	return false;
}

bool Grass::drinkableBy(Grass const*) const {
	return false;
}

bool Grass::drinkableBy(Water const*) const {
	return false;
}
