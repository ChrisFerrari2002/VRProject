/**
* @file light.cpp
* @brief Implementation of the Light class
*
* This file contains the implementation of the Light class methods.
*
* @see Light
* @see light.h
*
* @date 2025
*
* @details The Light class is a subclass of the Node class and represents a light source in the engine.
* @see Eng::Node
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
* Initializes a light with the given parameters.
*
* @param name The name of the light.
* @param lightNumber The number of the light.
* @param ambient The ambient color of the light.
* @param diffuse The diffuse color of the light.
* @param specular The specular color of the light.
*/
ENG_API Eng::Light::Light(const std::string name, const int lightNumber, const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular) : Node(name) {
    this->lightNumber = lightNumber;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    /*glEnable(GL_LIGHTING);
    glEnable(lightNumber);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);*/
}

/**
* @brief Destructor
*
* Destroys the light object.
*/
Eng::Light::~Light() {
    Light::nextNumber--;
}

/**
* @brief Set the position of the light
*/
void Eng::Light::setPosition() {
    glm::mat4 finalMatrix = getFinalMatrix();
    glm::vec3 finalPosition = finalMatrix[3];
    position = finalPosition;
}

/**
* @brief Get the position of the light
*
* @return The position of the light.
*/
glm::vec3 ENG_API Eng::Light::getPosition() {
    return position;
}

/**
* @brief Set the transformation matrix for the light
*
* Sets the transformation matrix of the light.
*
* @param transform The transformation matrix to be set.
*/
void ENG_API Eng::Light::setTransform(glm::mat4 transform) {
    Node::setTransform(transform);

    setPosition();
}

int Eng::Light::nextNumber = 0;

/**
* @brief Get the next light number
* 
* Gets the next available light number.
*
* @return The next available light number.
*/
int ENG_API Eng::Light::getNextLightNumber() {
    return GL_LIGHT0 + Light::nextNumber++;
}

/**
* @brief Get the light number
*
* @return The number of the light.
*/
int Eng::Light::getLightNumber() {
    return lightNumber;
}

/**
* @brief Get the constant attenuation factor
*
* @return The constant attenuation factor.
*/
float ENG_API Eng::Light::getConstantAttenuation() {
    return constantAttenuation;
}

/**
* @brief Get the linear attenuation factor
*
* @return The linear attenuation factor.
*/
float ENG_API Eng::Light::getLinearAttenuation() {
    return linearAttenuation;
}

/**
* @brief Get the quadratic attenuation factor
*
* @return The quadratic attenuation factor.
*/
float ENG_API Eng::Light::getQuadraticAttenuation() {
    return quadraticAttenuation;
}

/**
* @brief Set the constant attenuation factor
*
* @param constantAttenuation The constant attenuation factor to be set.
*/
void ENG_API Eng::Light::setConstantAttenuation(float constantAttenuation) {
    this->constantAttenuation = constantAttenuation;
}

/**
* @brief Set the linear attenuation factor
*
* @param linearAttenuation The linear attenuation factor to be set.
*/
void ENG_API Eng::Light::setLinearAttenuation(float linearAttenuation) {
    this->linearAttenuation = linearAttenuation;
}

/**
* @brief Set the quadratic attenuation factor
*
* @param quadraticAttenuation The quadratic attenuation factor to be set.
*/
void ENG_API Eng::Light::setQuadraticAttenuation(float quadraticAttenuation) {
    this->quadraticAttenuation = quadraticAttenuation;
}

/**
* @brief Set the intensity of the light
*
* @param intensity The intensity to be set.
*/
void ENG_API Eng::Light::setIntensity(float intensity) {
    this->intensity = intensity;
}

/**
    * @brief Get the light ambient
    *
    * @return The light ambient.
    */
glm::vec4 ENG_API Eng::Light::getAmbient() const {
   return ambient;
}

/**
* @brief Get the light diffuse
*
* @return The light diffuse.
*/
glm::vec4 ENG_API Eng::Light::getDiffuse() const {
   return diffuse;
}

/**
* @brief Get the light specular
*
* @return The light specular.
*/
glm::vec4 ENG_API Eng::Light::getSpecular() const {
   return specular;
}

/**
    * @brief Get the light intensity
    *
    * @return The light intensity.
    */
float ENG_API Eng::Light::getIntensity() const {
   return intensity;
}

/**
* @brief Render the light
*
* Renders the light using the given transformation matrix.
*
* @param matrix The transformation matrix.
* @param ptr A pointer to additional data.
* @return True if the rendering was successful, false otherwise.
*/
bool ENG_API Eng::Light::render(glm::mat4 matrix, void* ptr) {
   /*glEnable(GL_LIGHTING);
   glEnable(lightNumber);

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(matrix));

   glLightfv(lightNumber, GL_AMBIENT, glm::value_ptr(ambient));
   glLightfv(lightNumber, GL_DIFFUSE, glm::value_ptr(diffuse * intensity));
   glLightfv(lightNumber, GL_SPECULAR, glm::value_ptr(specular));

   glLightfv(lightNumber, GL_CONSTANT_ATTENUATION, &constantAttenuation);
   glLightfv(lightNumber, GL_LINEAR_ATTENUATION, &linearAttenuation);
   glLightfv(lightNumber, GL_QUADRATIC_ATTENUATION, &quadraticAttenuation);*/

   return true;
}