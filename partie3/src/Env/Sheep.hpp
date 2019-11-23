#include "Animal.hpp"
#include <string>
#include "Application.hpp"

#ifndef SHEEP_H
#define SHEEP_H

class Sheep : public Animal {
  public:
    //CONSTRUCTEUR
    Sheep(Vec2d position = Vec2d(0,0), double niveauEnergie = getAppConfig().sheep_energy_initial,
          double magnitude = 0, Genome* pere = nullptr, Genome* mere = nullptr);

    //DESTRUCTEUR
    ~Sheep() = default;

    //METHODES
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
    bool eatable(LivingEntity const* entity) const override;
    void isEating(LivingEntity* entity) override;
    bool matable(LivingEntity const* other) const override;
    double getMinEnergyMating() const override;
    int getMinChildren() const override;
		int getMaxChildren() const override;
		double getEnergyLossMatingMale() const override;
		double getEneryLossPerChildFemale() const override;
		sf::Time getGestationTime() const override;
    void giveBirth() override;

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
