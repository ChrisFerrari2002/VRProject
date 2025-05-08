/**
* @file spotLight.h
* @brief SpotLight class header file
*
* This file contains the definition of the SptoLight class.
*
* @details The class contains information and methods about the SpotLight.
*
* @date 2025
*
* @see PointLight
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/

#ifndef SPOT_LIGHT
#define SPOT_LIGHT

/**
* @brief SpotLight class
*
* The class allows to render a SpotLight.
*
*/
class ENG_API SpotLight : public Eng::Light {
// Public methods
public:
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
	SpotLight(const std::string name, const int lightNumber, const glm::vec4 ambient,
		const glm::vec4 diffuse, const glm::vec4 specular, const float cutOff, const glm::vec3 direction);

   /**
    * @brief Destructor
    *
    * Deinitialize the SpotLight.
    */
	~SpotLight() {};

   /**
    * @brief Render the SpotLight.
    *
    * @param transform The transform matrix.
    * @param data Other data.
    *
    * @return True if the render is succesful.
    */
	bool render(glm::mat4 transform, void* data) override;

   /**
    * @brief Get the light direction
    *
    * @return The light direction.
    */
   glm::vec3 getDirection() const;

// Private methods and fields
private:
	glm::vec3 direction; /**< The SpotLight direction */
   float cutOff; /**< The SpotLight cutoff */
};

#endif // SPOT_LIGHT