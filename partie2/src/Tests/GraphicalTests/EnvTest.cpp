/*
 * infosv
 * Feb 2017
 */

#include <Tests/GraphicalTests/EnvTest.hpp>
#include <iostream>

IMPLEMENT_MAIN(EnvTest)

void EnvTest::onEvent(sf::Event event, sf::RenderWindow&)
{
	 if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code){

			case sf::Keyboard::T:
			{
			Vec2d const coord = getCursorPositionInView();
			getAppEnv().addTarget(coord);
			}
			
            break;
			
			default: break;
		}
	 }
}

void EnvTest::onRun()
{
 // TODO add more stuff here
}

void EnvTest::onSimulationStart()
{
    // TODO add more stuff here
}
