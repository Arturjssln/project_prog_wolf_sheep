#include "Animal.hpp"
#include <string>
#include "Application.hpp"

#ifndef WOLF_H
#define WOLF_H

class Wolf : public Animal {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    Wolf(Vec2d position = Vec2d(0,0), int numTroupeau = -1, double energyLevel = getAppConfig().wolf_energy_initial,
          double magnitude = 0, Genome* pere = nullptr, Genome* mere = nullptr,
          double quenchLevel = getAppConfig().wolf_quench_initial);
    ~Wolf();

    //GETTERS
    double getStandardMaxSpeed() const override final;
    double getMass() const override final;
    double getRadius() const override final;
    double getViewRange() const override final;
    double getViewDistance() const override final;
    double getRandomWalkRadius() const override final;
    double getRandomWalkDistance() const override final;
    double getRandomWalkJitter() const override final;
    std::string getTexture() const override final;
    sf::Time getLongevity() const override final;
    double getEnergyLossFactor() const override final;
    double getWaterLossFactor() const override final;
    double getMinEnergyMating() const override final;
    double getMinQuenchMating() const override final;
    int getMinChildren() const override final;
    int getMaxChildren() const override final;
    double getEnergyLossMatingMale() const override final;
    double getEneryLossPerChildFemale() const override final;
    sf::Time getGestationTime() const override final;
    bool isLeader() const override final;
    int getNumHerd() const override final;
    bool getIsAbleToBeFollowed() const override final;

    //SETTERS
    void setIsAbleToBeFollowed(bool b) override final;
    void becomeLeader() override final;

    //AUTRES MÉTHODES
    //GESTION DE LA NOURRITURE
    bool eatable(LivingEntity const* entity) const override final;
    void isEating(LivingEntity* entity) override final;
    //GESTION DE LA BOISSON
    bool drinkable(LivingEntity const* other) const override final;
    void isDrinking(LivingEntity* entity) override final;
    //GESTION DE LA REPRODUCTION
    bool matable(LivingEntity const* other) const override final;
    void giveBirth() override final;
    //GESTION DU DÉPLACEMENT EN TROUPEAU
    void addAnimal(std::vector<LivingEntity*>& animals, int numTroupeau) override final;
    bool isFree() const override final;
    //GESTION DES INFECTIONS
    void isInfectedBy(LivingEntity* other) override final;
    void isCuring(Sheep*) override final;
    void isCuring(Wolf*) override final;
    void isCuring(Consommable*) override final;
    //GESTION DES HALLUCINATIONS (CHAMPIGNONS)
    void poison(Sheep* other) override final;
    void poison(Wolf* other) override final;
    void poison(Consommable* other) override final;
    bool isPoisoned() const override final;
    void decreasePoisonedTime(sf::Time dt) override final;
    //DESSIN
    void drawCrown(sf::RenderTarget& targetWindow) const override final;
    void drawInfoHerd(sf::RenderTarget& targetWindow) const override final;

  private:
    //GESTION DE LA NOURRITURE
    bool eatableBy(Wolf const*) const override final;
    bool eatableBy(Sheep const*) const override final;
    bool eatableBy(Grass const*) const override final;
    bool eatableBy(Water const*) const override final;
    //GESTION DES BOISSONS
    bool drinkableBy(Wolf const*) const override final;
    bool drinkableBy(Sheep const*) const override final;
    bool drinkableBy(Grass const*) const override final;
    bool drinkableBy(Water const*) const override final;
    //GSTION DE LA REPRODUCTION
    bool canMate(Wolf const* wolf) const override final;
    bool canMate(Sheep const*) const override final;
    bool canMate(Consommable const*) const override final;
    void meeting(LivingEntity* other) override final;
    void meet(Wolf* other) override final;
    void meet(Sheep*) override final;
    void meet(Consommable*) override final;
    //GESTION DES INFECTIONS
    void infect(Consommable*) override final;
    void infect(Wolf* other) override final;
    void infect(Sheep*) override final;
    bool canInfect(Wolf* other);

};

//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& sortie, const Wolf& w);

#endif
