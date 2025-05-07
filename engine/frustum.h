#pragma once
#include "engine.h"

/**
 * @brief Represents a plane of the view frustum.
 */
struct FrustumPlane {
   glm::vec3 normal; /**< The normal vector of the plane */
   float d;          /**< The plane's distance from the origin along the normal */

   /**
    * @brief Calculates the signed distance from a point to the plane.
    *
    * @param point The point to compute the distance from.
    * @return The signed distance between the point and the plane.
    */
   float distance(const glm::vec3& point) const;
};

/**
 * @brief Represents a view frustum defined by six planes.
 */
struct Frustum {
   FrustumPlane planes[6]; /**< The six frustum planes: Left, Right, Bottom, Top, Near, Far */

   /**
    * @brief Checks if a sphere is inside or intersects the frustum.
    *
    * @param center The center position of the sphere.
    * @param radius The radius of the sphere.
    * @return true if the sphere is inside or intersects the frustum, false otherwise.
    */
   bool sphereInFrustum(const glm::vec3& center, float radius) const;
};

/**
 * @brief Extracts the six frustum planes from a ViewProjection matrix.
 *
 * @param vpMatrix The combined View-Projection matrix.
 * @return A Frustum containing the six extracted planes.
 */
Frustum extractFrustumPlanes(const glm::mat4& vpMatrix);
