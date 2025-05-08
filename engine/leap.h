/**
 * @file		leap.h
 * @brief	Minimal Leap Motion API wrapper. Tested with Ultraleap 5.17.1
 *
 * @author	
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

   /**
    * @brief Constructor of the Leap object.
    *
    * Construct the Leap object.
    */
   Leap();

   /**
    * @brief Destructor of the Leap object.
    *
    * Destroys the Leap object.
    */
   ~Leap();

   // Init/free:
   /**
    * @brief Initialize the Leap Motion connection.
    *
    * @return True if the connection is established successfully.
    */
   bool init();

   /**
    * @brief Free the Leap Motion connection.
    *
    * @return True if the connection is closed successfully.
    */
   bool free();

   // Polling:
   bool update();
   const LEAP_TRACKING_EVENT* getCurFrame() const;

   /**
    * @brief Render the Leap Motion hand bones in normal mode.
    *
    * @param l The leapmotion event.
    * @param modelViewMat The modelViewMat.
    * @param projMatrix The projMatrix.
    */
   void renderNormalHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix);

   /**
    * @brief Render the Leap Motion hand bones in VR mode.
    *
    * @param l The leapmotion event.
    * @param modelViewMat The modelViewMat.
    * @param projMatrix The projMatrix.
    * @param leapToWorldMatrix The position of the leapMotion.
    */
   void renderVRHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix, const glm::mat4 leapToWorldMatrix);

   /**
    * @brief Set the list of pickable nodes.
    *
    * @param pickableNodes The list of pickable nodes.
    */
   void setPickableNodes(std::list<Node*> pickableNodes);

   ///////////	 
private:	//
   ///////////			

      // Leap Motion:
   LEAP_CONNECTION connection; /**< Leap connection */
   LEAP_DEVICE_REF leapDevice; /**< Leap device */
   LEAP_TRACKING_EVENT curFrame; /**< Current frame */
   signed long long lastFrameId; /**< Last frame ID */

   unsigned int globalVao, vertexVbo, colorVbo; /**< OpenGL buffers */
   std::vector<glm::vec3> vertices; /**< Vertices */

   std::list<Node*> pickableNodes; /**< List of pickable nodes */
};
