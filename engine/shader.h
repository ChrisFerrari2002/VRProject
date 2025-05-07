/**
 * @file		shader.h
 * @brief	Minimal OpenGL shader class
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once
#include "engine.h"



//////////////////
// CLASS Shader //
//////////////////
/**
 * @brief OGL Shader. Management and rendering of shaders.
 */
class ENG_API Shader : public Eng::Object
{
   //////////
public: //
   //////////

      // Constants:
   static const unsigned int MAX_LOGSIZE = 4096;  ///< Max output size in char for a shader log

   // Enums:
   enum ///< Kind of shader
   {
      TYPE_UNDEFINED = 0,
      TYPE_VERTEX,
      TYPE_FRAGMENT,
      TYPE_PROGRAM,
      TYPE_LAST
   };

   // Const/dest:	 
   Shader();
   ~Shader();

   // Get/set:
   int getParamLocation(const char* name);
   void setMatrix(std::string param, const glm::mat4& mat);
   void setMatrix3(std::string param, const glm::mat3& mat);
   void setFloat(std::string param, float value);
   void setInt(std::string param, int value);
   void setVec3(std::string param, const glm::vec3& vect);
   void setVec4(std::string param, const glm::vec4& vect);

   // Accessing data:
   bool loadFromMemory(int kind, const char* data);
   bool loadFromFile(int kind, const char* filename);
   bool build(Shader* vertexShader, Shader* fragmentShader);

   void bind(int location, const char* attribName);

   /**
    * @brief Insert a new shader into a map.
    *
    * Insert a new shader into a map.
    *
    * @param name The name of the shader.
    * @param shader The shader to be inserted.
    */
   static void mapShader(const std::string& name, Shader* shader);

   /**
    * @brief Get a shader from the map.
    *
    * Get a shader from the map.
    *
    * @param name The name of the shader.
    * @return Pointer to the shader.
    */
   static Shader* getShader(const std::string& name);

   // Rendering:				
   bool render(void* data = nullptr);

   virtual bool render(glm::mat4 transform, void* data) override;

   /**
    * @brief Get the active shader.
    *
    * Get the active shader.
    *
    * @return The active shader.
    */
   static Shader* getCurrentShader();


   ///////////	 
private:	//
   ///////////					

      // Object kind:
   int type;

   // OGL id:
   unsigned int glId;

   static Shader *currentShader; ///< Current shader
   std::map<std::string, int> bindingMap; ///< Map of binding names and locations
   static std::map<std::string, Shader*> shaders; ///< Map of shaders
};