/*
 * prjsv 2016
 * Marco Antognini
 */

#ifndef INFOSV_CONSTANTS_HPP
#define INFOSV_CONSTANTS_HPP

#include <string>

// Numerical constants
double const DEG_TO_RAD = 0.0174532925; ///< Degree to Radian conversion constant
double const TAU = 6.283185307;         ///< TAU constant (= 2 * PI)
double const PI = 3.141592654;          ///< PI constant
double const EPSILON = 1e-8;            ///< a small epsilon value

// Stats titles
namespace s
{

std::string const GENERAL = "general";
std::string const WOLVES = "wolves";
std::string const SHEEP   = "sheep";
std::string const GRASS  = "grass";

} // s

#endif // INFOSV_CONSTANTS_HPP
