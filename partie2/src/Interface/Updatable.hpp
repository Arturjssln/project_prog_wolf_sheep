/*
 * prjsv 2015, 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#ifndef INFOSV_UPDATABLE_HPP
#define INFOSV_UPDATABLE_HPP

#include <SFML/System.hpp>

/*!
 * @class Updatable
 *
 * @brief Represents an entity that evolves with time
 */
class Updatable
{
public:
    virtual ~Updatable() = default;

    virtual void update(sf::Time dt) = 0;
};

#endif // INFOSV_UPDATABLE_HPP
