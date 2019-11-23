#include "Flower.hpp"
#include "Sheep.hpp"
#include <Utility/Utility.hpp>
#include <iostream>
#include <Random/Random.hpp>

//CONSTRUCTEUR
Flower::Flower(Vec2d position)
: Grass(position)
{
  int flowerNumber(uniform(0,4));
  switch (flowerNumber) {
    case 0 : texture = getAppConfig().flower_texture_blue;
      break;
    case 1 : texture = getAppConfig().flower_texture_green;
      break;
    case 2 : texture = getAppConfig().flower_texture_pink;
      break;
    case 3 : texture = getAppConfig().flower_texture_purple;
      break;
    case 4 : texture = getAppConfig().flower_texture_red;
      break;
    default : texture = getAppConfig().flower_texture_pink;
  }
}

/**************************************************************************************************************/
//MÉTHODES
std::string Flower::getTexture() const {
  return texture;
}

void Flower::isCuring(Sheep* other) {
  other->isBeingCured(this);
}
