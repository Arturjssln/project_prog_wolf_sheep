#include <list>
#include <vector>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "LivingEntity.hpp"
#include "Animal.hpp"
#include "Obstacle.hpp"

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Environment : public sf::NonCopyable {
    public:
        //CONSTRUCTEUR
        Environment() = default;
        //DESTRUCTEUR
        ~Environment();
        //MÉTHODES
        void addEntity(LivingEntity* newEntity);
        std::vector<LivingEntity*> getEntitiesInSightForAnimal(const Animal* animal) const;
        void update(sf::Time dt);
        void drawOn(sf::RenderTarget& targetWindow) const;
        void reset();

    private:
        //ATTRIBUTS
        std::list<LivingEntity*> FauneEtFlore;
};

#endif
