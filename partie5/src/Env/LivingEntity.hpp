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
class Grass;

class LivingEntity : public Obstacle {
  public:
    //CONSTRUCTEUR
    LivingEntity(Vec2d position, double energyLevel);

    //DESTRUCTEUR
    virtual ~LivingEntity();

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
    virtual void isInfectedBy(LivingEntity* other) = 0;
    virtual void infect(Grass* other) = 0;
    virtual void infect(Wolf* other) = 0;
    virtual void infect(Sheep* other) = 0;
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
    virtual std::array<double,6> getAnimalData() const = 0;

    //METHODES
    Vec2d getPosition() const;
    double getEnergyLevel() const;
    sf::Time getLifeTime() const;
    Vec2d getCenter() const override final;
    static unsigned int getNbLivingEntities(std::string entite);
    bool isTracked() const;
    void setNiveauEnergie(double nrj);
    void setTrack(bool value);
    virtual void update(sf::Time dt);

  protected:
    //ATTRIBUTS
    Vec2d position;
    double energyLevel;
    sf::Time lifeTime;
    bool tracked;

    //METHODES
    virtual void setPosition(); //monde torique

    //ATTRIBUT DE CLASSE
    static std::unordered_map<std::string, unsigned int> nbLivingEntities;
};

#endif
