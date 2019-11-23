#include "Sheep.hpp"
#include <Utility/Constants.hpp>

//CONSTRUCTEUR
Sheep::Sheep(Vec2d position, int numTroupeau, double niveauEnergie, double magnitude, Genome* pere, Genome* mere)
: Animal(position, niveauEnergie, magnitude, pere, mere, numTroupeau)
{
	++nbLivingEntities["sheep"];
	if(getAppEnv().getLeader(numHerd) == nullptr) {
    getAppEnv().setNewLeaderSheep(numHerd, this);
    isAbleToBeFollowed = true;
	}
}

//DESTRUCTEUR
Sheep::~Sheep()
{
  --nbLivingEntities["sheep"];
  if(leader) {
    getAppEnv().setNewLeaderSheep(numHerd);
  }
}

	/**************************************************************************************************************/
	//METHODES GETTERS

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

double Sheep::getFacteurPerteEnergie() const {
	return getAppConfig().sheep_energy_loss_factor;
}

double Sheep::getMinEnergyMating() const {
	if (isFemale()) return getAppConfig().sheep_energy_min_mating_female;
	return getAppConfig().sheep_energy_min_mating_male;
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
//METHODES
//GESTION DE LA NOURRITURE
bool Sheep::eatable(LivingEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Sheep::eatableBy(Wolf const* wolf) const {
	return true;
}

bool Sheep::eatableBy(Sheep const* sheep) const {
	return false;
}

bool Sheep::eatableBy(Grass const* grass) const {
	return false;
}

void Sheep::isEating(LivingEntity* entity) {
	niveauEnergie += getAppConfig().animal_meal_retention * entity->getNiveauEnergie();
	entity->setNiveauEnergie(entity->getNiveauEnergie() - getAppConfig().sheep_energy_bite);
	breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_feed_time);
}

//GESTION DE LA REPRODUCTION
bool Sheep::matable(LivingEntity const* other) const  {
  return other->canMate(this);
}

bool Sheep::canMate(Wolf const* wolf) const {
	return false;
}

bool Sheep::canMate(Sheep const* sheep) const {
	if (niveauEnergie < getMinEnergyMating() or
			!oppositeSex(sheep) or
			sheep->isPregnant() or isPregnant() or
			!sheep->isTargetInSight(getPosition()) or //can mate seulement si les deux partenaires sont dans le champ de vision de l'autre !
			tempsDeVie < ANIMAL_TIME_BEFORE_MATING or
			breakTimeAfterDoingSomething > sf::Time::Zero)
	{
		return false;
	}
	return true;
}

bool Sheep::canMate(Grass const* grass) const {
	return false;
}

void Sheep::giveBirth() {
	Genome* genomeMere(new Genome(getGenome()));
  for (size_t i(0); i < nombreEnfants; ++i) {
		getAppEnv().addEntity(new Sheep(getPosition(),numHerd ,getAppConfig().sheep_energy_initial, 0, pere, genomeMere));
	}
	nombreEnfants = 0;
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
      niveauEnergie -= nombreEnfants * getEneryLossPerChildFemale();
      gestationTime = getGestationTime();
      pere = new Genome(other->getGenome());
    } else {
      niveauEnergie -= getEnergyLossMatingMale();
    }
    etat = MATING;
    breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_mating_time);
}

void Sheep::meet(Grass* other) {
	//Ne fait rien
}

void Sheep::meet(Wolf* other) {
	//Ne fait rien
}

//GESTION DU DEPLACEMENT EN TROUPEAU
bool Sheep::isFree() const {
	return (leader or (getAppEnv().getLeader(numHerd) == nullptr));
}

void Sheep::addSheep(std::vector<LivingEntity*>& tab, int numTroupeau)
{
  //Les moutons qui seront ajoutés dans le tableau serviront à regarder qui est le plus vieux après le leader,
  //on n'ajoute donc pas le leader à ce tableau
  if(numTroupeau == numHerd and !leader){
    tab.push_back(this);
  }
}

void Sheep::addWolf(std::vector<LivingEntity*>& tab, int numTroupeau)
{
  //ne fait rien
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

//DESSIN
//Représentation de la couronne avec sa texture si le mouton est leader
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
