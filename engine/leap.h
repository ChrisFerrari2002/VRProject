/**
 * @file		leap.h
 * @brief	Minimal Leap Motion API wrapper. Tested with Ultraleap 5.17.1
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once



 //////////////
 // #INCLUDE //
 //////////////
#include "engine.h"

////////////////
// CLASS Leap //
////////////////

/**
 * @brief Leap Motion wrapper.
 */
class Leap
{
   //////////
public: //
   //////////

      // Const/dest:	 
   Leap();
   ~Leap();

   // Init/free:
   bool init();
   bool free();

   // Polling:
   bool update();
   const LEAP_TRACKING_EVENT* getCurFrame() const;

   void renderNormalHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix);
   void renderVRHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix, const glm::mat4 leapToWorldMatrix);

   void setPickableNodes(std::list<Node*> pickableNodes);

   ///////////	 
private:	//
   ///////////			

      // Leap Motion:
   LEAP_CONNECTION connection;
   LEAP_DEVICE_REF leapDevice;
   LEAP_TRACKING_EVENT curFrame;
   signed long long lastFrameId;

   unsigned int globalVao, vertexVbo, colorVbo;
   std::vector<glm::vec3> vertices;

   std::list<Node*> pickableNodes;
};
