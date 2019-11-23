#include "Stats.hpp"
#include "Application.hpp"

int Stats::getActive() const {
	return activeIdentifier;
}

void Stats::setActive(int identifier) {
  activeIdentifier = identifier;
}

void Stats::reset() {
  for (auto& paire : graphs) {
   paire.second->reset();
 	}
}

void Stats::addGraph(int activeId, std::string libelle, std::vector<std::string> const& series, double min, double max, Vec2d tailleGraphes) {
	//Si il n'existe pas encore de paire avec cet identifiant, alors on en créé une dans libelles et dans graphs
	if(libelles.count(activeId) == 0) {
		libelles.insert(std::make_pair(activeId, libelle));
		graphs.insert(std::make_pair(activeId, std::unique_ptr<Graph>(new Graph(series, tailleGraphes, min, max))));
	} else {
	//Sinon on reset le graph déja existant et on la remplace par un nouveau
		libelles[activeId] = libelle;
		graphs[activeId]->reset();
		graphs[activeId] = std::unique_ptr<Graph>(new Graph(series, tailleGraphes, min, max));
	}
  setActive(activeId);
}

void Stats::update(sf::Time dt) {
	if (refreshTime <= sf::Time::Zero) {
		refreshTime = sf::seconds(getAppConfig().stats_refresh_rate);
		for (auto& pair : graphs) {
			//2e argument : On vérifie que le graph est actif pour mettre à jour ses données, sinon fetchData retournera une map vide
			std::unordered_map<std::string, double> new_data(getAppEnv().fetchData(libelles[pair.first], activeIdentifier == pair.first));
			if (!new_data.empty()) {
				pair.second->updateData(refreshTime, new_data);
			}
		}
	}
	refreshTime -= dt;
}


void Stats::drawOn(sf::RenderTarget& targetWindow) {
	//Si la clé (activeIdentifier) correspond bien à un graph, alors on le dessine
	if(graphs.count(activeIdentifier) != 0 and activeIdentifier != -1) graphs[activeIdentifier]->drawOn(targetWindow);
}

void Stats::focusOn (std::string libelle) {
	for (auto paire : libelles) {
		if (paire.second == libelle) {
			setActive(paire.first);
			graphs[paire.first]->reset();
		}
	}
}
