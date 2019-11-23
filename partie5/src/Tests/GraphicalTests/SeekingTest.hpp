/*
 * infosv
 * Feb 2017
 */

#ifndef INFOSV_SEEKING_TEST_HPP
#define INFOSV_SEEKING_TEST_HPP

#include <Application.hpp>
#include <Env/SeekingAutomaton.hpp>

/*!
 * Test the SeekingAutomaton
 *
 * On click, set the target of the seeking automaton
 */
class SeekingTest : public Application
{
public:
	SeekingTest(int argc, char const** argv)
		: Application(argc, argv)
		{
		}

    virtual ~SeekingTest();
    virtual void onSimulationStart() override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onUpdate(sf::Time dt) override final;
    virtual void onDraw(sf::RenderTarget& target) override final;
	virtual void onRun() override final;

private:
    SeekingAutomaton* mAutomaton;
};

#endif // INFOSV_SEEKING_TEST_HPP

