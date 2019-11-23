#include "Environment.hpp"
#include "Animal.hpp"

Environment::~Environment() {
	reset();
}

void Environment::addAnimal(Animal* newAnimal) {
    lesAnimaux.push_back(newAnimal);
}

void Environment::addTarget(const Vec2d& newCible) {
    lesCibles.push_back(newCible);
}

std::vector<Vec2d> Environment::getTargetsInSightForAnimal(const Animal* animal) const {
    std::vector<Vec2d> lesCiblesVisibles;
    for (auto cible : lesCibles) {
        /*On regarde pour chaque cible de l'environnement si elle est dans le champ de vision de l'animal
          si oui, on stocke cette cible dans le tableau lesCiblesVisibles*/
        if (animal->isTargetInSight(cible)) lesCiblesVisibles.push_back(cible);
    }
    return lesCiblesVisibles;
}

void Environment::update(sf::Time dt) {
    //FAIRE EVOLUER LES ANIMAUX DE LA FAUNE ICI
    for (auto animal : lesAnimaux) {
        animal->update(dt);
    }
}

void Environment::drawOn(sf::RenderTarget& targetWindow) const {
		const double rayonCible(5);
    sf::CircleShape cercle(rayonCible); //"Point" représentant la cible
    cercle.setFillColor(sf::Color(255,0,0));

    //On dessine un cercle pour chaque position des cibles de la liste
    for(auto cible : lesCibles) {
        cercle.setPosition(cible);
				cercle.setOrigin(rayonCible,rayonCible);
        targetWindow.draw(cercle);
    }

    //On dessine les animaux de la liste
    for (auto animal : lesAnimaux) {    // (Q2.7)
        animal->drawOn(targetWindow);
    }
}

void Environment::reset() {
    for (auto& animal : lesAnimaux) {
        delete animal;
    }
    lesAnimaux.clear();
	  lesCibles.clear();
}
