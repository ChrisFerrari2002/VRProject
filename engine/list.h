/**
* @file list.h
* @brief List class header file
*
* This file contains the definition of the List class that represents a list of nodes in the engine.
*
* @date 2025
*
* @details The List class is a subclass of the Object class and represents a list of nodes in the engine.
* @see Eng::Object, Eng::Node
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef LIST_H
#define LIST_H

#include "engine.h"

/**
* @brief List class
*
* The List class is a subclass of the Object class and represents a list of nodes in the engine.
*/
class ENG_API List : public Eng::Object {
public:
    /**
    * @brief Constructor
    *
    * Initializes an empty list.
    */
    List() {};

    /**
    * @brief Add an entry (node) to the list
    *
    * Adds a node to the list.
    * If the node is a light, it is placed at the beginning of the list.
    *
    * @param node Pointer to the node to be added.
    */
    void addEntry(Eng::Node* node);

    /**
    * @brief Remove the last entry from the list
    *
    * Removes the last node from the list.
    */
    void popEntry();

    /**
    * @brief Render the list
    *
    * Renders the list using the given transformation matrix.
    *
    * @param transform The transformation matrix.
    * @param data A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
    bool render(glm::mat4 transform, glm::mat4 projectionMatrix, void* data);

    /**
    * @brief Clear the list
    *
    * Clears all nodes from the list.
    */
    void clear();

    /**
    * @brief Get an object from the list by index
    *
    * Gets a node from the list by its index.
    *
    * @param index The index of the node to be retrieved.
    * @return Pointer to the node at the specified index.
    */
    Eng::Node* getObject(int index);

    /**
    * @brief Get an object from the list by ID
    *
    * Gets a node from the list by its ID.
    *
    * @param id The ID of the node to be retrieved.
    * @return Pointer to the node with the specified ID.
    */
    Eng::Node* getObjectById(int id);

    /**
    * @brief Get the list of objects
    *
    * Gets the list of nodes.
    *
    * @return A list of pointers to the nodes.
    */
    std::list<Eng::Node*> getObjectList();

    /**
    * @brief Get the size of the list
    *
    * Gets the number of nodes in the list.
    *
    * @return The number of nodes in the list.
    */
    int size();

    void addSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
       const std::string& face4, const std::string& face5, const std::string& face6);

    std::list<Eng::Node*> getPickableObjectsList() { return pickableObjectsList; };

    bool render(glm::mat4 transform, void* data) { return false; };

private:
    std::list<Eng::Node*> objectsList; /**< The list of nodes */
    std::list<Eng::Node*> lightsList; /**< The list of lights */
    std::list<Eng::Node*> pickableObjectsList; /**< The list of pickable objects */
    Eng::Shader* spotLightShader;
    Eng::Shader* directionalLightShader;
    Eng::Shader* pointLightShader;
};

#endif // LIST_H