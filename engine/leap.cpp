/**
 * @file		leap.cpp
 * @brief	Minimal Leap Motion API wrapper
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */



 //////////////
 // #INCLUDE //
 //////////////

    // Header:
#include <GL/glew.h>
#include "engine.h"

//////////////
// #SHADERs //
//////////////
const char* leapVertShader = R"(
#version 440 core

uniform mat4 projection;
uniform mat4 modelview;

layout(location = 0) in vec3 in_Position;

void main(void)
{
   gl_Position = projection * modelview * vec4(in_Position, 1.0f);      
})";
////////////////////////////
const char* leapFragShader = R"(
#version 440 core
   
uniform vec3 color;   
out vec4 frag_Output;
   
void main(void)
{      
   frag_Output = vec4(color, 1.0f);
})";

////////////////////////
// BODY OF CLASS Leap //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor.
 */
Eng::Leap::Leap() : connection{ nullptr }, curFrame{ nullptr },
lastFrameId{ 0 }
{}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Destructor.
 */
Eng::Leap::~Leap()
{}

std::vector<glm::vec3> debugSphereVertices;
std::vector<unsigned int> debugSphereIndices;
GLuint debugSphereVao, debugSphereVbo, debugSphereEbo;

void createDebugSphere(int slices = 16, int stacks = 16) {
   debugSphereVertices.clear();
   debugSphereIndices.clear();

   for (int i = 0; i <= stacks; ++i) {
      float phi = glm::pi<float>() * float(i) / float(stacks);
      for (int j = 0; j <= slices; ++j) {
         float theta = 2.0f * glm::pi<float>() * float(j) / float(slices);
         float x = sin(phi) * cos(theta);
         float y = cos(phi);
         float z = sin(phi) * sin(theta);
         debugSphereVertices.emplace_back(x, y, z);
      }
   }

   for (int i = 0; i < stacks; ++i) {
      for (int j = 0; j < slices; ++j) {
         int first = (i * (slices + 1)) + j;
         int second = first + slices + 1;

         // Line segments
         debugSphereIndices.push_back(first);
         debugSphereIndices.push_back(second);
         debugSphereIndices.push_back(first);
         debugSphereIndices.push_back(first + 1);
      }
   }

   glGenVertexArrays(1, &debugSphereVao);
   glGenBuffers(1, &debugSphereVbo);
   glGenBuffers(1, &debugSphereEbo);

   glBindVertexArray(debugSphereVao);

   glBindBuffer(GL_ARRAY_BUFFER, debugSphereVbo);
   glBufferData(GL_ARRAY_BUFFER, debugSphereVertices.size() * sizeof(glm::vec3), &debugSphereVertices[0], GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugSphereEbo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, debugSphereIndices.size() * sizeof(unsigned int), &debugSphereIndices[0], GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

   glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Initializes Leap Motion connection.
 * @return TF
 */
bool Eng::Leap::init()
{
   // Create and open connection:   
   if (LeapCreateConnection(nullptr, &connection) != eLeapRS_Success)
   {
      std::cout << "[ERROR] Unable to connect to the Leap Motion" << std::endl;
      return false;
   }
   if (LeapOpenConnection(connection) != eLeapRS_Success)
   {
      std::cout << "[ERROR] Unable to open connection to the Leap Motion" << std::endl;
      LeapDestroyConnection(connection);
      connection = nullptr;
      return false;
   }

   // Get device (only one supported):
   const uint32_t timeout = 1000;
   uint32_t computedArraySize = 0;
   for (uint32_t retry = 3; retry > 0; retry--)
   {
      LEAP_CONNECTION_MESSAGE msg;
      LeapPollConnection(connection, timeout, &msg);

      eLeapRS ret = LeapGetDeviceList(connection, nullptr, &computedArraySize);
      if (ret == eLeapRS_NotConnected)
         continue;
      else
         if (ret == eLeapRS_Success)
            if (computedArraySize)
               break;
   }

   std::cout << "Number of available devices: " << computedArraySize << std::endl;
   if (computedArraySize != 1)
   {
      std::cout << "[ERROR] Invalid number of devices (must be 1)" << std::endl;
      LeapCloseConnection(connection);
      LeapDestroyConnection(connection);
      connection = nullptr;
      return false;
   }

   // Get ref to device:      
   if (LeapGetDeviceList(connection, &leapDevice, &computedArraySize) != eLeapRS_Success)
   {
      std::cout << "[ERROR] Unable to get ref to device" << std::endl;
      LeapCloseConnection(connection);
      LeapDestroyConnection(connection);
      connection = nullptr;
      return false;
   }

   Shader* leapVs = new Shader();

   leapVs->loadFromMemory(Shader::TYPE_VERTEX, leapVertShader);
   
   Shader* leapFs = new Shader();
   leapFs->loadFromMemory(Shader::TYPE_FRAGMENT, leapFragShader);

   Shader* leapShader = new Shader();
   leapShader->build(leapVs, leapFs);
   Shader::mapShader("leapShader", leapShader);


   float x, y, z, alpha, beta;
   float radius = 5.0f;
   int gradation = 10;
   for (alpha = 0.0; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
      for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
      {
         x = radius * cos(beta) * sin(alpha);
         y = radius * sin(beta) * sin(alpha);
         z = radius * cos(alpha);
         vertices.push_back(glm::vec3(x, y, z));
         x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
         y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
         z = radius * cos(alpha + glm::pi<float>() / gradation);
         vertices.push_back(glm::vec3(x, y, z));
      }

   glGenVertexArrays(1, &globalVao);
   glBindVertexArray(globalVao);

   glGenBuffers(1, &vertexVbo);
   glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);

   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
   glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
   glEnableVertexAttribArray(0);
   Shader::getShader("leapShader")->bind(0, "in_Position");
   createDebugSphere();
   // Done:
   return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Releases Leap Motion resources.
 * @return TF
 */
bool Eng::Leap::free()
{
   // Clean up Leap Motion:
   LeapCloseConnection(connection);
   LeapDestroyConnection(connection);
   connection = nullptr;

   // Done:
   return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Acquire new events.
 * @return TF
 */
bool Eng::Leap::update()
{
   const uint32_t timeout = 1000;
   LEAP_CONNECTION_MESSAGE msg;

   // Get latest update:
   int64_t now = LeapGetNow();
   int32_t frameSkipping = -1;
   do
   {
      if (LeapPollConnection(connection, timeout, &msg) != eLeapRS_Success)
      {
         std::cout << "[ERROR] Unable to poll connection" << std::endl;
         return false;
      }
      frameSkipping++;
   } while (msg.tracking_event->info.timestamp < now - 100);

   // Only tracking events are processed:
   if (msg.type != eLeapEventType_Tracking)
      return false;

   // We are lagging badly:
   if (frameSkipping > 5)
      std::cout << "[WARNING] Lag detected (" << frameSkipping << " frames skipped)" << std::endl;

   // Update only when necessary:   
   if (msg.tracking_event->tracking_frame_id > lastFrameId)
   {
      lastFrameId = msg.tracking_event->tracking_frame_id;
      curFrame = *msg.tracking_event;
   }

   // Done:
   return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Gets a pointer to the last updated frame.
 * @return pointer to last frame, or nullptr on error
 */
const LEAP_TRACKING_EVENT* Eng::Leap::getCurFrame() const
{
   return &curFrame;
}

glm::vec3 leapToVec(const LEAP_VECTOR& leapVec) {
   // Leap Motion restituisce le coordinate in millimetri, quindi bisogna fare la conversione in metri.
   return glm::vec3(leapVec.x, leapVec.y, leapVec.z);
}

bool puntoNelRaggio(const glm::vec3& punto, const glm::vec3& centroSfera, float raggio) {
   // Calcolare la distanza quadrata tra il punto e il centro della sfera usando il prodotto scalare
   float distanzaQuadrata = glm::dot(punto - centroSfera, punto - centroSfera);
   // Confrontare con il quadrato del raggio
   return distanzaQuadrata <= raggio * raggio;
}
static std::vector<Eng::Node*> grabbedNodes = { nullptr, nullptr };
static std::vector<glm::vec3> grabOffsets;
static std::vector<glm::vec3> originalPositions;

void Eng::Leap::renderNormalHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix) {
   glBindVertexArray(globalVao);
   Shader::getCurrentShader()->setMatrix("projection", projMatrix);

   glm::vec3 scaleFactor(0.2f); // Riduce la dimensione e la distanza al 20%

   if (grabbedNodes.size() != l->nHands) {
      grabbedNodes.resize(l->nHands, nullptr);
      grabOffsets.resize(l->nHands, glm::vec3(0.0f));
   }

   for (unsigned int h = 0; h < l->nHands; h++) {
      LEAP_HAND hand = l->pHands[h];
      glm::mat4 f = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -30.0f, -30.0f));

      // Calcola la posizione media tra pollice e indice (punto di pinch)
      glm::vec3 thumbTip(hand.digits[0].bones[3].next_joint.x,
         hand.digits[0].bones[3].next_joint.y,
         hand.digits[0].bones[3].next_joint.z);

      glm::vec3 indexTip(hand.digits[1].bones[3].next_joint.x,
         hand.digits[1].bones[3].next_joint.y,
         hand.digits[1].bones[3].next_joint.z);

      glm::vec3 pinchPos = (thumbTip + indexTip) * 0.5f * scaleFactor;
      glm::vec3 pinchWorldPos = glm::vec3(modelViewMat * f * glm::vec4(pinchPos, 1.0f));

      bool isPinching = hand.pinch_strength > 0.7f;
      bool handIsGrabbing = false;

      Node* grabbedNode = grabbedNodes[h];
      glm::vec3 grabOffset = grabOffsets[h];

      for (auto& node : pickableNodes) {
         glm::vec3 nodeCenter = node->getWorldPosition();
         float radius = node->getBoundingSphereRadius();

         bool isInsideSphere = puntoNelRaggio(pinchWorldPos, nodeCenter, radius);
         bool alreadyGrabbed = std::find(grabbedNodes.begin(), grabbedNodes.end(), node) != grabbedNodes.end();

         if (grabbedNode == nullptr && isInsideSphere && isPinching && !alreadyGrabbed) {
            grabbedNode = node;
            grabOffset = pinchWorldPos - nodeCenter;
         }

         if (grabbedNode == node && isPinching) {
            handIsGrabbing = true;

            Shader::getCurrentShader()->setVec3("color", glm::vec3(0.5f, 0.0f, 0.5f));  // Colore viola quando si afferra un oggetto

            glm::vec3 newPosition = pinchWorldPos - grabOffset;
            glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), newPosition);
            grabbedNode->setTransform(newTransform);
            grabbedNode->setWorldPosition(newPosition);
         }

         if (!isPinching && grabbedNode == node) {
            grabbedNode = nullptr;
         }

         grabbedNodes[h] = grabbedNode;
         grabOffsets[h] = grabOffset;
      }

      // Se non si sta afferrando nulla, la mano prende il colore normale
      if (!isPinching || !handIsGrabbing) {
         Shader::getCurrentShader()->setVec3("color", glm::vec3((float)h, (float)(1 - h), 0.5f));  // Colore normale
      }
      // Se si sta facendo un pinch ma non si afferra nulla, coloriamo diversamente (ad esempio in giallo)
      else if (isPinching && !handIsGrabbing) {
         Shader::getCurrentShader()->setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));  // Colore giallo per pinch senza afferrare
      }

      // --- Elbow ---
      {
         glm::vec3 pos(hand.arm.prev_joint.x, hand.arm.prev_joint.y, hand.arm.prev_joint.z);
         glm::mat4 t = glm::translate(glm::mat4(1.0f), pos * scaleFactor);
         glm::mat4 s = glm::scale(glm::mat4(1.0f), scaleFactor);
         Shader::getCurrentShader()->setMatrix("modelview", f * t * s);
         glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
      }

      // --- Wrist ---
      {
         glm::vec3 pos(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z);
         glm::mat4 t = glm::translate(glm::mat4(1.0f), pos * scaleFactor);  // Applica la scala alla posizione
         glm::mat4 s = glm::scale(glm::mat4(1.0f), scaleFactor);  // Scala solo la geometria
         Shader::getCurrentShader()->setMatrix("modelview", f * t * s);
         glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
      }

      // --- Palm ---
      {
         glm::vec3 pos(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z);
         glm::mat4 t = glm::translate(glm::mat4(1.0f), pos * scaleFactor);  // Applica la scala alla posizione
         glm::mat4 s = glm::scale(glm::mat4(1.0f), scaleFactor);  // Scala solo la geometria
         Shader::getCurrentShader()->setMatrix("modelview", f * t * s);
         glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
      }

      // --- Finger Bones ---
      for (unsigned int d = 0; d < 5; d++) {
         LEAP_DIGIT finger = hand.digits[d];
         for (unsigned int b = 0; b < 4; b++) {
            LEAP_BONE bone = finger.bones[b];
            glm::vec3 pos(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z);
            glm::mat4 t = glm::translate(glm::mat4(1.0f), pos * scaleFactor);  // Applica la scala alla posizione
            glm::mat4 s = glm::scale(glm::mat4(1.0f), scaleFactor);  // Scala solo la geometria
            Shader::getCurrentShader()->setMatrix("modelview", f * t * s);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
         }
      }
   }

   glBindVertexArray(0);
}




glm::vec3 leapToMeters(const LEAP_VECTOR& leapVec) {
   // Leap Motion restituisce le coordinate in millimetri, quindi bisogna fare la conversione in metri.
   return glm::vec3(leapVec.x * 0.001f, leapVec.y * 0.001f, leapVec.z * 0.001f);
}
static bool firstRun = true;
void Eng::Leap::renderVRHandBones(const LEAP_TRACKING_EVENT* l, const glm::mat4 modelViewMat, const glm::mat4 projMatrix, const glm::mat4 leapToWorldMatrix) {
   glBindVertexArray(globalVao);
   Shader::getCurrentShader()->setMatrix("projection", projMatrix);

   const float margin = 0.10f;
   const float xMin = -0.054846f - (0.216109f * margin);
   const float xMax = 0.161263f + (0.216109f * margin);
   const float zMin = -0.108148f - (0.216647f * margin);
   const float zMax = 0.108499f + (0.216647f * margin);

   // Funzione di controllo area
   auto isInsideValidArea = [&](const glm::vec3& pos) {
      return (pos.x >= xMin && pos.x <= xMax) &&
         (pos.z >= zMin && pos.z <= zMax);
      };

   const float cemeteryMargin = 0.03f;
   const float cemeteryXMin = xMax + cemeteryMargin;
   const float cemeteryXMax = cemeteryXMin + 0.15;
   const float cemeteryZMin = zMin - 0.05;
   const float cemeteryZMax = zMax - 0.05;

   auto isInCemeteryArea = [&](const glm::vec3& position) {
      return (position.x >= cemeteryXMin && position.x <= cemeteryXMax) &&
         (position.z >= cemeteryZMin && position.z <= cemeteryZMax);
      };
  
   glm::vec3 scale = glm::vec3(0.001f);
   if (firstRun || grabbedNodes.size() != l->nHands) {
      grabbedNodes.resize(l->nHands, nullptr);
      grabOffsets.resize(l->nHands, glm::vec3(0.0f));
      originalPositions.resize(l->nHands, glm::vec3(0.0f));
      firstRun = false;
   }

   for (unsigned int h = 0; h < l->nHands; h++) {
      LEAP_HAND hand = l->pHands[h];
      glm::mat4 f = modelViewMat;

      bool isPinching = hand.pinch_strength > 0.4f;
      bool handIsGrabbing = false;

      // Converti le coordinate Leap da mm a metri
      glm::vec3 thumbTip = leapToMeters(hand.digits[0].bones[3].next_joint);
      glm::vec3 indexTip = leapToMeters(hand.digits[1].bones[3].next_joint);

      // Calcola la posizione del pinch in spazio mondo
      glm::vec3 pinchPos = (thumbTip + indexTip) * 0.5f;
      glm::vec3 pinchWorldPos = glm::vec3(leapToWorldMatrix * glm::vec4(pinchPos, 1.0f));

      Node* grabbedNode = grabbedNodes[h];
      glm::vec3 grabOffset = grabOffsets[h];

      if (!isPinching && grabbedNode != nullptr) {
         glm::vec3 currentPos = grabbedNode->getWorldPosition();
         if (!isInsideValidArea(currentPos) && !isInCemeteryArea(currentPos)) {
            glm::vec3 newPos = glm::vec3(originalPositions[h].x, originalPositions[h].y, originalPositions[h].z);
            grabbedNode->setWorldPosition(newPos);

            // Aggiorna la trasformazione completa
            const glm::mat4 currentTransform = grabbedNode->getTransform();
            const glm::vec3 currentScale(
               glm::length(glm::vec3(currentTransform[0])),
               glm::length(glm::vec3(currentTransform[1])),
               glm::length(glm::vec3(currentTransform[2]))
            );
            const glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), newPos) * glm::scale(glm::mat4(1.0f), currentScale);
            grabbedNode->setTransform(newTransform);

            grabbedNodes[h] = nullptr;
            grabbedNode = nullptr;
            handIsGrabbing = false;
         }
         else {
            glm::vec3 newPos = glm::vec3(currentPos.x, originalPositions[h].y, currentPos.z);
            grabbedNode->setWorldPosition(newPos);

            // Aggiorna la trasformazione completa
            const glm::mat4 currentTransform = grabbedNode->getTransform();
            const glm::vec3 currentScale(
               glm::length(glm::vec3(currentTransform[0])),
               glm::length(glm::vec3(currentTransform[1])),
               glm::length(glm::vec3(currentTransform[2]))
            );
            const glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), newPos) * glm::scale(glm::mat4(1.0f), currentScale);
            grabbedNode->setTransform(newTransform);

            grabbedNodes[h] = nullptr;
            grabbedNode = nullptr;
            handIsGrabbing = false;
         }
         
      }

      // Cerca un nodo da afferrare se non ne stai già afferrando uno
      if (grabbedNode == nullptr && isPinching) {
         for (auto& node : pickableNodes) {
            bool alreadyGrabbed = std::find(grabbedNodes.begin(), grabbedNodes.end(), node) != grabbedNodes.end();
            if (alreadyGrabbed) continue;

            const glm::vec3 nodeCenter = node->getWorldPosition();
            const float radius = node->getBoundingSphereRadius();
            const float distance = glm::distance(pinchWorldPos, nodeCenter);

            if (distance <= radius * 10.0f) {
               grabbedNode = node;
               grabOffset = pinchWorldPos - nodeCenter;
               originalPositions[h] = nodeCenter;
               break;
            }
         }
      }

      // Update grabbed object position
      if (isPinching && grabbedNode != nullptr) {
         Shader::getCurrentShader()->setVec3("color", glm::vec3(0.5f, 0.0f, 0.5f));  // Purple for grabbing

         const glm::mat4 currentTransform = grabbedNode->getTransform();

         // Extract scale
         const glm::vec3 currentScale(
            glm::length(glm::vec3(currentTransform[0])),
            glm::length(glm::vec3(currentTransform[1])),
            glm::length(glm::vec3(currentTransform[2]))
         );

         // Calculate new position
         const glm::vec3 newPosition = pinchWorldPos - grabOffset;

         // Create new transform
         const glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), newPosition) * glm::scale(glm::mat4(1.0f), currentScale);

         grabbedNode->setTransform(newTransform);
         grabbedNode->setWorldPosition(newPosition);

         grabbedNodes[h] = grabbedNode;
         grabOffsets[h] = grabOffset;
      }
      else if (isPinching) {
         Shader::getCurrentShader()->setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));  // Yellow for pinching
      }
      else {
         Shader::getCurrentShader()->setVec3("color", glm::vec3((float)h, (float)(1 - h), 0.5f));  // Normal color
      }

      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.001f));
      // Elbow (gomito)
      glm::mat4 c = glm::translate(glm::mat4(1.0f), leapToMeters(hand.arm.prev_joint));
      Shader::getCurrentShader()->setMatrix("modelview", f * c * scale);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());

      // Wrist (polso)
      c = glm::translate(glm::mat4(1.0f), leapToMeters(hand.arm.next_joint));
      Shader::getCurrentShader()->setMatrix("modelview", f * c * scale);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());

      // Palm (palmo)
      c = glm::translate(glm::mat4(1.0f), leapToMeters(hand.palm.position));
      Shader::getCurrentShader()->setMatrix("modelview", f * c * scale);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());

      // Distal ends of bones for each digit (distanza delle ossa per ogni dito)
      for (unsigned int d = 0; d < 5; d++) {
         LEAP_DIGIT finger = hand.digits[d];
         for (unsigned int b = 0; b < 4; b++) {
            LEAP_BONE bone = finger.bones[b];
            c = glm::translate(glm::mat4(1.0f), leapToMeters(bone.next_joint));
            Shader::getCurrentShader()->setMatrix("modelview", f * c * scale);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
         }
      }
    } 
   glBindVertexArray(0);
}

void Eng::Leap::setPickableNodes(std::list<Node*> pickableNodes) {
   this->pickableNodes = pickableNodes;
}
