/**
* @file material.cpp
* @brief Implementation of the Material class
*
* This file contains the implementation of the Material class methods.
*
* @see Material
* @see material.h
*
* @date 2025
*
* @details The Material class is a subclass of the Object class and represents a material in the engine.
* @see Eng::Object, Eng::Texture
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/
#include <GL/freeglut.h>
#include "engine.h"

/**
* @brief Default constructor
*
* Initializes a material with default values.
*/
Eng::Material::Material()
{
	Object::setId(Object::getNextId());
	Object::setName("default");
	this->setEmission(glm::vec4(0.0));
	this->setAmbient(glm::vec4(0.0));
	this->setDiffuse(glm::vec4(0.0));
	this->setSpecular(glm::vec4(0.0));
	this->setShininess(0.0);
}

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
Eng::Material::Material(const std::string name, glm::vec3 _emission, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess) {
	Object::setId(Object::getNextId());
	Object::setName(name);
	this->setEmission(_emission);
	this->setAmbient(_ambient);
	this->setDiffuse(_diffuse);
	this->setSpecular(_specular);
	this->setShininess(_shininess);
}

/**
* @brief Destructor
*
* Destroys the material object.
*/
Eng::Material::~Material() {}

/**
* @brief Get the emission color of the material
* 
* @return The emission color of the material.
*/
glm::vec3 ENG_API Eng::Material::getEmission() {
	return emission;
}

/**
* @brief Get the ambient color of the material
* 
* @return The ambient color of the material.
*/
glm::vec3 ENG_API Eng::Material::getAmbient() {
	return ambient;
}

/**
* @brief Get the diffuse color of the material
* 
* @return The diffuse color of the material.
*/
glm::vec3 ENG_API Eng::Material::getDiffuse() {
	return diffuse;
}

/**
* @brief Get the specular color of the material
* 
* @return The specular color of the material.
*/
glm::vec3 ENG_API Eng::Material::getSpecular() {
	return specular;
}

/**
* @brief Get the shininess of the material
* 
* @return The shininess of the material.
*/
float ENG_API Eng::Material::getShininess() {
	return shininess;
}

/**
* @brief Get the texture of the material
* 
* @return Pointer to the texture of the material.
*/
Eng::Texture* Eng::Material::getTexture() {
	return texture;
}

/**
* @brief Set the emission color of the material
* 
* @param emission The emission color to be set.
*/
void ENG_API Eng::Material::setEmission(glm::vec3 emission) {
	this->emission = emission;
}

/**
* @brief Set the ambient color of the material
* 
* @param ambient The ambient color to be set.
*/
void ENG_API Eng::Material::setAmbient(glm::vec3 ambient) {
	this->ambient = ambient;
}

/**
* @brief Set the diffuse color of the material
* 
* @param diffuse The diffuse color to be set.
*/
void ENG_API Eng::Material::setDiffuse(glm::vec3 diffuse) {
	this->diffuse = diffuse;
}

/**
* @brief Set the specular color of the material
* 
* @param specular The specular color to be set.
*/
void ENG_API Eng::Material::setSpecular(glm::vec3 specular) {
	this->specular = specular;
}

/**
* @brief Set the shininess of the material
* 
* @param shininess The shininess to be set.
*/
void ENG_API Eng::Material::setShininess(float shininess) {
	this->shininess = shininess;
}

/**
* @brief Set the texture of the material
* 
* @param texture Pointer to the texture to be set.
*/
void ENG_API Eng::Material::setTexture(Eng::Texture* texture) {
	this->texture = texture;
}

/**
* @brief Render the material
* 
* @param matrix The transformation matrix.
* @param ptr A pointer to additional data.
* @return True if the rendering was successful, false otherwise.
*/
bool ENG_API Eng::Material::render(glm::mat4 matrix, void* ptr) {

	/*if (texture != nullptr)
		texture->render(matrix, ptr);*/
	Shader::getCurrentShader()->setVec3("matEmission", this->emission);
	Shader::getCurrentShader()->setVec3("matAmbient", this->ambient);
	Shader::getCurrentShader()->setVec3("matDiffuse", this->diffuse);
	Shader::getCurrentShader()->setVec3("matSpecular", this->specular);
	Shader::getCurrentShader()->setFloat("matShininess", this->shininess);
	/*glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(emission));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));*/
	return true;
}