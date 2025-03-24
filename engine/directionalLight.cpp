/**
* @file directionalLight.cpp
* @brief Implementation of the DirectionalLight class
*
* This file contains the implementation of the DirectionalLight class methods.
*
* @see DirectionalLight
* @see directionalLight.h
*
* @date 2025
*
* @details The DirectionalLight class is a subclass of the Light class and represents a directional light in the engine.
*
* @see Eng::Light
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/
#include <GL/freeglut.h>
#include "engine.h"

/**
* @brief Constructor
*
* Initializes a directional light with the given parameters.
*
* @param name The name of the light.
* @param lightNumber The number of the light.
* @param ambient The ambient color of the light.
* @param diffuse The diffuse color of the light.
* @param specular The specular color of the light.
*/
ENG_API Eng::DirectionalLight::DirectionalLight(const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular) :
    Light{ name, lightNumber, ambient, diffuse, specular } {
   
}

/**
* @brief Render the directional light
*
* Renders the directional light using the given transformation matrix.
*
* @param matrix The transformation matrix.
* @param ptr A pointer to additional data.
* @return True if the rendering was successful, false otherwise.
*/
bool ENG_API Eng::DirectionalLight::render(glm::mat4 matrix, void* ptr) {
    /*Light::render(matrix, ptr);
    glm::vec4 position = glm::vec4(getPosition(), 0.0f);
    glLightfv(getLightNumber(), GL_POSITION, glm::value_ptr(position));*/
    return true;
}

/**
* @brief Set the transformation matrix for the directional light
*
* Sets the transformation matrix of the directional light.
*
* @param transform The transformation matrix to be set.
*/
void ENG_API Eng::DirectionalLight::setTransform(glm::mat4 transform) {
    Light::setTransform(transform);
}