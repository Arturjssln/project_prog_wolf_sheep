/*
 * prjsv 2016
 * 2016
 * Marco Antognini
 */

#include <Application.hpp>
#include <Env/Obstacle.hpp>

#include <catch.hpp>

#include <iostream>


class DummyObstacle : public Obstacle
{
	
  public:
	DummyObstacle(const Vec2d& pos, double rad)
		:  center(pos), radius(rad)
	{}
	
	Vec2d getCenter()  const override
	{
		return center;
	}
	
	double getRadius() const override
	{
		return radius;
	}
	
  private:
	Vec2d center;
	double radius;
};

SCENARIO("Collision", "[Obstacle]")
{
    GIVEN("Two identical Obstacles")
    {
        DummyObstacle o1({ 1, 1 }, 2);
        DummyObstacle o2({ 1, 1 }, 2);

        THEN("they collide")
        {
            CHECK(o1.isColliding(o2));
            CHECK(o2.isColliding(o1));
            CHECK((o1 | o2));
            CHECK((o2 | o1));
        }

        THEN("they have the same bounding circle")
        {
            CHECK(o1.getCenter() == o2.getCenter());
			CHECK(o1.getRadius() == o2.getRadius());
        }
    }

    GIVEN("An obstacle inside the other, with different centers and radius")
    {
        DummyObstacle o1({ 0, 0 }, 4);
        DummyObstacle o2({ 1, 1 }, 3);

        THEN("the two obstacles collide")
        {
            CHECK(o1.isColliding(o2));
            CHECK(o2.isColliding(o1));
            CHECK((o1 | o2));
            CHECK((o2 | o1));
        }
		
        THEN("their bounding circles are not the same")
        {
            CHECK_FALSE(o1.getRadius() == o2.getRadius());
			CHECK_FALSE(o1.getCenter() == o2.getCenter());
        }
    }

    GIVEN("Obstacles that overlap but are not inside of each other")
    {
        DummyObstacle o1 ({ 0, 0 }, 4);
        DummyObstacle o2 ({ 1, 1 }, 3);
		DummyObstacle o3 ({ 5, 5 }, 4);

        THEN("they collide")
        {
            CHECK(o1.isColliding(o3));
			CHECK(o3.isColliding(o1));
            CHECK(o2.isColliding(o3));
			CHECK(o3.isColliding(o2));
            CHECK((o1 | o3));
            CHECK((o3 | o1));
			CHECK((o2 | o3));
			CHECK((o3 | o2));
        }
	}
	

	GIVEN("Obstacles that don't overlap ")
		{
        DummyObstacle o1({ 0, 0 }, 4);
        DummyObstacle o2({ 1, 1 }, 3);
		DummyObstacle o3({ 5, 5 }, 4);
		DummyObstacle o4({-9,-9 }, 4);

        THEN("they don't collide")
        {
            CHECK_FALSE(o1.isColliding(o4));
			CHECK_FALSE(o4.isColliding(o1));
            CHECK_FALSE(o2.isColliding(o4));
			CHECK_FALSE(o4.isColliding(o2));
			CHECK_FALSE(o3.isColliding(o4));
			CHECK_FALSE(o4.isColliding(o3));
            CHECK_FALSE((o1 | o4));
            CHECK_FALSE((o4 | o1));
			CHECK_FALSE((o2 | o4));
			CHECK_FALSE((o4 | o2));
			CHECK_FALSE((o3 | o4));
			CHECK_FALSE((o4 | o3));
        }
		}
	
    GIVEN("An Obstacle and two points, one inside and one outside")
    {
        DummyObstacle o ({ 0, 0 }, 5);
        auto p1 = Vec2d(0, 0);
        auto p2 = Vec2d(6, 0);

        THEN("only one point is inside")
        {
			CHECK(o.isPointInside(p1));
            CHECK(o > p1);
			CHECK_FALSE(o.isPointInside(p2));
            CHECK_FALSE(o > p2);
        }
    }
	
}

	
