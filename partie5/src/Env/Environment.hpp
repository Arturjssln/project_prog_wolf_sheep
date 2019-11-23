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
        Environment();
        //DESTRUCTEUR
        ~Environment();
        //MÉTHODES
        void addEntity(LivingEntity* newEntity);

        //std::vector<LivingEntity*> getEntitiesInSightForAnimal(const Animal* animal) const;
        //On a enlevé cette méthode suite à la partie 5.3 : afin d'éviter des itérations multiples pour vérifier
        //les ennemis, partenaires, nourriture, et si une infection est possible, nous avons déplacer les vérifications
        //de la présence d'une entité dans le champ de vision dans la méthode updateClosest dans Animal
        std::list<LivingEntity*> getEntities() const;
        void update(sf::Time dt);
        void drawOn(sf::RenderTarget& targetWindow) const;
        void reset();
        std::unordered_map<std::string, double> fetchData(std::string libelle, bool active);
        LivingEntity* getLeader(int numHerd) const;
        void setNewLeader(int numHerd, LivingEntity* animal = nullptr);
        void trackEntity(Vec2d position);
        void stopTrackingAnyEntity();
        void infectEntity(Vec2d p, Virus* v = new Virus());
        void killEntity(Vec2d position);

    private:
        //ATTRIBUTS
        std::list<LivingEntity*> FaunaAndFlora;
        /*Map qui associe le leader au numéro de troupeau :
          les entiers positifs sont réservés aux moutons (0 compris),
          les entiers négatifs sont réservés aux loups*/
        std::unordered_map<int, LivingEntity*> herdsLeaders;
        LivingEntity* trackedEntity;

        //MÉTHODE
        LivingEntity* nextLeader(int numHerd);
        LivingEntity* theOldest(std::vector<LivingEntity*> animals, int numHerd);
};

#endif
