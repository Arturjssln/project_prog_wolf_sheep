/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#ifndef INFOSV_REPRODUCTION_TEST_HPP
#define INFOSV_REPRODUCTION_TEST_HPP

#include <Application.hpp>
#include <functional>

/*!
 * Test the reproduction.
 *
 * Run it with the correct json :
 * > scons InfectionTestManyAnimals-run --cfg=infectManyAnimals.json
 */
class InfectionTestManyAnimals : public Application
{
public:
	InfectionTestManyAnimals(int argc, char const** argv)
		:Application(argc, argv)
		{}
	virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onRun() override final
    {
        // Setup stats
        Application::onRun();
        addGraph(s::GENERAL, { s::WOLVES, s::SHEEP, s::GRASS, s::INFECTED,}, 0, 150);
        addGraph(s::GRASS_INDIVIDUAL, {s::ENERGY}, 0, 1000);
        addGraph(s::ANIMAL_INDIVIDUAL, {s::ENERGY, s::HEALTH, s::VIRUS, s::ADASCORE, s::IMUNAC, s::SCORE}, 0, 2250);
        focusOnStat(s::GENERAL);
    }


	void singleSheepCreate(const Vec2d& position);
	void singleWolfCreate(const Vec2d& position);

	void fieldSheepCreate(int p1, int p2, int nb, int p3, int p4, int nb2, int infect);
	void fieldWolvesCreate(int p1, int p2, int nb, int p3, int p4, int nb2, int infect);
	void fieldAnimalCreate(std::function<void(const Vec2d&)> singleAnimalCreate, int p1, int p2, int nb, int p3, int p4, int nb2, int infect);

	void singleAnimalInfect(const Vec2d& position, const std::array<double, 10>& virusProfile);
	void reset();
};

#endif // INFOSV_REPRODUCTION_TEST_HPP

