/**
* @file pointLight.cpp
* @brief PointLight class implementation file
*
* This file contains the implementation of the PointLight class.
*
* @details The class contains information and methods about the PointLight.
*
* @date 2025
*
* @see Light
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
#include <GL/freeglut.h>
#include "engine.h"

/**
 * @brief Constructor
 *
 * Initializes the PointLight with all the information for rendering.
 *
 * @param name The name of the Texture.
 * @param lightNumber The light number.
 * @param ambient The light ambient parameter.
 * @param diffuse The light diffuse parameter.
 * @param specular The light specular parameter.
 * @param cutOff The light cutoff parameter.
 */
ENG_API Eng::PointLight::PointLight(const std::string name, const int lightNumber, const glm::vec3 ambient,
	const glm::vec3 diffuse, const glm::vec3 specular, const float cutOff) :
	Light{ name, lightNumber, ambient, diffuse, specular }, cutOff(cutOff) {};

/**
 * @brief Render the PointLight.
 *
 * @param transform The transform matrix.
 * @param data Other data.
 *
 * @return True if the render is succesful.
 */
bool ENG_API Eng::PointLight::render(glm::mat4 inverseCamera, void* ptr) {

	glm::vec4 worldCoordinates = glm::vec4(getPosition(), 1.0f);
	glm::vec3 lightEyeCoordinates = inverseCamera * worldCoordinates;

	Shader::getCurrentShader()->setVec3("lightPosition", lightEyeCoordinates);
	Shader::getCurrentShader()->setVec3("lightAmbient", getAmbient());
	Shader::getCurrentShader()->setVec3("lightDiffuse", getDiffuse());
	Shader::getCurrentShader()->setVec3("lightSpecular", getSpecular());

	return true;
}

/**
    * @brief Get the light cutOff
    *
    * @return The light cutOff.
    */
float ENG_API Eng::PointLight::getCutoff() const {
	return cutOff;
}