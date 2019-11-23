// A COMPLETER LORS DES ETAPES FUTURES
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <list>
#include <vector>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>

class Animal;

class Environment : public sf::NonCopyable {
    public:
        //CONSTRUCTEUR
        Environment() = default;
        //DESTRUCTEUR
        ~Environment();
        //MÉTHODES
        void addAnimal(Animal* new_animal);
        void addTarget(const Vec2d& new_cible);
        std::vector<Vec2d> getTargetsInSightForAnimal(const Animal* animal) const;
        void update(sf::Time dt);
        void drawOn(sf::RenderTarget& targetWindow) const;
        void reset();

    private:
        //ATTRIBUTS
        std::list<Animal*> lesAnimaux;
        std::list<Vec2d> lesCibles;
};

#endif
