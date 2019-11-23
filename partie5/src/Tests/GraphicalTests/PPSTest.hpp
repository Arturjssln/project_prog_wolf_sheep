/*
 * infosv
 * jan 2017
 */

#ifndef INFOSV_PPS_TEST_HPP
#define INFOSV_PPS_TEST_HPP

#include <Application.hpp>

/*!
 *  Test prey-predator simulation
 *
 * Generate one target on click
 */

class PPSTest : public Application
{
public:
	PPSTest(int argc, char const** argv)
		: Application(argc, argv)
		{
		}
	virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};
#endif // INFOSV_PPS_TEST_HPP
