#include <string>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include "LivingEntity.hpp"
#include "Application.hpp"

#ifndef CONSOMMABLE_H
#define CONSOMMABLE_H

class Consommable : public LivingEntity {
  public:
    //CONSTRUCTEUR & DESTRUCTEUR
    Consommable(Vec2d position, double energyLevel);
    virtual ~Consommable() = default;

    //GETTERS
    sf::Time getLongevity() const override final;
    double getRadius() const override final;
    virtual double getMaxRadius() const = 0;
    bool isAlive() const override final;
    bool getIsAbleToBeFollowed() const override final;
    std::array<double,7> getAnimalData() const override final;
    //AUTRES MÉTHODES
    void addAnimal(std::vector<LivingEntity*>&, int) override final;
    void becomeLeader() override final;
    bool canGrow() const;
    //GESTION DE LA REPRODUCTION
    bool matable(LivingEntity const* other) const override final;
    //GESTION DES INFECTIONS
    void isInfectedBy(LivingEntity*) override final;
    void becomeInfected(Virus*) override final;
    //GESTION DES SOINS (FLEURS)
    virtual void isCuring(Sheep* other) override;
    virtual void isCuring(Wolf* other) override final;
    virtual void isCuring(Consommable* other) override final;
    //GESTION DES HALLUCINATIONS (CHAMPIGNONS)
    virtual void poison(Sheep* other) override;
    virtual void poison(Wolf* other) override final;
    virtual void poison(Consommable* other) override final;
    //MÉTHODES DE DESSIN
    void drawOn(sf::RenderTarget& targetWindow) const override final;
    void drawTrack(sf::RenderTarget& targetWindow) const override final;

  private:
    //la taille est représentée par "double energyLevel" dans la classe LivingEntity
    //GESTION DE LA REPRODUCTION
    bool canMate(Wolf const*) const override final;
    bool canMate(Sheep const*) const override final;
    bool canMate(Consommable const*) const override final;
    void meeting(LivingEntity*) override final;
    void meet(Wolf*) override final;
    void meet(Sheep*) override final;
    void meet(Consommable*) override final;
    //GESTION DES INFECTIONS
    void infect(Sheep*) override final;
    void infect(Wolf*) override final;
    void infect(Consommable*) override final;
};

#endif
