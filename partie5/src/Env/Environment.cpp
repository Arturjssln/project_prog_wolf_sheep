#include "Environment.hpp"
#include "Animal.hpp"
#include "Application.hpp"
#include <algorithm> //utilisation de remove

Environment::Environment()
: trackedEntity(nullptr)
{}

Environment::~Environment() {
	reset();
}

void Environment::addEntity(LivingEntity* newEntity) {
    FaunaAndFlora.push_back(newEntity);
}

/*Cette méthode n'est plus utile
std::vector<LivingEntity*> Environment::getEntitiesInSightForAnimal(const Animal* animal) const {
    std::vector<LivingEntity*> lesEntitesVisibles;
    for (auto entity : FaunaAndFlora) {
        On regarde pour chaque entité de l'environnement si elle est dans le champ de vision de l'animal
          si oui, on stocke cette entité dans le tableau lesEntitesVisibles
        if (animal->isTargetInSight(entity->getPosition())) lesEntitesVisibles.push_back(entity);
    }
    return lesEntitesVisibles;
}*/

std::list<LivingEntity*> Environment::getEntities() const {
    return FaunaAndFlora;
}


void Environment::update(sf::Time dt) {
		//Mise à jour des entités
    for (auto entity : FaunaAndFlora) {
        entity->update(dt);
    }

		//Suppression des animaux morts
		for (auto& entity : FaunaAndFlora) {
			//On met à nullptr les animaux morts après leur suppression
			if(!(entity->isAlive())) {
				delete entity;
				entity = nullptr;
			}
		}

		//Supprime tous les pointeurs sur les animaux morts de l'environnement
		 FaunaAndFlora.erase(std::remove(FaunaAndFlora.begin(), FaunaAndFlora.end(), nullptr),
		 										FaunaAndFlora.end());
}

void Environment::drawOn(sf::RenderTarget& targetWindow) const {
    //On dessine les entités de la liste
    for (auto entity : FaunaAndFlora) {
        entity->drawOn(targetWindow); //Dessine les LivingEntity
				if (isDebugOn()) {
					entity->drawObstacle(targetWindow);
				}
    }
}

void Environment::reset() {
		herdsLeaders.clear();
    for (auto& entity : FaunaAndFlora) {
        delete entity;
				entity = nullptr;
    }
		stopTrackingAnyEntity();
    FaunaAndFlora.clear();
}

std::unordered_map<std::string, double> Environment::fetchData(std::string libelle, bool active) {
	std::unordered_map<std::string, double> new_data;
	if (active) {
		if (libelle == s::GENERAL) {
			new_data = {{"wolves", LivingEntity::getNbLivingEntities("wolf")},
									{"sheep", LivingEntity::getNbLivingEntities("sheep")},
									{"grass", LivingEntity::getNbLivingEntities("grass")},
									{"infected", ImmuneSystem::getNbInfected()}};
		}

		if (libelle == s::GRASS_INDIVIDUAL and trackedEntity != nullptr) {
			new_data = {{s::ENERGY, trackedEntity->getEnergyLevel()}};
		}


		if (libelle == s::ANIMAL_INDIVIDUAL and trackedEntity != nullptr) {
				std::array<double,6> data(trackedEntity->getAnimalData());
				if (!data.empty()) {
					new_data = {{ s::ENERGY, data[0]},
								 	 		{ s::HEALTH, data[1]},
								 	 		{ s::VIRUS, data[2]},
								 	 		{ s::ADASCORE, data[3]},
								 	 		{ s::IMUNAC, data[4]},
								 	 		{ s::SCORE, data[5]}};
				 }
			}
		}
	//Si le graph n'est pas actif new_data sera vide
	return new_data;
}


LivingEntity* Environment::getLeader(int numHerd) const {
	if(herdsLeaders.count(numHerd) != 0) { //La méthode count retourne 0 si la clé n'existe pas
		return herdsLeaders.at(numHerd);
	}
	return nullptr;
}

//Met à jour le leader : si c'est le premier du troupeau (animal != nullptr) il sera automatiquement leader
//											 sinon l'animal le plus vieux devriendra leader (appel à la fonction de le destructeur de l'animal en question)
void Environment::setNewLeader(int numHerd, LivingEntity* animal) {
	if (animal != nullptr) {
		//Si le mouton est le premier il est automatiquement le leader
		herdsLeaders.insert(std::make_pair(numHerd, animal));
		animal->becomeLeader();
	} else {
		animal = nextLeader(numHerd);
		if (animal != nullptr) {
			herdsLeaders[numHerd] = animal;
			animal->becomeLeader();
		}
	}
}

//Retourne la prochain leader ou nullptr si il n'y a plus d'autres moutons du même troupeau
LivingEntity* Environment::nextLeader(int numHerd) {
	std::vector<LivingEntity*> animals;

		for (auto entity : FaunaAndFlora) {
				if (entity != nullptr) {
					entity->addAnimal(animals, numHerd);
				}
		}
		return theOldest(animals, numHerd);
}

//Méthode qui retourne l'animal le plus vieux d'un groupe
LivingEntity* Environment::theOldest(std::vector<LivingEntity*> animals, int numHerd) {
	size_t indice(0);
	if(animals.empty()) {
		//Supprime la paire si il n'y a plus de moutons ou de loups du troupeau numHerd
		herdsLeaders.erase(numHerd);
		animals.clear();
		return nullptr;
	} else {
		//Le nouveau leader est choisi comme étant le plus vieux (car le plus sage)
		sf::Time maxTpsDeVie(sf::Time::Zero);
		for (size_t i(0); i < animals.size(); ++i) {
			if(maxTpsDeVie <= animals[i]->getLifeTime()) {
				indice = i;
				maxTpsDeVie = animals[i]->getLifeTime();
			}
		}
		animals.clear();
		return animals[indice];
	}
}

void Environment::trackEntity(Vec2d position) {
	for (auto& entity : FaunaAndFlora) {
			if (*entity > position) { //Surcharge opérateur pour isPointInside : class Obstacle
				if (trackedEntity != nullptr) {
					trackedEntity->setTrack(false); //l'ancienne entité traquée si elle existe ne l'est plus
				}
					trackedEntity = entity;
					entity->setTrack(true);
					getApp().focusOnStat(entity->getStatLabel());
			}
	}
}

void Environment::stopTrackingAnyEntity() {
	if (trackedEntity != nullptr) {
		trackedEntity->setTrack(false);
		trackedEntity = nullptr;
	}
	getApp().focusOnStat(s::GENERAL);
}

void Environment::infectEntity(Vec2d p, Virus* v) {
	bool firstInfection(false);
	//Si une entité est infectée, firstInfection prend la valeur true, pour empecher l'infection d'autres entités situées au même endroit
	//On infecte une seule entité à la fois!!
	for(auto& entity : FaunaAndFlora) {
		if (*entity > p and !firstInfection) {
			entity->becomeInfected(v);
			firstInfection = true;
		}
	}
}

void Environment::killEntity(Vec2d position) {
	bool firstInfection(false);
	//Si une entité est tuée, firstInfection prend la valeur true, pour empecher la mort d'autres entités situées au même endroit
	//On tue une seule entité à la fois!!
	for(auto& entity : FaunaAndFlora) {
		if (*entity > position and !firstInfection) {
			entity->setNiveauEnergie(0); //on tue l'animal concerné en lui enlevant toute son énergie
			firstInfection = true;
		}
	}
}
