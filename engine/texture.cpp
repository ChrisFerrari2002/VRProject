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
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
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

   if (texId)
      glDeleteTextures(1, &texId);

   FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filePath.c_str(), 0), filePath.c_str());
   if (!bitmap) {
      std::cerr << "Failed to load image: " << filePath << std::endl;
      return false;
   }

   // Convert to 32bpp if needed
   if (FreeImage_GetBPP(bitmap) != 32) {
      FIBITMAP* temp = FreeImage_ConvertTo32Bits(bitmap);
      FreeImage_Unload(bitmap);
      bitmap = temp;
   }

   FreeImage_FlipVertical(bitmap);

   width = FreeImage_GetWidth(bitmap);
   height = FreeImage_GetHeight(bitmap);

   // Check for power of two dimensions
   if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0) {
      std::cout << "Warning: Texture dimensions not power of 2 ("
         << width << "x" << height << ")" << std::endl;
   }

   glGenTextures(1, &texId);
   glBindTexture(GL_TEXTURE_2D, texId);

   // Modern OpenGL texture loading
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
      0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));

   glGenerateMipmap(GL_TEXTURE_2D);

   // Default texture parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   FreeImage_Unload(bitmap);

   std::cout << "Texture loaded successfully. ID: " << texId
      << ", Size: " << width << "x" << height << std::endl;

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
   return height;
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

   return true;
}