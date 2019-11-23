#include "Obstacle.hpp"
#include <cmath>

//MÉTHODES
bool Obstacle::isColliding(const Obstacle& other) const {
	return (distance(getCenter(), other.getCenter()) <= (getRadius() + other.getRadius()));
}

bool Obstacle::isColliding(const Obstacle& other, double radius) const {
	return (distance(getCenter(), other.getCenter()) <= (radius + other.getRadius()));
}

bool Obstacle::isPointInside(const Vec2d& point) const {
	return (distance(getCenter(), point) <= getRadius());
}

//Fonction calculant la distance entre 2 points
double Obstacle::distance(const Vec2d& p1, const Vec2d& p2) const {
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

void Obstacle::drawObstacle(sf::RenderTarget& targetWindow) const {
	sf::CircleShape cercleObstacle(getRadius());
	cercleObstacle.setFillColor(sf::Color(0,150,0,50));
	cercleObstacle.setPosition(getCenter());
	cercleObstacle.setOrigin(getRadius(), getRadius());
	targetWindow.draw(cercleObstacle);
}

//SURCHARGE OPÉRATEURS
bool operator|(const Obstacle& body1, const Obstacle& body2) {
	return body1.isColliding(body2);
}

bool operator>(const Obstacle& body, const Vec2d& point) {
	return body.isPointInside(point);
}
