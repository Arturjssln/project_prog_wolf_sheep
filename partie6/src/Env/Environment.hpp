#include <list>
#include <vector>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "LivingEntity.hpp"
#include "Animal.hpp"
#include "Obstacle.hpp"

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

enum Season {SPRING, SUMMER, AUTUMN, WINTER};

class Environment : public sf::NonCopyable {
    public:
        //CONSTRUCTEUR & DESTRUCTEUR
        Environment();
        ~Environment();

        //GESTION DES ENTITÉS
        void addEntity(LivingEntity* newEntity);
        std::list<LivingEntity*> getEntities() const;
        void update(sf::Time dt);
        void drawOn(sf::RenderTarget& targetWindow) const;
        void reset();
        //LEADERS
        LivingEntity* getLeader(int numHerd) const;
        void setNewLeader(int numHerd, LivingEntity* animal = nullptr);
        //GRAPHS
        std::unordered_map<std::string, double> fetchData(std::string libelle, bool active);
        void trackEntity(Vec2d position);
        void stopTrackingAnyEntity();
        //INFECTION
        void infectEntity(Vec2d p, Virus* v = new Virus());
        void killEntity(Vec2d position);
        //INCENDIE
        void fire(sf::RenderTarget& targetWindow) const;
        std::string getTextureFlamme() const;
        bool fireOff() const;
        //SAISONS
        Season getSeason() const;
        void updateSeason(sf::Time dt);
        std::string getTexture() const;
        void drawSeason(sf::RenderTarget& targetWindow) const;

    private:
        //ATTRIBUTS
        std::list<LivingEntity*> FaunaAndFlora;
        /*Map qui associe le leader au numéro de troupeau :
          les entiers positifs sont réservés aux moutons (0 compris),
          les entiers négatifs sont réservés aux loups*/
        std::unordered_map<int, LivingEntity*> herdsLeaders;
        LivingEntity* trackedEntity;
        sf::Time fireTime;
        sf::Time seasonTime;
        unsigned int nbSeason;

        //MÉTHODES
        LivingEntity* nextLeader(int numHerd);
        LivingEntity* theOldest(std::vector<LivingEntity*> animals, int numHerd);
        void addRandomConsommable();
        bool canBePut(LivingEntity* entity, double radius) const;
};

#endif
