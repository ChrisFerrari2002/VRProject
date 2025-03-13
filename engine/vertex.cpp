/**
* @file vertex.cpp
* @brief Implementation of Vertex class
*
* This file contains the implementation of the Vertex class.
*
* @details The class contains informations about the vertex.
*
* @date 2025
*
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include "engine.h"

/**
    * @brief Constructor
    *
    * Initializes an empty chessboard and sets up the cemetery.
    *
    * @param position The position of the vertex.
    * @param normal The normal of the vertex.
    */
Eng::Vertex::Vertex(glm::vec3 position, glm::vec3 normal) :
	position(position), normal(normal), textureCoordinates(glm::vec2(0.0f)) {}

/**
    * @brief Destructor
    *
    * Deinitialize the vertex.
    */
Eng::Vertex::~Vertex() {}

/**
 * @brief Get the vertex position.
 *
 * @return The currently vertex position.
 */
glm::vec3 Eng::Vertex::getPosition() {
	return position;
}

/**
 * @brief Get the vertex normal.
 *
 * @return The currently vertex normal.
 */
glm::vec3 Eng::Vertex::getNormal() {
	return normal;
}

/**
 * @brief Get the vertex texture coordinate.
 *
 * @return The currently vertex texture coordinate.
 */
glm::vec2 Eng::Vertex::getTextureCoordinates() {
	return textureCoordinates;
}

/**
 * @brief Set the vertex texture coordinate.
 *
 * @param textCoord The new texture coordinate position.
 */
void Eng::Vertex::setTextureCoordinates(glm::vec2 textCoord) {
	textureCoordinates = textCoord;
}