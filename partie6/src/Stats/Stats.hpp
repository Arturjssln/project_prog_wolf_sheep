#ifndef STATS_HPP
#define STATS_HPP

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>
#include <SFML/System.hpp>
#include <Utility/Constants.hpp>
#include <string>
#include <vector>
#include <memory>

class Stats {
  public:
	  int getActive() const;
    void setActive(int identifier);
    void reset();
    void addGraph(int activeId, std::string title, std::vector<std::string> const& series, double min, double max, Vec2d tailleGraphes);
    std::string toString(int i);
    int toInt(std::string libelle);
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& targetWindow);
    void focusOn (std::string libelle);

  private:
   std::unordered_map<int, std::string> libelles;
   std::unordered_map<int, std::unique_ptr<Graph>> graphs;
   int activeIdentifier;
   sf::Time refreshTime;
};

#endif // STATS_HPP
