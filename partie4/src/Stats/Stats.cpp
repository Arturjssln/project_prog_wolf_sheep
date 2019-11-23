#include "Stats.hpp"
#include "Application.hpp"

int Stats::getActive() const {
	return identifiantActif;
}

void Stats::setActive(int identifiant) {
  identifiantActif = identifiant;
}

void Stats::reset() {
  for (auto& paire : graphes) {
   paire.second->reset();
  }
}

void Stats::addGraph(int idActif, std::string libelle, std::vector<std::string> const& series, double min, double max, Vec2d tailleGraphes) {
	//Si il n'existe pas encore de paire avec cet identifiant, alors on en créé une dans libelles et dans graphs
	if(libelles.count(activeId) == 0) {
		libelles.insert(std::make_pair(idActif, libelle));
		graphes.insert(std::make_pair(idActif, std::unique_ptr<Graph>(new Graph(series, tailleGraphes, min, max))));
	} else {
		//Sinon on reset le graph déja existant et on la remplace par un nouveau
		libelles[idActif] = libelle;
		graphes[idActif]->reset();
		graphes[idActif] = std::unique_ptr<Graph>(new Graph(series, tailleGraphes, min, max));
	}
  setActive(idActif);
}

void Stats::update(sf::Time dt) {
	for (auto& pair : graphs) {
		if (refreshTime <= sf::Time::Zero) {
			std::unordered_map<std::string, double> new_data(getAppEnv().fetchData(libelles[pair.first]));
			if (!new_data.empty()) {
				pair.second->updateData(dt, new_data);
			}
		}
	}
	refreshTime -= dt;
}


void Stats::drawOn(sf::RenderTarget& targetWindow) {
	//Si la clé (identifiantActif) correspond bien à un graph, alors on le dessine
	if(graphes.count(identifiantActif) != 0) graphes[identifiantActif]->drawOn(targetWindow);
}
