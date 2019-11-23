#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "Obstacle.hpp"
#include <string>
#include <Genetics/Genome.hpp>
#include <Genetics/ImmuneSystem.hpp>


#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H

class Wolf;
class Sheep;
class Consommable;
class Grass;
class Water;

class LivingEntity : public Obstacle {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    LivingEntity(Vec2d position, double energyLevel);
    virtual ~LivingEntity();

    //METHODES VIRTUELLES PURES
    virtual void drawOn(sf::RenderTarget& targetWindow) const = 0;
    virtual std::string getTexture() const = 0;
    virtual sf::Time getLongevity() const = 0;
    virtual bool eatable(LivingEntity const* entity) const = 0;
    virtual bool eatableBy(Wolf const* wolf) const = 0;
    virtual bool eatableBy(Sheep const* sheep) const = 0;
    virtual bool eatableBy(Grass const* grass) const = 0;
    virtual bool eatableBy(Water const* water) const = 0;
    virtual bool drinkable(LivingEntity const* entity) const = 0;
    virtual bool drinkableBy(Wolf const* wolf) const = 0;
    virtual bool drinkableBy(Sheep const* sheep) const = 0;
    virtual bool drinkableBy(Grass const* grass) const = 0;
    virtual bool drinkableBy(Water const* water) const = 0;
    virtual bool matable(LivingEntity const* other) const = 0;
    virtual bool canMate(Wolf const* wolf) const = 0;
    virtual bool canMate(Sheep const* sheep) const = 0;
    virtual bool canMate(Consommable const* grass) const = 0;
    virtual void meeting(LivingEntity* other) = 0;
		virtual void meet(Sheep* other) = 0;
    virtual void meet(Wolf* other) = 0;
    virtual void meet(Consommable* other) = 0;
    virtual void isInfectedBy(LivingEntity* other) = 0;
    virtual void infect(Consommable* other) = 0;
    virtual void infect(Wolf* other) = 0;
    virtual void infect(Sheep* other) = 0;
    virtual void isCuring(Sheep* other) = 0;
    virtual void isCuring(Wolf* other) = 0; //personne ne mange le loup donc il ne peut pas être une consommation curative
    virtual void isCuring(Consommable* other) = 0;
    virtual void poison(Sheep* other) = 0;
    virtual void poison(Wolf* other) = 0;
    virtual void poison(Consommable* other) = 0;

    /*Ajoute les animaux en fonction du numéro du troupeau
      moutons et loups seront différenciés car les numéros de troupeau des moutons sont les entiers positifs (0 compris)
      alors que les numéros de troupeau des loups sont les entiers négatifs*/
    virtual void addAnimal(std::vector<LivingEntity*>& animals, int numTroupeau) = 0;
    virtual void becomeLeader() = 0;
    virtual bool getIsAbleToBeFollowed() const = 0;
    virtual std::string getStatLabel() const = 0;
    virtual void drawTrack(sf::RenderTarget& targetWindow) const = 0;
    virtual bool isAlive() const = 0;
    virtual void becomeInfected(Virus* v) = 0;
    virtual std::array<double,7> getAnimalData() const = 0;

    //GETTERS
    Vec2d getPosition() const;
    double getEnergyLevel() const;
    sf::Time getLifeTime() const;
    Vec2d getCenter() const override final;
    static unsigned int getNbLivingEntities(std::string entite);
    bool isTracked() const;
    //SETTERS
    void setEnergyLevel(double nrj);
    void setTrack(bool value);
    //MÉTHODE DE MISE À JOUR
    virtual void update(sf::Time dt);

  protected:
    //ATTRIBUTS
    Vec2d position;
    double energyLevel;
    sf::Time lifeTime;
    bool tracked;

    //MÉTHODE
    virtual void setPosition(); //monde torique

    //ATTRIBUT DE CLASSE
    static std::unordered_map<std::string, unsigned int> nbLivingEntities;
};

#endif
