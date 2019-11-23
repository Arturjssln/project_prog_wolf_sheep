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
enum TypeInSight { ennemi, partenaire, nourriture, promenade };

class Animal : public LivingEntity {
	public:
		//CONSTRUCTEUR
		Animal(Vec2d position, double energyLevel, double magnitude, Genome* pere, Genome* mere, int numTroupeau, Etat state = WANDERING);
    Animal(Animal const&) = delete;
		Animal& operator=(Animal const&) = delete;

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
		Genome getGenome() const;
		void update(sf::Time dt) override;
		void drawOn(sf::RenderTarget& targetWindow) const override final;
		bool isFemale() const;
		bool oppositeSex(Animal const* other) const;
		bool isPregnant() const;
		void setNumberChildren();
		std::string getStatLabel() const override;
		std::array<double,6> getAnimalData() const override final;
		bool isAlive() const override;
		void becomeInfected(Virus* v) override final;
		void setImmuneGenes(std::array<double, TAILLE_PROFIL_IMMU> profil);
		void afficheInnateProfile(std::ostream& out) const;

	protected:
		//METHODE VIRTUELLE PURE
		virtual double getStandardMaxSpeed() const = 0;
		virtual double getEnergyLossFactor() const = 0;
		virtual void drawCrown(sf::RenderTarget& targetWindow) const = 0;
		virtual void drawInfoHerd(sf::RenderTarget& targetWindow) const = 0;
		virtual bool isFree() const = 0;
		virtual int getNumHerd() const = 0;
		virtual void setIsAbleToBeFollowed(bool b) = 0;
		virtual bool isLeader() const = 0;

    //MÉTHODES
		double getRotation() const;
		Vec2d ConvertToGlobalCoord(const Vec2d& vecteur) const;

		//ATTRIBUTS
		Vec2d direction;
		Genome genome;
		Etat state;
		sf::Time breakTimeAfterDoingSomething; //temps de pause après FEEDING, MATING et GIVING_BIRTH
		sf::Time breakTime; //temps de pause pour le cas IDLE
		sf::Time gestationTime;
		unsigned int numberOfChild;
    Genome* pere;
    bool pregnant;
    int numHerd;
    bool leader;
    bool isAbleToBeFollowed;

		ImmuneSystem immunity;


	private:
		//ATTRIBUTS
		double magnitude;
    Vec2d currentTarget;
		Vec2d positionCible;
		double deceleration; //qui peut être modifié avec la méthode setDeceleration()
		TypeInSight typeInSight;
    std::vector<LivingEntity*> closestEnemies;
		LivingEntity* closestPartner;
		LivingEntity* closestFood;
		std::vector<LivingEntity*> neighbors;

		//MÉTHODES
		//GETTERS
		Vec2d getSpeedVector() const;
		double getMaxSpeed() const;
		double getAnimalCriticalEnergyThreshold() const;

 		//SETTERS
		void setRotation(double angle);
		void setTargetPosition(const Vec2d& newPosition);
    void setDeceleration(Deceleration d);
		//MISE À JOUR
		void updateDonnees(sf::Time dt, const Vec2d& force); //prend en compte la force
		void updateState();
		void updateInfection();
		//CALCUL DE FORCE
		Vec2d calculForce() const;
		Vec2d calculStoppingForce() const;
    Vec2d calculPredatorForce() const;
		Vec2d calculAvoidanceForce() const;
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
		//AUTRES
		void analyzeEnvironnement();
		void updateClosest(std::list<LivingEntity*> ensembleEntities);
		Vec2d randomWalk();
		bool satisfied() const;
};

#endif
