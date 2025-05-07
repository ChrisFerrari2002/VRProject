/**
* @file node.h
* @brief Node class header file
*
* This file contains the definition of the Node class.
*
* @details The class contains information and methods about the Node.
*
* @date 2025
*
* @see Node
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef NODE_H
#define NODE_H

/**
* @brief Node class
*
* The class define a node.
*
*/
class ENG_API Node : public Eng::Object {
// Public methods
public:
	/**
	 * @brief Constructor
	 *
	 * Initializes the Node.
	 *
	 * @param name The node name
	 */
   Node(const std::string name);

	/**
	 * @brief Destructor
	 *
	 * Deinitialize the Node.
	 */
   ~Node();

	/**
	 * @brief Get the node transform.
	 *
	 * @return The node transform.
	 */
	glm::mat4 getTransform() const;

	/**
	 * @brief Set the node transform.
	 *
	 * @param transform The node transform.
	 */
	virtual void setTransform(glm::mat4 transform);

	glm::mat4 updateFinalMatrix();

	/**
	 * @brief Get the node final matrix.
	 *
	 * @return The node final matrix.
	 */
	glm::mat4 getFinalMatrix();

	/**
	 * @brief Get the node child at an index.
	 * 
	 * @param index The child index.
	 *
	 * @return The node child at the index.
	 */
	Node* getChildAt(int index);

	/**
	 * @brief Get the node world position.
	 *
	 * @return The node world position.
	 */
	glm::vec3 getWorldPosition();

	/**
	 * @brief Set the node world position.
	 *
	 * @param The node world position.
	 */
	virtual void setWorldPosition(glm::vec3 position);

	/**
	 * @brief Add a child to the node.
	 * 
	 * @param child The node to add as a child.
	 *
	 * @return True if the node is added.
	 */
	bool addChild(Node* child);

	/**
	 * @brief Remove a child from the node.
	 *
	 * @param child The node to remove.
	 *
	 * @return True if the node is removed.
	 */
	bool removeChild(Node* child);

	/**
	 * @brief Get the list of children of the node.
	 *
	 *
	 * @return The list of children.
	 */
	std::vector<Node*> getChildren();

	/**
	 * @brief Set the list of children of the node.
	 *
	 * @param The list of children.
	 */
	void setChildren(std::vector<Node*> children);

	/**
	 * @brief Get the number of children of the node.
	 *
	 *
	 * @return The number of children.
	 */
	unsigned int getNumberOfChildren();

	/**
	 * @brief Get the parent of the node.
	 *
	 *
	 * @return The parent.
	 */
	Node* getParent();

	/**
	 * @brief Set the parent of the node.
	 *
	 *
	 * @param The parent.
	 */
	void setParent(Node* parent);

	/**
	 * @brief Node render method.
	 *
	 * @param transform The transform matrix.
	 * @param data Other data.
	 *
	 * @return True if the render is succesful.
	 */
	virtual bool render(glm::mat4 transform, void* data) override;

	/**
	 * @brief Set the node scale.
	 *
	 *
	 * @param The scale.
	 */
	void setScale(float scale);

	/**
	 * @brief Get the node scale.
	 *
	 *
	 * @return The scale.
	 */
	float getScale() const;

	virtual float getBoundingSphereRadius() const { return 0.0f; }

	bool isGrabbablee() const { return isGrabbable; }


// Private methods and fields
private:
	glm::mat4 finalMatrix = glm::mat4(1.0f);
	glm::mat4 transform = glm::mat4(1.0f); /**< The node transform */
	std::vector<Node*> children; /**< The list of children */
	Node* parent; /**< The node parent. */                          
	float scale; /**< The node scale. */   
	bool isDirty; /**< The node dirty flag. */
	bool isGrabbable; /**< The node grabbable flag. */
};

#endif // NODE_H
