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
	 DRINK_IN_SIGHT,// boisson en vue
	 FEEDING,       // en train de manger (là en principe il arrête de se déplacer)
	 DRINKING,			// en train de boire
	 RUNNING_AWAY,  // en fuite
	 MATE_IN_SIGHT, // partenaire en vue
	 MATING,        // vie privée (rencontre avec un partenaire!)
	 GIVING_BIRTH,  // donne naissance
	 WANDERING,     // déambule
	 IDLE,          // au repos
	 HALLUCINATING	// hallucine à cause des champignons (empoisonné)
 };

enum TypeInSight { ennemi, partenaire, nourriture, drink };


class Animal : public LivingEntity {
	public:
		//CONSTRUCTEUR & DESTRUCTEUR
		Animal(Vec2d position, double energyLevel, double magnitude, Genome* pere, Genome* mere,
			     int numTroupeau, double quenchLevel, Etat state = WANDERING);
    Animal(Animal const&) = delete;
		Animal& operator=(Animal const&) = delete;

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
		virtual void isDrinking(LivingEntity* entity) = 0;
		virtual double getMinEnergyMating() const = 0;
		virtual double getMinQuenchMating() const = 0;
		virtual int getMinChildren() const = 0;
		virtual int getMaxChildren() const = 0;
		virtual double getEnergyLossMatingMale() const = 0;
		virtual double getEneryLossPerChildFemale() const = 0;
		virtual sf::Time getGestationTime() const = 0;
    virtual void giveBirth() = 0;

		//GETTERS
		Genome getGenome() const;
		std::array<double,7> getAnimalData() const override final;
		std::string getStatLabel() const override;
		//AUTRES
		bool isAlive() const override;
		bool isFemale() const;
		bool oppositeSex(Animal const* other) const;
		bool isPregnant() const;
		//SETTERS
		void setNumberChildren();
		void setImmuneGenes(std::array<double, TAILLE_PROFIL_IMMU> profil);
		//AUTRES
		void becomeInfected(Virus* v) override final;
		//MÉTHODE DE MISE À JOUR
		void update(sf::Time dt) override;
    bool isTargetInSight(const Vec2d& pos_cible) const ;
		void drawOn(sf::RenderTarget& targetWindow) const override final;
		//MÉTHODE DE DESSIN/AFFICHAGE
		void afficheInnateProfile(std::ostream& out) const;

	protected:
		//MÉTHODES VIRTUELLES PURES
		virtual double getStandardMaxSpeed() const = 0;
		virtual double getEnergyLossFactor() const = 0;
		virtual double getWaterLossFactor() const = 0;
		virtual void drawCrown(sf::RenderTarget& targetWindow) const = 0;
		virtual void drawInfoHerd(sf::RenderTarget& targetWindow) const = 0;
		virtual bool isFree() const = 0;
		virtual int getNumHerd() const = 0;
		virtual void setIsAbleToBeFollowed(bool b) = 0;
		virtual bool isLeader() const = 0;
		virtual bool isPoisoned() const = 0;
		virtual void decreasePoisonedTime(sf::Time dt) = 0;

    //MÉTHODES
		double getRotation() const;
		Vec2d ConvertToGlobalCoord(const Vec2d& vecteur) const;

		//ATTRIBUTS
		Vec2d direction;
		Genome genome;
		Etat state;
		//PAUSES
		sf::Time breakTimeAfterDoingSomething; //temps de pause après FEEDING, MATING et GIVING_BIRTH
		sf::Time breakTime; //temps de pause pour le cas IDLE
		//REPRODUCTION
		sf::Time gestationTime;
		unsigned int numberOfChild;
    Genome* pere;
    bool pregnant;
		//LEADER
    int numHerd;
    bool leader;
    bool isAbleToBeFollowed;
		//IMMUNITÉ
		ImmuneSystem immunity;
		//SOIF
		double quenchLevel;

	private:
		//ATTRIBUTS
		double magnitude;
    Vec2d currentTarget;
		Vec2d positionCible;
		double deceleration;
		TypeInSight typeInSight;
    std::vector<LivingEntity*> closestEnemies;
		LivingEntity* closestPartner;
		LivingEntity* closestFood;
		LivingEntity* closestDrink;
		std::vector<LivingEntity*> neighbors;

		//MÉTHODES
		//GETTERS
		Vec2d getSpeedVector() const;
		double getMaxSpeed() const;
		double getAnimalCriticalEnergyThreshold() const;
		double getAnimalCriticalQuenchThreshold() const;
		//AUTRES
		bool satisfied() const;
		bool quenched() const;
 		//SETTERS
		void setRotation(double angle);
		void setTargetPosition(const Vec2d& newPosition);
    void setDeceleration(Deceleration d);
		//MÉTHODE DE MISE À JOUR
		void updateDonnees(sf::Time dt, const Vec2d& force); //prend en compte la force
		void updateState();
		void updateClosest(std::list<LivingEntity*> ensembleEntities);
		//CALCUL DE FORCE
		Vec2d calculForce() const;
		Vec2d calculStoppingForce() const;
    Vec2d calculPredatorForce() const;
		Vec2d calculAvoidanceForce() const;
		//MÉTHODES DE MARCHE
		Vec2d randomWalk();
		Vec2d randomPoisonedWalk();
		//AUTRES
		void analyzeEnvironnement();
		//DESSIN/AFFICHAGE
		void drawVision(sf::RenderTarget& targetWindow) const;
		void drawCircleVision(sf::RenderTarget& targetWindow) const;
		void drawVirtualTarget(sf::RenderTarget& targetWindow) const;
    void drawPregnantCircle(sf::RenderTarget& targetWindow) const;
		void drawTextDebug(sf::RenderTarget& targetWindow) const;
		void drawInfected(sf::RenderTarget& targetWindow) const;
		void drawTrack(sf::RenderTarget& targetWindow) const override final;
		std::string displayState() const;
		std::string displaySex() const;


};

#endif
