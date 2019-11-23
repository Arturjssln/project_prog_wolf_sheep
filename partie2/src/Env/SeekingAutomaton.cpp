#include "SeekingAutomaton.hpp"

//CONSTRUCTEUR
SeekingAutomaton::SeekingAutomaton(Vec2d position, double magnitude)
: position(position), direction(Vec2d(1,0)), magnitude(magnitude)
{
    setDeceleration(moyenne);
}

//MÉTHODES
double SeekingAutomaton::getStandardMaxSpeed() const {
	return SEEKING_AUTOMATON_MAX_SPEED;
}

double SeekingAutomaton::getMass() const {
	return SEEKING_AUTOMATON_MASS;
}

double SeekingAutomaton::getRadius() const {
	return SEEKING_AUTOMATON_RADIUS;
}

Vec2d SeekingAutomaton::getSpeedVector() const {
    return direction*magnitude;
}

void SeekingAutomaton::setTargetPosition(const Vec2d& newPosition) {
	positionCible = newPosition;
}

void SeekingAutomaton::setDeceleration(Deceleration d) {
    deceleration = d * DECELERATION;
    //où DECELERATION est une constante (dans le ficheir Constants.hpp)
}

void SeekingAutomaton::update(sf::Time dt) {
	updateDonnees(dt);
}

void SeekingAutomaton::drawOn(sf::RenderTarget& targetWindow) const {
	sf::CircleShape cercleCible(5);
	cercleCible.setFillColor(sf::Color(255,0,0));
	cercleCible.setPosition(positionCible);
	cercleCible.setOrigin(5,5);

	sf::CircleShape cercleAutomaton(getRadius());
	cercleAutomaton.setFillColor(sf::Color(136,66,29));
	cercleAutomaton.setPosition(position);
	cercleAutomaton.setOrigin(getRadius(), getRadius());

	targetWindow.draw(cercleCible);
	targetWindow.draw(cercleAutomaton);
}


Vec2d SeekingAutomaton::calculForce() const {
	Vec2d toTarget(positionCible - position);
	if (toTarget.lengthSquared() == 0.0) { return Vec2d(0,0); }
    //on utilise lenghtSquared et pas lenght car elle est moins coûteuse en mémoire

	double speed(std::min(toTarget.length()/deceleration, getStandardMaxSpeed()));

	Vec2d vTarget((toTarget.normalised())*speed);
	return vTarget - getSpeedVector();
}

//Mise à jour des données de l'animal (position, magnitude, direction)
void SeekingAutomaton::updateDonnees(sf::Time dt) {
	Vec2d acceleration(calculForce()/getMass());
	Vec2d newVitesse(getSpeedVector() + acceleration * dt.asSeconds());
	direction = newVitesse.normalised();

	if (newVitesse.length() > getStandardMaxSpeed()) {
		newVitesse = direction * getStandardMaxSpeed();
	}

	position += newVitesse * dt.asSeconds();
	magnitude = newVitesse.length();
}
