/**
* @file spotLight.cpp
* @brief SpotLight class implementation file
*
* This file contains the implementation of the SptoLight class.
*
* @details The class contains information and methods about the SpotLight.
*
* @date 2025
*
* @see PointLight
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include <GL/freeglut.h>
#include "engine.h"

/**
    * @brief Constructor
    *
    * Initializes the Spotlight with all the information for rendering.
    *
    * @param name The name of the Texture.
    * @param lightNumber The light number.
    * @param ambient The light ambient parameter.
    * @param diffuse The light diffuse parameter.
    * @param specular The light specular parameter.
    * @param cutOff The light cutoff parameter.
    * @param direction The light direction parameter.
    */
ENG_API Eng::SpotLight::SpotLight(const std::string name, const int lightNumber, const glm::vec4 ambient,
	const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff, const glm::vec3 direction) :
   Light{ name, lightNumber, ambient, diffuse, specular }, cutOff(cutOff) {
};

/**
    * @brief Render the SpotLight.
    *
    * @param transform The transform matrix.
    * @param data Other data.
    *
    * @return True if the render is succesful.
    */
bool ENG_API Eng::SpotLight::render(glm::mat4 inverseCamera, void* ptr) {
   glm::vec4 worldCoordinates = glm::vec4(getPosition(), 1.0f);
   glm::vec3 lightEyeCoordinates = inverseCamera * worldCoordinates;
   Shader::getCurrentShader()->setVec3("lightPosition", lightEyeCoordinates);
   Shader::getCurrentShader()->setVec3("lightDirection", glm::normalize(getDirection()));
   Shader::getCurrentShader()->setVec3("cameraPosition", lightEyeCoordinates);
   Shader::getCurrentShader()->setVec3("lightAmbient", getAmbient());
   Shader::getCurrentShader()->setVec3("lightDiffuse", getDiffuse());
   Shader::getCurrentShader()->setVec3("lightSpecular", getSpecular());
   Shader::getCurrentShader()->setFloat("lightCutoff", glm::cos(glm::radians(cutOff)));
   Shader::getCurrentShader()->setFloat("lightConstant", getConstantAttenuation());
   Shader::getCurrentShader()->setFloat("lightLinear", getLinearAttenuation());
   Shader::getCurrentShader()->setFloat("lightQuadratic", getQuadraticAttenuation());
	return true;
}

/**
    * @brief Get the light cutOff
    *
    * @return The light cutOff.
    */
glm::vec3 ENG_API Eng::SpotLight::getDirection() const {
   return direction;
}