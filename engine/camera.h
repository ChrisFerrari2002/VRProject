/**
* @file camera.h
* @brief Camera class header file
*
* This file contains the definition of the Camera class that represents a camera in the engine.
*
* @date 2025
*
* @details The Camera class is a subclass of the Node class and represents a camera in the engine.
* @see Eng::Node
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "engine.h"

/**
* @brief Camera class
*
* The Camera class is a subclass of the Node and represents a camera in the engine.
*/
class ENG_API Camera : public Eng::Node {
public:
    /**
    * @brief Constructor
    *
    * Initializes a camera with the given name.
    *
    * @param name The name of the camera.
    */
    Camera(const std::string& name) : Node(name) {};

    /**
    * @brief Destructor
    *
    * Destroys the camera object.
    */
    ~Camera();

    /**
    * @brief Get the inverse camera matrix
    *
    * @return The inverse of the camera's transformation matrix.
    */
    glm::mat4 getInverseCameraMat();

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
    void setUserTransform(float x, float y, float z, float rx, float ry, float rz);

    /**
    * @brief Set the camera transformation matrix
    *
    * Sets the transformation matrix of the camera.
	* The transformation matrix is used to position and orient the camera in the scene.
    *
    * @param viewMatrix The transformation matrix to be set.
    */
    void setTransformCamera(glm::mat4 viewMatrix);
};

#endif // CAMERA_H