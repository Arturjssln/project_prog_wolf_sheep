#include <Utility/Vec2d.hpp>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm> //utiliser fonction std::min

#ifndef	SEEKING_AUTOMATON_H
#define SEEKING_AUTOMATON_H

enum Deceleration {forte = 1, moyenne, faible};

class SeekingAutomaton {
	public:
		//CONSTRUCTEUR
		SeekingAutomaton(Vec2d position = Vec2d(0,0), double magnitude = 0);
		//DESTRUCTEUR
		~SeekingAutomaton() = default;

		//MÉTHODES
		double getStandardMaxSpeed() const;
		double getMass() const;
		double getRadius() const;
    Vec2d getSpeedVector() const;
		void setTargetPosition(const Vec2d& new_position);
    void setDeceleration(Deceleration d);
		void update(sf::Time dt);
		void drawOn(sf::RenderTarget& targetWindow) const;

	private:
		//ATTRIBUTS
		Vec2d position;
		Vec2d direction;
		double magnitude;
		Vec2d positionCible;
    double deceleration; //qui peut être modifié avec la méthode setDeceleration()

		//MÉTHODES
		Vec2d calculForce() const;
		void updateDonnees(sf::Time dt);

};

#endif
