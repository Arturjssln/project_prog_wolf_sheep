#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>

#ifndef OBSTACLE_H
#define OBSTACLE_H

class LivingEntity;

class Obstacle  {
	public:
		//CONSTRUCTEUR
		Obstacle() = default;

		//DESTRUCTEUR
		virtual ~Obstacle() = default;

		//MÉTHODES
		bool isColliding(const Obstacle& other) const;
		bool isPointInside(const Vec2d& point) const;
		void drawObstacle(sf::RenderTarget& targetWindow) const;

		//MÉTHODES VURTUELLES PURES
		virtual double getRadius() const = 0;
		virtual Vec2d getCenter() const = 0;

	private:
		//MÉTHODE
		double distance(const Vec2d& p1, const Vec2d& p2) const;
};

//SURCHARGE OPÉRATEURS
bool operator|(const Obstacle& body1, const Obstacle& body2);
bool operator>(const Obstacle& body, const Vec2d& point);


#endif
