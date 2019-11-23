/*
 * infosv
 * Mar 2017
 */

#ifndef INFOSV_ANIMAL_TEST_HPP
#define INFOSV_ANIMAL_TEST_HPP

#include <Application.hpp>

/*!
 * Test the Animal : random walk + target in sight
 *
 * Generate one target on click
 */
class AnimalTest : public Application
{
public:
     AnimalTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual void onRun() override final;
    virtual void onSimulationStart() override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

#endif // INFOSV_ANIMAL_TEST_HPP

