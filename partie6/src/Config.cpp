#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
// window
Config::Config(std::string path) : mConfig(j::readFromFile(path))
    , simulation_debug(mConfig["debug"].toBool())
    , simulation_automatic_generator(mConfig["automatic generator"].toBool())
    , window_simulation_width(mConfig["window"]["simulation"]["width"].toDouble())
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

// consommable
    , consommable_apparition_probability(mConfig["simulation"]["consommable"]["apparition probability"].toDouble())

// grass
    , grass_texture(mConfig["simulation"]["grass"]["texture"].toString())
//, grass_size(mConfig["simulation"]["grass"]["size"].toDouble())
    , grass_initial_energy(mConfig["simulation"]["grass"]["energy"]["initial"].toDouble())
    , grass_max_energy(mConfig["simulation"]["grass"]["energy"]["max"].toDouble())
    , grass_growth_factor(mConfig["simulation"]["grass"]["energy"]["growth factor"].toDouble())
//flower
    , flower_cure_power(mConfig["simulation"]["flower"]["cure power"].toDouble())
    , flower_texture_blue(mConfig["simulation"]["flower"]["texture"]["blue"].toString())
    , flower_texture_green(mConfig["simulation"]["flower"]["texture"]["green"].toString())
    , flower_texture_pink(mConfig["simulation"]["flower"]["texture"]["pink"].toString())
    , flower_texture_purple(mConfig["simulation"]["flower"]["texture"]["purple"].toString())
    , flower_texture_red(mConfig["simulation"]["flower"]["texture"]["red"].toString())

//magicMushroom
    , poisoned_time(sf::seconds(mConfig["simulation"]["magicMushroom"]["hallucinating time"].toDouble()))
    , texture_magicMushroom(mConfig["simulation"]["magicMushroom"]["texture"].toString())

// water
    , water_texture(mConfig["simulation"]["water"]["texture"].toString())
    , water_initial_energy(mConfig["simulation"]["water"]["energy"]["initial"].toDouble())
    , water_max_energy(mConfig["simulation"]["water"]["energy"]["max"].toDouble())
    , water_growth_factor(mConfig["simulation"]["water"]["energy"]["growth factor"].toDouble())

// animal
    , animal_random_walk_low_energy(mConfig["simulation"]["animal"]["random walk low energy"].toBool())
//, animal_safe_distance(mConfig["simulation"]["animal"]["random walk low energy"].toDouble())
    , animal_meal_retention(mConfig["simulation"]["animal"]["meal retention"].toDouble())
    , animal_drink_retention(mConfig["simulation"]["animal"]["drink retention"].toDouble())
    , animal_feed_or_drink_time(mConfig["simulation"]["animal"]["feed or drink time"].toDouble())
    , animal_delivery_time(mConfig["simulation"]["animal"]["reproduction"]["delivery time"].toDouble())
    , animal_mating_time(mConfig["simulation"]["animal"]["reproduction"]["mating time"].toDouble())
    , animal_time_before_mating(sf::seconds(mConfig["simulation"]["animal"]["reproduction"]["time before mating"].toDouble()))
    , animal_satiety_min(mConfig["simulation"]["animal"]["satiety"]["min"].toDouble())
    , animal_satiety_max(mConfig["simulation"]["animal"]["satiety"]["max"].toDouble())
    , animal_quench_min(mConfig["simulation"]["animal"]["quench"]["min"].toDouble())
    , animal_quench_max(mConfig["simulation"]["animal"]["quench"]["max"].toDouble())
    , animal_idle_probability(mConfig["simulation"]["animal"]["idle"]["probability"].toDouble())
    , animal_idle_time_min(mConfig["simulation"]["animal"]["idle"]["min"].toDouble())
    , animal_idle_time_max(mConfig["simulation"]["animal"]["idle"]["max"].toDouble())
    , animal_base_energy_consumption(mConfig["simulation"]["animal"]["base energy consumption"].toDouble())
    , animal_base_water_consumption(mConfig["simulation"]["animal"]["base water consumption"].toDouble())
    , animal_min_energy(mConfig["simulation"]["animal"]["min energy"].toDouble())
    , animal_min_quench(mConfig["simulation"]["animal"]["min quench"].toDouble())
    , proximity_threshold(mConfig["simulation"]["animal"]["proximity threshold"].toDouble())
    , avoidance_coeff(mConfig["simulation"]["animal"]["avoidance coefficient"].toDouble())

// sheep
    , sheep_max_speed(mConfig["simulation"]["animal"]["sheep"]["max speed"].toDouble())
    , sheep_mass(mConfig["simulation"]["animal"]["sheep"]["mass"].toDouble())
    , sheep_energy_loss_factor(mConfig["simulation"]["animal"]["sheep"]["energy"]["loss factor"].toDouble())
    , sheep_water_loss_factor(mConfig["simulation"]["animal"]["sheep"]["quench"]["loss factor"].toDouble())
    , sheep_view_range(mConfig["simulation"]["animal"]["sheep"]["view"]["range"].toDouble())
    , sheep_view_distance(mConfig["simulation"]["animal"]["sheep"]["view"]["distance"].toDouble())
    , sheep_random_walk_jitter(mConfig["simulation"]["animal"]["sheep"]["random walk"]["jitter"].toDouble())
    , sheep_random_walk_radius(mConfig["simulation"]["animal"]["sheep"]["random walk"]["radius"].toDouble())
    , sheep_random_walk_distance(mConfig["simulation"]["animal"]["sheep"]["random walk"]["distance"].toDouble())
    , sheep_longevity(sf::seconds(mConfig["simulation"]["animal"]["sheep"]["longevity"].toDouble()))
    , sheep_energy_initial(mConfig["simulation"]["animal"]["sheep"]["energy"]["initial"].toDouble())
    , sheep_energy_min_mating_female(mConfig["simulation"]["animal"]["sheep"]["energy"]["min mating female"].toDouble())
    , sheep_energy_min_mating_male(mConfig["simulation"]["animal"]["sheep"]["energy"]["min mating male"].toDouble())
    , sheep_quench_min_mating_female(mConfig["simulation"]["animal"]["sheep"]["quench"]["min mating female"].toDouble())
    , sheep_quench_min_mating_male(mConfig["simulation"]["animal"]["sheep"]["quench"]["min mating male"].toDouble())
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
    , sheep_drink_swallow(mConfig["simulation"]["animal"]["sheep"]["quench"]["drink swallow"].toDouble())
    , sheep_quench_initial(mConfig["simulation"]["animal"]["sheep"]["quench"]["initial"].toDouble())

// wolf
    , wolf_max_speed(mConfig["simulation"]["animal"]["wolf"]["max speed"].toDouble())
    , wolf_mass(mConfig["simulation"]["animal"]["wolf"]["mass"].toDouble())
    , wolf_energy_loss_factor(mConfig["simulation"]["animal"]["wolf"]["energy"]["loss factor"].toDouble())
    , wolf_water_loss_factor(mConfig["simulation"]["animal"]["wolf"]["quench"]["loss factor"].toDouble())
    , wolf_view_range(mConfig["simulation"]["animal"]["wolf"]["view"]["range"].toDouble())
    , wolf_view_distance(mConfig["simulation"]["animal"]["wolf"]["view"]["distance"].toDouble())
    , wolf_random_walk_jitter(mConfig["simulation"]["animal"]["wolf"]["random walk"]["jitter"].toDouble())
    , wolf_random_walk_radius(mConfig["simulation"]["animal"]["wolf"]["random walk"]["radius"].toDouble())
    , wolf_random_walk_distance(mConfig["simulation"]["animal"]["wolf"]["random walk"]["distance"].toDouble())
    , wolf_longevity(sf::seconds(mConfig["simulation"]["animal"]["wolf"]["longevity"].toDouble()))
    , wolf_energy_initial(mConfig["simulation"]["animal"]["wolf"]["energy"]["initial"].toDouble())
    , wolf_energy_min_mating_female(mConfig["simulation"]["animal"]["wolf"]["energy"]["min mating female"].toDouble())
    , wolf_energy_min_mating_male(mConfig["simulation"]["animal"]["wolf"]["energy"]["min mating male"].toDouble())
    , wolf_quench_min_mating_female(mConfig["simulation"]["animal"]["wolf"]["quench"]["min mating female"].toDouble())
    , wolf_quench_min_mating_male(mConfig["simulation"]["animal"]["wolf"]["quench"]["min mating male"].toDouble())
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
    , wolf_energy_bite(mConfig["simulation"]["animal"]["wolf"]["energy"]["bite"].toDouble())
    , wolf_drink_swallow(mConfig["simulation"]["animal"]["wolf"]["quench"]["drink swallow"].toDouble())
    , wolf_idle_probability(mConfig["simulation"]["animal"]["wolf"]["idle"]["probability"].toDouble())
    , wolf_idle_time_min(mConfig["simulation"]["animal"]["wolf"]["idle"]["min"].toDouble())
    , wolf_idle_time_max(mConfig["simulation"]["animal"]["wolf"]["idle"]["max"].toDouble())
    , wolf_satiety_min(mConfig["simulation"]["animal"]["wolf"]["satiety"]["min"].toDouble())
    , wolf_satiety_max(mConfig["simulation"]["animal"]["wolf"]["satiety"]["max"].toDouble())
    , wolf_quench_initial(mConfig["simulation"]["animal"]["wolf"]["quench"]["initial"].toDouble())

// Virus
    , virus_sparsity(mConfig["simulation"]["virus"]["sparsity"].toDouble())
    , virus_initial_quantity(mConfig["simulation"]["virus"]["initial quantity"].toDouble())
    , virus_min_quantity_for_infection(mConfig["simulation"]["virus"]["min quantity for infection"].toDouble())
    , virus_infection_probability(mConfig["simulation"]["virus"]["infection probability"].toDouble())
    , virus_texture_infected(mConfig["simulation"]["virus"]["texture"].toString())
    , virus_infection_range(mConfig["simulation"]["virus"]["infection range"].toDouble())

//ImmuneSystem
    , immune_health_recovery(mConfig["simulation"]["immune system"]["health recovery"].toDouble())
    , immune_health_penalty(mConfig["simulation"]["immune system"]["health penalty"].toDouble())
    , immune_defense_effectiveness(mConfig["simulation"]["immune system"]["defense effectiveness"].toDouble())
    , immune_defense_random_variability(mConfig["simulation"]["immune system"]["defense variability"].toDouble())
    , immune_adaptive_norm(mConfig["simulation"]["immune system"]["adaptive range"].toDouble())
    , immune_adaptive_baseline(mConfig["simulation"]["immune system"]["adaptive baseline"].toDouble())

// Genome
    , genome_black_allelle_prevalence(mConfig["simulation"]["genome"]["black allele prevalence"].toDouble())
    , genome_mutability_innate_immune_system(mConfig["simulation"]["genome"]["mutability innate immune system"].toDouble())
// Fire
    , fire_time(sf::seconds(mConfig["simulation"]["fire"]["time"].toDouble()))
    , fire_texture_1(mConfig["simulation"]["fire"]["texture"]["1"].toString())
    , fire_texture_2(mConfig["simulation"]["fire"]["texture"]["2"].toString())
    , fire_texture_3(mConfig["simulation"]["fire"]["texture"]["3"].toString())
// Season
    , season_time_max(sf::seconds(mConfig["simulation"]["season"]["time max"].toDouble()))
    , texture_spring(mConfig["simulation"]["season"]["texture"]["spring"].toString())
    , texture_summer(mConfig["simulation"]["season"]["texture"]["summer"].toString())
    , texture_autumn(mConfig["simulation"]["season"]["texture"]["autumn"].toString())
    , texture_winter(mConfig["simulation"]["season"]["texture"]["winter"].toString())
    , texture_icon_spring(mConfig["simulation"]["season"]["icon texture"]["spring"].toString())
    , texture_icon_summer(mConfig["simulation"]["season"]["icon texture"]["summer"].toString())
    , texture_icon_autumn(mConfig["simulation"]["season"]["icon texture"]["autumn"].toString())
    , texture_icon_winter(mConfig["simulation"]["season"]["icon texture"]["winter"].toString())
{
}

// TODO : getter for debug
void Config::switchDebug() {
    mConfig["debug"] = j::boolean(!mConfig["debug"].toBool());
    simulation_debug = mConfig["debug"].toBool();
}

bool Config::getDebug() {
    return simulation_debug;
}


void Config::switchAutomaticGenerator() {
  mConfig["automatic generator"] = j::boolean(!mConfig["automatic generator"].toBool());
  simulation_automatic_generator = mConfig["automatic generator"].toBool();
}


bool Config::getAutomaticGenerator() {
    return simulation_automatic_generator;
}
