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
    Grass(Vec2d position = Vec2d(0,0), double niveauEnergie = getAppConfig().grass_initial_energy);

    //DESTRUCTEUR
    ~Grass();

    //METHODES
    std::string getTexture() const override;
    sf::Time getLongevity() const override;
    double getRadius() const override;
    void drawOn(sf::RenderTarget& targetWindow) const override;
    void update(sf::Time dt) override;
    bool eatable(LivingEntity const* entity) const override;
    bool matable(LivingEntity const* other) const override;
    void addSheep(std::vector<LivingEntity*>& tab, int numTroupeau) override;
    void addWolf(std::vector<LivingEntity*>& tab, int numTroupeau) override;
    void becomeLeader() override;
    bool getIsAbleToBeFollowed() const override;


  private:
    //la taille est représentée par "double niveauEnergie" dans la classe LivingEntity
    void setPosition() override; //le massif d'herbe est entièrement contenu dans l'environnement
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
