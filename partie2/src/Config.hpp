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


class Config
{
    private:
        j::Value mConfig;
        bool simulation_debug;

    public:
        Config(std::string path);

        // enables / disables debug mode
        void switchDebug();
        bool getDebug();

        // returns read
        j::Value& getJsonRead(){
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

        // debug
        const sf::Color debug_text_color = sf::Color::White;

        //simulation
        const std::string simulation_world_texture;
	    const std::string simulation_world_debug_texture;
        const int  simulation_world_size;
        const double  simulation_time_factor;
        const sf::Time  simulation_time_max_dt;

        // grass
        const std::string grass_texture;
//        const double grass_size; // deprecated
        const double grass_initial_energy = 50;
        const double grass_max_energy = 500;
        const double grass_growth_factor = 5;

        // animal
        const bool animal_random_walk_low_energy;
//        const double animal_safe_distance = 1000;
        const double animal_meal_retention ;
        const float animal_feed_time;
        const float animal_delivery_time;
        const float animal_mating_time;
        const double animal_satiety_min;
        const double animal_satiety_max;
        const double animal_idle_probability;
        const float animal_idle_time_min;
        const float animal_idle_time_max;

        // sheep
        const double sheep_max_speed;
        const double sheep_mass;
        const double sheep_energy_loss_factor;
        const double sheep_view_range;
        const double sheep_view_distance;
        const double sheep_random_walk_jitter;
        const double sheep_random_walk_radius;
        const double sheep_random_walk_distance;
        const sf::Time sheep_longevity;
        const double sheep_energy_initial;
        const double sheep_energy_min_mating_female;
        const double sheep_energy_min_mating_male;
        const int sheep_reproduction_min_children;
        const int sheep_reproduction_max_children;
        const double sheep_reproduction_gestation_time;
        const double sheep_energy_loss_female_per_child;
        const double sheep_energy_loss_mating_male;
        const std::string sheep_texture_leader;
        
        const std::string sheep_texture_white; // QUESTION : changer la couleur dans sfml ?
        const std::string sheep_texture_black;
	    const double sheep_size;
        const double sheep_energy_bite;
        // TODO, leader csts

        // wolf
        const double wolf_max_speed;
        const double wolf_mass;
        const double wolf_energy_loss_factor;
        const double wolf_view_range;
        const double wolf_view_distance;
        const double wolf_random_walk_jitter;
        const double wolf_random_walk_radius;
        const double wolf_random_walk_distance;
        const sf::Time wolf_longevity;
        const double wolf_energy_initial;
        const double wolf_energy_min_mating_female;
        const double wolf_energy_min_mating_male;
        const int wolf_reproduction_min_children;
        const int wolf_reproduction_max_children;
        const double wolf_reproduction_gestation_time;
        const double wolf_energy_loss_female_per_child;
        const double wolf_energy_loss_mating_male;
//        const std::string wolf_texture_leader;
//        const std::string wolf_texture_male;
//        const std::string wolf_texture_female;
        const std::string wolf_texture;
        const double wolf_size;
//        const double wolf_energy_bite;
        const double wolf_idle_probability;
        const float wolf_idle_time_min;
        const float wolf_idle_time_max;

        // Virus
        const double virus_sparsity;
        const double virus_initial_quantity;
        const double virus_min_quantity_for_infection;
        const double virus_infection_probability;
        const std::string virus_texture_infected;
        const double virus_infection_range;

        // ImmuneSystem
        const double immune_health_recovery;
        const double immune_health_penalty;
        const double immune_defense_effectiveness;
        const double immune_defense_random_variability;
        const double immune_adaptive_norm;

        // Genome
        const double genome_black_allelle_prevalence;
        const double genome_mutability_innate_immmune_system;


};

#endif // INFOSV_CONFIG_HPP

