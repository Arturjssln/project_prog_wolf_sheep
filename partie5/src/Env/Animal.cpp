#include "Animal.hpp"
#include "Application.hpp"
#include <Random/Random.hpp>
#include <iostream>
#include <memory>

//CONSTRUCTEUR
Animal::Animal(Vec2d position, double energyLevel, double magnitude, Genome* pere, Genome* mere, int numTroupeau, Etat state)
: LivingEntity(position, energyLevel), direction(Vec2d(1,0)), genome(Genome(pere, mere)), state(state),
  breakTimeAfterDoingSomething(sf::Time::Zero), breakTime(sf::Time::Zero),
  gestationTime(sf::Time::Zero), numberOfChild(0), pere(nullptr),
  pregnant(false),numHerd(numTroupeau), leader(false), isAbleToBeFollowed(false),
  immunity(this), magnitude(magnitude), currentTarget(Vec2d(1,0)),
  positionCible(Vec2d(0,0)), typeInSight(ennemi), closestPartner(nullptr), closestFood(nullptr)
{
  setDeceleration(moyenne);
}

/**************************************************************************************************************/
//METHODES GETTERS

Vec2d Animal::getSpeedVector() const {
    return direction*magnitude;
}

double Animal::getMaxSpeed() const {
	if (energyLevel < getAnimalCriticalEnergyThreshold())
    return getStandardMaxSpeed() * deceleration;
	switch (state) {
		case MATE_IN_SIGHT : return getStandardMaxSpeed() * 2;
			break;
		case FOOD_IN_SIGHT : return getStandardMaxSpeed() * 3;
			break;
		case RUNNING_AWAY : return getStandardMaxSpeed() * 4;
			break;
		default : return getStandardMaxSpeed();
	}
}

double Animal::getAnimalCriticalEnergyThreshold() const {
	return getAppConfig().animal_min_energy * 2;
}

double Animal::getRotation() const {
	return direction.angle();
}

Genome Animal::getGenome() const {
  return genome;
}

bool Animal::isAlive() const {
  if(energyLevel < getAppConfig().animal_min_energy or
     lifeTime > getLongevity() or
     immunity.getHealthState() < 0)
  {
     return false;
  }
  return true;
}

std::array<double,6> Animal::getAnimalData() const {
  std::array<double,6> data = {{ energyLevel,
                                immunity.getHealthState(),
                                immunity.getQuantityVirus(),
                                immunity.getAdaScore(),
                                immunity.getActivationLevel(),
                                immunity.getGlobalScore() }};
  return data;
}

/**************************************************************************************************************/
//METHODES SETTERS

void Animal::setRotation(double angle) {
	direction = {cos(angle), sin(angle)};
}

void Animal::setTargetPosition(const Vec2d& newPosition) {
	positionCible = newPosition;
}

void Animal::setDeceleration(Deceleration d) {
    deceleration = d * DECELERATION;
    //où DECELERATION est une constante (dans le ficheir Constants.hpp), voir Q2.5
}

//Met à jour le nombre d'enfants (attribut) entre deux valeurs limites
void Animal::setNumberChildren() {
  numberOfChild = uniform(getMinChildren(), getMaxChildren());
}

//Change le profil immunitaire de l'attribut génome
void Animal::setImmuneGenes(std::array<double, TAILLE_PROFIL_IMMU> profil) {
  genome.setProfilImmunitaire(profil);
}

/**************************************************************************************************************/
//METHODES DE MISE À JOUR

//Met à jour les données en fonction de l'état
void Animal::update(sf::Time dt) {
    //Met à jours les attributs closestEnemies, closestPartner et closestFood
    analyzeEnvironnement();
    updateState();
    Vec2d f;
    switch (state) {
    //Dans les cas où le leader est en FEEDING, MATING et GIVING_BIRTH, il sera à l'arrêt, les autres ne doivent donc plus le suivre
    //sinon on aurait un amas d'animaux d'où la mise à jour de l'attribut isAbleToBeFollowed
      case FOOD_IN_SIGHT :
        if(isLeader()) setIsAbleToBeFollowed(true);
        f = calculForce();
        break;

      case FEEDING :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        if(isLeader()) setIsAbleToBeFollowed(false);
        break;

      case WANDERING :
        if(isLeader())setIsAbleToBeFollowed(true);
        positionCible = randomWalk();
        //Pour l'instant on calcul la force de la cible virtuelle
        f = positionCible - position;
        if(!isFree() and (getAppEnv().getLeader(this->getNumHerd()))->getIsAbleToBeFollowed()) {
          //On rajoute maintenant la force liée au leader
          positionCible = (getAppEnv().getLeader(this->getNumHerd()))->getPosition(); //Position du leader
          f += calculForce() + calculAvoidanceForce();
        }
        break;

      case MATE_IN_SIGHT :
        if(isLeader()) setIsAbleToBeFollowed(true);
        f = calculForce();
        break;

      case MATING :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        if(isLeader()) setIsAbleToBeFollowed(false);
        break;

      case GIVING_BIRTH :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        if(isLeader()) setIsAbleToBeFollowed(false);
        if(isPregnant()) giveBirth();
        break;

      case RUNNING_AWAY :
        if(isLeader()) setIsAbleToBeFollowed(true);
        f = calculPredatorForce();
        closestEnemies.clear();
        break;

      case IDLE :
        f = calculStoppingForce();
        breakTime -= dt;
        if(isLeader()) setIsAbleToBeFollowed(false);
        break;

      default : f = Vec2d(0,0);
    }

    updateDonnees(dt, f);
}

//Mise à jour des données de l'animal (position, magnitude, direction, niveau d'énergie)
void Animal::updateDonnees(sf::Time dt, const Vec2d& force) {

	Vec2d acceleration(force/getMass());
	Vec2d newVitesse(getSpeedVector() + (acceleration * dt.asSeconds()));

  //Si la vitesse ou la vitesse maximale est égale à 0 on garde la même direction
  if (!isEqual(newVitesse.length(), 0.0) and (getMaxSpeed() != 0.0)) direction = newVitesse.normalised();

	if (newVitesse.length() > getMaxSpeed()) {
		newVitesse = direction * getMaxSpeed();
	}
	position += newVitesse * dt.asSeconds();
	magnitude = newVitesse.length();
  energyLevel -= getAppConfig().animal_base_energy_consumption + magnitude * getEnergyLossFactor() * dt.asSeconds();

  if(isPregnant()) {
    gestationTime -= dt;
  }

  immunity.update(dt);
  LivingEntity::update(dt);
}

//Met à jour l'état de l'animal
void Animal::updateState() {
  //Si il a fait quelque chose qui nécessite un temps de pause après, il ne fait rien tant que ce temps n'est pas écoulé
  if (breakTimeAfterDoingSomething > sf::Time::Zero or breakTime > sf::Time::Zero) {
    //On ne change pas l'état : l'animal ne fait rien pendant sa pause

  } else {
    //Avant de repasser en WANDERING si l'animal était dans un des 3 états dans les condition ci-dessous,
    //l'animal pourra passer en IDLE avec une certaine probabilité (s'il n'a pas faim)
    if (((state == FEEDING) or (state == GIVING_BIRTH) or (state == MATING)) and (satisfied())) {
      if (bernoulli(getAppConfig().animal_idle_probability)) {
        state = IDLE;
        breakTime = sf::seconds(uniform(getAppConfig().animal_idle_time_min, getAppConfig().animal_idle_time_max));
      }
    }

    //Si il ne s'est pas mis en IDLE
    if(breakTime <= sf::Time::Zero) {
      if (isPregnant() and (gestationTime <= sf::Time::Zero)) {
        state = GIVING_BIRTH;
        breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_delivery_time); //temps de pause après accouchement

      } else if (closestEnemies.empty() and closestPartner == nullptr and closestFood == nullptr) {
        //Si il n'y a pas d'entités dans le champ de vision : l'animal se balade au hasard
        state = WANDERING;

      } else {
        switch (typeInSight) {
          //Si il y a des ennemis dans son champ de vision
          case ennemi :
            state = RUNNING_AWAY;
            break;

          //Si il y a une partenaire dans son champ de vision mais pas d'ennemis
          case partenaire :
            if(isColliding(*closestPartner)) {
              meeting(closestPartner);
              closestPartner->meeting(this);
            } else {
              state = MATE_IN_SIGHT;
            }
            positionCible = closestPartner->getPosition();
            break;

          //Si il y a de la nourriture dans son champ de vision mais pas d'ennemis, ni de partenaire
          case nourriture :
            if(satisfied()) {
              state = WANDERING;
            } else if (isColliding(*closestFood)) {
              state = FEEDING;
              isEating(closestFood);
            } else {
              state = FOOD_IN_SIGHT;
            }
            positionCible = closestFood->getPosition();
            break;

            default : state = WANDERING;
          }
        }
      }
    }
}

/**************************************************************************************************************/
//METHODES DE CALCUL DE FORCE

//Calcul de la force en fonction d'une cible (enregistrée dans l'attribut positionCible)
Vec2d Animal::calculForce() const {
	Vec2d toTarget(positionCible - position);
    if (toTarget.lengthSquared() == 0.0)  return Vec2d(0,0);
    //on utilise lenghtSquared et pas length car elle est moins coûteuse en mémoire
	double speed(std::min(toTarget.length()/deceleration, getMaxSpeed()));
	Vec2d vTarget(toTarget.normalised()*speed);

	return vTarget - getSpeedVector() ;
}

//Calcul de la force d'arrêt quand l'animal doit s'arrêter
Vec2d Animal::calculStoppingForce() const {
  if (isEqual(magnitude, 0.0)) return Vec2d(0,0);
  Vec2d cibleNegative(position - magnitude * direction); //Plus sa magnitude est grande plus l'animal ralentit (cible virtuelle derrière l'animal)
  Vec2d toTarget(cibleNegative - position);
  double speed(toTarget.length()/deceleration);
  Vec2d vTarget(toTarget.normalised()*speed);
  return vTarget - getSpeedVector();
}

//Calcul de la force afin d'échapper aux prédateurs
Vec2d Animal::calculPredatorForce() const {
  Vec2d force(0,0);

  for (size_t i(0); i < closestEnemies.size(); ++i) {
    Vec2d toTarget(closestEnemies[i]->getPosition() - position);
    force -= 900 * toTarget / pow(toTarget.length(), 1.2);
  }
  return force;
}

//Calcul de la force d'évitement pour que les animaux ne se marchent pas l'un sur l'autre quand ils marchent en troupeau
Vec2d Animal::calculAvoidanceForce() const {
  Vec2d force(0,0);
  for (size_t i(0); i < neighbors.size(); ++i) {
    Vec2d toTarget(position - neighbors[i]->getPosition());
    force += (getAppConfig().avoidance_coeff * toTarget) / toTarget.lengthSquared();
  }
  return force;
}

/**************************************************************************************************************/
//METHODES

//Retourne si la cible est dans le champ de vision de l'animal ou pas
bool Animal::isTargetInSight(const Vec2d& posCible) const {
    Vec2d distance(posCible - position);
    //on utilise lenghtSquared et pas length car elle est moins coûteuse en mémoire
    if(isEqual(distance.lengthSquared(), 0)) {
        return true;
    }
    //même raison que précedemment sur l'utilisation de lengthSquared
    if((distance.lengthSquared() <= pow(getViewDistance(),2)) and
       direction.dot(distance.normalised()) >= cos((getViewRange() + 0.001)/2)) {
        return true;
    }
    return false;
}

/*Analyse les alentours et enregistre dans des attributs :
  - tous les ennemis visibles
  - le partenaire le plus proche
  - l'entité mangeable la plus proche
  - les voisins proches pour la force d'évitement */
void Animal::analyzeEnvironnement() {
  updateClosest(getAppEnv().getEntities());

  if(!closestEnemies.empty()) {
    typeInSight = ennemi;
  } else if(closestPartner != nullptr) {
    typeInSight = partenaire;
  } else if(closestFood != nullptr) {
    typeInSight = nourriture;
  }
}


void Animal::updateClosest(std::list<LivingEntity*> ensembleEntities) {

  //On initialise la position possible de la cible la plus proche au point le plus loin
  //du champ de vision (toute cible visible sera donc plus proche)
  Vec2d posPlusProche(getViewDistance()*cos(getRotation()), getViewDistance()*sin(getRotation()));
  Vec2d posPlusProchePartner = ConvertToGlobalCoord(posPlusProche);
  Vec2d posPlusProcheFood = ConvertToGlobalCoord(posPlusProche);

  closestEnemies.clear();
  closestPartner = nullptr;
  closestFood = nullptr;
  neighbors.clear();

      for (auto entite : ensembleEntities) {
        if (isTargetInSight(entite->getPosition())) {

          //ENNEMIS
          if (entite->eatable(this)) closestEnemies.push_back(entite);

          //PARTNER
          if ((this->matable(entite)) and
            (entite->matable(this)) and
            ((position - entite->getPosition()).lengthSquared() < (position - posPlusProchePartner).lengthSquared()) and
            (position != entite->getPosition())) //L'animal ne peut pas se prendre comme cible
          {
              posPlusProchePartner = entite->getPosition(); //on mémorise la position de l'entité la plus proche
              closestPartner = entite;
          }

          //FOOD
          if ((this->eatable(entite)) and
            ((position - entite->getPosition()).lengthSquared() < (position - posPlusProcheFood).lengthSquared()))
          {
              posPlusProcheFood = entite->getPosition(); //on mémorise la position de l'entité la plus proche
              closestFood = entite;
  	      }

        }

        /*Analyse les alentours et retourne les entites plus proches qu'un certain seuil
          -> permet de calculer la force d'évitement */
          if (((position - entite->getPosition()).lengthSquared()) < pow(getAppConfig().proximity_threshold, 2) and
              (position != entite->getPosition()))
          {
              neighbors.push_back(entite);
          }

        //Regarde les entités dans pouvant être infectées et tente de les infecter
          if (((position - entite->getPosition()).lengthSquared()) < pow(getAppConfig().virus_infection_range, 2) and
              (position != entite->getPosition()))
          {
            //si il est assez proche, alors on tente une infection qui aura lieu ou pas selon le type,
            //les caractéristiques de chaque entité et la probabilité de transmission
            //-> double dispatch
            entite->isInfectedBy(this);
          }

      }
}

//On définit une cible virtuelle qui permet de se promener au hasard
Vec2d Animal::randomWalk() {
    //Tous les vecteurs de cette méthode sont en coordonées locales
    Vec2d random_vec(uniform(-1.0,1.0), uniform(-1.0,1.0));
    currentTarget += random_vec * getRandomWalkJitter();
    currentTarget = currentTarget.normalised() * getRandomWalkRadius();
    Vec2d movedCurrentTarget = currentTarget + direction * getRandomWalkDistance();
    return ConvertToGlobalCoord(movedCurrentTarget); //Conversion en coordonnées globales
}

Vec2d Animal::ConvertToGlobalCoord(const Vec2d& local) const {
    sf::Transform matTransform;
    matTransform.translate(position);
    matTransform.rotate(getRotation());
    return matTransform.transformPoint(local);
}

//Retourne si l'animal est rassasié ou non
bool Animal::satisfied() const {
  //si l'animal est en train de se nourrir, il mange jusqu'à atteindre le seuil max
  if ((state == FEEDING) and (energyLevel < getAppConfig().animal_satiety_max)) return false;
  if (energyLevel < getAppConfig().animal_satiety_min) return false;
  return true;
}

bool Animal::isFemale() const {
  return (genome.getSex() == FEMALE);
}

//Retourne true si les animaux sont de sexe opposés, false sinon
bool Animal::oppositeSex(Animal const* other) const {
  return ((isFemale() and !other->isFemale()) or (!isFemale() and other->isFemale()));
}

//Retourne true si c'est une femelle enceinte, false dans tous les autres cas
bool Animal::isPregnant() const {
  if (isFemale()) {
    return pregnant;
  } else {
    return false;
  }
}

std::string Animal::getStatLabel() const {
  return s::ANIMAL_INDIVIDUAL;
}

void Animal::becomeInfected(Virus* v) {
  if(!immunity.isInfected()) {
    immunity.setVirus(v);
  }
}

/**************************************************************************************************************/
//METHODES DE DESSINS
//MÉTHODE DE DESSIN GÉNÉRALE
void Animal::drawOn(sf::RenderTarget& targetWindow) const {
  //Représentation de l'animal avec une texture
  auto animalSprite = buildSprite(position, 2 * getRadius(), getAppTexture(getTexture()));
  animalSprite.setRotation(getRotation() / DEG_TO_RAD);
  targetWindow.draw(animalSprite);

  //Affichage de la couronne du leader
  drawCrown(targetWindow);

  //Affichage de la "cible" lorsqu'on considère statistiques individuelles
  if (isTracked()) drawTrack(targetWindow);
  //Affichage du virus si il est infecté
  if (immunity.isInfected()) drawInfected(targetWindow);

  //Affichage du champ de vision, des infos sur l'animal et de la cible virtuelle si debug est activé
  if (isDebugOn()) {
    drawVision(targetWindow);
    drawTextDebug(targetWindow);
    drawInfoHerd(targetWindow);
    if(isPregnant())  drawPregnantCircle(targetWindow);

    //On affiche la cible virtuelle seulement si aucune cible réelle n'est visible
    if (state == WANDERING) {
      drawCircleVision(targetWindow);
      drawVirtualTarget(targetWindow);
    }
  }
}

void Animal::drawVision(sf::RenderTarget& targetWindow) const {
    sf::Color color = sf::Color::Black;
    color.a = 16; //light, transparent grey
    //Déclaration du champ de vision centré sur l'axe horizontal (repère local de l'animal)
    Arc arcgraphics(-0.5*(getViewRange()/DEG_TO_RAD), 0.5*(getViewRange()/DEG_TO_RAD),
                    getViewDistance(), color, getViewDistance());
    //Rotation de l'arc pour l'aligner avec la direction de l'animal
    arcgraphics.rotate(getRotation()/DEG_TO_RAD);
    //Placement de l'origine de l'arc sur l'animal
    arcgraphics.setPosition(position);
    arcgraphics.setOrigin(getViewDistance(), getViewDistance());
    targetWindow.draw(arcgraphics);
}

void Animal::drawCircleVision(sf::RenderTarget& targetWindow) const {
    sf::CircleShape cercleCible(getRandomWalkRadius());
    cercleCible.setFillColor(sf::Color::Transparent);
    cercleCible.setOutlineThickness(2);
    cercleCible.setOutlineColor(sf::Color(255,165,0));
    cercleCible.setPosition(position + direction * getRandomWalkDistance());
    cercleCible.setOrigin(getRandomWalkRadius(), getRandomWalkRadius());
    targetWindow.draw(cercleCible);
}

void Animal::drawVirtualTarget(sf::RenderTarget& targetWindow) const {
  const double rayonCibleVirtuelle(5);
  sf::CircleShape cibleVirtuelle(rayonCibleVirtuelle);
  cibleVirtuelle.setFillColor(sf::Color(0,0,255));
  cibleVirtuelle.setPosition(positionCible);
  cibleVirtuelle.setOrigin(rayonCibleVirtuelle,rayonCibleVirtuelle);
  targetWindow.draw(cibleVirtuelle);
}

void Animal::drawPregnantCircle(sf::RenderTarget& targetWindow) const {
  const double rayonCercle(getRadius());
  sf::CircleShape cerclePregnant(rayonCercle);
  cerclePregnant.setOutlineColor(sf::Color::Magenta);
  cerclePregnant.setOutlineThickness(4);
  cerclePregnant.setFillColor(sf::Color::Transparent);
  cerclePregnant.setPosition(position);
  cerclePregnant.setOrigin(rayonCercle,rayonCercle);
  targetWindow.draw(cerclePregnant);
}

void Animal::drawTextDebug(sf::RenderTarget& targetWindow) const {
  Vec2d positionTexteEtat(direction * 35 + direction.normal() * getRadius() * 4);
  auto textEtat = buildText(displayState(), ConvertToGlobalCoord(positionTexteEtat), getAppFont(),
                        getAppConfig().default_debug_text_size, sf::Color::White);
  textEtat.setRotation(getRotation() / DEG_TO_RAD + 90);

  Vec2d positionTexteSex(direction * 15 + direction.normal() * getRadius() * 4);
  auto textSex = buildText(displaySex(), ConvertToGlobalCoord(positionTexteSex), getAppFont(),
                        getAppConfig().default_debug_text_size, sf::Color::White);
  textSex.setRotation(getRotation() / DEG_TO_RAD + 90);

  Vec2d positionTexteNrj(-5 * direction + direction.normal() * getRadius() * 4);
  std::string message(to_nice_string(energyLevel));
  if (satisfied()) {
    message += "    Satisfied";
  } else {
    message += "    Hungry";
  }
  auto textNrj = buildText(message, ConvertToGlobalCoord(positionTexteNrj),
                          getAppFont(), getAppConfig().default_debug_text_size, sf::Color::White);
  textNrj.setRotation(getRotation() / DEG_TO_RAD + 90);

  targetWindow.draw(textEtat);
  targetWindow.draw(textSex);
  targetWindow.draw(textNrj);
}

void Animal::drawInfected(sf::RenderTarget& targetWindow) const {
  auto infectedSprite = buildSprite(position, getRadius() * 0.7, getAppTexture(getAppConfig().virus_texture_infected));
  infectedSprite.setRotation((getRotation())/ DEG_TO_RAD + 90);
  targetWindow.draw(infectedSprite);
}

void Animal::drawTrack(sf::RenderTarget& targetWindow) const {
  auto trackSprite = buildSprite(position - direction * getRadius() , getRadius() * 0.7, getAppTexture(getAppConfig().entity_texture_tracked));
  trackSprite.setRotation((getRotation())/ DEG_TO_RAD + 90);
  targetWindow.draw(trackSprite);
}

//MÉTHODE D'AFFICHAGE DU PROFIL IMMUNITAIRE INNÉ
void Animal::afficheInnateProfile(std::ostream& out) const {
  genome.afficheImmuneProfile(out);
}

std::string Animal::displayState() const {
    switch (state) {
      case FOOD_IN_SIGHT : return "FOOD_IN_SIGHT";
        break;
      case FEEDING : return "FEEDING";
        break;
      case RUNNING_AWAY : return "RUNNING_AWAY";
        break;
      case MATE_IN_SIGHT : return "MATE_IN_SIGHT";
        break;
      case MATING : return "MATING";
        break;
      case GIVING_BIRTH : return "GIVING_BIRTH";
        break;
      case WANDERING : return "WANDERING";
        break;
      case IDLE : return "IDLE";
        break;
      default : return "ETAT INCONNU";
    }
}

std::string Animal::displaySex() const {
    switch (genome.getSex()) {
      case MALE : return "MALE";
        break;
      case FEMALE : return "FEMALE";
        break;
      default : return "SEXE INCONNU";
    }
}

/**************************************************************************************************************/
//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& out, const Animal& animal) {
  animal.afficheInnateProfile(out);
  return out;
}
