/**
* @file mesh.h
* @brief Mesh class header file
*
* This file contains the definition of the Mesh class that represents a mesh in the engine.
*
* @date 2025
*
* @details The Mesh class is a subclass of the Node class and represents a mesh in the engine.
* @see Eng::Node, Eng::Material, Eng::Vertex
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

#ifndef MESH_H
#define MESH_H

#include "engine.h"

/////////////
// FORWARD DECLARATION //
/////////////

/**
* @brief Node class forward declaration
*/
class Node;

/**
* @brief Mesh class
*
* The Mesh class is a subclass of the Node class and represents a mesh in the engine.
*/
class ENG_API Mesh : public Eng::Node {
public:
    /**
    * @brief Constructor
    *
    * Initializes a mesh with the given name and material.
    *
    * @param name The name of the mesh.
    * @param material The material of the mesh.
    */
    Mesh(const std::string name, Eng::Material material);

    /**
    * @brief Destructor
    *
    * Destroys the mesh object.
    */
    ~Mesh();

    /**
    * @brief Render the mesh
    *
    * Renders the mesh using the given transformation matrix.
    *
    * @param transform The transformation matrix.
    * @param data A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
    virtual bool render(glm::mat4 transform, void* data) override;

    /**
    * @brief Get the material of the mesh
    *
    * @return Pointer to the material of the mesh.
    */
    Eng::Material* getMaterial();

    /**
    * @brief Get the color based on ID
    *
    * Gets the color of the mesh based on the specified ID.
    *
    * @param id The ID for which the color should be retrieved.
    * @return The color of the mesh based on the specified ID.
    */
    glm::vec4 getColorBasedOnId(int id);

    void setVertices(std::vector<glm::vec3> vertices);
    void setNormals(std::vector<glm::vec3> normals);
    void setTexCoords(std::vector<glm::vec2> texCoords);
    void setFaces(std::vector<unsigned int> faces);

    void setupMesh();

protected:
    int lod = 0; /**< The level of detail (LOD) of the mesh */

private:
    Eng::Material material; /**< The material of the mesh */
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> faces;

    unsigned int vao, vertexVBO, normalsVBO, texCoordVBO, facesVBO;
    unsigned int facesCount;
    int mvLoc;
};

#endif // MESH_H