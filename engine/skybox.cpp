/**
 * @file		skybox.cpp
 * @brief	skybox class
 *
 * @authors
 * - Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
 */

#include "GL/glew.h"
#include "engine.h"
#include <GL/freeglut.h>
#include <FreeImage.h>

const char* skyboxVertShader = R"(
   #version 440 core

   uniform mat4 projection;
   uniform mat4 modelview;

   layout(location = 0) in vec3 in_Position;

   out vec3 texCoord;

   void main(void)
   {
      texCoord = in_Position;
      gl_Position = projection * modelview * vec4(in_Position, 1.0f);
      gl_Position = gl_Position.xyww;
   }
)";

//////////////////////////////////////////
const char* skyboxFragShader = R"(
   #version 440 core

   in vec3 texCoord;

   // Texture mapping (cubemap):
   layout(binding = 0) uniform samplerCube cubemapSampler;

   out vec4 fragOutput;

   void main(void)
   {
      fragOutput = texture(cubemapSampler, texCoord);
   }
)";

/**
    * @brief Default constructor
    *
    * Initializes a skybox with default values.
    */
Eng::Skybox::Skybox(const std::string& name) : Node(name) {

   Shader* skyboxVs = new Shader();
   skyboxVs->loadFromMemory(Shader::TYPE_VERTEX, skyboxVertShader);

   Shader* skyboxFs = new Shader();
   skyboxFs->loadFromMemory(Shader::TYPE_FRAGMENT, skyboxFragShader);

   Shader* skyboxShader = new Shader();
   skyboxShader->build(skyboxVs, skyboxFs);
   Shader::mapShader("skyboxShader", skyboxShader);

}


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
void Eng::Skybox::setupSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
   const std::string& face4, const std::string& face5, const std::string& face6) {

   cubemapNames = { face1, face2, face3, face4, face5, face6 };
   cubeVertices = 
   {
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
   };

   cubeFaces =
   {
      0, 1, 2,
      0, 2, 3,
      3, 2, 6,
      3, 6, 7,
      4, 0, 3,
      4, 3, 7,
      6, 5, 4,
      7, 6, 4,
      4, 5, 1,
      4, 1, 0,
      1, 5, 6,
      1, 6, 2,
   };
}

/**
 * @brief Build the cubemap of the skybox.
 */
void Eng::Skybox::buildCubemap() {
   // Create and bind cubemap:
   glGenTextures(1, &cubemapId);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

   // Set params:
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   // Set filters:
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // Load sides:
   for (int curSide = 0; curSide < 6; curSide++)
   {
      // Load texture:
      FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(cubemapNames[curSide].c_str(), 0), cubemapNames[curSide].c_str());
      if (fBitmap == nullptr)
         std::cout << "[ERROR] loading file '" << cubemapNames[curSide] << "'" << std::endl;
      int intFormat = GL_RGB;
      GLenum extFormat = GL_BGR;
      if (FreeImage_GetBPP(fBitmap) == 32)
      {
         intFormat = GL_RGBA;
         extFormat = GL_BGRA;
      }

      // Fix mirroring:
      FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
      FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

      // Send texture to OpenGL:
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, 0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap));

      // Free resources:
      FreeImage_Unload(fBitmap);
   }

   glGenVertexArrays(1, &globalVAO);
   glBindVertexArray(globalVAO);

   glGenBuffers(1, &cubeVboVertices);
   glBindBuffer(GL_ARRAY_BUFFER, cubeVboVertices);
   glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   Shader::getShader("skyboxShader")->bind(0, "in_Position");

   glGenBuffers(1, &cubeVboFaces);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVboFaces);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeFaces.size() * sizeof(unsigned short), cubeFaces.data(), GL_STATIC_DRAW);
}
bool Eng::Skybox::render(glm::mat4 transform, void* data) {
   glBindVertexArray(globalVAO);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
   Eng::Shader::getCurrentShader()->setMatrix("modelview", transform);

   glDrawElements(GL_TRIANGLES, cubeFaces.size(), GL_UNSIGNED_SHORT, nullptr);
   glBindVertexArray(0);
   return true;
}