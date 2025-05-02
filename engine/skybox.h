#pragma once
#include "engine.h"

class Skybox : public Eng::Node {
public:
   Skybox(const std::string& name);

   void setupSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
      const std::string& face4, const std::string& face5, const std::string& face6);
   
   void buildCubemap();
   virtual bool render(glm::mat4 transform, void* data) override;

private:
   unsigned int cubemapId, globalVAO, cubeVboVertices, cubeVboFaces;
   std::vector<std::string> cubemapNames;
   std::vector<float> cubeVertices;
   std::vector<short> cubeFaces;
   float skyboxSize = 100.0f;
};