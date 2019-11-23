#include "Virus.hpp"
#include <Interface/Updatable.hpp>
#include <SFML/Graphics.hpp>

#ifndef IMMUNE_SYSTEM_H
#define IMMUNE_SYSTEM_H

class Animal;

class ImmuneSystem : public Updatable {
  public :
    //CONSTRUCTEUR
    ImmuneSystem(Animal* host);

    //DESTRUCTEUR
    ~ImmuneSystem();

    //GETTERS
    double getHealthState() const;
    double getAdaScore() const;
    double getGlobalScore() const;
    double getActivationLevel() const;
    double getQuantityVirus() const;
    Virus* getVirus() const;
    static unsigned int getNbInfected();

    //SETTERS
    void setVirus(Virus* v);

    //MÉTHODES
    bool isInfected() const;
    void update(sf::Time dt) override;

  private :
    //ATTRIBUTS
    double healthState;
    std::array<double,10> immuneProfile;
    double activationLevel;
    Animal* host;
    Virus* virus;
    double adaScore;
    double globalScore;
    bool isFighting;

    //MÉTHODES
    void calculAdaScore();
    void calculScore();

    //ATTRIBUT DE CLASSE
    static unsigned int nbInfected;
    //sera décrémenté dans le destructeur ou si l'animal combat l'infection (dans update)
    //sera incrémenté dans la méthode setVirus(Virus*)
};

#endif
