/*
* prjsv 2016
* 2016
* Marco Antognini
*/

#include <Application.hpp>
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/LivingEntity.hpp>
#include <Env/Grass.hpp>
#include <Config.hpp>
#include <catch.hpp>
#include <iostream>

SCENARIO("Sheep vs LivingEntity", "[Sheep]")
{

		LivingEntity* sheep1(new Sheep({0,0}));
        LivingEntity* sheep2(new Sheep({0,0}));
        LivingEntity* wolf1(new Wolf({0,0}));
        LivingEntity* food1(new Grass({0,0}));

		GIVEN("a sheep")
		{
			THEN("no autophagy")
			{
				CHECK_FALSE(sheep1->eatable(sheep1));
			}
		}
		GIVEN("two sheeps")
		{
			THEN("no cannibalism")
			{
				CHECK_FALSE(sheep1->eatable(sheep2));
				CHECK_FALSE(sheep2->eatable(sheep1));
			}
		}

		GIVEN("a sheep and a plant")
		{
	   
			THEN("the plant is eatable by the sheep")
			{
				CHECK(sheep1->eatable(food1));
			}
			THEN("the sheep is not eatable by the plant")	
			{
				CHECK_FALSE(food1->eatable(sheep1));
			}
		}

		GIVEN("a sheep and a wolf")
		{
	   
			THEN("the sheep is eatable by the wolf")
			{
				CHECK(wolf1->eatable(sheep1));
			}
			THEN("the wolf is not eatable by the sheep")	
			{
				CHECK_FALSE(sheep1->eatable(wolf1));
			}
		}
}

SCENARIO("Grass vs LivingEntity", "[Grass]")
{

		LivingEntity* sheep1(new Sheep({0,0}));
        LivingEntity* wolf1(new Wolf({0,0}));
        LivingEntity* food1(new Grass({0,0}));
        LivingEntity* food2(new Grass({0,0}));
		GIVEN("a plant")
		{
			THEN("no autophagy")
			{
				CHECK_FALSE(food1->eatable(food1));
			}
		}
		GIVEN("two plants")
		{
			THEN("no cannibalism")
			{
				CHECK_FALSE(food1->eatable(food2));
				CHECK_FALSE(food2->eatable(food1));
			}
		}

		GIVEN("a plant and a wolf")
		{
	   
			THEN("the plant is not eatable by the wolf")
			{
				CHECK_FALSE(wolf1->eatable(food1));
			}
			THEN("the wolf is not eatable by the plant")	
			{
				CHECK_FALSE(food1->eatable(wolf1));
			}
		}

		GIVEN("a plant and a sheep")
		{
	   
			THEN("the plant is eatable by the sheep")
			{
				CHECK(sheep1->eatable(food1));
			}
			THEN("the sheep is not eatable by the plant")	
			{
				CHECK_FALSE(food1->eatable(sheep1));
			}
		}
}

SCENARIO("Wolf vs LivingEntity", "[Wolf]")
{

		LivingEntity* sheep1(new Sheep({0,0}));
        LivingEntity* wolf1(new Wolf({0,0}));
        LivingEntity* wolf2(new Wolf({0,0}));
        LivingEntity* food1(new Grass({0,0}));
		
		GIVEN("a wolf")
		{
			THEN("no autophagy")
			{
				CHECK_FALSE(wolf1->eatable(wolf1));
			}
		}
		GIVEN("two wolves")
		{
			THEN("no cannibalism")
			{
				CHECK_FALSE(wolf1->eatable(wolf2));
				CHECK_FALSE(wolf2->eatable(wolf1));
			}
		}

		GIVEN("a Wolf and a plant")
		{
	   
			THEN("the plant is not eatable by the wolf")
			{
				CHECK_FALSE(wolf1->eatable(food1));
			}
			THEN("the wolf is not eatable by the plant")	
			{
				CHECK_FALSE(food1->eatable(wolf1));
			}
		}

		GIVEN("a wolf and a sheep")
		{
	   
			THEN("the sheep is eatable by the wolf")
			{
				CHECK(wolf1->eatable(sheep1));
			}
			THEN("the wolf is not eatable by the sheep")	
			{
				CHECK(wolf1->eatable(sheep1));
			}
		}
}
