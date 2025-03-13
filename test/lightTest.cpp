#include "pch.h"
#include "engine.h"
#include "GL/freeglut.h"

TEST(LightTest, ConstructorInitializesCorrectly) {
   // Impostare i parametri per la luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   // Creare un oggetto Light
   Eng::Light light("TestLight", lightNumber, ambient, diffuse, specular);

   // Verifica che i valori siano inizializzati correttamente
   EXPECT_EQ(light.getAmbient(), ambient);
   EXPECT_EQ(light.getDiffuse(), diffuse);
   EXPECT_EQ(light.getSpecular(), specular);
   EXPECT_EQ(light.getLightNumber(), lightNumber);
}

TEST(LightTest, SetAndGetPosition) {
   // Impostare i parametri per la luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   // Creare un oggetto Light
   Eng::Light light("TestLight", lightNumber, ambient, diffuse, specular);

   // Impostare una trasformazione per la luce
   glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
   light.setTransform(transform);

   // Verifica che la posizione sia corretta
   EXPECT_EQ(light.getPosition(), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
}

TEST(LightTest, SetAttenuationValues) {
   // Impostare i parametri per la luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   // Creare un oggetto Light
   Eng::Light light("TestLight", lightNumber, ambient, diffuse, specular);

   // Impostare i valori di attenuazione
   light.setConstantAttenuation(1.0f);
   light.setLinearAttenuation(0.5f);
   light.setQuadraticAttenuation(0.25f);

   // Verifica che i valori di attenuazione siano impostati correttamente
   EXPECT_FLOAT_EQ(light.getConstantAttenuation(), 1.0f);
   EXPECT_FLOAT_EQ(light.getLinearAttenuation(), 0.5f);
   EXPECT_FLOAT_EQ(light.getQuadraticAttenuation(), 0.25f);
}

TEST(LightTest, SetIntensity) {
   // Impostare i parametri per la luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   // Creare un oggetto Light
   Eng::Light light("TestLight", lightNumber, ambient, diffuse, specular);

   // Impostare l'intensità della luce
   light.setIntensity(2.0f);

   // Verifica che l'intensità sia stata impostata correttamente
   EXPECT_FLOAT_EQ(light.getIntensity(), 2.0f);
}

TEST(LightTest, RenderLight) {
   // Impostare i parametri per la luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   // Creare un oggetto Light
   Eng::Light light("TestLight", lightNumber, ambient, diffuse, specular);

   // Creare una matrice di trasformazione
   glm::mat4 transform = glm::mat4(1.0f);

   // Verifica che la funzione render non generi errori
   EXPECT_TRUE(light.render(transform, nullptr));
}