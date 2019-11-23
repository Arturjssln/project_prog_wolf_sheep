/*
 * infosv
 * jan 2017
 * Marco Antognini, Ari Sarfatis
 */

#include "ReproductionTest.hpp"
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/Grass.hpp>

IMPLEMENT_MAIN(ReproductionTest)

Animal* create_female(const Vec2d& position, char type)
{
	Animal* female(nullptr);

	constexpr  size_t COUNT_MAX(1000);
	size_t count(0);
	while (female == nullptr && count < COUNT_MAX)
	{
		switch (type)
		{
			case 'W' :
				female = new Wolf(position);
				break;
			case 'S' :
				female = new Sheep(position);
				break;
			default:
				female = nullptr;
		}
		if (!(female->isFemale()))
		{
			delete female;
			female = nullptr;
		}
	}
	
	if (count >= COUNT_MAX) throw "test failed: no female could be created";
	return female;
}

Animal* create_male(const Vec2d& position, char type)
{
	Animal* male(nullptr);

	constexpr  size_t COUNT_MAX(1000);
	size_t count(0);
	while (male == nullptr && count < COUNT_MAX)
	{
		switch (type)
		{
			case 'W' :
				male = new Wolf(position);
				break;
			case 'S' :
				male = new Sheep(position);
				break;
			default:
				male = nullptr;
		}
		if (male->isFemale())
		{
			delete male;
			male = nullptr;
		}
	}
	
	if (count >= COUNT_MAX) throw "test failed: no male could be created";
	return male;
}


void ReproductionTest::onEvent(sf::Event event, sf::RenderWindow&)
{
if (event.type == sf::Event::KeyPressed) {
	switch (event.key.code) {
		case sf::Keyboard::W:
			if(event.key.shift)
				getAppEnv().addEntity(create_male(getCursorPositionInView(), 'W'));
			else
				getAppEnv().addEntity(create_female(getCursorPositionInView(), 'W'));
			break; 
		case sf::Keyboard::S:
			if(event.key.shift)
				getAppEnv().addEntity(create_male(getCursorPositionInView(), 'S'));
			else
				getAppEnv().addEntity(create_female(getCursorPositionInView(), 'S'));
			break;
		case sf::Keyboard::G:
          getAppEnv().addEntity(new Grass(getCursorPositionInView()));
			break;
                
		default:
			break;
	}

}

}
