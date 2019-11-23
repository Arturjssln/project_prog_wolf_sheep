#include "Animal.hpp"
#include "Application.hpp"
#include <Random/Random.hpp>
#include <iostream>
#include <memory>

//CONSTRUCTEUR
Animal::Animal(Vec2d position, int niveauEnergie, double magnitude, Genome* pere, Genome* mere, Etat etat)
: LivingEntity(position, niveauEnergie), genome(Genome(pere, mere)), etat(etat),
  breakTimeAfterDoingSomething(sf::Time::Zero), gestationTime(sf::Time::Zero), nombreEnfants(0), pere(nullptr),
  pregnant(false), direction(Vec2d(1,0)), magnitude(magnitude), currentTarget(Vec2d(1,0))
{
  setDeceleration(moyenne);
}

/**************************************************************************************************************/
//METHODES GETTERS

Vec2d Animal::getSpeedVector() const {
    return direction*magnitude;
}

double Animal::getMaxSpeed() const {
	if (niveauEnergie < getAnimalCriticalEnergyThreshold())
    return getStandardMaxSpeed() * deceleration;
	switch (etat) {
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

Etat Animal::getEtat() const {
  return etat;
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

void Animal::setNumberChildren() {
  nombreEnfants = uniform(getMinChildren(), getMaxChildren());
}

/**************************************************************************************************************/
//METHODES DE MISE À JOUR

void Animal::update(sf::Time dt) {
    updateState();
    Vec2d f;
    switch (etat) {
      case FOOD_IN_SIGHT :
        f = calculForce();
        break;

      case FEEDING :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        break;

      case WANDERING :
        positionCible = randomWalk();
        f = positionCible - position;
        break;

      case MATE_IN_SIGHT :
        f = calculForce();
        break;

      case MATING :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        break;

      case GIVING_BIRTH :
        f = calculStoppingForce();
        breakTimeAfterDoingSomething -= dt;
        if(isPregnant()) giveBirth();
        break;

      case RUNNING_AWAY :
        f = calculPredatorForce();
        ennemisLesPlusProches.clear();
        break;

      default : f = Vec2d(0,0);
    }
    updateDonnees(dt, f);
}

//Mise à jour des données de l'animal (position, magnitude, direction, niveau d'énergie)
void Animal::updateDonnees(sf::Time dt, const Vec2d& force) {

	Vec2d acceleration(force/getMass());
	Vec2d newVitesse(getSpeedVector() + (acceleration * dt.asSeconds()));
  //Si la vitesse est égale à 0 on garde la même direction
  if (!isEqual(newVitesse.length(), 0.0)) direction = newVitesse.normalised();

	if (newVitesse.length() > getMaxSpeed()) {
		newVitesse = direction * getMaxSpeed();
	}
	position += newVitesse * dt.asSeconds();
	magnitude = newVitesse.length();
  niveauEnergie -= getAppConfig().animal_base_energy_consumption + magnitude * getFacteurPerteEnergie() * dt.asSeconds();

  if(isPregnant()) {
    gestationTime -= dt;
  }
  LivingEntity::update(dt);
}

void Animal::updateState() {
  //tableau contenant l'entité mangeable, le partenaire le plus proche, ou les prédateurs les plus proches
  std::vector<LivingEntity*> entities(analyzeEnvironnement());

  //Si il a fait quelque chose qui nécessite un temps de pause après, il ne fait rien tant que ce temps n'est pas écoulé
  if (breakTimeAfterDoingSomething > sf::Time::Zero) {
    //On ne change pas l'état : l'animal ne fait rien pendant sa pause

  } else if (isPregnant() and (gestationTime <= sf::Time::Zero)) {
    etat = GIVING_BIRTH;
    breakTimeAfterDoingSomething = sf::seconds(getAppConfig().animal_delivery_time); //temps de pause après accouchement

  } else if (entities.empty()) {
    //Si il n'y a pas d'entités dans le champ de vision : l'animal se balade au hasard
    etat = WANDERING;

  } else {
    switch (typeInSight) {
      //Si il y a des ennemis dans son champ de vision
      case ennemi :
        etat = RUNNING_AWAY;
        break;

      //Si il y a une partenaire dans son champ de vision mais pas d'ennemis
      case partenaire :
        if(isColliding(*entities[0])) {
          meeting(entities[0]);
          entities[0]->meeting(this);

        } else {
          etat = MATE_IN_SIGHT;
        }
        break;

        //Si il y a de la nourriture dans son champ de vision mais pas d'ennemis, ni de partenaire
        case nourriture :
          if(satisfied()) {
            etat = WANDERING;
          } else if (isColliding(*entities[0])) {
            etat = FEEDING;
            isEating(entities[0]);
          } else {
            etat = FOOD_IN_SIGHT;
          }
          break;

        default : etat = WANDERING;
      }
      /*On prend le partenaire ou la nourriture le plus proche, dans le cas des ennemis on prends le premier
      mais cela n'a pas d'influence puisque le calculPredatorForce() prends tous les ennemis en compte*/
      positionCible = entities[0]->getPosition();
      entities.clear();
    }
}

/**************************************************************************************************************/
//METHODES DE CALCUL DE FORCE

Vec2d Animal::calculForce() const {
	Vec2d toTarget(positionCible - position);
    if (toTarget.lengthSquared() == 0.0)  return Vec2d(0,0);
    //on utilise lenghtSquared et pas length car elle est moins coûteuse en mémoire
	double speed(std::min(toTarget.length()/deceleration, getMaxSpeed()));
	Vec2d vTarget(toTarget.normalised()*speed);

	return vTarget - getSpeedVector() ;
}

Vec2d Animal::calculStoppingForce() const {
  if (isEqual(magnitude, 0.0)) return Vec2d(0,0);
  Vec2d cibleNegative(position - magnitude * direction); //Plus sa magnitude est grande plus l'animal ralentit
  Vec2d toTarget(cibleNegative - position);
  double speed(toTarget.length()/deceleration);
  Vec2d vTarget(toTarget.normalised()*speed);
  return vTarget - getSpeedVector();
}

Vec2d Animal::calculPredatorForce() const {
  Vec2d force(0,0);

  for (size_t i(0); i < ennemisLesPlusProches.size(); ++i) {
    Vec2d toTarget(ennemisLesPlusProches[i]->getPosition() - position);
    force -= 900 * toTarget / pow(toTarget.length(), 1.2);
  }
  return force;
}

/**************************************************************************************************************/
//METHODES

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

std::vector<LivingEntity*> Animal::analyzeEnvironnement() {
  std::vector<LivingEntity*> lesEntitesVisibles(getAppEnv().getEntitiesInSightForAnimal(this));

  //Retourne les entités ennemies les plus proches si il y en a
  std::vector<LivingEntity*> plusProches(lesPlusProches(ennemi, lesEntitesVisibles));
  if(!plusProches.empty()) {
    typeInSight = ennemi;
    ennemisLesPlusProches = plusProches;
    return plusProches; //retourne le tableau d'ennmis
  }

  //Retourne l'entité partenaire la plus proche (tableau à une valeur) si il y en a une
  plusProches = lesPlusProches(partenaire, lesEntitesVisibles);
  if(!plusProches.empty()) {
    typeInSight = partenaire;
    return plusProches;
  }

  //Retourne l'entité mangeable la plus proche (tableau à une valeur) si il y en a une
  plusProches = lesPlusProches(nourriture, lesEntitesVisibles);
  if(!plusProches.empty()) {
    typeInSight = nourriture;
    return plusProches;
  }

  //Sinon retourne un tableau vide
  return plusProches;
}

std::vector<LivingEntity*> Animal::lesPlusProches(TypeInSight elementRecherche, std::vector<LivingEntity*> ensembleEntities) const {
  //Déclaration du tableau qui sera retourné
  std::vector<LivingEntity*> plusProches;
  //Si il est seul au monde (il n'a pas de cible potentielle) -> on retourne le tableau vide
  if (ensembleEntities.size() <= 1) return plusProches;

  //On initialise la position possible de la cible la plus proche au point le plus loin
  //du champ de vision (toute cible visible sera donc plus proche)
  Vec2d posPlusProche(getViewDistance()*cos(getRotation()), getViewDistance()*sin(getRotation()));
  posPlusProche = ConvertToGlobalCoord(posPlusProche);
  int indice(-1);

  //En fonction du type de l'élément recherché, on cherchera le(s) plus proche(s) de ce type
  switch (elementRecherche) {
    case ennemi :
      for (auto entite : ensembleEntities) {
        if (entite->eatable(this)) plusProches.push_back(entite);
      }
      break;

    case partenaire :
      for (size_t i(0); i < ensembleEntities.size(); ++i) {
        //Si deux cibles sont à même distance, on prend la première
        if ((this->matable(ensembleEntities[i])) and
           (ensembleEntities[i]->matable(this)) and
           ((position - ensembleEntities[i]->getPosition()).lengthSquared() < (position - posPlusProche).lengthSquared()) and
           (position != ensembleEntities[i]->getPosition())) //L'animal ne peut pas se prendre comme cible
        {
          posPlusProche = ensembleEntities[i]->getPosition(); //on mémorise la position de l'entité la plus proche
          indice = i;
        }
      }
      break;

    case nourriture :
      for (size_t i(0); i < ensembleEntities.size(); ++i) {
        if ((this->eatable(ensembleEntities[i])) and
           ((position - ensembleEntities[i]->getPosition()).lengthSquared() < (position - posPlusProche).lengthSquared()))
        {
          posPlusProche = ensembleEntities[i]->getPosition(); //on mémorise la position de l'entité la plus proche
          indice = i;
  	    }
	    }
      break;

    default : return plusProches;
  }
  if (indice >= 0) plusProches.push_back(ensembleEntities[indice]);
  return plusProches;
}

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
  if ((etat == FEEDING) and (niveauEnergie < getAppConfig().animal_satiety_max)) return false;
  if (niveauEnergie < getAppConfig().animal_satiety_min) return false;
  return true;
}

bool Animal::isFemale() const {
  return (genome.getSex() == FEMALE);
}

bool Animal::oppositeSex(Animal const* other) const {
  return ((isFemale() and !other->isFemale()) or (!isFemale() and other->isFemale()));
}

bool Animal::isPregnant() const {
  if (isFemale()) {
    return pregnant;
  } else {
    return false;
  }
}

/**************************************************************************************************************/
//METHODES DE DESSINS

void Animal::drawOn(sf::RenderTarget& targetWindow) const {
  //Représentation de l'animal avec une texture
  auto animalSprite = buildSprite(position, 2 * getRadius(), getAppTexture(getTexture()));
  animalSprite.setRotation(getRotation() / DEG_TO_RAD);
  targetWindow.draw(animalSprite);

  //Affichage du champ de vision, des infos sur l'animal, et de la cible virtuelle si debug est activé
  if (isDebugOn()) {
    drawVision(targetWindow);
    drawTextDebug(targetWindow);
    if(isPregnant())  drawPregnantCircle(targetWindow);

    //On affiche la cible virtuelle seulement si aucune cible réelle n'est visible
    if (etat == WANDERING) {
      drawCercleVision(targetWindow);
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

void Animal::drawCercleVision(sf::RenderTarget& targetWindow) const {
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
  Vec2d positionTexteEtat(direction * 40 + direction.normal() * 125);
  auto textEtat = buildText(afficheState(), ConvertToGlobalCoord(positionTexteEtat), getAppFont(),
                        getAppConfig().default_debug_text_size, sf::Color::White);
  textEtat.setRotation(getRotation() / DEG_TO_RAD + 90);

  Vec2d positionTexteSex(direction * 20 + direction.normal() * 125);
  auto textSex = buildText(afficheSex(), ConvertToGlobalCoord(positionTexteSex), getAppFont(),
                        getAppConfig().default_debug_text_size, sf::Color::White);
  textSex.setRotation(getRotation() / DEG_TO_RAD + 90);

  Vec2d positionTexteNrj(direction.normal() * 125);
  std::string message(to_nice_string(niveauEnergie));
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

std::string Animal::afficheState() const {
    switch (etat) {
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

std::string Animal::afficheSex() const {
    switch (genome.getSex()) {
      case MALE : return "MALE";
        break;
      case FEMALE : return "FEMALE";
        break;
      default : return "SEXE INCONNU";
    }
}
