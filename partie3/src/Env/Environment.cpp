#include "Environment.hpp"
#include "Animal.hpp"
#include "Application.hpp"
#include <algorithm> //utilisation de remove

Environment::~Environment() {
	reset();
}

void Environment::addEntity(LivingEntity* newEntity) {
    FauneEtFlore.push_back(newEntity);
}

std::vector<LivingEntity*> Environment::getEntitiesInSightForAnimal(const Animal* animal) const {
    std::vector<LivingEntity*> lesEntitesVisibles;
    for (auto entity : FauneEtFlore) {
        /*On regarde pour chaque entité de l'environnement si elle est dans le champ de vision de l'animal
          si oui, on stocke cette entité dans le tableau lesEntitesVisibles*/
        if (animal->isTargetInSight(entity->getPosition())) lesEntitesVisibles.push_back(entity);
    }
    return lesEntitesVisibles;
}

void Environment::update(sf::Time dt) {
    for (auto entity : FauneEtFlore) {
        entity->update(dt);
    }

		for (auto& entity : FauneEtFlore) {
			//Supprime les animaux qui sont morts
			if(!(entity->isAlive())) {
				delete entity;
				entity = nullptr;
			}
		}

		//Supprime tous les pointeurs sur les animaux morts de l'environnement
		 FauneEtFlore.erase(std::remove(FauneEtFlore.begin(), FauneEtFlore.end(), nullptr),
		 										FauneEtFlore.end());
}

void Environment::drawOn(sf::RenderTarget& targetWindow) const {
    //On dessine les entités de la liste
    for (auto entity : FauneEtFlore) {
        entity->drawOn(targetWindow); //Dessine les LivingEntity
				if (isDebugOn()) {
					entity->drawObstacle(targetWindow); //Dessine les obstacles
				}
    }
}

void Environment::reset() {
    for (auto& entity : FauneEtFlore) {
        delete entity;
    }
    FauneEtFlore.clear();
}
