#include "Grass.hpp"
#include <Utility/Utility.hpp>
#include <iostream>

//CONSTRUCTEUR
Grass::Grass(Vec2d position, double energyLevel)
: LivingEntity(position, energyLevel)
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
  return energyLevel / 2;
}

sf::Time Grass::getLongevity() const {
  return sf::seconds(1E9);
}

bool Grass::isAlive() const {
  if(energyLevel < 0 or lifeTime > getLongevity()) return false;
  return true;
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

  if (isTracked()) drawTrack(targetWindow);
}

void Grass::update(sf::Time dt) {
  if (energyLevel < getAppConfig().grass_max_energy) {
    energyLevel += getAppConfig().grass_growth_factor * dt.asSeconds();
  }
  LivingEntity::update(dt);
}

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

bool Grass::matable(LivingEntity const* other) const  {
  return other->canMate(this);
}

bool Grass::canMate(Wolf const*) const {
	return false;
}

bool Grass::canMate(Sheep const*) const {
	return false;
}

bool Grass::canMate(Grass const*) const {
	return false;
}

void Grass::meeting(LivingEntity*) {
  //Ne fait rien
}

void Grass::meet(Wolf*) {
  //Ne fait rien
}

void Grass::meet(Grass*) {
	//Ne fait rien
}

void Grass::meet(Sheep*) {
	//Ne fait rien
}

void Grass::isInfectedBy(LivingEntity* other) {
	other->infect(this);
}

void Grass::infect(Grass*) {
  //Ne fait rien
}

void Grass::infect(Wolf*) {
  //Ne fait rien
}

void Grass::infect(Sheep*) {
  //Ne fait rien
}

void Grass::addAnimal(std::vector<LivingEntity*>&, int ) {
  //Ne fait rien
}

void Grass::becomeLeader() {
	//Ne fait rien
}

bool Grass::getIsAbleToBeFollowed() const {
  return false;
}

std::string Grass::getStatLabel() const {
  return s::GRASS_INDIVIDUAL;
}

void Grass::drawTrack(sf::RenderTarget& targetWindow) const {
  auto trackSprite = buildSprite(position, getRadius() * 0.5 , getAppTexture(getAppConfig().entity_texture_tracked));
  targetWindow.draw(trackSprite);
}

void Grass::becomeInfected(Virus*) {
  //Ne peut pas être infectée
}

std::array<double,6> Grass::getAnimalData() const {
  std::array<double,6> empty;
  return empty;
}
