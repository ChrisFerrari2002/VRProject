#include "pch.h"
#include "engine.h"

TEST(PointLightTest, ConstructorTest) {
   // Parametri per la creazione di una PointLight
   std::string name = "TestLight";
   int lightNumber = 1;
   glm::vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   float cutOff = 45.0f;

   // Crea l'oggetto PointLight
   Eng::PointLight pointLight(name, lightNumber, ambient, diffuse, specular, cutOff);

   // Verifica che i parametri siano stati correttamente assegnati
   EXPECT_EQ(pointLight.getName(), name);
   EXPECT_EQ(pointLight.getLightNumber(), lightNumber);
   EXPECT_EQ(pointLight.getCutoff(), cutOff);
   EXPECT_EQ(pointLight.getAmbient(), ambient);
   EXPECT_EQ(pointLight.getDiffuse(), diffuse);
   EXPECT_EQ(pointLight.getSpecular(), specular);
}

TEST(PointLightTest, RenderTest) {
   // Parametri per la creazione di una PointLight
   std::string name = "TestLight";
   int lightNumber = 1;
   glm::vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   float cutOff = 45.0f;

   // Crea l'oggetto PointLight
   Eng::PointLight pointLight(name, lightNumber, ambient, diffuse, specular, cutOff);

   // Crea una matrice di trasformazione
   glm::mat4 matrix = glm::mat4(1.0f);

   // Verifica se il metodo render non causa crash e che cutOff venga impostato correttamente
   EXPECT_TRUE(pointLight.render(matrix, nullptr));

   // Dovremmo aggiungere un mock per la funzione OpenGL `glLightfv` e verificare che venga invocato
   // con i giusti parametri (ad esempio, GL_SPOT_CUTOFF e il valore cutOff)
}

