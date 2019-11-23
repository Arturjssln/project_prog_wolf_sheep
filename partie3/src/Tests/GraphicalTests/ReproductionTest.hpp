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
 * Run it with the correct cfg (res/tests/reprod.cfg) :
 * > scons test_reproduction-run --cfg=tests/reprod.cfg
 */
class ReproductionTest : public Application
{
public:
	ReproductionTest(int argc, char const** argv)
		:Application(argc, argv)
		{}
	virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

#endif // INFOSV_REPRODUCTION_TEST_HPP

