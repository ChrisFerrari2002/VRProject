/**
* @file directionalLight.h
* @brief DirectionalLight class header file
*
* This file contains the definition of the DirectionalLight class that represents a directional light in the engine.
*
* @date 2025
*
* @details The DirectionalLight class is a subclass of the Light class and represents a directional light in the engine.
* @see Eng::Light
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

/**
* @brief DirectionalLight class
*
* The DirectionalLight class is a subclass of the Light class and represents a directional light in the engine.
*/
class ENG_API DirectionalLight : public Eng::Light
{
public:
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
	DirectionalLight(const std::string name, const int lightNumber,
		const glm::vec4 ambient, const glm::vec4 diffuse, const glm::vec4 specular);

	/**
	* @brief Destructor of the DirectionalLight object
	*
	* Destroys the directional light object.
	*/
	~DirectionalLight() {};

    /**
    * @brief Render the directional light
    *
    * Renders the directional light using the given transformation matrix.
    *
    * @param matrix The transformation matrix.
    * @param ptr A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
	bool render(glm::mat4 transform, void* data) override;

    /**
    * @brief Set the transformation matrix for the directional light
    *
    * Sets the transformation matrix of the directional light.
    *
    * @param transform The transformation matrix to be set.
    */
	void setTransform(glm::mat4 transform) override;
};

#endif // DIRECTIONAL_LIGHT