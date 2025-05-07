#include "engine.h"


/**
 * @brief Calculates the signed distance from a point to the plane.
 *
 * @param point The point to compute the distance from.
 * @return The signed distance between the point and the plane.
 */
float Eng::FrustumPlane::distance(const glm::vec3& point) const {
   return glm::dot(normal, point) + d;
}

/**
    * @brief Checks if a sphere is inside or intersects the frustum.
    *
    * @param center The center position of the sphere.
    * @param radius The radius of the sphere.
    * @return true if the sphere is inside or intersects the frustum, false otherwise.
    */
bool Eng::Frustum::sphereInFrustum(const glm::vec3& center, float radius) const {
   for (int i = 0; i < 6; ++i) {
      if (planes[i].distance(center) < -radius)
         return false; // Totalmente fuori
   }
   return true;
}

/**
 * @brief Extracts the six frustum planes from a ViewProjection matrix.
 *
 * @param vpMatrix The combined View-Projection matrix.
 * @return A Frustum containing the six extracted planes.
 */
Eng::Frustum Eng::extractFrustumPlanes(const glm::mat4& m) {
   Frustum f;

   // Left
   f.planes[0].normal = glm::vec3(m[0][3] + m[0][0],
      m[1][3] + m[1][0],
      m[2][3] + m[2][0]);
   f.planes[0].d = m[3][3] + m[3][0];

   // Right
   f.planes[1].normal = glm::vec3(m[0][3] - m[0][0],
      m[1][3] - m[1][0],
      m[2][3] - m[2][0]);
   f.planes[1].d = m[3][3] - m[3][0];

   // Bottom
   f.planes[2].normal = glm::vec3(m[0][3] + m[0][1],
      m[1][3] + m[1][1],
      m[2][3] + m[2][1]);
   f.planes[2].d = m[3][3] + m[3][1];

   // Top
   f.planes[3].normal = glm::vec3(m[0][3] - m[0][1],
      m[1][3] - m[1][1],
      m[2][3] - m[2][1]);
   f.planes[3].d = m[3][3] - m[3][1];

   // Near
   f.planes[4].normal = glm::vec3(m[0][3] + m[0][2],
      m[1][3] + m[1][2],
      m[2][3] + m[2][2]);
   f.planes[4].d = m[3][3] + m[3][2];

   // Far
   f.planes[5].normal = glm::vec3(m[0][3] - m[0][2],
      m[1][3] - m[1][2],
      m[2][3] - m[2][2]);
   f.planes[5].d = m[3][3] - m[3][2];

   for (int i = 0; i < 6; ++i) {
      float length = glm::length(f.planes[i].normal);
      if (length > 0.0f) {
         f.planes[i].normal /= length;
         f.planes[i].d /= length;
      }
   }

   return f;
}
