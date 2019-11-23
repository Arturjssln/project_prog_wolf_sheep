#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "Consommable.hpp"
#include "Application.hpp"

#ifndef GRASS_H
#define GRASS_H

class Grass : public Consommable {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    Grass(Vec2d position = Vec2d(0,0), double energyLevel = getAppConfig().grass_initial_energy);
    virtual ~Grass();

    //GETTERS
    std::string getTexture() const override;
    std::string getStatLabel() const override final;
    double getMaxRadius() const override final;
    //MÉTHODE DE MISE À JOUR
    void update(sf::Time dt) override;

    bool eatable(LivingEntity const* entity) const override final;
    bool drinkable(LivingEntity const* other) const override final;

  private:
    //la taille est représentée par "double energyLevel" dans la classe LivingEntity
    void setPosition() override final; //le massif d'herbe est entièrement contenu dans l'environnement
    //GESTINO DE LA NOURRITURE
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
