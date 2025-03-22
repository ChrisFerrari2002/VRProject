/**
* @file mesh.cpp
* @brief Implementation of the Mesh class
*
* This file contains the implementation of the Mesh class methods.
*
* @see Mesh
* @see mesh.h
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
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "engine.h"

/**
* @brief Constructor
*
* Initializes a mesh with the given name and material.
*
* @param name The name of the mesh.
* @param material The material of the mesh.
*/
Eng::Mesh::Mesh(std::string name, Material material) : Node(name) {
   this->material = material;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vertexVBO);
   glGenBuffers(1, &normalsVBO);
   glGenBuffers(1, &texCoordVBO);
   glGenBuffers(1, &facesVBO);
}

/**
* @brief Destructor
*
* Destroys the mesh object.
*/
Eng::Mesh::~Mesh() {
   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vertexVBO);
   glDeleteBuffers(1, &normalsVBO);
   glDeleteBuffers(1, &texCoordVBO);
   glDeleteBuffers(1, &facesVBO);
}

/**
* @brief Render the mesh
*
* Renders the mesh using the given transformation matrix.
*
* @param transform The transformation matrix.
* @param data A pointer to additional data.
* @return True if the rendering was successful, false otherwise.
*/
bool ENG_API Eng::Mesh::render(glm::mat4 matrix, void* ptr) { 
   material.render(matrix, ptr);

   Shader::getCurrentShader()->setMatrix("modelview", matrix);
   Shader::getCurrentShader()->setMatrix3("normalMatrix", glm::inverseTranspose(glm::mat3(matrix)));

   glBindVertexArray(vao);
   glDrawElements(GL_TRIANGLES, facesCount, GL_UNSIGNED_INT, nullptr);
   //glDrawArrays(GL_TRIANGLES, 0, facesCount);
   glBindVertexArray(0);

   return true;
}

/**
* @brief Get the material of the mesh
*
* @return Pointer to the material of the mesh.
*/
Eng::Material* Eng::Mesh::getMaterial() {
   return &material;
}

void Eng::Mesh::setVertices(std::vector<glm::vec3> vertices) {
   this->vertices = vertices;
}
void Eng::Mesh::setNormals(std::vector<glm::vec3> normals) {
   this->normals = normals;
}
void Eng::Mesh::setTexCoords(std::vector<glm::vec2> texCoords) {
   this->texCoords = texCoords;
}
void Eng::Mesh::setFaces(std::vector<unsigned int> faces) {
   this->faces = faces;

}

void Eng::Mesh::setupMesh() {
   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
   glEnableVertexAttribArray(0);

   Shader* shader = Shader::getCurrentShader();
   shader->bind(0, "in_Position");

   glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
   glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
   glEnableVertexAttribArray(1);
   shader->bind(1, "in_Normal");

   glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
   glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
   glEnableVertexAttribArray(2);
   shader->bind(2, "inTexCoord");

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), faces.data(), GL_STATIC_DRAW);

   glBindVertexArray(0);
   this->facesCount = faces.size();
}

