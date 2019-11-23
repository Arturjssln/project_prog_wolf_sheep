#include <Utility/Vec2d.hpp>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm> //utiliser fonction std::min
#include "Config.hpp"
#include <Utility/Arc.hpp>
#include <Utility/Utility.hpp>

#ifndef ANIMAL_H
#define ANIMAL_H

enum Deceleration {forte = 1, moyenne, faible};

class Animal {
	public:
		//CONSTRUCTEUR
		Animal(Vec2d position = Vec2d(0,0), double magnitude = 0);
		//DESTRUCTEUR
		~Animal() = default;

		//MÉTHODES
		double getStandardMaxSpeed() const;
		double getMass() const;
		double getRadius() const;
    Vec2d getSpeedVector() const;
    double getViewRange() const;
    double getViewDistance() const;
    double getRotation() const;
    Vec2d getPosition() const;
    double getRandomWalkRadius() const;
    double getRandomWalkDistance() const;
    double getRandomWalkJitter() const;
    void setRotation(double angle);
		void setTargetPosition(const Vec2d& newPosition);
    void setDeceleration(Deceleration d);
    void setPosition(); //monde torique
    bool isTargetInSight(const Vec2d& posCible) const;
		void update(sf::Time dt);
		void drawOn(sf::RenderTarget& targetWindow) const; //Faudrait le mettre const !

	private:
		//ATTRIBUTS
		Vec2d position;
		Vec2d direction;
		double magnitude;
		Vec2d positionCible;
    double deceleration; //qui peut être modifié avec la méthode setDeceleration()

    Vec2d currentTarget;


		//MÉTHODES
		Vec2d calculForce() const;
		void updateDonnees(sf::Time dt, const Vec2d& force); //prend en compte la force
		void drawVision(sf::RenderTarget& targetWindow) const;
		void drawCercleVision(sf::RenderTarget& targetWindow) const;
		void drawVirtualTarget(sf::RenderTarget& targetWindow) const;
		Vec2d randomWalk();
		Vec2d ConvertToGlobalCoord(const Vec2d& vecteur) const;
		bool aucuneCibleReelle() const;
		Vec2d cibleLaPlusProche(std::vector<Vec2d> ensembleCibles) const;
};

#endif
