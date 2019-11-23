#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "Consommable.hpp"
#include "Application.hpp"

#ifndef WATER_H
#define WATER_H

class Water : public Consommable {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    Water(Vec2d position = Vec2d(0,0), double energyLevel = getAppConfig().water_initial_energy);
    ~Water();

    //GETTERS
    std::string getTexture() const override final;
    std::string getStatLabel() const override final;
    double getMaxRadius() const override final;
    //MÉTHODE DE MISE À JOUR
    void update(sf::Time dt) override final;

    bool eatable(LivingEntity const* entity) const override final;
    bool drinkable(LivingEntity const* other) const override final;

  private:
    //la taille est représentée par energyLevel dans la classe LivingEntity
    //la flaque d'eau est entièrement contenu dans l'environnement
    void setPosition() override final;
    //GESTION DE LA NOURRITURE
    bool eatableBy(Wolf const*) const override final;
    bool eatableBy(Sheep const*) const override final;
    bool eatableBy(Grass const*) const override final;
    bool eatableBy(Water const*) const override final;
    //GESTION DE LA BOISSON
    bool drinkableBy(Wolf const*) const override final;
    bool drinkableBy(Sheep const*) const override final;
    bool drinkableBy(Grass const*) const override final;
    bool drinkableBy(Water const*) const override final;
};

#endif
