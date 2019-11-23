#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
// window
Config::Config(std::string path) : mConfig(j::readFromFile(path))
, simulation_debug(mConfig["debug"].toBool())
,window_simulation_width(mConfig["window"]["simulation"]["width"].toDouble())
, window_simulation_height(mConfig["window"]["simulation"]["height"].toDouble())
, window_stats_height(mConfig["window"]["stats"]["height"].toDouble())
, window_title(mConfig["window"]["title"].toString())
, window_antialiasing_level(mConfig["window"]["antialiasing level"].toInt())

// stats
, stats_refresh_rate(mConfig["stats"]["refresh rate"].toDouble())

// simulation
, simulation_world_texture(mConfig["simulation"]["world"]["texture"].toString())
, simulation_world_debug_texture(mConfig["simulation"]["world"]["debug texture"].toString())
, simulation_world_size(mConfig["simulation"]["world"]["size"].toDouble())
, simulation_time_factor(mConfig["simulation"]["time"]["factor"].toDouble())
, simulation_time_max_dt(sf::seconds(mConfig["simulation"]["time"]["max dt"].toDouble()))

// grass
, grass_texture(mConfig["simulation"]["grass"]["texture"].toString())
//, grass_size(mConfig["simulation"]["grass"]["size"].toDouble())
, grass_initial_energy(mConfig["simulation"]["grass"]["energy"]["initial"].toDouble())
, grass_max_energy(mConfig["simulation"]["grass"]["energy"]["max"].toDouble())
, grass_growth_factor(mConfig["simulation"]["grass"]["energy"]["growth factor"].toDouble())


// animal
, animal_random_walk_low_energy(mConfig["simulation"]["animal"]["random walk low energy"].toBool())
//, animal_safe_distance(mConfig["simulation"]["animal"]["random walk low energy"].toDouble())
, animal_meal_retention(mConfig["simulation"]["animal"]["meal retention"].toDouble())
, animal_feed_time(mConfig["simulation"]["animal"]["feed time"].toDouble())
, animal_delivery_time(mConfig["simulation"]["animal"]["reproduction"]["delivery time"].toDouble())
, animal_mating_time(mConfig["simulation"]["animal"]["reproduction"]["mating time"].toDouble())
, animal_satiety_min(mConfig["simulation"]["animal"]["satiety"]["min"].toDouble())
, animal_satiety_max(mConfig["simulation"]["animal"]["satiety"]["max"].toDouble())
, animal_idle_probability(mConfig["simulation"]["animal"]["idle"]["probability"].toDouble())
, animal_idle_time_min(mConfig["simulation"]["animal"]["idle"]["min"].toDouble())
, animal_idle_time_max(mConfig["simulation"]["animal"]["idle"]["max"].toDouble())
, animal_min_energy(mConfig["simulation"]["animal"]["min energy"].toDouble())
// sheep
, sheep_max_speed(mConfig["simulation"]["animal"]["sheep"]["max speed"].toDouble())
, sheep_mass(mConfig["simulation"]["animal"]["sheep"]["mass"].toDouble())
, sheep_energy_loss_factor(mConfig["simulation"]["animal"]["sheep"]["energy"]["loss factor"].toDouble())
, sheep_view_range(mConfig["simulation"]["animal"]["sheep"]["view"]["range"].toDouble())
, sheep_view_distance(mConfig["simulation"]["animal"]["sheep"]["view"]["distance"].toDouble())
, sheep_random_walk_jitter(mConfig["simulation"]["animal"]["sheep"]["random walk"]["jitter"].toDouble())
, sheep_random_walk_radius(mConfig["simulation"]["animal"]["sheep"]["random walk"]["radius"].toDouble())
, sheep_random_walk_distance(mConfig["simulation"]["animal"]["sheep"]["random walk"]["distance"].toDouble())
, sheep_longevity(sf::seconds(mConfig["simulation"]["animal"]["sheep"]["longevity"].toDouble()))
, sheep_energy_initial(mConfig["simulation"]["animal"]["sheep"]["energy"]["initial"].toDouble())
, sheep_energy_min_mating_female(mConfig["simulation"]["animal"]["sheep"]["energy"]["min mating female"].toDouble())
, sheep_energy_min_mating_male(mConfig["simulation"]["animal"]["sheep"]["energy"]["min mating male"].toDouble())
, sheep_reproduction_min_children(mConfig["simulation"]["animal"]["sheep"]["reproduction"]["min children"].toInt())
, sheep_reproduction_max_children(mConfig["simulation"]["animal"]["sheep"]["reproduction"]["max children"].toInt())
, sheep_reproduction_gestation_time(mConfig["simulation"]["animal"]["sheep"]["reproduction"]["gestation time"].toDouble())
, sheep_energy_loss_female_per_child(mConfig["simulation"]["animal"]["sheep"]["energy"]["loss female per child"].toDouble())
, sheep_energy_loss_mating_male(mConfig["simulation"]["animal"]["sheep"]["energy"]["loss mating male"].toDouble())
, sheep_texture_leader(mConfig["simulation"]["animal"]["sheep"]["texture"]["leader"].toString())
, sheep_texture_white(mConfig["simulation"]["animal"]["sheep"]["texture"]["white"].toString())
, sheep_texture_black(mConfig["simulation"]["animal"]["sheep"]["texture"]["black"].toString())
, sheep_size(mConfig["simulation"]["animal"]["sheep"]["size"].toDouble())
, sheep_energy_bite(mConfig["simulation"]["animal"]["sheep"]["energy"]["bite"].toDouble())

// wolf
, wolf_max_speed(mConfig["simulation"]["animal"]["wolf"]["max speed"].toDouble())
, wolf_mass(mConfig["simulation"]["animal"]["wolf"]["mass"].toDouble())
, wolf_energy_loss_factor(mConfig["simulation"]["animal"]["wolf"]["energy"]["loss factor"].toDouble())
, wolf_view_range(mConfig["simulation"]["animal"]["wolf"]["view"]["range"].toDouble())
, wolf_view_distance(mConfig["simulation"]["animal"]["wolf"]["view"]["distance"].toDouble())
, wolf_random_walk_jitter(mConfig["simulation"]["animal"]["wolf"]["random walk"]["jitter"].toDouble())
, wolf_random_walk_radius(mConfig["simulation"]["animal"]["wolf"]["random walk"]["radius"].toDouble())
, wolf_random_walk_distance(mConfig["simulation"]["animal"]["wolf"]["random walk"]["distance"].toDouble())
, wolf_longevity(sf::seconds(mConfig["simulation"]["animal"]["wolf"]["longevity"].toDouble()))
, wolf_energy_initial(mConfig["simulation"]["animal"]["wolf"]["energy"]["initial"].toDouble())
, wolf_energy_min_mating_female(mConfig["simulation"]["animal"]["wolf"]["energy"]["min mating female"].toDouble())
, wolf_energy_min_mating_male(mConfig["simulation"]["animal"]["wolf"]["energy"]["min mating male"].toDouble())
, wolf_reproduction_min_children(mConfig["simulation"]["animal"]["wolf"]["reproduction"]["min children"].toInt())
, wolf_reproduction_max_children(mConfig["simulation"]["animal"]["wolf"]["reproduction"]["max children"].toInt())
, wolf_reproduction_gestation_time(mConfig["simulation"]["animal"]["wolf"]["reproduction"]["gestation time"].toDouble())
, wolf_energy_loss_female_per_child(mConfig["simulation"]["animal"]["wolf"]["energy"]["loss female per child"].toDouble())
, wolf_energy_loss_mating_male(mConfig["simulation"]["animal"]["wolf"]["energy"]["loss mating male"].toDouble())
//, wolf_texture_leader(mConfig["simulation"]["animal"]["wolf"]["texture"]["leader"].toString())
//, wolf_texture_male(mConfig["simulation"]["animal"]["wolf"]["texture"]["male"].toString())
//, wolf_texture_female(mConfig["simulation"]["animal"]["wolf"]["texture"]["female"].toString())
, wolf_texture((mConfig["simulation"]["animal"]["wolf"]["texture"].toString()))
, wolf_size(mConfig["simulation"]["animal"]["wolf"]["size"].toDouble())
, wolf_idle_probability(mConfig["simulation"]["animal"]["wolf"]["idle"]["probability"].toDouble())
, wolf_idle_time_min(mConfig["simulation"]["animal"]["wolf"]["idle"]["min"].toDouble())
, wolf_idle_time_max(mConfig["simulation"]["animal"]["wolf"]["idle"]["max"].toDouble())
, wolf_satiety_min(mConfig["simulation"]["animal"]["wolf"]["satiety"]["min"].toDouble())
, wolf_satiety_max(mConfig["simulation"]["animal"]["wolf"]["satiety"]["max"].toDouble())

// Virus
, virus_sparsity(mConfig["simulation"]["virus"]["sparsity"].toDouble())
, virus_initial_quantity(mConfig["simulation"]["virus"]["initial quantity"].toDouble())
, virus_min_quantity_for_infection(mConfig["simulation"]["virus"]["min quantity for infection"].toDouble())
, virus_infection_probability(mConfig["simulation"]["virus"]["infection probaility"].toDouble())
, virus_texture_infected(mConfig["simulation"]["virus"]["texture"].toString())
, virus_infection_range(mConfig["simulation"]["virus"]["infection range"].toDouble())

//ImmuneSystem
, immune_health_recovery(mConfig["simulation"]["immune system"]["health recovery"].toDouble())
, immune_health_penalty(mConfig["simulation"]["immune system"]["health penalty"].toDouble())
, immune_defense_effectiveness(mConfig["simulation"]["immune system"]["defense effectiveness"].toDouble())
, immune_defense_random_variability(mConfig["simulation"]["immune system"]["defense variability"].toDouble())
, immune_adaptive_norm(mConfig["simulation"]["immune system"]["adaptive range"].toDouble())

// Genome
, genome_black_allelle_prevalence(mConfig["simulation"]["genome"]["black allele prevalence"].toDouble())
, genome_mutability_innate_immmune_system(mConfig["simulation"]["genome"]["mutability innate immune system"].toDouble())

{
}

// TODO : getter for debug
void Config::switchDebug(){
	mConfig["debug"] = j::boolean(!mConfig["debug"].toBool());
	simulation_debug = mConfig["debug"].toBool();
}

bool Config::getDebug(){
return simulation_debug;
}
