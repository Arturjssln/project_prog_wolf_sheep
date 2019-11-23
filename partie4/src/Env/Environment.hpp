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
        static std::unordered_map<std::string, double> fetchData(std::string libelle);
        LivingEntity* getLeader(int numHerd) const;
        void setNewLeaderSheep(int numHerd, LivingEntity* sheep = nullptr);
        void setNewLeaderWolf(int numHerd, LivingEntity* sheep = nullptr);
        LivingEntity* nextLeader(int numHerd);
    private:
        //ATTRIBUTS
        std::list<LivingEntity*> FauneEtFlore;
        //map qui associe le leader au numéro de troupeau
        std::unordered_map<int, LivingEntity*> herdsLeaders;
};

#endif
