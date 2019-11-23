/*
 * infosv
 * Feb 2017
 *
 */

#ifndef INFOSV_ENV_TEST_HPP
#define INFOSV_ENV_TEST_HPP

#include <Application.hpp>

/*!
 * Test the Env and its targets
 *
 * Generate one target on click
 */
class EnvTest : public Application
{
public:
     EnvTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual void onRun() override final;
    virtual void onSimulationStart() override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

#endif // INFOSV_ENV_TEST_HPP
