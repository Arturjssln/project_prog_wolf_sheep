#include <array>
#include <SFML/Graphics.hpp>

#ifndef VIRUS_H
#define VIRUS_H

class Virus {
  public :
    //CONSTRUCTEURS
    Virus();
    Virus(Virus const& other);
    Virus(std::array<double,10> virProfile);

    //DESTRUCTEUR
    ~Virus() = default;

    //GETTERS
    std::array<double,10> getVirulenceProfile() const;
    double getQuantityInfectiousAgents() const;

    //SETTERS
    void setQuantityInfectiousAgents(double qte);

    //MÉTHODES
    void evolve(sf::Time dt);
    bool isDead() const;
    bool isAbleToInfect() const;
    std::ostream& afficheVirus(std::ostream& out) const;

  private:
    //ATTRIBUTS
    std::array<double,10> virulenceProfile;
    double quantityInfectiousAgents;
    double growthRate;

};

std::ostream& operator<<(std::ostream& out, const Virus& v);

#endif
