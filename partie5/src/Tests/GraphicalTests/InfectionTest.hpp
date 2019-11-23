/*
 * infosv
 * jan 2012
 * Marco Antognini
 */

#ifndef INFOSV_REPRODUCTION_TEST_HPP
#define INFOSV_REPRODUCTION_TEST_HPP

#include <Application.hpp>

/*!
 * Test the reproduction.
 *
 * Run it with the correct cfg (res/infect.cfg) :
 * > scons test_reproduction-run --cfg=infect.cfg
 */
class InfectionTest : public Application
{
public:
	InfectionTest(int argc, char const** argv)
		:Application(argc, argv)
		{}
    virtual void onRun() override final;
	virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
//	void singleSheepTest(std::array<double, 10>& virusProfile);
	void singleSheepCreate(const Vec2d& position, const std::array<double, 10>& immuneProfile, bool track);
	void singleSheepInfect(const Vec2d& position, const std::array<double, 10>& virusProfile);
	void reset();
};

	
#endif // INFOSV_REPRODUCTION_TEST_HPP

