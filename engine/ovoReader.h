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
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
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
