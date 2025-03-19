/**
* @file light.h
* @brief Light class header file
*
* This file contains the definition of the Light class that represents a light source in the engine.
* Lights are placed in the first position of the list.
*
* @date 2025
*
* @details The Light class is a subclass of the Node class and represents a light in the engine.
* @see Eng::Node
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "engine.h"

/**
* @brief Light class
*
* The Light class is a subclass of the Node class and represents a light in the engine.
*/
class ENG_API Light : public Eng::Node {
public:
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
    Light(const std::string name, const int lightNumber,
        const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular);

    /**
    * @brief Destructor
    *
    * Destroys the light object.
    */
    ~Light();

    /**
    * @brief Get the light number
    *
    * @return The number of the light.
    */
    int getLightNumber();

    /**
    * @brief Set the position of the light
    */
    void setPosition();

    /**
    * @brief Get the position of the light
    *
    * @return The position of the light.
    */
    glm::vec3 getPosition();

    /**
    * @brief Get the constant attenuation factor
    *
    * @return The constant attenuation factor.
    */
    float getConstantAttenuation();

    /**
    * @brief Get the linear attenuation factor
    *
    * @return The linear attenuation factor.
    */
    float getLinearAttenuation();

    /**
    * @brief Get the quadratic attenuation factor
    *
    * @return The quadratic attenuation factor.
    */
    float getQuadraticAttenuation();

    /**
    * @brief Set the constant attenuation factor
    *
    * @param constantAttenuation The constant attenuation factor to be set.
    */
    void setConstantAttenuation(float constantAttenuation);

    /**
    * @brief Set the linear attenuation factor
    *
    * @param linearAttenuation The linear attenuation factor to be set.
    */
    void setLinearAttenuation(float linearAttenuation);

    /**
    * @brief Set the quadratic attenuation factor
    *
    * @param quadraticAttenuation The quadratic attenuation factor to be set.
    */
    void setQuadraticAttenuation(float quadraticAttenuation);

    /**
    * @brief Set the intensity of the light
    *
    * @param intensity The intensity to be set.
    */
    void setIntensity(float intensity);

    /**
    * @brief Get the next light number
    *
    * @return The next available light number.
    */
    static int getNextLightNumber();

    /**
    * @brief Render the light
    *
    * Renders the light using the given transformation matrix.
    *
    * @param transform The transformation matrix.
    * @param data A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
    virtual bool render(glm::mat4 transform, void* data) override;

    /**
    * @brief Set the transformation matrix for the light
    *
    * Sets the transformation matrix of the light.
    *
    * @param transform The transformation matrix to be set.
    */
    virtual void setTransform(glm::mat4 transform) override;

    /**
    * @brief Get the light ambient
    *
    * @return The light ambient.
    */
    glm::vec3 getAmbient() const;

    /**
    * @brief Get the light diffuse
    *
    * @return The light diffuse.
    */
    glm::vec3 getDiffuse() const;

    /**
    * @brief Get the light specular
    *
    * @return The light specular.
    */
    glm::vec3 getSpecular() const;

    /**
    * @brief Get the light intensity
    *
    * @return The light intensity.
    */
    float getIntensity() const;

private:
    static int nextNumber;  /**< The next available light number */
    int lightNumber;        /**< The number of the light */
    glm::vec3 ambient;      /**< The ambient color of the light */
    glm::vec3 diffuse;      /**< The diffuse color of the light */
    glm::vec3 specular;     /**< The specular color of the light */
    glm::vec3 position{ 0.0f, 0.0f, 0.0f};  /**< The position of the light */

    float constantAttenuation = 1.0f;   /**< The constant attenuation factor */
    float linearAttenuation = 0.0f;     /**< The linear attenuation factor */
    float quadraticAttenuation = 0.0f;  /**< The quadratic attenuation factor */
    float intensity = 7.0f;             /**< The intensity of the light */

};

#endif // LIGHT_H