#include "engine.h"


// Calcola la distanza punto-piano
float Eng::FrustumPlane::distance(const glm::vec3& point) const {
   return glm::dot(normal, point) + d;
}

// Sphere culling: ritorna true se dentro o interseca
bool Eng::Frustum::sphereInFrustum(const glm::vec3& center, float radius) const {
   for (int i = 0; i < 6; ++i) {
      if (planes[i].distance(center) < -radius)
         return false; // Totalmente fuori
   }
   return true;
}

// Estrazione e normalizzazione dei piani dal VP
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
