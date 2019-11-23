#include "Wolf.hpp"
#include <Utility/Constants.hpp>
#include <vector>
#include <Random/Random.hpp>


//CONSTRUCTEUR
Wolf::Wolf(Vec2d position, int numTroupeau, double energyLevel, double magnitude , Genome* pere, Genome* mere, double quenchLevel)
: Animal(position, energyLevel, magnitude, pere, mere, numTroupeau, quenchLevel)
{
	++nbLivingEntities[s::WOLVES];
  if(getAppEnv().getLeader(numHerd) == nullptr) {
		getAppEnv().setNewLeader(numHerd, this);
    isAbleToBeFollowed = true;
	}
}

//DESTRUCTEUR
Wolf::~Wolf()
{
	--nbLivingEntities[s::WOLVES];
	if(leader) {
    getAppEnv().setNewLeader(numHerd);
  }
}

/**************************************************************************************************************/
//GETTERS
double Wolf::getStandardMaxSpeed() const {
	return getAppConfig().wolf_max_speed;
}

double Wolf::getMass() const {
	return getAppConfig().wolf_mass;
}

double Wolf::getRadius() const {
	return getAppConfig().wolf_size / 2.0;
}

double Wolf::getViewRange() const {
	return getAppConfig().wolf_view_range;
}

double Wolf::getViewDistance() const {
	return getAppConfig().wolf_view_distance;
}

double Wolf::getRandomWalkRadius() const {
  return getAppConfig().wolf_random_walk_radius;
}

double Wolf::getRandomWalkDistance() const {
  return getAppConfig().wolf_random_walk_distance;
}

double Wolf::getRandomWalkJitter() const {
  return getAppConfig().wolf_random_walk_jitter;
}

std::string Wolf::getTexture() const {
	return getAppConfig().wolf_texture;
}

sf::Time Wolf::getLongevity() const {
	return getAppConfig().wolf_longevity;
}

double Wolf::getEnergyLossFactor() const {
	return getAppConfig().wolf_energy_loss_factor;
}

double Wolf::getWaterLossFactor() const {
	return getAppConfig().wolf_water_loss_factor;
}

double Wolf::getMinEnergyMating() const {
	if (isFemale()) return getAppConfig().wolf_quench_min_mating_female;
	return getAppConfig().wolf_quench_min_mating_male;
}

double Wolf::getMinQuenchMating() const {
	if (isFemale()) return getAppConfig().wolf_energy_min_mating_female;
	return getAppConfig().wolf_energy_min_mating_male;
}


int Wolf::getMinChildren() const {
	return getAppConfig().wolf_reproduction_min_children;
}

int Wolf::getMaxChildren() const {
	return getAppConfig().wolf_reproduction_max_children;
}

double Wolf::getEnergyLossMatingMale() const {
	return getAppConfig().wolf_energy_loss_mating_male;
}

double Wolf::getEneryLossPerChildFemale() const {
	return getAppConfig().wolf_energy_loss_female_per_child;
}

sf::Time Wolf::getGestationTime() const {
	return sf::seconds(getAppConfig().wolf_reproduction_gestation_time);
}

bool Wolf::isLeader() const {
	return leader;
}

int Wolf::getNumHerd() const {
	return numHerd;
}

bool Wolf::getIsAbleToBeFollowed() const {
	return isAbleToBeFollowed;

}

/**************************************************************************************************************/
//AUTRES MÉTHODES
//GESTION DE LA NOURRITURE
bool Wolf::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Wolf::eatableBy(Wolf const*) const {
	return false;
}

bool Wolf::eatableBy(Sheep const*) const {
	return false;
}

bool Wolf::eatableBy(Grass const*) const {
	return false;
}

bool Wolf::eatableBy(Water const*) const {
	return false;
}

void Wolf::isEating(LivingEntity* entity) {
	energyLevel += getAppConfig().animal_meal_retention * entity->getEnergyLevel();
	entity->setEnergyLevel(0); //le niveau d'énergie de l'entité mangée est 0
	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_or_drink_time);
}

//GESTION DE LA BOISSON
bool Wolf::drinkable(LivingEntity const* entity) const {
	return entity->drinkableBy(this);
}

bool Wolf::drinkableBy(Wolf const*) const {
	return false;
}

bool Wolf::drinkableBy(Sheep const*) const {
	return false;
}

bool Wolf::drinkableBy(Grass const*) const {
	return false;
}

bool Wolf::drinkableBy(Water const*) const {
	return false;
}

void Wolf::isDrinking(LivingEntity* entity) {
	quenchLevel += getAppConfig().animal_drink_retention * entity->getEnergyLevel();
	entity->setEnergyLevel(entity->getEnergyLevel() - getAppConfig().wolf_drink_swallow);
	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_or_drink_time);
}

//GESTION DE LA REPRODUCTION
bool Wolf::matable(LivingEntity const* other) const  {
	return other->canMate(this);
}

bool Wolf::canMate(Wolf const* wolf) const {
	if (energyLevel < getMinEnergyMating() or
			quenchLevel < getMinQuenchMating() or
			!oppositeSex(wolf) or
			wolf->isPregnant() or isPregnant() or
			!wolf->isTargetInSight(getPosition()) or //can mate seulement si les deux partenaires sont dans le champ de vision de l'autre !
			lifeTime < getAppConfig().animal_time_before_mating or
			breakTimeAfterDoingSomething > sf::Time::Zero)
  {
		return false;
	}
	return true;
}

bool Wolf::canMate(Sheep const*) const {
	return false;
}

bool Wolf::canMate(Consommable const*) const {
	return false;
}

void Wolf::giveBirth() {
	Genome* genomeMere(new Genome(getGenome()));
  for (size_t i(0); i < numberOfChild; ++i) {
		getAppEnv().addEntity(new Wolf(getPosition(), numHerd ,getAppConfig().wolf_energy_initial, 0, pere, genomeMere));
	}
	numberOfChild = 0;
	pregnant = false;
	delete pere;
	delete genomeMere;
	pere = nullptr;
	genomeMere = nullptr;
}

void Wolf::meeting(LivingEntity* other) {
  other->meet(this);
}

void Wolf::meet(Wolf* other) {
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

void Wolf::meet(Consommable*) {
	//Ne fait rien
}

void Wolf::meet(Sheep*) {
	//Ne fait rien
}

//GESTION DU DÉPLACEMENT EN TROUPEAU
void Wolf::addAnimal(std::vector<LivingEntity*>& animals, int numTroupeau) {
	//Les loups qui seront ajoutés dans le tableau serviront à regarder qui est le plus vieux après le leader,
  //on n'ajoute donc pas le leader à ce tableau
  if(numTroupeau == numHerd and !leader){
    animals.push_back(this);
  }
}

void Wolf::becomeLeader() {
	leader = true;
}

bool Wolf::isFree() const {
	return (leader or (getAppEnv().getLeader(numHerd) == nullptr));
}

void Wolf::setIsAbleToBeFollowed(bool b) {
	isAbleToBeFollowed = b;
}

//GESTION DES INFECTIONS
void Wolf::isInfectedBy(LivingEntity* other) {
	other->infect(this);
}

void Wolf::isCuring(Sheep*) {
	//Ne fait rien
}

void Wolf::isCuring(Wolf*) {
	//Ne fait rien
}

void Wolf::isCuring(Consommable*) {
	//Ne fait rien
}

void Wolf::infect(Consommable*) {
  //Ne fait rien
}

void Wolf::infect(Wolf* other) {
	if(canInfect(other)) other->becomeInfected(new Virus(*immunity.getVirus()));
}

void Wolf::infect(Sheep*) {
  //Ne fait rien
}

bool Wolf::canInfect(Wolf* other) {
	//On vérifie que le loup est infecté par un virus et que celui ci est capable d'infecter un autre loup,
	//et que le loup other n'est pas lui même (en comparant les positions) !
	//On ajoute aussi une probabilité d'infecter l'autre entité
	return (immunity.isInfected() and
					position != other->getPosition() and
					immunity.getQuantityVirus() >= getAppConfig().virus_min_quantity_for_infection and
				 	bernoulli(getAppConfig().virus_infection_probability));
}

//GESTION DES HALLUCINATIONS (CHAMPIGNONS)
void Wolf::poison(Sheep*) {
	//Ne fait rien
}

void Wolf::poison(Wolf*) {
	//Ne fait rien
}

void Wolf::poison(Consommable*) {
	//Ne fait rien
}

bool Wolf::isPoisoned() const {
	return false;
}

void Wolf::decreasePoisonedTime(sf::Time) {
	//Ne fait rien
}

/**************************************************************************************************************/
//DESSIN
void Wolf::drawCrown(sf::RenderTarget& targetWindow) const {
	if(leader) {
  	//Représentation de la couronne avec sa texture
  	auto crownSprite = buildSprite(position + direction * getRadius() * 1.15 , getRadius() * 0.7, getAppTexture(getAppConfig().animal_texture_leader));
  	crownSprite.setRotation((getRotation())/ DEG_TO_RAD + 90);
  	targetWindow.draw(crownSprite);
	}
}

void Wolf::drawInfoHerd(sf::RenderTarget& targetWindow) const {
	std::string message(to_nice_string(-numHerd));
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
std::ostream& operator<<(std::ostream& sortie, const Wolf& w) {
	sortie << "Loup appartenant au troupeau " << to_nice_string(w.getNumHerd());
	if(w.isLeader()) sortie << " dont il est le leader";
	sortie << std::endl << "Niveau d'énergie : " << to_nice_string(w.getEnergyLevel()) << std::endl;
	sortie <<  "Temps de vie : " << to_nice_string(w.getLifeTime().asSeconds()) << std::endl;
	sortie << w.getGenome();
	return sortie;
}
