/**
* @file texture.cpp
* @brief Implementation of Texture class
*
* This file contains the implementation of the Texture class.
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

//////////////
// #INCLUDE //
//////////////
#include <GL/glew.h>
#include "engine.h"
#include <GL/freeglut.h>
#include <FreeImage.h>

/**
 * @brief Constructor
 *
 * Initializes the texture with his name.
 *
 * @param name The name of the Texture.
 */
ENG_API Eng::Texture::Texture(const std::string& name) : width(0), height(0) {
   Object::setId(Object::getNextId());
   Object::setName(name);
}

/**
 * @brief Destructor
 *
 * Deinitialize the texture.
 */
ENG_API Eng::Texture::~Texture() {}


/**
 * @brief Load the texture from a file.
 *
 * @param filePath The file location on the system.
 *
 * @return True if the texture is loaded correctly.
 */
bool ENG_API Eng::Texture::loadFromFile(const std::string& filePath) {
   std::cout << "Loading texture from file: " << filePath << std::endl;

   // Load texture:
   FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(filePath.c_str(), 0), filePath.c_str());
   if (fBitmap == nullptr) {
      std::cout << "[ERROR] Unable to load texture" << std::endl;
      return false;
   }

   int intFormat = GL_RGB;
   GLenum extFormat = GL_BGR;

   if (FreeImage_GetBPP(fBitmap) == 32) {
      intFormat = GL_RGBA;
      extFormat = GL_BGRA;
   }

   // Generate and fill texture content:
   glGenTextures(1, &texId);
   glBindTexture(GL_TEXTURE_2D, texId);
   glTexImage2D(GL_TEXTURE_2D, 0, intFormat, 
      FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap),
      0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap));

   glGenerateMipmap(GL_TEXTURE_2D); // <-- we can use this now!

   // Set circular coordinates:
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set min/mag filters:
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   // Free resources:
   FreeImage_Unload(fBitmap);
   return true;
}


/**
 * @brief Get texture width.
 *
 * @retrun Texture width.
 */
int ENG_API Eng::Texture::getWidth() {
   return width;
}

/**
 * @brief Get texture Height.
 *
 * @retrun Texture height.
 */
int Eng::Texture::getHeight() {
   return width;
}

/**
 * @brief Set various texture settings.
 *
 * @param width Width of the texture.
 * @param height Height of the texture.
 */
void ENG_API Eng::Texture::setTextureSettings(int width, int height) {
   this->width = width;
   this->height = height;
   // Add any other texture settings as needed
}

/**
 * @brief Set texture id.
 *
 * @param textureId The id of the texture.
 */
void ENG_API Eng::Texture::setTextureId(std::string filepath) {
   loadFromFile(filepath);
}

/**
 * @brief Render the texture.
 *
 * @param transform The transform matrix.
 * @param data Other data.
 *
 * @return True if the render is succesful.
 */
bool ENG_API Eng::Texture::render(glm::mat4 matrix, void* ptr) {


   glBindTexture(GL_TEXTURE_2D, texId);

   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   return true;
}