/**
* @file pointLight.h
* @brief pointLight class header file
*
* This file contains the definition of the pointLight class.
*
* @details The class contains information and methods about the pointLight.
*
* @date 2025
*
* @see Light
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef POINT_LIGHT
#define POINT_LIGHT

/**
* @brief PointLight class
*
* The class allows to render a PointLight.
*
*/
class ENG_API PointLight : public Eng::Light {
// Public methods
public:
   /**
    * @brief Constructor
    *
    * Initializes the PointLight with all the information for rendering.
    *
    * @param name The name of the Texture.
    * @param lightNumber The light number.
    * @param ambient The light ambient parameter.
    * @param diffuse The light diffuse parameter.
    * @param specular The light specular parameter.
    * @param cutOff The light cutoff parameter.
    */
   PointLight(const std::string name, const int lightNumber, const glm::vec3 ambient,
      const glm::vec3 diffuse, const glm::vec3 specular, const float cutOff = 180.0f);

   /**
    * @brief Destructor
    *
    * Deinitialize the PointLight.
    */
   ~PointLight() {};

   /**
    * @brief Render the PointLight.
    *
    * @param transform The transform matrix.
    * @param data Other data.
    *
    * @return True if the render is succesful.
    */
   virtual bool render(glm::mat4 transform, void* data) override;

   /**
    * @brief Get the light cutOff
    *
    * @return The light cutOff.
    */
   float getCutoff() const;

// Private methods and fields
private:
   float cutOff; /**< The PointLight cutoff */
};

#endif