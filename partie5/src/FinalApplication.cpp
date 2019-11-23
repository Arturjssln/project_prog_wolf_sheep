/*
 * prjsv 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#include <Config.hpp>
#include <FinalApplication.hpp>
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/Grass.hpp>
#include <iostream>
#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
    // Setup stats
	Application::onRun();
	addGraph(s::GENERAL, { s::WOLVES, s::SHEEP, s::GRASS, s::INFECTED}, 0, 300);
	addGraph(s::GRASS_INDIVIDUAL, {s::ENERGY}, 0, 1000);
	addGraph(s::ANIMAL_INDIVIDUAL, {s::ENERGY, s::HEALTH, s::VIRUS, s::ADASCORE, s::IMUNAC, s::SCORE}, 0, 1000);
	focusOnStat(s::GENERAL);

}

void FinalApplication::onSimulationStart()
{
    // TODO add more stuff here
}

void FinalApplication::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code){

			case sf::Keyboard::W:
				getAppEnv().addEntity(new Wolf(getCursorPositionInView()));
				break;

			case sf::Keyboard::S:
				getAppEnv().addEntity(new Sheep(getCursorPositionInView()));
				break;

			case sf::Keyboard::Num1:
				getAppEnv().addEntity(new Sheep(getCursorPositionInView(), 1));
				break;

			case sf::Keyboard::Num2:
				getAppEnv().addEntity(new Sheep(getCursorPositionInView(), 2));
				break;

			case sf::Keyboard::F1:
        getAppEnv().addEntity(new Wolf(getCursorPositionInView(), -1));
        break;

		  case sf::Keyboard::F2:
        getAppEnv().addEntity(new Wolf(getCursorPositionInView(), -2));
        break;

      case sf::Keyboard::F3:
        getAppEnv().addEntity(new Wolf(getCursorPositionInView(), -3));
        break;

			case sf::Keyboard::I:
				getAppEnv().infectEntity(getCursorPositionInView());
				break;

			case sf::Keyboard::K:
				getAppEnv().killEntity(getCursorPositionInView());
				break;

			case sf::Keyboard::T:
				getAppEnv().trackEntity(getCursorPositionInView());
				break;

			case sf::Keyboard::Z:
				getAppEnv().stopTrackingAnyEntity();
				break;

			case sf::Keyboard::G:
				getAppEnv().addEntity(new Grass(getCursorPositionInView()));
				break;

        default:
            break;
        }

    }
}
