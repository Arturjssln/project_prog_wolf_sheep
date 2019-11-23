#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "LivingEntity.hpp"
#include "Application.hpp"

#ifndef GRASS_H
#define GRASS_H

class Grass : public LivingEntity {
  public:
    //CONSTRUCTEUR
    Grass(Vec2d position = Vec2d(0,0), double energyLevel = getAppConfig().grass_initial_energy);

    //DESTRUCTEUR
    ~Grass();

    //METHODES
    std::string getTexture() const override final;
    sf::Time getLongevity() const override final;
    double getRadius() const override final;
    bool isAlive() const override final;
    void drawOn(sf::RenderTarget& targetWindow) const override final;
    void update(sf::Time dt) override final;
    bool eatable(LivingEntity const* entity) const override final;
    bool matable(LivingEntity const* other) const override final;
    void isInfectedBy(LivingEntity* other) override final;
    void addAnimal(std::vector<LivingEntity*>&, int) override final;
    void becomeLeader() override final;
    bool getIsAbleToBeFollowed() const override final;

    std::string getStatLabel() const override final;
    void drawTrack(sf::RenderTarget& targetWindow) const override final;
    void becomeInfected(Virus*) override final;
    std::array<double,6> getAnimalData() const override final;

  private:
    //la taille est représentée par "double energyLevel" dans la classe LivingEntity
    void setPosition() override final; //le massif d'herbe est entièrement contenu dans l'environnement
    bool eatableBy(Wolf const*) const override final;
    bool eatableBy(Sheep const*) const override final;
    bool eatableBy(Grass const*) const override final;
    bool canMate(Wolf const*) const override final;
    bool canMate(Sheep const*) const override final;
    bool canMate(Grass const*) const override final;
    void meeting(LivingEntity*) override final;
    void meet(Wolf*) override final;
    void meet(Sheep*) override final;
    void meet(Grass*) override final;
    void infect(Grass*) override final;
    void infect(Wolf*) override final;
    void infect(Sheep*) override final;
};

#endif
