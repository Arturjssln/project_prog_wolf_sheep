#include "MagicMushroom.hpp"
#include "Sheep.hpp"
#include "LivingEntity.hpp"
#include <Utility/Utility.hpp>
#include <iostream>
#include <Random/Random.hpp>

//CONSTRUCTEUR
MagicMushroom::MagicMushroom(Vec2d position)
: Grass(position)
{}

/**************************************************************************************************************/
//MÉTHODES
void MagicMushroom::update(sf::Time dt) {
  LivingEntity::update(dt);
  //Les champignons gardent leur taille initiale
}

void MagicMushroom::poison(Sheep* other) {
  other->isBeingPoisoned(this);
  //Lors de l'empoisonnement, le champignon "meurt" en étant mangé
  setEnergyLevel(-1);
}

std::string MagicMushroom::getTexture() const {
  return getAppConfig().texture_magicMushroom;
}
