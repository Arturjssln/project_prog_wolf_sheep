/*
 * infosv
 * Feb 2017
 */

#include <Tests/GraphicalTests/SeekingTest.hpp>
#include<iostream>

IMPLEMENT_MAIN(SeekingTest)

SeekingTest::~SeekingTest()
{
    delete mAutomaton;
    mAutomaton = nullptr;
}

void SeekingTest::onSimulationStart()
{
    mAutomaton = new SeekingAutomaton({ 300, 300 });
}

void SeekingTest::onEvent(sf::Event event, sf::RenderWindow&)
{
     if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code){

			case sf::Keyboard::T:
			{
			Vec2d const coord = getCursorPositionInView();
			 mAutomaton->setTargetPosition(coord);
			}
			
            break;
			
			default: break;
		}
	 }
}

void SeekingTest::onUpdate(sf::Time dt)
{
    mAutomaton->update(dt);
}

void SeekingTest::onDraw(sf::RenderTarget& target)
{

    mAutomaton->drawOn(target);
}

void SeekingTest::onRun()
{
 // TODO add more stuff here
}
