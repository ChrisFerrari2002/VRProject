/**
* @file camera.cpp
* @brief Implementation of the Camera class
*
* This file contains the implementation of the Camera class methods.
*
* @see Camera
* @see camera.h
*
* @date 2025
*
* @details The Camera class is a subclass of the Node class and represents a camera in the engine.
*
* @see Eng::Node
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/

#include "engine.h"

/**
* @brief Destructor
*
* Destroys the camera object.
*/
Eng::Camera::~Camera() {}

/**
* @brief Get the inverse camera matrix
*
* @return The inverse of the camera's transformation matrix.
*/
glm::mat4 ENG_API Eng::Camera::getInverseCameraMat() {
    return glm::inverse(getTransform());
}

/**
* @brief Set the user-defined transformation for the camera
*
* Sets the transformation of the camera based on user-defined parameters.
* The camera is positioned at the given coordinates and rotated around the x, y, and z axes.
*
* @param x The x-coordinate of the camera position.
* @param y The y-coordinate of the camera position.
* @param z The z-coordinate of the camera position.
* @param rx The rotation around the x-axis.
* @param ry The rotation around the y-axis.
* @param rz The rotation around the z-axis.
*/
void Eng::Camera::setUserTransform(float x, float y, float z, float rx, float ry, float rz) {
   glm::mat4 t = glm::mat4(1.0f);

   // Applica prima le rotazioni (nell'ordine corretto)
   t = glm::rotate(t, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
   t = glm::rotate(t, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
   t = glm::rotate(t, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

   // Poi applica la traslazione
   t = glm::translate(t, glm::vec3(x, y, z));

   setTransform(t); // Imposta la trasformazione del nodo camera
}

/**
* @brief Set the camera transformation matrix
*
* Sets the transformation matrix of the camera.
* The transformation matrix is used to position and orient the camera in the scene.
*
* @param viewMatrix The transformation matrix to be set.
*/
void Eng::Camera::setTransformCamera(glm::mat4 viewMatrix) {
    setTransform(viewMatrix);
}