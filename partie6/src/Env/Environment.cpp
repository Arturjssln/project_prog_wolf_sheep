#include "Environment.hpp"
#include "Animal.hpp"
#include "Application.hpp"
#include <Random/Random.hpp>
#include <algorithm> //utilisation de remove
#include "Flower.hpp"
#include "Grass.hpp"
#include "MagicMushroom.hpp"
#include "Water.hpp"

//CONSTRUCTEUR & DESTRUCTEUR
Environment::Environment()
: trackedEntity(nullptr), fireTime(sf::Time::Zero), seasonTime(sf::Time::Zero), nbSeason(0)
{}

Environment::~Environment() {
	reset();
}
/**************************************************************************************************************/
//GESTION DES ENTITÉS
void Environment::addEntity(LivingEntity* newEntity) {
    FaunaAndFlora.push_back(newEntity);
}

std::list<LivingEntity*> Environment::getEntities() const {
    return FaunaAndFlora;
}

void Environment::update(sf::Time dt) {

		updateSeason(dt);

		if(fireTime > sf::Time::Zero) {
			fireTime -= dt;
		} else {
			if (getAppConfig().getAutomaticGenerator()) {
				//Ajoute une entité (seulement avec une certaine probabilité) au hasard en fonction de la saison
				if(bernoulli(getAppConfig().consommable_apparition_probability)) addRandomConsommable();
			}

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
}

//MÉTHODE DE DESSIN
void Environment::drawOn(sf::RenderTarget& targetWindow) const {
    for (auto entity : FaunaAndFlora) {
        entity->drawOn(targetWindow); //Dessine les LivingEntities
				if (isDebugOn()) {
					entity->drawObstacle(targetWindow);
				}
    }
		if (!fireOff()) {
			fire(targetWindow); //dessin de l'incendie
		}

		drawSeason(targetWindow); //Dessine le sigle de la saison (en haut à droite de la fenêtre)
}

void Environment::reset() {
		herdsLeaders.clear();
    for (auto& entity : FaunaAndFlora) {
        delete entity;
				entity = nullptr;
    }
		stopTrackingAnyEntity();
    FaunaAndFlora.clear();
		fireTime = getAppConfig().fire_time; //un incendie se produit lors du reset
}

/**************************************************************************************************************/
//LEADERS
LivingEntity* Environment::getLeader(int numHerd) const {
	if(herdsLeaders.count(numHerd) != 0) { //La méthode count retourne 0 si la clé n'existe pas
		return herdsLeaders.at(numHerd);
	}
	return nullptr;
}

//Met à jour le leader : si c'est le premier du troupeau (animal != nullptr) il sera automatiquement leader
//sinon l'animal le plus vieux deviendra leader (appel à la fonction dans le destructeur de l'animal en question)
void Environment::setNewLeader(int numHerd, LivingEntity* animal) {
	if (animal != nullptr) {
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

/**************************************************************************************************************/
//GRAPHS
std::unordered_map<std::string, double> Environment::fetchData(std::string libelle, bool active) {
	std::unordered_map<std::string, double> new_data;
	if (active) {
		if (libelle == s::GENERAL) {
			new_data = {{s::WOLVES, LivingEntity::getNbLivingEntities(s::WOLVES)},
									{s::SHEEP, LivingEntity::getNbLivingEntities(s::SHEEP)},
									{s::GRASS, LivingEntity::getNbLivingEntities(s::GRASS)},
									{s::WATER, LivingEntity::getNbLivingEntities(s::WATER)},
									{s::INFECTED, ImmuneSystem::getNbInfected()}};
		}

		if (libelle == s::GRASS_INDIVIDUAL and trackedEntity != nullptr) {
			new_data = {{s::ENERGY, trackedEntity->getEnergyLevel()}};
		}

		if (libelle == s::WATER_INDIVIDUAL and trackedEntity != nullptr) {
			new_data = {{s::ENERGY, trackedEntity->getEnergyLevel()}};
		}

		if (libelle == s::ANIMAL_INDIVIDUAL and trackedEntity != nullptr) {
				std::array<double,7> data(trackedEntity->getAnimalData());
				if (!data.empty()) {
					new_data = {{ s::ENERGY, data[0]},
											{ s::QUENCH, data[1]},
								 	 		{ s::HEALTH, data[2]},
								 	 		{ s::VIRUS, data[3]},
								 	 		{ s::ADASCORE, data[4]},
								 	 		{ s::IMUNAC, data[5]},
								 	 		{ s::SCORE, data[6]}};
				 }
			}
		}
	//Si le graph n'est pas actif new_data sera vide
	return new_data;
}

void Environment::trackEntity(Vec2d position) {
	for (auto& entity : FaunaAndFlora) {
			if (*entity > position) { //Surcharge opérateur pour isPointInside : class Obstacle
				if (trackedEntity != nullptr) {
					trackedEntity->setTrack(false); //l'ancienne entité traquée, si elle existe, ne l'est plus
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

/**************************************************************************************************************/
//INFECTION
void Environment::infectEntity(Vec2d p, Virus* v) {
	bool firstInfection(false);
	//Si une entité est infectée, firstInfection prend la valeur true
	//pour empêcher l'infection d'autres entités situées au même endroit
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
	//Si une entité est tuée, firstInfection prend la valeur true,
	//pour empêcher la mort d'autres entités situées au même endroit
	//On tue une seule entité à la fois!!
	for(auto& entity : FaunaAndFlora) {
		if (*entity > position and !firstInfection) {
			entity->setEnergyLevel(0); //on tue l'animal concerné en lui enlevant toute son énergie
			firstInfection = true;
		}
	}
}

/**************************************************************************************************************/
//INCENDIE
void Environment::fire(sf::RenderTarget& targetWindow) const {
	sf::RectangleShape rectangle(Vec2d(getAppConfig().simulation_world_size,getAppConfig().simulation_world_size));
	rectangle.setFillColor(sf::Color(200,0,0,85));
	targetWindow.draw(rectangle);

	for (int j(50); j < (getAppConfig().simulation_world_size); j += 150) {
		if(bernoulli(1)) {
			auto fire1 = buildSprite(Vec2d(j,getAppConfig().simulation_world_size-90), 300, getAppTexture(getTextureFlamme()));
			auto fire2 = buildSprite(Vec2d(j,90), 300, getAppTexture(getTextureFlamme()));
			fire2.setRotation(180);
			targetWindow.draw(fire1);
			targetWindow.draw(fire2);
		}
	}
}

std::string Environment::getTextureFlamme() const {
	int flammeNumber(uniform(0,2));
	switch (flammeNumber) {
		case 0 : return getAppConfig().fire_texture_1;
			break;
		case 1 : return getAppConfig().fire_texture_2;
			break;
		case 2 : return getAppConfig().fire_texture_3;
			break;
		default : return getAppConfig().fire_texture_1;
	}
}

bool Environment::fireOff() const {
	return fireTime <= sf::Time::Zero;
}

/**************************************************************************************************************/
//SAISONS
void Environment::updateSeason(sf::Time dt) {
	seasonTime += dt;
	if(seasonTime > getAppConfig().season_time_max) {
		++nbSeason;
		seasonTime = sf::Time::Zero;
		getApp().updateBackground();
	}
}

Season Environment::getSeason() const {
	switch (nbSeason%4) {
		case 0 : return SPRING;
			break;
		case 1 : return SUMMER;
			break;
		case 2 : return AUTUMN;
			break;
		case 3 : return WINTER;
			break;
		default : return SUMMER;
	}
}

std::string Environment::getTexture() const {
	switch (getSeason()) {
		case SPRING : return getAppConfig().texture_spring;
			break;
		case SUMMER : return getAppConfig().texture_summer;
			break;
		case AUTUMN : return getAppConfig().texture_autumn;
			break;
		case WINTER : return getAppConfig().texture_winter;
			break;
		default : return "unknown";
	}
}

void Environment::drawSeason(sf::RenderTarget& targetWindow) const {
	std::string icon;
	switch (getSeason()) {
		case SPRING : icon = getAppConfig().texture_icon_spring;
			break;
		case SUMMER : icon = getAppConfig().texture_icon_summer;
			break;
		case AUTUMN : icon = getAppConfig().texture_icon_autumn;
			break;
		case WINTER : icon = getAppConfig().texture_icon_winter;
			break;
		default : icon = "unknow";
	}
	auto seasonSprite = buildSprite(Vec2d(getAppConfig().simulation_world_size - 90, 90), 100, getAppTexture(icon));
	targetWindow.draw(seasonSprite);
}

/**************************************************************************************************************/
//GÉNÉRATEUR ALÉATOIRE D'ÉLÉMENTS CONSOMMABLES
void Environment::addRandomConsommable() {
	Vec2d randomPos;

//Créé une position au hasard qui n'est pas en collision avec le logo en haut à droite
 do {
	 randomPos = Vec2d(uniform(0, getAppConfig().simulation_world_size), uniform(0, getAppConfig().simulation_world_size));
 } while ((randomPos.x > getAppConfig().simulation_world_size - 135) and (randomPos.y < 135));

//Création des pointeurs sur les entités
 Grass* g(nullptr);
 Grass* g1(nullptr);
 Flower* f(nullptr);
 MagicMushroom* m(nullptr);
 MagicMushroom* m1(nullptr);
 Water* w(nullptr);
	switch (getSeason()) {
		 //Crée des massifs d'herbe ou des fleurs au printemps
		case SPRING:
			switch (uniform(0,2)) {
				case 0:
					g = new Grass(randomPos);
					if (canBePut(g, g->getMaxRadius())) { addEntity(g); } else { delete g;}
					break;
				case 1:
					f = new Flower(randomPos);
					if (canBePut(f, f->getMaxRadius())) { addEntity(f); } else { delete f;}
					break;
				case 2:
					w = new Water(randomPos);
					if (canBePut(w, w->getMaxRadius())) { addEntity(w); } else { delete w;}
					break;
				default:
					g1 = new Grass(randomPos);
					if (canBePut(g1, g1->getMaxRadius())) { addEntity(g1); } else { delete g1;}
			}
			break;
		case AUTUMN:
			//Crée des flaques d'eau ou des champignons au printemps
			switch (uniform(0,1)) {
				case 0:
					m = new MagicMushroom(randomPos);
					if (canBePut(m, m->getMaxRadius())) { addEntity(m);} else { delete m;}
					break;
				case 1:
					w = new Water(randomPos);
					if (canBePut(w, w->getMaxRadius())) { addEntity(w); } else { delete w;}
					break;
				default:
					m1 = new MagicMushroom(randomPos);
					if (canBePut(m1, m1->getMaxRadius())) { addEntity(m1);} else { delete m1;}
				}
			break;
		default:; //ne crée rien durant les autres saisons
	}
}

bool Environment::canBePut(LivingEntity* entity, double radius) const {
	for (auto entite : FaunaAndFlora) {
		if (entity->isColliding(*entite, radius)) return false;
	}
	return true;
}
