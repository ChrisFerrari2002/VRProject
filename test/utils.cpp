#include "pch.h"
#include "utils.h"

bool matricesAreEqual(const glm::mat4& matA, const glm::mat4& matB, float epsilon) {
   for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
         if (glm::abs(matA[i][j] - matB[i][j]) > epsilon) {
            return false;
         }
      }
   }
   return true;
}