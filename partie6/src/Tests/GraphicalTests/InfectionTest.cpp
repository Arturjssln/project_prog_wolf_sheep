/*
 * infosv
 * jan 2017
 * Marco Antognini, Ari Sarfatis
 */

#include "InfectionTest.hpp"
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/Grass.hpp>
#include <Genetics/Virus.hpp>

/*!
 * Test the infection.
 *
 * Run it with the correct cfg (res/infect.json) :
 * > scons InfectionTest-run --cfg=infect.json
 */

IMPLEMENT_MAIN(InfectionTest)

// immune profile of the animals
const std::array<double, 10> immuneProfile = {-3.73957, -0.656295, 3.14636, 1.75901, -0.038125, -3.99062, -2.04938, -3.33484, 0.784387, 1.58424};
const std::array<double, 10> immuneProfile2 = {0.981064, -0.00819049, -2.41029, 3.6962, 2.72891, 3.648, -3.51971, -3.65621, 1.71243, -1.21399};


// profiles of the tested virus
const std::array<double, 10> benignVirusProfile = {-4, -2, 2, 4, -4, -4, -2, -2, 4, 4};
const std::array<double, 10> mildVirusProfile = {-1.867503,-3.033759,-1.162219,-2.642944,4.530631,-3.841232,-3.323430,1.215671,-2.977775,0.000000};
const std::array<double, 10> mildVirusProfile2 = {0.000000,-3.835128,-4.059105,-4.655577,2.709509,-1.769458,0.000000,-4.953543,-0.961479,2.873127};
//std::array<double, 10> deadlyVirusProfile = {2.487443,-0.970834,-2.884520,0.000000,2.560836,0.670099,-2.393422,1.828886,-3.881594,2.032454};
const std::array<double, 10> deadlyVirusProfile = {-3.487443,-0.970834,-2.884520,0.000000,2.560836,0.670099,-2.393422,1.828886,-3.881594,2.032454};

// positions of the animals
const Vec2d pos1(500,500);

void InfectionTest::onRun()
{
    // Setup stats
	Application::onRun();
	addGraph(s::GENERAL, { s::WOLVES, s::SHEEP, s::GRASS, s::INFECTED,}, 0, 300);
	//addGraph(s::GENERAL, { s::WOLVES, s::SHEEP, s::GRASS,}, 0, 300);
	addGraph(s::GRASS_INDIVIDUAL, {s::ENERGY}, 0, 1000);
	addGraph(s::ANIMAL_INDIVIDUAL, {s::ENERGY, s::QUENCH, s::HEALTH, s::VIRUS, s::ADASCORE, s::IMUNAC, s::SCORE}, 0, 200);
	focusOnStat(s::GENERAL);
}

void InfectionTest::reset()
{
    mIsResetting = true;
    getEnv().reset();
    getStats().reset();
    onSimulationStart();
    createViews();
}

void InfectionTest::singleSheepCreate(const Vec2d& position, const std::array<double, 10>& immuneProfile, bool track = 1)
{
    // create sheep with specific profile & position
    Sheep* one = new Sheep(position);
    one->setImmuneGenes(immuneProfile);
    getAppEnv().addEntity(one);
    // enable tracking
    if (track)
        getAppEnv().trackEntity(position);
}

void InfectionTest::singleSheepInfect(const Vec2d& position, const std::array<double, 10>& virusProfile)
{
    // create virus & infect
    getAppEnv().infectEntity(position, new Virus(virusProfile));
}

void InfectionTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    Sheep* tmp;
    Wolf* tmp2;
    Virus* tmp3;
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::W)
        {
            tmp2 = new Wolf(getCursorPositionInView());
            std::cout << tmp2->getGenome() << std::endl;
            getAppEnv().addEntity(tmp2);
        }
        else if (event.key.code == sf::Keyboard::S)
        {
            tmp = new Sheep(getCursorPositionInView(), 1);
            std::cout << *tmp << std::endl;
            getAppEnv().addEntity(tmp);
        }
        // reset simulation and place sheep in the center
        else if (event.key.code == sf::Keyboard::Num0)
        {
            reset();
            singleSheepCreate(pos1, immuneProfile);
        }
        // testing a virus for which the immune system has a high affinity. Score rises very quickly, adaptive immune system has no time to plateau.
        // further increasing the affinity results in no visible infection at all
        else if (event.key.code == sf::Keyboard::Num1)
        {
            singleSheepInfect(pos1, benignVirusProfile);
        }
        // testing a virus for which the immune system has an average affinity. Virus rises, score rises (sigmoidally), virus reaches max and goes down.
        // in case sheep has already been infected, we observe that the sheep is immunised
        else if (event.key.code == sf::Keyboard::Num2)
        {
            singleSheepInfect(pos1, mildVirusProfile);
            //singleSheepTest(mildVirusProfile2);
        }
        // meant to infect an existing sheep with mildVirusProfile2. In case of infection with mildVirusProfile, we observe that immune system reacts more quickly, albeit less than if
        // it had been reinfected with mildVirusProfile.
        else if (event.key.code == sf::Keyboard::Num3)
        {
            singleSheepInfect(pos1, mildVirusProfile2);
        }
        // testing a virus for which the immune system has a poor affinity -> immune system cannot get rid of virus and sheep dies.
        else if (event.key.code == sf::Keyboard::Num4)
        {
            singleSheepInfect(pos1, deadlyVirusProfile);
        }
        // testing a virus for which the immune system has a poor affinity -> immune system cannot get rid of virus and sheep dies.
        else if (event.key.code == sf::Keyboard::Num9)
        {
            reset();
            singleSheepCreate(pos1, immuneProfile2);
        }
        // generate a new virus and infects. Useful when developing this test
        else if (event.key.code == sf::Keyboard::I)
        {
            Virus* tmp3 = new Virus();
			// décommentez si l'opérateur d'affichage est codé
            std::cout << "NEW VIRUS: " << *tmp3 << std::endl;
            getAppEnv().infectEntity(getCursorPositionInView(), tmp3);

        }

        else if (event.key.code == sf::Keyboard::K)
        {
            getAppEnv().killEntity(getCursorPositionInView());
        }

        else if (event.key.code == sf::Keyboard::T)
        {
            getAppEnv().trackEntity(getCursorPositionInView());
        }

        else if (event.key.code == sf::Keyboard::Z)
        {
            getAppEnv().stopTrackingAnyEntity();
        }

        else
        {

        }
    }

}
