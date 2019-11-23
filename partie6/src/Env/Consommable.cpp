#include "Consommable.hpp"
#include <Utility/Utility.hpp>
#include <iostream>

//CONSTRUCTEUR
Consommable::Consommable(Vec2d position, double energyLevel)
: LivingEntity(position, energyLevel)
{}

/**************************************************************************************************************/
//GETTERS
double Consommable::getRadius() const {
  return energyLevel/2;
}

sf::Time Consommable::getLongevity() const {
  return sf::seconds(1E9);
}

bool Consommable::isAlive() const {
  if(energyLevel < 0 or lifeTime > getLongevity()) return false;
  return true;
}

bool Consommable::getIsAbleToBeFollowed() const {
  return false;
}

std::array<double,7> Consommable::getAnimalData() const {
  std::array<double,7> empty;
  return empty;
}

/**************************************************************************************************************/
//AUTRES MÉTHODES
void Consommable::addAnimal(std::vector<LivingEntity*>&, int ) {
  //Ne fait rien
}

void Consommable::becomeLeader() {
	//Ne fait rien
}

//Regarde s'il n'y a pas collision avec d'autres entités, pour éviter de se chevaucher
bool Consommable::canGrow() const {
  std::list<LivingEntity*> entities(getAppEnv().getEntities());
  for (auto entity : entities) {
    if (isColliding(*entity) and (entity->getPosition() != position)) return false;
  }
  return true;
}

//GESTION DE LA REPRODUCTION
bool Consommable::matable(LivingEntity const* other) const  {
  return other->canMate(this);
}

bool Consommable::canMate(Wolf const*) const {
	return false;
}

bool Consommable::canMate(Sheep const*) const {
	return false;
}

bool Consommable::canMate(Consommable const*) const {
	return false;
}

void Consommable::meeting(LivingEntity*) {
  //Ne fait rien
}

void Consommable::meet(Wolf*) {
  //Ne fait rien
}

void Consommable::meet(Sheep*) {
	//Ne fait rien
}

void Consommable::meet(Consommable*) {
	//Ne fait rien
}

//GESTION DES INFECTIONS
void Consommable::becomeInfected(Virus*) {
  //Ne peut pas être infectée
}

void Consommable::isInfectedBy(LivingEntity*) {
	//Ne peut pas transmettre de virus
}

void Consommable::infect(Sheep*) {
  //Ne fait rien
}

void Consommable::infect(Wolf*) {
  //Ne fait rien
}

void Consommable::infect(Consommable*) {
  //Ne fait rien
}

//GESTION DES SOINS (FLEURS)
void Consommable::isCuring(Sheep*) {
  //Ne fait rien en général, redéfinition dans Flower
}

void Consommable::isCuring(Wolf*) {
  //Ne fait rien
}

void Consommable::isCuring(Consommable*) {
  //Ne fait rien
}

//GESTION DES HALLUCINATIONS (CHAMPIGNONS)
void Consommable::poison(Sheep*) {
  //Ne fait rien en général, redéfinition dans MagicMushroom
}

void Consommable::poison(Wolf*) {
  //Ne fait rien
}

void Consommable::poison(Consommable*) {
  //Ne fait rien
}

/**************************************************************************************************************/
//MÉTHODES DE DESSIN
void Consommable::drawOn(sf::RenderTarget& targetWindow) const {
  //Représentation de l'herbe avec une texture
  auto animalSprite = buildSprite(position, getRadius() * 2 , getAppTexture(getTexture()));
  targetWindow.draw(animalSprite);

  if (isTracked()) drawTrack(targetWindow);
}

void Consommable::drawTrack(sf::RenderTarget& targetWindow) const {
  auto trackSprite = buildSprite(position, getRadius() * 0.5 , getAppTexture(getAppConfig().entity_texture_tracked));
  targetWindow.draw(trackSprite);
}
