/**
* @file vertex.h
* @brief Vertex class header file
*
* This file contains the definition of the Vertex class.
*
* @details The class contains informations about the vertex.
*
* @date 2025
*
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef VERTEX_H
#define VERTEX_H

/**
* @brief Vertex class
*
*/
class ENG_API Vertex {
   // Public methods
public:
   /**
    * @brief Constructor
    *
    * Initializes an empty chessboard and sets up the cemetery.
    * 
    * @param position The position of the vertex.
    * @param normal The normal of the vertex.
    */
   Vertex(glm::vec3 position, glm::vec3 normal);

   /**
    * @brief Destructor
    *
    * Deinitialize the vertex.
    */
   ~Vertex();

   /**
    * @brief Get the vertex position.
    *
    * @return The currently vertex position.
    */
   glm::vec3 getPosition();

   /**
    * @brief Get the vertex normal.
    *
    * @return The currently vertex normal.
    */
   glm::vec3 getNormal();

   /**
    * @brief Get the vertex texture coordinate.
    *
    * @return The currently vertex texture coordinate.
    */
   glm::vec2 getTextureCoordinates();

   /**
    * @brief Set the vertex texture coordinate.
    *
    * @param textCoord The new texture coordinate position.
    */
   void setTextureCoordinates(glm::vec2 textCoord);

// Private methods and fields
private:
   glm::vec3 position; /**< Vertex position */
   glm::vec3 normal; /**< Vertex normal */
   glm::vec2 textureCoordinates; /**< Vertex texture coordinates */
};

#endif // VERTEX_H