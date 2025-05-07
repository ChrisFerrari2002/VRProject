#pragma once
#include "engine.h"

class Skybox : public Eng::Node {
public:
   /**
    * @brief Default constructor
    *
    * Initializes a skybox with default values.
    */
   Skybox(const std::string& name);

   /**
    * @brief Parameterized constructor
    *
    * Initializes a skybox with the given parameters.
    *
    * @param name The name of the skybox.
    * @param face1 The path to the first face texture.
    * @param face2 The path to the second face texture.
    * @param face3 The path to the third face texture.
    * @param face4 The path to the fourth face texture.
    * @param face5 The path to the fifth face texture.
    * @param face6 The path to the sixth face texture.
    */
   void setupSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
      const std::string& face4, const std::string& face5, const std::string& face6);
   
   /**
    * @brief Build the cubemap of the skybox.
    */
   void buildCubemap();

   /**
    * @brief Render the skybox
    *
    * @param transform The transformation matrix to be applied.
    * @param data A pointer to additional data.
    */
   virtual bool render(glm::mat4 transform, void* data) override;

private:
   unsigned int cubemapId, globalVAO, cubeVboVertices, cubeVboFaces; /**< OpenGL buffers */
   std::vector<std::string> cubemapNames; /**< Cubemap path names */
   std::vector<float> cubeVertices; /**< Cube vertices */
   std::vector<short> cubeFaces; /**< Cube faces */
   float skyboxSize = 100.0f; /**< Size of the skybox */
};