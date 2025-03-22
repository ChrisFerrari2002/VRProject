#pragma once
#include "engine.h"

class ENG_API LODData {
public:
   std::vector<glm::vec3> verticesCoords;
   std::vector<glm::vec3> normalsCoords;
   std::vector<glm::vec2> texCoords;
   std::vector<unsigned int> facesArray;
};