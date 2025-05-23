/**
 * @file		fbo.h
 * @brief	Basic FBO class
 *
 * @authors	
 * - Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
 */
#pragma once



 //////////////
 // #INCLUDE //
 //////////////

// Glew (include it before GL.h):
#include "engine.h"



///////////////
// CLASS Fbo //
///////////////

/**
 * @brief Frame buffer class to deal with OpenGL FBOs.
 */
class ENG_API Fbo
{
   //////////
public: //
   //////////

      // Constants:
   static const unsigned int MAX_ATTACHMENTS = 8; ///< Maximum number of available render buffers or textures per FBO	

   // Enumerations:
   enum : unsigned int ///< Kind of operation
   {
      BIND_DEPTHBUFFER = 0,
      BIND_COLORTEXTURE,
      BIND_DEPTHTEXTURE,
   };

   // Const/dest:	 
   Fbo();
   ~Fbo();

   static Fbo* getCurrentFbo();

   // Get/set:   
   unsigned int getTexture(unsigned int textureNumber);
   int getSizeX();
   int getSizeY();
   int getSizeZ();
   unsigned int getHandle();

   // Management:
   bool isOk();
   bool bindTexture(unsigned int textureNumber, unsigned int operation, unsigned int texture, int param1 = 0, int param2 = 0);
   bool bindRenderBuffer(unsigned int renderBuffer, unsigned int operation, int sizeX, int sizeY);

   // Rendering:     
   bool render(void* data = nullptr);
   static void disable();


   ///////////
private: //
   ///////////

   static Fbo* currentFbo;

      // Generic data:
   int sizeX, sizeY, sizeZ;	         					///< FBO width, height and depth
   unsigned int texture[MAX_ATTACHMENTS];             ///< Attached textures
   int drawBuffer[MAX_ATTACHMENTS];       		      ///< Set color attachment per texture

   // OGL stuff:
   unsigned int glId;                                 ///< OpenGL ID
   unsigned int glRenderBufferId[MAX_ATTACHMENTS];    ///< Render buffer IDs

   // MRT cache:   
   int nrOfMrts;                                      ///< Number of MRTs
   unsigned int* mrt;                                 ///< Cached list of buffers 

   // Cache:
   bool updateMrtCache();
};