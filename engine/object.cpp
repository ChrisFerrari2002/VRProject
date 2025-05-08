/**
* @file object.cpp
* @brief Object class implementation file
*
* This file contains the implementation of the Object class.
*
* @details The class contains information and methods about the Object.
*
* @date 2025
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include "engine.h"

unsigned int Eng::Object::idCounter = 1; /**< The id counter */

/**
 * @brief Get the object Id.
 *
 * @return The object Id.
 */
unsigned int Eng::Object::getId() {
	return id;
}

/**
 * @brief Set the object Id.
 *
 * @param id The id to assign.
 *
 */
void Eng::Object::setId(unsigned int newId) {
	id = newId;
}

/**
 * @brief Get the object name.
 *
 * @return The object name.
 */
std::string ENG_API Eng::Object::getName() {
	return name;
}

/**
 * @brief Set the object name.
 *
 * @param name The name to assign.
 *
 */
void ENG_API Eng::Object::setName(std::string newName) {
	name = newName;
}

/**
 * @brief Get the next available Id.
 *
 * @return The next Id.
 */
unsigned int ENG_API Eng::Object::getNextId() {
	idCounter++;
	return idCounter;
}