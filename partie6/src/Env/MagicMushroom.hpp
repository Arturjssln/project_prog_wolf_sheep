#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "Grass.hpp"
#include "Application.hpp"

#ifndef MAGICMUSHROOM_H
#define MAGICMUSHROOM_H

class MagicMushroom : public Grass {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    MagicMushroom(Vec2d position = Vec2d(0,0));
    ~MagicMushroom() = default;

    //MÉTHODES
    void update(sf::Time dt) override final;
    void poison(Sheep* other) override final;
    std::string getTexture() const override final;

};

#endif
