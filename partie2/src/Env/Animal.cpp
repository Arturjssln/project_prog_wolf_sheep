#include "Animal.hpp"
#include "Application.hpp"
#include <Random/Random.hpp>

//CONSTRUCTEUR
Animal::Animal(Vec2d position, double magnitude)
: position(position), direction(Vec2d(1,0)), magnitude(magnitude),
  currentTarget(Vec2d(1,0))
{
  setDeceleration(moyenne);
}

/**************************************************************************************************************/
//METHODES GETTERS

double Animal::getStandardMaxSpeed() const {
	return ANIMAL_MAX_SPEED;
}

double Animal::getMass() const {
	return ANIMAL_MASS;
}

double Animal::getRadius() const {
	return ANIMAL_RADIUS;
}

Vec2d Animal::getSpeedVector() const {
    return direction*magnitude;
}

double Animal::getViewRange() const {
	return ANIMAL_VIEW_RANGE;
}

double Animal::getViewDistance() const {
	return ANIMAL_VIEW_DISTANCE;
}

double Animal::getRotation() const {
	return direction.angle();
}

Vec2d Animal::getPosition() const {
    return position;
}

double Animal::getRandomWalkRadius() const {
    return ANIMAL_RANDOM_WALK_RADIUS;
}

double Animal::getRandomWalkDistance() const {
    return ANIMAL_RANDOM_WALK_DISTANCE;
}

double Animal::getRandomWalkJitter() const {
    return ANIMAL_RANDOM_WALK_JITTER;
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

void Animal::setPosition() {
    //dépassement à droite de la fenêtre
    if(position.x >= getAppConfig().simulation_world_size) {
        position.x = 0.0;
    }
    //dépassement à gauche de la fenêtre
    if(position.x < 0.0) {
        position.x = getAppConfig().simulation_world_size;
    }
    //dépassement en bas de la fenêtre
    if(position.y >= getAppConfig().simulation_world_size) {
        position.y = 0.0;
    }
    //dépassement en haut de la fenêtre
    if(position.y < 0.0) {
        position.y = getAppConfig().simulation_world_size;
    }
}

/**************************************************************************************************************/
//METHODES DE MISE A JOUR

void Animal::update(sf::Time dt) {
    /*On déclare un tableau contenant les cibles visibles par l'animal de son environnement,
    pour faciliter son utilisation*/
    std::vector<Vec2d> lesCiblesVisibles(getAppEnv().getTargetsInSightForAnimal(this));
    //déclaration de la variable force (en fonction de la présence ou non de cible, la force sera différente)
    Vec2d f;
    if (lesCiblesVisibles.empty()) {
        //Dans le cas ou il n'y a pas de cible dans son champ de vision -> Promenade au hasard
        positionCible = randomWalk(); //On affecte une cible au hasard
        f = positionCible - position;
    } else {
        //On choisit la cible la plus proche parmi les cibles visibles
        positionCible = cibleLaPlusProche(lesCiblesVisibles);
        f = calculForce();
    }
    updateDonnees(dt, f); // la force est donée en argument dans la méthode update_donnees()
}

//Mise à jour des données de l'animal (position, magnitude, direction)
void Animal::updateDonnees(sf::Time dt, const Vec2d& force) {
	Vec2d acceleration(force/getMass());
	Vec2d newVitesse(getSpeedVector() + acceleration * dt.asSeconds());
	direction = newVitesse.normalised();

	if (newVitesse.length() > getStandardMaxSpeed()) {
		newVitesse = direction * getStandardMaxSpeed();
	}
	position += newVitesse * dt.asSeconds();
	magnitude = newVitesse.length();

  setPosition(); //mise à jour position de l'animal si dépasse du monde
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

Vec2d Animal::randomWalk() {
    //Tous les vecteurs de cette méthode sont en coordonées locales
    Vec2d randomVec(uniform(-1.0,1.0), uniform(-1.0,1.0));
    currentTarget += randomVec * getRandomWalkJitter();
    currentTarget = currentTarget.normalised() * getRandomWalkRadius();
    Vec2d movedCurrentTarget = currentTarget + direction * getRandomWalkDistance();
    return ConvertToGlobalCoord(movedCurrentTarget); //Conversion en coordonnées globales
}

Vec2d Animal::cibleLaPlusProche(std::vector<Vec2d> ensembleCibles) const {
  //On initialise la cible la plus proche au point le plus loin du champ de vision (toute cible visible sera donc plus proche)
  Vec2d laPlusProche(getViewDistance()*cos(getRotation()), getViewDistance()*sin(getRotation()));
  laPlusProche = ConvertToGlobalCoord(laPlusProche);
  for (auto cible : ensembleCibles) {
    if ((position - cible).length() < (position - laPlusProche).length()) laPlusProche = cible;
}
return laPlusProche;
}

bool Animal::aucuneCibleReelle() const {
  std::vector<Vec2d> lesCiblesVisibles(getAppEnv().getTargetsInSightForAnimal(this));
  return lesCiblesVisibles.empty();
}

Vec2d Animal::calculForce() const {
	Vec2d toTarget(positionCible - position);
    if (toTarget.lengthSquared() == 0.0) { return Vec2d(0,0); }
    //on utilise lenghtSquared et pas length car elle est moins coûteuse en mémoire

	double speed(std::min(toTarget.length()/deceleration, getStandardMaxSpeed()));
	Vec2d vTarget(toTarget.normalised()*speed);

	return (vTarget - getSpeedVector())*2 ;
}

Vec2d Animal::ConvertToGlobalCoord(const Vec2d& local) const {
    sf::Transform matTransform;
    matTransform.translate(position);
    matTransform.rotate(getRotation());
    return matTransform.transformPoint(local);
}

/**************************************************************************************************************/
//METHODES DE DESSIN

void Animal::drawOn(sf::RenderTarget& targetWindow) const {
/*Représentation de l'animal avec un cercle
  sf::CircleShape cercle_animal(getRadius());
  cercle_animal.setFillColor(sf::Color(136,66,29));
	cercle_animal.setPosition(position);
	cercle_animal.setOrigin(getRadius(), getRadius());

	targetWindow.draw(cercle_animal);
*/
  //Représentation de l'animal avec une texture
  auto animalSprite = buildSprite(position, 2 * getRadius(), getAppTexture("wolf-black.png"));
  animalSprite.setRotation(getRotation() / DEG_TO_RAD);
  targetWindow.draw(animalSprite);

  drawVision(targetWindow);

  //On affiche la cible virtuelle seulement si aucune cible réelle n'est visible
  if (aucuneCibleReelle()) {
    drawCercleVision(targetWindow);
    drawVirtualTarget(targetWindow);
  }
}

void Animal::drawVision(sf::RenderTarget& targetWindow) const {
    sf::Color color = sf::Color::Black;
    color.a = 16; //light, transparent grey
    //Déclaration du champ de vision centré sur l'axe horizontal (repère local de l'animal)
    Arc arcgraphics(-0.5*(getViewRange()/DEG_TO_RAD), 0.5*(getViewRange()/DEG_TO_RAD), getViewDistance(), color, getViewDistance());
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
