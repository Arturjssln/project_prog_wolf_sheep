#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "Grass.hpp"
#include "Application.hpp"

#ifndef FLOWER_H
#define FLOWER_H

class Flower : public Grass {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    Flower(Vec2d position = Vec2d(0,0));
    ~Flower() = default;

    //MÉTHODES
    std::string getTexture() const override final;
    void isCuring(Sheep* other) override final;

  private:
    std::string texture;

};

#endif
