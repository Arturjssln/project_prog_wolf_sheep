/*
 * infosv
 * Mar 2017
 */

#include <Tests/GraphicalTests/AnimalTest.hpp>
#include <Env/Animal.hpp>
#include <Utility/Utility.hpp>

IMPLEMENT_MAIN(AnimalTest)


void AnimalTest::onEvent(sf::Event event, sf::RenderWindow&)
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

void AnimalTest::onRun()
{
 // TODO add more stuff here
}

void AnimalTest::onSimulationStart()
{
	getAppEnv().reset();
	getAppEnv().addAnimal(new Animal({ 300, 300 }));
}
