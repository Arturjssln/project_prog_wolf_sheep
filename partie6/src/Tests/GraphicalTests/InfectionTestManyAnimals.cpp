/*
 * infosv
 * jan 2017
 * Marco Antognini, Ari Sarfatis
 */

#include "InfectionTestManyAnimals.hpp"
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/Grass.hpp>
#include <Genetics/Virus.hpp>

/*!
 * Test the infection.
 *
 * Run it with the correct cfg (res/infect2Animals.json) :
 * > scons InfectionTestManyAnimals-run --cfg=infect2Animals.json
 */



IMPLEMENT_MAIN(InfectionTestManyAnimals)

// immune profile of the animals
std::array<double, 10> immuneProfile = {-3.73957, -0.656295, 3.14636, 1.75901, -0.038125, -3.99062, -2.04938, -3.33484, 0.784387, 1.58424};

// profiles of the tested virus
std::array<double, 10> benignVirusProfile = {-4, -2, 2, 4, -4, -4, -2, -2, 4, 4};
std::array<double, 10> mildVirusProfile = {-1.867503,-3.033759,-1.162219,-2.642944,4.530631,-3.841232,-3.323430,1.215671,-2.977775,0.000000};
std::array<double, 10> mildVirusProfile2 = {0.000000,-3.835128,-4.059105,-4.655577,2.709509,-1.769458,0.000000,-4.953543,-0.961479,2.873127};
//std::array<double, 10> deadlyVirusProfile = {2.487443,-0.970834,-2.884520,0.000000,2.560836,0.670099,-2.393422,1.828886,-3.881594,2.032454};
std::array<double, 10> deadlyVirusProfile = {-3.487443,-0.970834,-2.884520,0.000000,2.560836,0.670099,-2.393422,1.828886,-3.881594,2.032454};

// positions of the animals
Vec2d pos1(1500,1500);
Vec2d pos2(1325,1325);

double original = 0;

void InfectionTestManyAnimals::reset()
{
    mIsResetting = true;
    getEnv().reset();
    getAppEnv().stopTrackingAnyEntity();
}

// to make manual tests
void InfectionTestManyAnimals::singleSheepCreate(const Vec2d& position)
{
    // create sheep with specific profile & position
    Sheep* one = new Sheep(position);
    one->setImmuneGenes(immuneProfile);
    getAppEnv().addEntity(one);
}

// to make manual tests
void InfectionTestManyAnimals::singleWolfCreate(const Vec2d& position)
{
    // create sheep with specific profile & position
    Wolf* one = new Wolf(position);
    one->setImmuneGenes(immuneProfile);
    getAppEnv().addEntity(one);
}

void InfectionTestManyAnimals::fieldSheepCreate(int p1, int p2, int nb, int p3, int p4, int nb2, int infect = 1)
{
    std::function<void(Vec2d)> singleAnimalCreate = std::bind(&InfectionTestManyAnimals::singleSheepCreate, this, std::placeholders::_1);
    fieldAnimalCreate(singleAnimalCreate, p1, p2, nb, p3, p4, nb2, infect);
}

void InfectionTestManyAnimals::fieldWolvesCreate(int p1, int p2, int nb, int p3, int p4, int nb2, int infect = 1)
{
    std::function<void(Vec2d)> singleAnimalCreate = std::bind(&InfectionTestManyAnimals::singleWolfCreate, this, std::placeholders::_1);
    fieldAnimalCreate(singleAnimalCreate, p1, p2, nb, p3, p4, nb2, infect);
}

// creates a field of animals : nb animals linearly spaced from p1 to p2 along x axis, and nb2 animals from p3 to p4 on y axis.
void InfectionTestManyAnimals::fieldAnimalCreate(std::function<void(const Vec2d&)> singleAnimalCreate, int p1, int p2, int nb, int p3, int p4, int nb2, int infect)
{
    int interval = (p2-p1)/nb;
    int interval2 = (p4-p3)/nb2;
    int i = 0;
    int j = 0;
    while (i < nb)
    {
        j = 0;
        while (j < nb2)
        {
            Vec2d pos(p1+i*interval,p3+j*interval2);
            singleAnimalCreate(pos);
            if (i == nb/2 && j == nb2/2 && infect == 1)
                singleAnimalInfect(Vec2d(p1+i*interval,p3+j*interval2), mildVirusProfile);
            else if (i == 0 && infect == 2)
                singleAnimalInfect(Vec2d(p1+i*interval,p3+j*interval2), deadlyVirusProfile);
            j++;
        }
        i++;
    }
}

// infects an animal with Virus possessing the given virusProfile
void InfectionTestManyAnimals::singleAnimalInfect(const Vec2d& position, const std::array<double, 10>& virusProfile)
{
//    Virus nasty(virusProfile);
    getAppEnv().infectEntity(position, new Virus(virusProfile));
}

void InfectionTestManyAnimals::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed)
    {
        // for manual testing, random profile
        if (event.key.code == sf::Keyboard::W)
        {
            Wolf* tmp2;
            tmp2 = new Wolf(getCursorPositionInView());
//            std::cout << *tmp2  << std::endl;
            getAppEnv().addEntity(tmp2);
        }
        // for manual testing, random profile
        else if (event.key.code == sf::Keyboard::S)
        {
            Sheep* tmp;
            tmp = new Sheep(getCursorPositionInView(), 1);
//            std::cout << *tmp  << std::endl;
            getAppEnv().addEntity(tmp);
        }
        // for manual testing, set profile
        else if (event.key.code == sf::Keyboard::Num1)
        {
            singleSheepCreate(getCursorPositionInView());
        }
        // for manual testing, set profile
        else if (event.key.code == sf::Keyboard::Num2)
        {
            singleWolfCreate(getCursorPositionInView());
        }
        // creates independent lines of sheep and infects leftmost sheep of each line.
        // Observe that there is 1) linear propagation (one neighbor, fixed probability), no jump (infection radius respected)
        else if (event.key.code == sf::Keyboard::Num3)
        {
            reset();
            fieldSheepCreate(2000,3650,11, 500, 5000, 11,2);
        }
        // creates dense field of sheep. Observe 1) exponential growth of the number of infected
        // 2) the number of infected decreases to 0 symmetrically (because virus is non-lethal and sheep should be immunised against second infection)
        else if (event.key.code == sf::Keyboard::Num4)
        {
            reset();
            fieldSheepCreate(2000,3650,11, 2000, 3650, 11,1);
        }
        // check that wolves behave the same way
        else if (event.key.code == sf::Keyboard::Num5)
        {
            reset();
            fieldWolvesCreate(2000,3650,11, 2000, 3650, 11,1);
        }
        // check that double dispatch works correctly
        // test grass ?
        else if (event.key.code == sf::Keyboard::Num6)
        {
            reset();
            fieldSheepCreate(2000,2750,5, 800, 1650, 5,2);
            fieldWolvesCreate(2000,2750,5, 900, 1750, 5,0);
            fieldSheepCreate(2000,2750,5, 1800, 2650, 5,0);
            fieldWolvesCreate(2000,2750,5, 1900, 2750, 5,2);
        }

//        else if (event.key.code == sf::Keyboard::Num0)
//        {
//            reset();
//            getAppConfig().virus_infection_probability *= 2.;
//            fieldSheepCreate(2000,3650,11, 2000, 3650, 11,1);
//        }
        else if (event.key.code == sf::Keyboard::Num7)
        {
            singleAnimalInfect(getCursorPositionInView(), mildVirusProfile);
        }
        else if (event.key.code == sf::Keyboard::Num8)
        {
            singleAnimalInfect(getCursorPositionInView(), mildVirusProfile2);
        }
        else if (event.key.code == sf::Keyboard::Num9)
        {
            singleAnimalInfect(getCursorPositionInView(), deadlyVirusProfile);
        }
        else if (event.key.code == sf::Keyboard::G)
        {
            getAppEnv().addEntity(new Grass(getCursorPositionInView()));
        }

        else if (event.key.code == sf::Keyboard::I)
        {
            Virus* tmp3 = new Virus();
            //std::cout << "NEW VIRUS: " << *tmp3 << std::endl;
            getAppEnv().infectEntity(getCursorPositionInView(), tmp3);
            delete tmp3;
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
