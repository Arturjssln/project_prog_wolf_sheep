#include <Utility/Vec2d.hpp>
#include <Utility/Constants.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm> //utiliser fonction std::min
#include "Config.hpp"
#include <Utility/Arc.hpp>
#include <Utility/Utility.hpp>
#include "LivingEntity.hpp"
#include <Genetics/Genome.hpp>
#include <string>
#include <vector>


#ifndef ANIMAL_H
#define ANIMAL_H

enum Deceleration {forte = 1, moyenne, faible};
enum Etat {
	 FOOD_IN_SIGHT, // nourriture en vue
	 FEEDING,       // en train de manger (là en principe il arrête de se déplacer)
	 RUNNING_AWAY,  // en fuite
	 MATE_IN_SIGHT, // partenaire en vue
	 MATING,        // vie privée (rencontre avec un partenaire!)
	 GIVING_BIRTH,  // donne naissance
	 WANDERING,     // déambule
	 IDLE,          // au repos
 };
enum TypeInSight { ennemi, partenaire, nourriture };

class Animal : public LivingEntity {
	public:
		//CONSTRUCTEUR
		Animal(Vec2d position, int niveauEnergie, double magnitude, Genome* pere, Genome* mere, Etat etat = WANDERING);
    Animal(Animal const&) = delete;

		//DESTRUCTEUR
    //Pas besoin de redéfinir de destructeur car les attributs contenant des pointeurs sont gérés à la fin de leur utilisation
		virtual ~Animal() = default;

		//MÉTHODES VIRTUELLES PURES
		virtual double getMass() const = 0;
    virtual double getViewRange() const = 0;
    virtual double getViewDistance() const = 0;
    virtual double getRandomWalkRadius() const = 0;
    virtual double getRandomWalkDistance() const = 0;
    virtual double getRandomWalkJitter() const = 0;
		virtual void isEating(LivingEntity* entity) = 0;
		virtual double getMinEnergyMating() const = 0;
		virtual int getMinChildren() const = 0;
		virtual int getMaxChildren() const = 0;
		virtual double getEnergyLossMatingMale() const = 0;
		virtual double getEneryLossPerChildFemale() const = 0;
		virtual sf::Time getGestationTime() const = 0;
    virtual void giveBirth() = 0;

		//METHODES
    bool isTargetInSight(const Vec2d& pos_cible) const ;
		void update(sf::Time dt) override;
		void drawOn(sf::RenderTarget& targetWindow) const override;
		bool isFemale() const;
		bool oppositeSex(Animal const* other) const;
		bool isPregnant() const;
		void setNumberChildren();

	protected:
		//METHODE VIRTUELLE PURE
		virtual double getStandardMaxSpeed() const = 0;
		virtual double getFacteurPerteEnergie() const = 0;

    //MÉTHODES
    Genome getGenome() const;
    Etat getEtat() const;

		//ATTRIBUTS
		Genome genome;
		Etat etat;
		sf::Time breakTimeAfterDoingSomething; //temps de pause après FEEDING, MATING et GIVING_BIRTH
		sf::Time gestationTime;
		unsigned int nombreEnfants;
    Genome* pere;
    bool pregnant;


	private:
		//ATTRIBUTS
		Vec2d direction;
		double magnitude;
    Vec2d currentTarget;
		Vec2d positionCible;
		double deceleration; //qui peut être modifié avec la méthode setDeceleration()
		TypeInSight typeInSight;
    std::vector<LivingEntity*> ennemisLesPlusProches;

		//MÉTHODES
		//GETTERS
		Vec2d getSpeedVector() const;
		double getMaxSpeed() const;
		double getAnimalCriticalEnergyThreshold() const;
		double getRotation() const;
 		//SETTERS
		void setRotation(double angle);
		void setTargetPosition(const Vec2d& newPosition);
    void setDeceleration(Deceleration d);
		//MISE À JOUR
		void updateDonnees(sf::Time dt, const Vec2d& force); //prend en compte la force
		void updateState();
		//CALCUL DE FORCE
		Vec2d calculForce() const;
		Vec2d calculStoppingForce() const;
    Vec2d calculPredatorForce() const;
		//DESSIN/AFFICHAGE
		void drawVision(sf::RenderTarget& targetWindow) const;
		void drawCercleVision(sf::RenderTarget& targetWindow) const;
		void drawVirtualTarget(sf::RenderTarget& targetWindow) const;
    void drawPregnantCircle(sf::RenderTarget& targetWindow) const;
		void drawTextDebug(sf::RenderTarget& targetWindow) const;
		std::string afficheState() const;
		std::string afficheSex() const;
		//AUTRES
		std::vector<LivingEntity*> analyzeEnvironnement();
		std::vector<LivingEntity*> lesPlusProches(TypeInSight elementRecherche, std::vector<LivingEntity*> ensembleEntities) const;
		Vec2d randomWalk();
		Vec2d ConvertToGlobalCoord(const Vec2d& vecteur) const;
		bool satisfied() const;
};

#endif
