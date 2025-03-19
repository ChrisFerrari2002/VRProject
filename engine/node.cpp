/**
* @file node.cpp
* @brief Node class implementation file
*
* This file contains the implementation of the Node class.
*
* @details The class contains information and methods about the Node.
*
* @date 2025
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include <GL/freeglut.h>
#include "engine.h"
#include <algorithm>

/**
 * @brief Constructor
 *
 * Initializes the Node.
 *
 * @param name The node name
 */
Eng::Node::Node(std::string name) {
	Object::setId(Object::getNextId());
	Object::setName(name);
	this->setParent(nullptr);
	scale = 1.0f;
}

/**
 * @brief Destructor
 *
 * Deinitialize the Node.
 */
Eng::Node::~Node() {}

/**
 * @brief Add a child to the node.
 *
 * @param child The node to add as a child.
 *
 * @return True if the node is added.
 */
bool ENG_API Eng::Node::addChild(Node* nodeToAdd) {
	nodeToAdd->setParent(this);
	children.push_back(nodeToAdd);
	return true;
}

/**
 * @brief Remove a child from the node.
 *
 * @param child The node to remove.
 *
 * @return True if the node is removed.
 */
bool ENG_API Eng::Node::removeChild(Node* nodeToRemove) {
	auto child = std::find(children.begin(), children.end(), nodeToRemove);
	if (child != children.end()) {
		// Delete the node before erasing it from the vector
		// delete *child;

		// Erase the node from the vector
		children.erase(child);
		return true;
	}
	return false;
}

/**
 * @brief Get the list of children of the node.
 *
 *
 * @return The list of children.
 */
std::vector<Eng::Node*> ENG_API Eng::Node::getChildren() {
	return children;
}

/**
 * @brief Set the list of children of the node.
 *
 * @param The list of children.
 */
void ENG_API Eng::Node::setChildren(std::vector<Node*> newChildren) {
	children = newChildren;
}

/**
 * @brief Get the parent of the node.
 *
 *
 * @return The parent.
 */
Eng::Node* Eng::Node::getParent() {
	return parent;
}

/**
 * @brief Set the parent of the node.
 *
 *
 * @param The parent.
 */
void ENG_API Eng::Node::setParent(Node* newParent) {
	parent = newParent;
}

/**
 * @brief Node render method.
 *
 * @param transform The transform matrix.
 * @param data Other data.
 *
 * @return True if the render is succesful.
 */
bool ENG_API Eng::Node::render(glm::mat4 mat, void* attr) {
	return false;
}

/**
 * @brief Set the node scale.
 *
 *
 * @param The scale.
 */
void ENG_API Eng::Node::setScale(float scale)
{
	this->scale = scale;
}

/**
 * @brief Get the node scale.
 *
 *
 * @return The scale.
 */
float Eng::Node::getScale() const
{
	return scale;
}

/**
 * @brief Get the node transform.
 *
 * @return The node transform.
 */
glm::mat4 ENG_API Eng::Node::getTransform() const
{
	return transform;
}

/**
 * @brief Set the node transform.
 *
 * @param transform The node transform.
 */
void ENG_API Eng::Node::setTransform(glm::mat4 transform)
{
	Node::transform = transform;
}

/**
 * @brief Get the node final matrix.
 *
 * @return The node final matrix.
 */
glm::mat4 ENG_API Eng::Node::getFinalMatrix() const {

	Node* par = parent;
	std::vector<Node*> parents;
	while (par != nullptr) {
		parents.push_back(par);
		par = par->getParent();
	}

	glm::mat4 m = glm::mat4(1.0f);
	for (int i = parents.size() - 1; i >= 0; i--) {
		par = parents.at(i);
		m *= par->getTransform();
	}
	m *= transform;

	return m;
}

/**
 * @brief Get the node world position.
 *
 * @return The node world position.
 */
glm::vec3 ENG_API Eng::Node::getWorldPosition() const {
	return glm::vec3(getFinalMatrix()[3]);
}

/**
 * @brief Set the node world position.
 *
 * @param The node world position.
 */
void ENG_API Eng::Node::setWorldPosition(glm::vec3 position) {
	transform[3] = glm::vec4(position, 1.0f);
}

/**
 * @brief Get the number of children of the node.
 *
 *
 * @return The number of children.
 */
unsigned int ENG_API Eng::Node::getNumberOfChildren() {
	return getChildren().size();
}

/**
 * @brief Get the node child at an index.
 *
 * @param index The child index.
 *
 * @return The node child at the index.
 */
Eng::Node* Eng::Node::getChildAt(int index) {
	if (children.empty() || index >= (int)children.size())
		return nullptr;

	return children.at(index);
}
