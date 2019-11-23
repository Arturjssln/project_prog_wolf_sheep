#include "Sheep.hpp"
#include <Utility/Constants.hpp>
#include <vector>
#include <Random/Random.hpp>

//CONSTRUCTEUR
Sheep::Sheep(Vec2d position, int numTroupeau, double energyLevel, double magnitude,
						 Genome* pere, Genome* mere, double quenchLevel)
: Animal(position, energyLevel, magnitude, pere, mere, numTroupeau, quenchLevel), poisonedTime(sf::Time::Zero)
{
	++nbLivingEntities[s::SHEEP];
	if(getAppEnv().getLeader(numHerd) == nullptr) {
    getAppEnv().setNewLeader(numHerd, this);
    isAbleToBeFollowed = true;
	}
}

//DESTRUCTEUR
Sheep::~Sheep()
{
  --nbLivingEntities[s::SHEEP];
  if(leader) {
    getAppEnv().setNewLeader(numHerd);
  }
}

/**************************************************************************************************************/
//GETTERS
double Sheep::getStandardMaxSpeed() const {
	return getAppConfig().sheep_max_speed;
}

double Sheep::getMass() const {
	return getAppConfig().sheep_mass;
}

double Sheep::getRadius() const {
	return getAppConfig().sheep_size / 2.0;
}

double Sheep::getViewRange() const {
	return getAppConfig().sheep_view_range;
}

double Sheep::getViewDistance() const {
	return getAppConfig().sheep_view_distance;
}

double Sheep::getRandomWalkRadius() const {
  return getAppConfig().sheep_random_walk_radius;
}

double Sheep::getRandomWalkDistance() const {
  return getAppConfig().sheep_random_walk_distance;
}

double Sheep::getRandomWalkJitter() const {
  return getAppConfig().sheep_random_walk_jitter;
}

std::string Sheep::getTexture() const {
	if(genome.getColorPhenotype() == BLACK) {
		return getAppConfig().sheep_texture_black;
	} else {
		return getAppConfig().sheep_texture_white;
	}
}
sf::Time Sheep::getLongevity() const {
	return getAppConfig().sheep_longevity;
}

double Sheep::getEnergyLossFactor() const {
	return getAppConfig().sheep_energy_loss_factor;
}

double Sheep::getWaterLossFactor() const {
	return getAppConfig().sheep_water_loss_factor;
}

double Sheep::getMinEnergyMating() const {
	if (isFemale()) return getAppConfig().sheep_energy_min_mating_female;
	return getAppConfig().sheep_energy_min_mating_male;
}

double Sheep::getMinQuenchMating() const {
	if (isFemale()) return getAppConfig().sheep_quench_min_mating_female;
	return getAppConfig().sheep_quench_min_mating_male;
}

int Sheep::getMinChildren() const {
	return getAppConfig().sheep_reproduction_min_children;
}

int Sheep::getMaxChildren() const {
	return getAppConfig().sheep_reproduction_max_children;
}

double Sheep::getEnergyLossMatingMale() const {
	return getAppConfig().sheep_energy_loss_mating_male;
}

double Sheep::getEneryLossPerChildFemale() const {
	return getAppConfig().sheep_energy_loss_female_per_child;
}

sf::Time Sheep::getGestationTime() const {
	return sf::seconds(getAppConfig().sheep_reproduction_gestation_time);
}

bool Sheep::isLeader() const {
  return leader;
}

/**************************************************************************************************************/
//AUTRES MÉTHODES
//GESTION DE LA NOURRITURE
bool Sheep::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Sheep::eatableBy(Wolf const*) const {
	return true;
}

bool Sheep::eatableBy(Sheep const*) const {
	return false;
}

bool Sheep::eatableBy(Grass const*) const {
	return false;
}

bool Sheep::eatableBy(Water const*) const {
	return false;
}

void Sheep::isEating(LivingEntity* entity) {
	energyLevel += getAppConfig().animal_meal_retention * entity->getEnergyLevel();
	entity->setEnergyLevel(entity->getEnergyLevel() - getAppConfig().sheep_energy_bite);

	//Dans le cas où le mouton mange une fleur et qu'il est infecté, cela le soignera un peu
	if (immunity.isInfected() and (immunity.getHealthState() < getAppConfig().immune_health_max)) {
		entity->isCuring(this);
	}

	//Empoisonne l'entité si l'entité mangée est une baie empoisonée
	entity->poison(this);

	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_or_drink_time);
}

//GESTION DE LA BOISSON
bool Sheep::drinkable(LivingEntity const* entity) const {
	return entity->drinkableBy(this);
}

bool Sheep::drinkableBy(Wolf const*) const {
	return false;
}

bool Sheep::drinkableBy(Sheep const*) const {
	return false;
}

bool Sheep::drinkableBy(Grass const*) const {
	return false;
}

bool Sheep::drinkableBy(Water const*) const {
	return false;
}

void Sheep::isDrinking(LivingEntity* entity) {
	quenchLevel += getAppConfig().animal_drink_retention * entity->getEnergyLevel();
	entity->setEnergyLevel(entity->getEnergyLevel() - getAppConfig().sheep_drink_swallow);
	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_or_drink_time);
}

//GESTION DE LA REPRODUCTION
bool Sheep::matable(LivingEntity const* other) const  {
  return other->canMate(this);
}

bool Sheep::canMate(Wolf const*) const {
	return false;
}

bool Sheep::canMate(Sheep const* sheep) const {
	if (energyLevel < getMinEnergyMating() or
			quenchLevel < getMinQuenchMating() or
			!oppositeSex(sheep) or
			sheep->isPregnant() or isPregnant() or
			!sheep->isTargetInSight(getPosition()) or //can mate seulement si les deux partenaires sont dans le champ de vision de l'autre !
			lifeTime < getAppConfig().animal_time_before_mating or
			breakTimeAfterDoingSomething > sf::Time::Zero)
	{
		return false;
	}
	return true;
}

bool Sheep::canMate(Consommable const*) const {
	return false;
}

void Sheep::giveBirth() {
	Genome* genomeMere(new Genome(getGenome()));
  for (size_t i(0); i < numberOfChild; ++i) {
		getAppEnv().addEntity(new Sheep(getPosition(), numHerd, getAppConfig().sheep_energy_initial, 0, pere, genomeMere));
	}
	numberOfChild = 0;
	pregnant = false;
	delete pere;
	delete genomeMere;
	pere = nullptr;
	genomeMere = nullptr;
}

void Sheep::meeting(LivingEntity* other) {
  other->meet(this);
}

void Sheep::meet(Sheep* other) {
    if(isFemale()) {
      pregnant = true;
      setNumberChildren();
      energyLevel -= numberOfChild * getEneryLossPerChildFemale();
      gestationTime = getGestationTime();
      pere = new Genome(other->getGenome());
    } else {
      energyLevel -= getEnergyLossMatingMale();
    }
    state = MATING;
    breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_mating_time);
}

void Sheep::meet(Consommable*) {
	//Ne fait rien
}

void Sheep::meet(Wolf*) {
	//Ne fait rien
}

//GESTION DU DEPLACEMENT EN TROUPEAU
bool Sheep::isFree() const {
	return (leader or (getAppEnv().getLeader(numHerd) == nullptr));
}

void Sheep::addAnimal(std::vector<LivingEntity*>& animals, int numTroupeau) {
	//Les moutons qui seront ajoutés dans le tableau serviront à regarder qui est le plus vieux après le leader,
  //on n'ajoute donc pas le leader à ce tableau
  if(numTroupeau == numHerd and !leader){
		animals.push_back(this);
  }
}

void Sheep::becomeLeader() {
	leader = true;
}

int Sheep::getNumHerd() const {
  return numHerd;
}

void Sheep::setIsAbleToBeFollowed(bool b) {
  isAbleToBeFollowed = b;
}

bool Sheep::getIsAbleToBeFollowed() const {
  return isAbleToBeFollowed;
}

//GESTION DES INFECTIONS
void Sheep::isInfectedBy(LivingEntity* other) {
	other->infect(this);
}

void Sheep::isCuring(Sheep*) {
	//Ne fait rien
}

void Sheep::isCuring(Wolf*) {
	//Ne fait rien, le fait qu'un loup mange un mouton ne soigne pas le loup
}

void Sheep::isCuring(Consommable*) {
	//Ne fait rien
}

void Sheep::isBeingCured(Flower*) {
	immunity.increaseHealthState(getAppConfig().flower_cure_power);
}


void Sheep::infect(Consommable*) {
  //Ne fait rien
}

void Sheep::infect(Wolf*)  {
  //Ne fait rien
}

void Sheep::infect(Sheep* other) {
	if(canInfect(other)) other->becomeInfected(new Virus(*immunity.getVirus()));
}

bool Sheep::canInfect(Sheep* other) {
	//On vérifie que le mouton est infecté par un virus et que celui ci est capable d'infecter un autre mouton,
	//et que le mouton other n'est pas lui même (en comparant les positions) !
	//On ajoute aussi une probabilité d'infecter l'autre entité
	return (immunity.isInfected() and
					position != other->getPosition() and
					immunity.getQuantityVirus() >= getAppConfig().virus_min_quantity_for_infection and
				 	bernoulli(getAppConfig().virus_infection_probability));
}

//GESTION DES HALLUCINATIONS (CHAMPIGNONS)
void Sheep::poison(Sheep*) {
	//Ne fait rien
}

void Sheep::poison(Wolf*) {
	//Ne fait rien
}

void Sheep::poison(Consommable*) {
	//Ne fait rien
}

void Sheep::isBeingPoisoned(MagicMushroom*) {
	poisonedTime = getAppConfig().poisoned_time;
}

bool Sheep::isPoisoned() const {
	return poisonedTime > sf::Time::Zero;
}

void Sheep::decreasePoisonedTime(sf::Time dt) {
	poisonedTime -= dt;
}

/**************************************************************************************************************/
//MÉTHODES DE DESSIN
//Représentation de la couronne si le mouton est leader
void Sheep::drawCrown(sf::RenderTarget& targetWindow) const {
	if(leader) {
  	auto crownSprite = buildSprite(position + direction * getRadius() , getRadius() * 0.7, getAppTexture(getAppConfig().animal_texture_leader));
  	crownSprite.setRotation((getRotation())/ DEG_TO_RAD + 90);
  	targetWindow.draw(crownSprite);
	}
}

void Sheep::drawInfoHerd(sf::RenderTarget& targetWindow) const {
	std::string message(to_nice_string(numHerd));
	Vec2d positionTexte(direction * 15 - direction.normal() * getRadius() * 3);
	if(leader) {
		message += " Leader";
	} else {
		message += " Follower";
	}
	auto text = buildText(message, ConvertToGlobalCoord(positionTexte), getAppFont(),
												getAppConfig().default_debug_text_size, sf::Color::White);
	text.setRotation(getRotation() / DEG_TO_RAD + 90);
	targetWindow.draw(text);
}

/**************************************************************************************************************/
//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& sortie, const Sheep& s) {
	sortie << "Mouton appartenant au troupeau : " << to_nice_string(s.getNumHerd());
	if(s.isLeader()) sortie << " dont il est le leader";
	sortie << std::endl << "Niveau d'énergie : " << to_nice_string(s.getEnergyLevel()) << std::endl;
	sortie <<  "Temps de vie : " << to_nice_string(s.getLifeTime().asSeconds()) << std::endl;
	sortie << s.getGenome();
	return sortie;
}
