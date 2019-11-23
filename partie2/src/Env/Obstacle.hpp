#include <Utility/Vec2d.hpp> 

#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle {
	public:
		//CONSTRUCTEUR
		Obstacle(Vec2d position, double rayon);
		
		//MÉTHODES
		Vec2d getCenter() const;
		double getRadius() const;
		bool isColliding(const Obstacle& other) const;
		bool isPointInside(const Vec2d& point) const;
		
	private:
		//ATTRIBUTS
		Vec2d centre;
		double rayon;
		
		//MÉTHODE
		double distance(const Vec2d& p1, const Vec2d& p2) const;
};

//SURCHARGE OPÉRATEURS
bool operator|(const Obstacle& body1, const Obstacle& body2);
bool operator>(const Obstacle& body, const Vec2d& point);


#endif
