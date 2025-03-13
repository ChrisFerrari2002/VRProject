#include "perspective.h"
// GLM:      
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeGLUT:
#include <GL/freeglut.h>

Perspective::Perspective(const std::string& name) : Camera(name) {}

glm::mat4 Perspective::getProjectionMatrix() const {
   return projectionMatrix;
}

void Perspective::setProjectionMatrix(const glm::mat4& m) {
   const float aspect_ratio = static_cast<float>(this->width) / static_cast<float>(this->height);
   projectionMatrix = glm::perspective(glm::radians(this->fov), aspect_ratio, nearClipping, farClipping);
   setMatrix(m);
}

void Perspective::render(glm::mat4& m) {
	if (isActive) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projectionMatrix));
		glMatrixMode(GL_MODELVIEW);
	}
}