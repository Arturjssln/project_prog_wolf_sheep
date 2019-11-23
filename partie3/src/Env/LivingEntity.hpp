#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "Obstacle.hpp"
#include <string>
#include <Genetics/Genome.hpp>

#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H

class Wolf;
class Sheep;
class Grass;

class LivingEntity : public Obstacle {
  public:
    //CONSTRUCTEUR
    LivingEntity(Vec2d position, double niveauEnergie);

    //DESTRUCTEUR
    virtual ~LivingEntity() = default;

    //METHODES VIRTUELLES PURES
    virtual void drawOn(sf::RenderTarget& targetWindow) const = 0;
    virtual std::string getTexture() const = 0;
    virtual sf::Time getLongevity() const = 0;
    virtual bool eatable(LivingEntity const* entity) const = 0;
    virtual bool eatableBy(Wolf const* wolf) const = 0;
    virtual bool eatableBy(Sheep const* sheep) const = 0;
    virtual bool eatableBy(Grass const* grass) const = 0;
    virtual bool matable(LivingEntity const* other) const = 0;
    virtual bool canMate(Wolf const* wolf) const = 0;
    virtual bool canMate(Sheep const* sheep) const = 0;
    virtual bool canMate(Grass const* grass) const = 0;
    virtual void meeting(LivingEntity* other) = 0;
		virtual void meet(Grass* other) = 0;
    virtual void meet(Wolf* other) = 0;
    virtual void meet(Sheep* other) = 0;

    //METHODES
    Vec2d getPosition() const;
    double getNiveauEnergie() const;
    Vec2d getCenter() const override;
    void setNiveauEnergie(double nrj);
    bool isAlive() const;
    virtual void update(sf::Time dt);

  protected:
    //ATTRIBUTS
    Vec2d position;
    double niveauEnergie;
    sf::Time tempsDeVie;

    //METHODES
    virtual void setPosition(); //monde torique
};

#endif
