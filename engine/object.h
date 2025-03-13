/**
* @file object.h
* @brief Object class header file
*
* This file contains the definition of the Object class.
*
* @details The class contains information and methods about the Object.
*
* @date 2025
*
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef OBJECT_H
#define OBJECT_H

/**
* @brief Object class
*
* The class define an object.
*
*/
class ENG_API Object {
// Public methods
public:
	static unsigned int idCounter; /**< The objects Id counter */
	/**
	 * @brief Get the object Id.
	 *
	 * @return The object Id.
	 */
	unsigned int getId();

	/**
	 * @brief Set the object Id.
	 *
	 * @param id The id to assign.
	 * 
	 */
	void setId(unsigned int id);

	/**
	 * @brief Get the object name.
	 *
	 * @return The object name.
	 */
	std::string getName();

	/**
	 * @brief Set the object name.
	 *
	 * @param name The name to assign.
	 *
	 */
	void setName(std::string name);

	/**
	 * @brief Virtual render method.
	 *
	 * @param transform The transform matrix.
	 * @param data Other data.
	 *
	 * @return True if the render is succesful.
	 */
	virtual bool render(glm::mat4 transform, void* data) = 0;

	/**
	 * @brief Get the next available Id.
	 *
	 * @return The next Id.
	 */
	unsigned int getNextId();

// Private methods and fields
private:
	unsigned int id; /**< The object Id. */
	std::string name; /**< The object name. */
};

#endif // OBJECT_H
