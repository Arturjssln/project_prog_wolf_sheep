#include "Animal.hpp"
#include <string>
#include "Application.hpp"

#ifndef WOLF_H
#define WOLF_H

class Wolf : public Animal {
  public:
    //CONSTRUCTEUR
    Wolf(Vec2d position = Vec2d(0,0), int numTroupeau = -1, double niveauEnergie = getAppConfig().sheep_energy_initial,
          double magnitude = 0, Genome* pere = nullptr, Genome* mere = nullptr);

    //DESTRUCTEUR
    ~Wolf();

    //METHODES
    //GETTERS
    double getStandardMaxSpeed() const override;
    double getMass() const override;
    double getRadius() const override;
    double getViewRange() const override;
    double getViewDistance() const override;
    double getRandomWalkRadius() const override;
    double getRandomWalkDistance() const override;
    double getRandomWalkJitter() const override;
    std::string getTexture() const override;
    sf::Time getLongevity() const override;
    double getFacteurPerteEnergie() const override;
    double getMinEnergyMating() const override;
    int getMinChildren() const override;
    int getMaxChildren() const override;
    double getEnergyLossMatingMale() const override;
    double getEneryLossPerChildFemale() const override;
    sf::Time getGestationTime() const override;
    bool isLeader() const override;
    int getNumHerd() const override;
    bool getIsAbleToBeFollowed() const override;

    //SETTERS
    void setIsAbleToBeFollowed(bool b) override;
    void becomeLeader() override;

    //AUTRES
    //GESTION DE LA NOURRITURE
    bool eatable(LivingEntity const* entity) const override;
    void isEating(LivingEntity* entity) override;
    //GESTION DE LA REPRODUCTION
    bool matable(LivingEntity const* other) const override;
    void giveBirth() override;
    //GESTION DU DÉPLACEMENT EN TROUPEAU
    void addSheep(std::vector<LivingEntity*>& tab, int numTroupeau) override;
    void addWolf(std::vector<LivingEntity*>& tab, int numTroupeau) override;
    bool isFree() const override;

    //DESSIN
    void drawCrown(sf::RenderTarget& targetWindow) const override;
    void drawInfoHerd(sf::RenderTarget& targetWindow) const override;

  private:
    //METHODE
    bool eatableBy(Wolf const* wolf) const override;
    bool eatableBy(Sheep const* sheep) const override;
    bool eatableBy(Grass const* grass) const override;
    bool canMate(Wolf const* wolf) const override;
    bool canMate(Sheep const* sheep) const override;
    bool canMate(Grass const* grass) const override;
    void meeting(LivingEntity* other) override;
    void meet(Wolf* wolf) override;
    void meet(Sheep* sheep) override;
    void meet(Grass* grass) override;


};

#endif
