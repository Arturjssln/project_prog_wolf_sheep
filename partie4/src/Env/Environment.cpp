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
		herdsLeaders.clear();
    for (auto& entity : FauneEtFlore) {
        delete entity;
				entity = nullptr;
    }
    FauneEtFlore.clear();

}

std::unordered_map<std::string, double> Environment::fetchData(std::string libelle) {
	std::unordered_map<std::string, double> new_data;
	if(libelle == s::GENERAL) {
		new_data = {{"wolves", LivingEntity::getNbLivingEntities("wolf")},
								{"sheep", LivingEntity::getNbLivingEntities("sheep")},
								{"grass", LivingEntity::getNbLivingEntities("grass")}};
	}
	return new_data;
}


LivingEntity* Environment::getLeader(int numHerd) const {
	if(herdsLeaders.count(numHerd) != 0) { //La méthode count retourne 0 si la clé n'existe pas
		return herdsLeaders.at(numHerd);
	}
	return nullptr;
}

//Met à jour le leader : si c'est le premier du troupeau (sheep != nullptr) il sera automatiquement leader
//											 sinon le mouton le plus vieux devriendra leader (appel à la fonction de le destructeur de sheep)
void Environment::setNewLeaderSheep(int numHerd, LivingEntity* sheep) {
	if (sheep != nullptr) {
		//Si le mouton est le premier il est automatiquement le leader
		herdsLeaders.insert(std::make_pair(numHerd, sheep));
		sheep->becomeLeader();
	} else {
		sheep = nextLeader(numHerd);
		if (sheep != nullptr) {
			herdsLeaders[numHerd] = sheep;
			sheep->becomeLeader();
		}
	}
}

//Met à jour le leader : si c'est le premier du troupeau (wolf != nullptr) il sera automatiquement leader
//											 sinon le loup le plus vieux devriendra leader (appel à la fonction de le destructeur de wolf)
void Environment::setNewLeaderWolf(int numHerd, LivingEntity* wolf) {
	if (wolf != nullptr) {
		//Si le loup est le premier il est automatiquement le leader
		herdsLeaders.insert(std::make_pair(numHerd, wolf));
		wolf->becomeLeader();
	} else {
		wolf = nextLeader(numHerd);
		if (wolf != nullptr) {
			herdsLeaders[numHerd] = wolf;
			wolf->becomeLeader();
		}
	}
}

//Retourne la prochain leader ou nullptr si il n'y a plus d'autres moutons du même troupeau
LivingEntity* Environment::nextLeader(int numHerd) {
	std::vector<LivingEntity*> sheep;

		for (auto entity : FauneEtFlore) {
				if (entity != nullptr) {
					entity->addSheep(sheep, numHerd);
				}
		}

		size_t indice(0);
		if(sheep.empty()) {
			//Supprime la paire si il n'y a plus de moutons du troupeau numHerd
			herdsLeaders.erase(numHerd);
			return nullptr;
		} else {
			//Le nouveau leader est choisi comme étant le plus vieux (car le plus sage)
			sf::Time maxTpsDeVie(sf::Time::Zero);
			for (size_t i(0); i < sheep.size(); ++i) {
				if(maxTpsDeVie <= sheep[i]->getTempsDeVie()) {
					indice = i;
					maxTpsDeVie = sheep[i]->getTempsDeVie();
				}
			}
			return sheep[indice];
		}
		sheep.clear();
}
