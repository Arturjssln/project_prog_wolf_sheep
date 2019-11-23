/*
 * prjsv 2016
 * Marco Antognini
 */

#ifndef INFOSV_CONFIG_HPP
#define INFOSV_CONFIG_HPP

#include <string>
#include "JSON/JSON.hpp"
#include <SFML/Graphics.hpp>

// Define resources location
std::string const RES_LOCATION = "../res/";

// Define the location of the config file
std::string const DEFAULT_CFG = "app.json";

// Define the location of the font file (could be in the config instead of here)
std::string const FONT_LOCATION = RES_LOCATION + "sansation.ttf";


class Config {
private:
    j::Value mConfig;
    bool simulation_debug;
    bool simulation_automatic_generator;

public:
    Config(std::string path);

    // enables / disables debug mode
    void switchDebug();
    // enables / disables automatic generator mode
    void switchAutomaticGenerator();

    bool getDebug();

    bool getAutomaticGenerator();

    // returns read
    j::Value& getJsonRead() {
        return mConfig;
    }

    j::Value const& getJsonRead() const {
        return mConfig;
    }


    //window
    const double   window_simulation_width;
    const double  window_simulation_height;
    const double  window_stats_height;
    const std::string  window_title;
    const int window_antialiasing_level;

    // stats
    const double stats_refresh_rate;
    const std::string stats_log_folder = "logs/";
    const std::string stats_log_prefix = "log_";
    const std::string stats_log_header = "# Plot with GNUPLOT";

    // debug
    const sf::Color debug_text_color = sf::Color::White;
    const size_t default_debug_text_size = 20;


    //simulation
    const std::string simulation_world_texture;
    const std::string simulation_world_debug_texture;
    const int  simulation_world_size;
    const double  simulation_time_factor;
    const sf::Time  simulation_time_max_dt;

    //Living entity
    const std::string entity_texture_tracked = "target.png";

    //Consommable
    const double consommable_apparition_probability;

    //Grass
    const std::string grass_texture;
    //const double grass_size; // deprecated
    const double grass_initial_energy = 50;
    const double grass_max_energy = 500;
    const double grass_growth_factor = 5;

    //Flower
    const double flower_cure_power;
    const std::string flower_texture_blue;
    const std::string flower_texture_green;
    const std::string flower_texture_pink;
    const std::string flower_texture_purple;
    const std::string flower_texture_red;

    //MagicMushroom
    const sf::Time poisoned_time;
    const std::string texture_magicMushroom;

    // Water
    const std::string water_texture;
    const double water_initial_energy = 50;
    const double water_max_energy = 500;
    const double water_growth_factor = 5;

    // animal
    const bool animal_random_walk_low_energy;
//        const double animal_safe_distance = 1000;
    const double animal_meal_retention;
    const double animal_drink_retention;
    const float animal_feed_or_drink_time;
    const float animal_delivery_time;
    const float animal_mating_time;
    const sf::Time animal_time_before_mating;
    const double animal_satiety_min;
    const double animal_satiety_max;
    const double animal_quench_min;
    const double animal_quench_max;
    const double animal_idle_probability;
    const float animal_idle_time_min;
    const float animal_idle_time_max;
    const float animal_base_energy_consumption;
    const float animal_base_water_consumption;
    const double animal_min_energy;
    const double animal_min_quench;
    double const proximity_threshold;
    double const avoidance_coeff;
    const std::string animal_texture_leader = "crown.png";

    // sheep
    const double sheep_max_speed;
    const double sheep_mass;
    const double sheep_energy_loss_factor;
    const double sheep_water_loss_factor;
    const double sheep_view_range;
    const double sheep_view_distance;
    const double sheep_random_walk_jitter;
    const double sheep_random_walk_radius;
    const double sheep_random_walk_distance;
    const sf::Time sheep_longevity;
    const double sheep_energy_initial;
    const double sheep_energy_min_mating_female;
    const double sheep_energy_min_mating_male;
    const double sheep_quench_min_mating_female;
    const double sheep_quench_min_mating_male;
    const int sheep_reproduction_min_children;
    const int sheep_reproduction_max_children;
    const double sheep_reproduction_gestation_time;
    const double sheep_energy_loss_female_per_child;
    const double sheep_energy_loss_mating_male;
    const std::string sheep_texture_leader;
    const std::string sheep_texture_white;
    const std::string sheep_texture_black;
    const double sheep_size;
    const double sheep_energy_bite;
    const double sheep_drink_swallow;
    const double sheep_quench_initial;

    // wolf
    const double wolf_max_speed;
    const double wolf_mass;
    const double wolf_energy_loss_factor;
    const double wolf_water_loss_factor;
    const double wolf_view_range;
    const double wolf_view_distance;
    const double wolf_random_walk_jitter;
    const double wolf_random_walk_radius;
    const double wolf_random_walk_distance;
    const sf::Time wolf_longevity;
    const double wolf_energy_initial;
    const double wolf_energy_min_mating_female;
    const double wolf_energy_min_mating_male;
    const double wolf_quench_min_mating_female;
    const double wolf_quench_min_mating_male;
    const int wolf_reproduction_min_children;
    const int wolf_reproduction_max_children;
    const double wolf_reproduction_gestation_time;
    const double wolf_energy_loss_female_per_child;
    const double wolf_energy_loss_mating_male;
    const std::string wolf_texture;
    const double wolf_size;
    const double wolf_energy_bite;
    const double wolf_drink_swallow;
    const double wolf_idle_probability;
    const float wolf_idle_time_min;
    const float wolf_idle_time_max;
    const double wolf_satiety_min;
    const double wolf_satiety_max;
    const double wolf_quench_initial;

    // Virus
    const double virus_sparsity; // probability of component in mProfile to be non-zero
    const double virus_initial_quantity; // initial quantity of virus when infection occurs
    const double virus_min_quantity_for_infection; // minimal quantity of virus needed to infect other animals
    // TODO : make it dt-dependent
    const double virus_infection_probability; // probability of infecting an animal in range at each step.
    const std::string virus_texture_infected;
    const double virus_infection_range; // range within which the virus may propagate to another animal
    const double virus_max = 150; // maximal quantity the virus can reach
    const double virus_growth_rate = 1.5; // growth rate of the virus (*dt*saturation term)

    // ImmuneSystem
    const double immune_health_recovery; // speed at which health recovers when virus has been eradicated
    const double immune_health_penalty; // health loss due to virus quantity (*dt)
    const double immune_defense_effectiveness; // virus quantity reduction (*score*dt)
    const double immune_defense_random_variability; // random score added to each component of the score computation
    const double immune_adaptive_norm; // maximum norm of the adaptive immune profile
    const double immune_adaptive_rate = 0.5; // determines speed at which the immune system adapts (*dt*virus profile)
    const double immune_adaptive_baseline = 0.5; // determines speed at which the immune system adapts (*dt*virus profile)
    const double immune_health_max = 100; // max health of the animals


    // Genome
    const double genome_black_allelle_prevalence;
    const double genome_mutability_innate_immune_system;

    // Fire
    const sf::Time fire_time;
    const std::string fire_texture_1;
    const std::string fire_texture_2;
    const std::string fire_texture_3;

    // Season
    const sf::Time season_time_max;
    const std::string texture_spring;
    const std::string texture_summer;
    const std::string texture_autumn;
    const std::string texture_winter;
    const std::string texture_icon_spring;
    const std::string texture_icon_summer;
    const std::string texture_icon_autumn;
    const std::string texture_icon_winter;
};

#endif // INFOSV_CONFIG_HPP
