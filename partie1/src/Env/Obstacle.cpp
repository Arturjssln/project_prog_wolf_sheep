#include "Obstacle.hpp"
#include <cmath>

//CONSTRUCTEUR
Obstacle::Obstacle(Vec2d position, double rayon)
	: centre(position), rayon(rayon)
	{}
	
//MÉTHODES	
Vec2d Obstacle::getCenter() const {	
	return centre;
}
	
double Obstacle::getRadius() const {
	return rayon;
}

bool Obstacle::isColliding(const Obstacle& other) const {
	return (distance(centre, other.centre) <= (rayon + other.rayon));
}

bool Obstacle::isPointInside(const Vec2d& point) const {
	return (distance(centre, point) <= rayon);
}

//Fonction calculant la distance entre 2 points
double Obstacle::distance(const Vec2d& p1, const Vec2d& p2) const {
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

//SURCHARGE OPÉRATEURS
bool operator|(const Obstacle& body1, const Obstacle& body2) {
	return body1.isColliding(body2);
}

bool operator>(const Obstacle& body, const Vec2d& point) {
	return body.isPointInside(point);
}













