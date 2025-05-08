/**
* @file material.h
* @brief Material class header file
*
* This file contains the definition of the Material class that represents a material in the engine.
*
* @date 2025
*
* @details The Material class is a subclass of the Object class and represents a material in the engine.
* @see Eng::Object, Eng::Texture
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine.h"

/**
* @brief Material class
*
* The Material class is a subclass of the Object class and represents a material in the engine.
*/
class ENG_API Material : public Eng::Object {
public:
    /**
    * @brief Default constructor
    *
    * Initializes a material with default values.
    */
    Material();

    /**
    * @brief Parameterized constructor
    *
    * Initializes a material with the given parameters.
    *
    * @param name The name of the material.
    * @param _emission The emission color of the material.
    * @param _ambient The ambient color of the material.
    * @param _diffuse The diffuse color of the material.
    * @param _specular The specular color of the material.
    * @param _shininess The shininess of the material.
    */
    Material(const std::string name, glm::vec3 _emission,
        glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess);

    /**
    * @brief Destructor
    *
    * Destroys the material object.
    */
    ~Material();

    /**
    * @brief Set the shininess of the material
    *
    * @param shininess The shininess to be set.
    */
    void setShininess(float shininess);

    /**
    * @brief Set the emission color of the material
    *
    * @param emission The emission color to be set.
    */
    void setEmission(glm::vec3 emission);

    /**
    * @brief Set the ambient color of the material
    *
    * @param ambient The ambient color to be set.
    */
    void setAmbient(glm::vec3 ambient);

    /**
    * @brief Set the diffuse color of the material
    *
    * @param diffuse The diffuse color to be set.
    */
    void setDiffuse(glm::vec3 diffuse);

    /**
    * @brief Set the specular color of the material
    *
    * @param specular The specular color to be set.
    */
    void setSpecular(glm::vec3 specular);

    /**
    * @brief Set the texture of the material
    *
    * @param texture Pointer to the texture to be set.
    */
    void setTexture(Eng::Texture* texture);

    /**
    * @brief Get the emission color of the material
    *
    * @return The emission color of the material.
    */
    glm::vec3 getEmission();

    /**
    * @brief Get the ambient color of the material
    *
    * @return The ambient color of the material.
    */
    glm::vec3 getAmbient();

    /**
    * @brief Get the diffuse color of the material
    *
    * @return The diffuse color of the material.
    */
    glm::vec3 getDiffuse();

    /**
    * @brief Get the specular color of the material
    *
    * @return The specular color of the material.
    */
    glm::vec3 getSpecular();

    /**
    * @brief Get the shininess of the material
    *
    * @return The shininess of the material.
    */
    float getShininess();

    /**
    * @brief Get the texture of the material
    *
    * @return Pointer to the texture of the material.
    */
    Eng::Texture* getTexture();

    /**
    * @brief Render the material
    *
    * Renders the material using the given transformation matrix.
    *
    * @param transform The transformation matrix.
    * @param data A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
    virtual bool render(glm::mat4 transform, void* data) override;

private:
    glm::vec3 emission;     /**< The emission color of the material */
    glm::vec3 ambient;      /**< The ambient color of the material */
    glm::vec3 diffuse;      /**< The diffuse color of the material */
    glm::vec3 specular;     /**< The specular color of the material */
    float shininess;        /**< The shininess of the material */
    Eng::Texture* texture;  /**< Pointer to the texture of the material */
};

#endif // MATERIAL_H