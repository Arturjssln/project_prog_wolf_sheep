#include "Water.hpp"
#include <Utility/Utility.hpp>
#include <iostream>

//CONSTRUCTEUR & DESTRUCTEUR
Water::Water(Vec2d position, double energyLevel)
: Consommable(position, energyLevel)
{ ++nbLivingEntities[s::WATER];}

Water::~Water()
{ --nbLivingEntities[s::WATER]; }

/**************************************************************************************************************/
//GETTERS
std::string Water::getTexture() const {
  return getAppConfig().water_texture;
}

std::string Water::getStatLabel() const {
  return s::WATER_INDIVIDUAL;
}

double Water::getMaxRadius() const {
  return getAppConfig().water_max_energy/2;
}

/**************************************************************************************************************/
//SETTER
void Water::setPosition() {
  //dépassement à droite de la fenêtre
  if(position.x >= getAppConfig().simulation_world_size - getAppConfig().water_max_energy/2) {
      position.x = getAppConfig().simulation_world_size - getAppConfig().water_max_energy/2;
  }
  //dépassement à gauche de la fenêtre
  if(position.x < getAppConfig().water_max_energy/2) {
      position.x = getAppConfig().water_max_energy/2;
  }
  //dépassement en bas de la fenêtre
  if(position.y >= getAppConfig().simulation_world_size - getAppConfig().water_max_energy/2) {
      position.y = getAppConfig().simulation_world_size - getAppConfig().water_max_energy/2;
  }
  //dépassement en haut de la fenêtre
  if(position.y < getAppConfig().water_max_energy/2) {
      position.y = getAppConfig().water_max_energy/2;
  }
}

/**************************************************************************************************************/
//MÉTHODE DE MISE À JOUR (UPDATE)
void Water::update(sf::Time dt) {
  switch (getAppEnv().getSeason()) {
    case SUMMER: if (energyLevel > 0) energyLevel -= getAppConfig().water_growth_factor * 0.5 * dt.asSeconds();
      //Les flaques d'eau diminuent -> il ne pleut pas souvent et l'eau s'évapore
      break;
    case AUTUMN: if ((energyLevel < getAppConfig().water_max_energy) and (canGrow())) {
      energyLevel += getAppConfig().water_growth_factor * 2 * dt.asSeconds(); }
      //On multiplie le growth facteur pas 2 -> il pleut souvent en automne
      break;
    default: if ((energyLevel < getAppConfig().water_max_energy) and (canGrow())) {
      energyLevel += getAppConfig().water_growth_factor * dt.asSeconds(); }
  }
  LivingEntity::update(dt);
}

/**************************************************************************************************************/
//AUTRES MÉTHODES
//GESTION DE LA NOURRITURE
bool Water::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Water::eatableBy(Wolf const*) const {
	return false;
}

bool Water::eatableBy(Sheep const*) const {
	return false;
}

bool Water::eatableBy(Grass const*) const {
	return false;
}

bool Water::eatableBy(Water const*) const {
	return false;
}

//GESTION DE LA BOISSON
bool Water::drinkable(LivingEntity const* entity) const {
	return entity->drinkableBy(this);
}

bool Water::drinkableBy(Wolf const*) const {
	return true;
}

bool Water::drinkableBy(Sheep const*) const {
	return true;
}

bool Water::drinkableBy(Grass const*) const {
	return false;
}

bool Water::drinkableBy(Water const*) const {
	return false;
}
