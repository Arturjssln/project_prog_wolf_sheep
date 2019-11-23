#include "Wolf.hpp"
#include <Utility/Constants.hpp>

//CONSTRUCTEUR
Wolf::Wolf(Vec2d position, double niveauEnergie, double magnitude , Genome* pere, Genome* mere)
: Animal(position, niveauEnergie, magnitude, pere, mere)
{}

/**************************************************************************************************************/
//METHODES GETTERS

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

double Wolf::getFacteurPerteEnergie() const {
	return getAppConfig().wolf_energy_loss_factor;
}

double Wolf::getMinEnergyMating() const {
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

/**************************************************************************************************************/
//METHODES


bool Wolf::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Wolf::eatableBy(Wolf const* wolf) const {
	return false;
}

bool Wolf::eatableBy(Sheep const* sheep) const {
	return false;
}

bool Wolf::eatableBy(Grass const* grass) const {
	return false;
}

bool Wolf::matable(LivingEntity const* other) const  {
	return other->canMate(this);
}

bool Wolf::canMate(Wolf const* wolf) const {
	if (niveauEnergie < getMinEnergyMating() or
			!oppositeSex(wolf) or
			wolf->isPregnant() or isPregnant() or
			!wolf->isTargetInSight(getPosition()) or //can mate seulement si les deux partenaires sont dans le champ de vision de l'autre !
			tempsDeVie < ANIMAL_TIME_BEFORE_MATING or
			breakTimeAfterDoingSomething > sf::Time::Zero)
  {
		return false;
	}
	return true;
}

bool Wolf::canMate(Sheep const* sheep) const {
	return false;
}

bool Wolf::canMate(Grass const* grass) const {
	return false;
}

void Wolf::isEating(LivingEntity* entity) {
	niveauEnergie += getAppConfig().animal_meal_retention * entity->getNiveauEnergie();
	entity->setNiveauEnergie(0); //le niveau d'énergie de l'entité mangée est 0
	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_time);
}

void Wolf::giveBirth() {
	Genome* genomeMere(new Genome(getGenome()));
  for (size_t i(0); i < nombreEnfants; ++i) {
		getAppEnv().addEntity(new Wolf(getPosition(), getAppConfig().wolf_energy_initial, 0, pere, genomeMere));
	}
	nombreEnfants = 0;
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
      niveauEnergie -= nombreEnfants * getEneryLossPerChildFemale();
      gestationTime = getGestationTime();
      pere = new Genome(other->getGenome());
    } else {
      niveauEnergie -= getEnergyLossMatingMale();
    }
    etat = MATING;
    breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_mating_time);
}

void Wolf::meet(Grass*  other) {
	//Ne fait rien
}

void Wolf::meet(Sheep* other) {
	//Ne fait rien
}
