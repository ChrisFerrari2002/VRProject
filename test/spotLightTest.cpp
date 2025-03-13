#include "pch.h"
#include "engine.h"

TEST(SpotLightTest, ConstructorTest) {
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(0.8f, 0.8f, 0.8f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   float cutOff = 45.0f;
   glm::vec3 direction(0.0f, -1.0f, 0.0f);  // La direzione della luce

   // Crea l'oggetto SpotLight
   Eng::SpotLight spotLight("TestSpotLight", 1, ambient, diffuse, specular, cutOff, direction);

   // Verifica che la direzione sia stata correttamente impostata
   EXPECT_EQ(spotLight.getDirection(), direction);
}