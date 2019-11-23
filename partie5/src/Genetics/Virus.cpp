#include "Virus.hpp"
#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include <iostream>
#include <Random/Random.hpp>

//CONSTRUCTEURS
Virus::Virus()
:quantityInfectiousAgents(getAppConfig().virus_initial_quantity),
 growthRate(getAppConfig().virus_growth_rate)
{
  for (auto& virus : virulenceProfile) {
    if (bernoulli(getAppConfig().virus_sparsity)) {
      virus = uniform(-5.0, 5.0);
    } else {
      virus = 0.0;
    }
  }
}

Virus::Virus(Virus const& other)
: quantityInfectiousAgents(0.5 * other.quantityInfectiousAgents),
  growthRate(other.growthRate), virulenceProfile(other.virulenceProfile)
{}

Virus::Virus(std::array<double,10> virProfile)
:quantityInfectiousAgents(getAppConfig().virus_initial_quantity),
 growthRate(getAppConfig().virus_growth_rate), virulenceProfile(virProfile)
{}

//DESTRUCTEUR PARTICULIER?

/**************************************************************************************************************/
//GETTERS
std::array<double,10> Virus::getVirulenceProfile() const {
  return virulenceProfile;
}

double Virus::getQuantityInfectiousAgents() const {
  return quantityInfectiousAgents;
}

//SETTERS
void Virus::setQuantityInfectiousAgents(double qte) {
  quantityInfectiousAgents = qte;
}

//MÉTHODES
void Virus::evolve(sf::Time dt) {
  if (quantityInfectiousAgents < getAppConfig().virus_max) {
    quantityInfectiousAgents *= (1 + growthRate * (1 - quantityInfectiousAgents/getAppConfig().virus_max) * dt.asSeconds());
  }
}

bool Virus::isDead() const {
  return (quantityInfectiousAgents < getAppConfig().virus_initial_quantity);
}

bool Virus::isAbleToInfect() const {
  return (quantityInfectiousAgents > getAppConfig().virus_min_quantity_for_infection);
}

std::ostream& Virus::afficheVirus(std::ostream& out) const {
  out << "Nombre d'agents infectieux : " << to_nice_string(quantityInfectiousAgents) << std::endl << "Profil de virulence : ";
  for (auto nb : virulenceProfile) {
    out << std::endl << to_nice_string(nb);
  }
  return out;
}

/**************************************************************************************************************/
//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& out, const Virus& v) {

  return v.afficheVirus(out);
}
