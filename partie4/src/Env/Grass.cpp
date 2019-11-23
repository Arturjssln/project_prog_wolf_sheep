#include "Grass.hpp"
#include <Utility/Utility.hpp>
#include <iostream>

//CONSTRUCTEUR
Grass::Grass(Vec2d position, double niveauEnergie)
: LivingEntity(position, niveauEnergie)
{ ++nbLivingEntities["grass"]; }

//DESTRUCTEUR
Grass::~Grass()
{ --nbLivingEntities["grass"]; }

/**************************************************************************************************************/
//METHODES GETTERS

std::string Grass::getTexture() const {
  return getAppConfig().grass_texture;
}

double Grass::getRadius() const {
  return niveauEnergie / 2;
}

sf::Time Grass::getLongevity() const {
  return sf::seconds(1E9);
}

/**************************************************************************************************************/
//METHODE SETTERS

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
//METHODES

void Grass::drawOn(sf::RenderTarget& targetWindow) const {
  //Représentation de l'herbe avec une texture
  auto animalSprite = buildSprite(position, getRadius() * 2 , getAppTexture(getTexture()));
  targetWindow.draw(animalSprite);
}

void Grass::update(sf::Time dt) {
  if (niveauEnergie < getAppConfig().grass_max_energy) {
    niveauEnergie += getAppConfig().grass_growth_factor * dt.asSeconds();
  }
  LivingEntity::update(dt);
}

bool Grass::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Grass::eatableBy(Wolf const* wolf) const {
	return false;
}

bool Grass::eatableBy(Sheep const* sheep) const {
	return true;
}

bool Grass::eatableBy(Grass const* grass) const {
	return false;
}

bool Grass::matable(LivingEntity const* other) const  {
  return other->canMate(this);
}

bool Grass::canMate(Wolf const* wolf) const {
	return false;
}

bool Grass::canMate(Sheep const* sheep) const {
	return false;
}

bool Grass::canMate(Grass const* grass) const {
	return false;
}

void Grass::meeting(LivingEntity* other) {
  //Ne fait rien
}

void Grass::meet(Wolf* other) {
  //Ne fait rien
}

void Grass::meet(Grass* other) {
	//Ne fait rien
}

void Grass::meet(Sheep* other) {
	//Ne fait rien
}

void Grass::addSheep(std::vector<LivingEntity*>& tab, int numTroupeau) {
  //ne fait rien
}

void Grass::addWolf(std::vector<LivingEntity*>& tab, int numTroupeau) {
  //ne fait rien
}

void Grass::becomeLeader() {
	//ne fait rien
}

bool Grass::getIsAbleToBeFollowed() const {
  return false;
}
