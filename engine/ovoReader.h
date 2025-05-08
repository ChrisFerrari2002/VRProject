/**
* @file ovoReader.h
* @brief OvoReader class header file
*
* This file contains the definition of the OvoReader class.
*
* @details The class contains information and methods about the OvoReader.
*
* @date 2025
*
* @see Material, Node, Vertex
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/

#ifndef OVOREADER_H
#define OVOREADER_H

/**
* @brief OvoReader class
*
* The class allows to read .ovo files.
*
*/
class ENG_API OvoReader {
// Public methods
public:
   /**
    * @brief Constructor
    *
    * Initializes the OvoReader.
    *
    */
   OvoReader();

   /**
    * @brief Destructor
    *
    * Deinitialize the OvoReader.
    */
   ~OvoReader();

   /**
    * @brief Read the .ovo file.
    *
    * @param filePath The file path to the .ovo.
    *
    * @return The root of the scene.
    */
   Eng::Node* readFile(const char* filePath);

// Private methods and fields
private:
   /**
    * @brief Recursively read the file.
    *
    * @param dat file data.
    *
    * @return The root of the scene.
    */
   Eng::Node* recursiveLoad(FILE* dat);

   std::map<std::string, Eng::Material*> materials; /**< Map of the material name and his referred material. */
   std::vector<glm::vec3> verticesCoords;
   std::vector<glm::vec3> normalsCoords;
   std::vector<glm::vec2> texCoords;
   std::vector<unsigned int> facesArray;
};
#endif
