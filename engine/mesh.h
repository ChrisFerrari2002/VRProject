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
    * @brief Render the mesh.
    *
    * Renders the mesh using the given transformation matrix.
    *
    * @param transform The transformation matrix.
    * @param data A pointer to additional data.
    * @return True if the rendering was successful, false otherwise.
    */
    virtual bool render(glm::mat4 transform, void* data) override;

    /**
    * @brief Get the material of the mesh.
    *
    * @return Pointer to the material of the mesh.
    */
    Eng::Material* getMaterial();

   /**
   * @brief Set the vertices of the mesh.
   *
   * Set the vertices of the mesh.
   *
   * @param vertices The vertices to be set.
   * 
   */
    void setVertices(std::vector<glm::vec3> vertices);

    /**
   * @brief Set the normals of the mesh.
   *
   * Set the normals of the mesh.
   *
   * @param normals The normals to be set.
   *
   */
    void setNormals(std::vector<glm::vec3> normals);

    /**
    * @brief Set the texture coordinates of the mesh.
    *
    * Set the texture coordinates of the mesh.
    *
    * @param texCoords The texture coordinates to be set.
    *
    */
    void setTexCoords(std::vector<glm::vec2> texCoords);

    /**
    * @brief Set the faces of the mesh.
    *
    * Set the faces coordinates of the mesh.
    *
    * @param faces The texture faces to be set.
    *
    */
    void setFaces(std::vector<unsigned int> faces);

    /**
    * @brief Initialize mesh buffers and context.
    *
    * Initialize mesh buffers and context.
    * 
    */
    void setupMesh();

    /**
    * @brief Set the sphere bounding radius the mesh.
    *
    * Set the sphere bounding radius the mesh.
    *
    * @param radius The radius to be set.
    *
    */
    void setSphereRadius(float radius) { sphereRadius = radius; }

    /**
    * @brief Get the bounding sphere radius of the mesh.
    *
    * @return Bounding sphere radius of the mesh.
    */
    float getBoundingSphereRadius() const override { return sphereRadius; };

protected:
    int lod = 0; /**< The level of detail (LOD) of the mesh */

private:
    Eng::Material material; /**< The material of the mesh */
    std::vector<glm::vec3> vertices, normals; /**< The vertices and normals of the mesh */
    std::vector<glm::vec2> texCoords; /**< The texture coordinates of the mesh */
    std::vector<unsigned int> faces; /**< The faces of the mesh */

    unsigned int vao, vertexVBO, normalsVBO, texCoordVBO, facesVBO; /**< OpenGL buffers */
    unsigned int facesCount; /**< Number of faces in the mesh */

    float sphereRadius = 0; /**< The radius of the bounding sphere of the mesh */
};

#endif // MESH_H