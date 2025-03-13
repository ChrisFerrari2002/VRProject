/**
* @file texture.h
* @brief Texture class header file
*
* This file contains the definition of the Texture class.
*
* @details The class contains methods that allow you to work with textures.
*
* @date 2025
*
* @see Object
* 
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF


/**
* @brief Texture class
*
* The texture contains information and methods that allow you to work with textures.
*
*/
class ENG_API Texture : public Eng::Object {
// Public methods
public:
   /**
    * @brief Constructor
    *
    * Initializes the texture with his name.
    * 
    * @param name The name of the Texture.
    */
   Texture(const std::string& name);

   /**
    * @brief Destructor
    *
    * Deinitialize the texture.
    */
   ~Texture();

   /**
    * @brief Load the texture from a file.
    *
    * @param filePath The file location on the system.
    * 
    * @return True if the texture is loaded correctly.
    */
   bool loadFromFile(const std::string& filePath);

   /**
    * @brief Set various texture settings.
    * 
    * @param width Width of the texture.
    * @param height Height of the texture.
    */
   void setTextureSettings(int width, int height);

   /**
    * @brief Set texture id.
    *
    * @param textureId The id of the texture.
    */
   void setTextureId(std::string textureId);

   /**
    * @brief Render the texture.
    *
    * @param transform The transform matrix.
    * @param data Other data.
    * 
    * @return True if the render is succesful.
    */
   virtual bool render(glm::mat4 transform, void* data) override;

   /**
    * @brief Get texture width.
    *
    * @retrun Texture width.
    */
   int getWidth();

   /**
    * @brief Get texture Height.
    *
    * @retrun Texture height.
    */
   int getHeight();


// Private methods and fields
private:
   std::string filePath; /**< The texture file path on the file system */
   int width; /**< The texture width */
   int height; /**< The texture height */
   unsigned int texId = 0; /**< The texture id */
   unsigned char* bitmap = new unsigned char[256 * 256 * 3]; /**< The texture bitmap */
};

#endif