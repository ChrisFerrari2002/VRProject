/**
* @file list.cpp
* @brief Implementation of the List class
*
* This file contains the implementation of the List class methods.
*
* @see List
* @see list.h
*
* @date 2025
*
* @details The List class is a subclass of the Object class and represents a list of nodes in the engine.
* @see Eng::Object, Eng::Node
*
 * @authors
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "engine.h"

/**
* @brief Add an entry (node) to the list
*
* Adds a node to the list.
* If the node is a light, it is placed at the beginning of the list.
*
* @param root Pointer to the root node to be added.
*/
void ENG_API Eng::List::addEntry(Node* root) {
    //don't get the root
    Node* node = root->getChildAt(0);

    int children = root->getNumberOfChildren();
    //insert nodes to list
    for (int i = 1; i <= children; i++) {
        //Add node
        Light* v;
        if ((v = dynamic_cast<Light*>(node))) {
           lightsList.push_back(node);
        }
        else {
           if (node->isGrabbablee()) {
              pickableObjectsList.push_back(node);
           }
           
            objectsList.push_back(node);
        }
        //Add children of children
        if (node != NULL && node->getNumberOfChildren() > 0) {
            addEntry(node);
        }
        node = root->getChildAt(i);
    }
}

/**
* @brief Remove the last entry from the list
*
* Removes the last node from the list.
*/
void ENG_API Eng::List::popEntry() {
    objectsList.pop_back();
}

/**
* @brief Render the list
*
* Renders the list using the given transformation matrix.
*
* @param cameraMatrix The transformation matrix.
* @param ptr A pointer to additional data.
* @return True if the rendering was successful, false otherwise.
*/
bool Eng::List::render(glm::mat4 inverseCameraMatrix, glm::mat4 projectionMatrix, void* ptr) {

   std::list<Node*>::iterator lightsIt;
   int index = 0;

   for (lightsIt = lightsList.begin(); lightsIt != lightsList.end(); lightsIt++, index++) {
      if (index == 1) {
         glEnable(GL_BLEND);
         glBlendFunc(GL_ONE, GL_ONE);
      }
      
      Light* light = dynamic_cast<Light*>(*lightsIt);
      if (light) {
         if (dynamic_cast<DirectionalLight*>(light)) {
         }
         else if (dynamic_cast<SpotLight*>(light)) {
         }
         else if (dynamic_cast<PointLight*>(light)) {
         }
         light->render(inverseCameraMatrix * light->getFinalMatrix(), ptr);
      }

      glm::mat4 vp = projectionMatrix * inverseCameraMatrix;
      Eng::Frustum frustum = extractFrustumPlanes(vp);

      std::list<Node*>::iterator nodesIt;
      for (auto& nodePtr : objectsList) {
         Node* node = dynamic_cast<Node*>(nodePtr);
         if (!node) {
            continue;  // Salta se l'elemento non è un nodo
         }

         glm::vec3 worldPosition = glm::vec3(node->getFinalMatrix()[3]);
         float radius = node->getBoundingSphereRadius();

         // Controllo se la sfera del nodo è visibile nel frustum
         if (frustum.sphereInFrustum(worldPosition, radius)) {
            glm::mat4 nodeTransform = inverseCameraMatrix * node->getFinalMatrix();
            node->render(nodeTransform, ptr);
         }
      }

   }

   if (lightsList.size() > 1)
      glDisable(GL_BLEND);

   return true;
}

/**
* @brief Clear the list
*
* Clears all nodes from the list.
*/
void Eng::List::clear()
{
    std::list<Node*>::iterator it;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        Light* v;
        if ((v = dynamic_cast<Light*>(*it)))
            delete (Light*)(*it);
        else
            delete (*it);
    }

    objectsList.clear();
}

/**
* @brief Get an object from the list by index
*
* Gets a node from the list by its index.
*
* @param i The index of the node to be retrieved.
* @return Pointer to the node at the specified index.
*/
Eng::Node ENG_API* Eng::List::getObject(int i) {
    std::list<Node*>::iterator it;
    int counter = 0;
    //Render each list element
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if (counter == i)
            return (*it);
        counter++;
    }
    return NULL;
}

/**
* @brief Get the size of the list
*
* Gets the number of nodes in the list.
*
* @return The number of nodes in the list.
*/
int ENG_API Eng::List::size() {
    return objectsList.size();
}

/**
* @brief Get an object from the list by ID
*
* Gets a node from the list by its ID.
*
* @param id The ID of the node to be retrieved.
* @return Pointer to the node with the specified ID.
*/
Eng::Node ENG_API* Eng::List::getObjectById(int id) {
    std::list<Node*>::iterator it;
    for (it = objectsList.begin(); it != objectsList.end(); it++)
    {
        if ((*it)->getId() == id)
            return (*it);
    }
    return NULL;
}

/**
* @brief Get the list of objects
*
* Gets the list of nodes.
*
* @return A list of pointers to the nodes.
*/
std::list<Eng::Node*> ENG_API Eng::List::getObjectList() {
    return this->objectsList;
}