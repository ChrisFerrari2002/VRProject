#pragma once
#include "engine.h"

struct FrustumPlane {
   glm::vec3 normal;
   float d;

   // Calcola la distanza firmata di un punto dal piano
   float distance(const glm::vec3& point) const;
};

struct Frustum {
   FrustumPlane planes[6]; // Left, Right, Bottom, Top, Near, Far

   // Controlla se una sfera è dentro o interseca il frustum
   bool sphereInFrustum(const glm::vec3& center, float radius) const;
};

// Estrae i sei piani del frustum da una matrice ViewProjection
Frustum extractFrustumPlanes(const glm::mat4& vpMatrix);
