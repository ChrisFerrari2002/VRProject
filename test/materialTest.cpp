#include "pch.h"
#include "engine.h"

// Test del costruttore di default
TEST(MaterialTest, DefaultConstructor) {
   Eng::Material material;

   // Verifica che i valori di default siano quelli corretti
   EXPECT_EQ(material.getEmission(), glm::vec4(0.0f));
   EXPECT_EQ(material.getAmbient(), glm::vec4(0.0f));
   EXPECT_EQ(material.getDiffuse(), glm::vec4(0.0f));
   EXPECT_EQ(material.getSpecular(), glm::vec4(0.0f));
   EXPECT_FLOAT_EQ(material.getShininess(), 0.0f);
}

// Test del costruttore parametrizzato
TEST(MaterialTest, ParameterizedConstructor) {
   glm::vec4 emission(1.0f, 0.0f, 0.0f, 1.0f);
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(0.7f, 0.7f, 0.7f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   float shininess = 32.0f;

   Eng::Material material("RedMaterial", emission, ambient, diffuse, specular, shininess);

   // Verifica che il materiale sia stato creato correttamente
   EXPECT_EQ(material.getEmission(), emission);
   EXPECT_EQ(material.getAmbient(), ambient);
   EXPECT_EQ(material.getDiffuse(), diffuse);
   EXPECT_EQ(material.getSpecular(), specular);
   EXPECT_FLOAT_EQ(material.getShininess(), shininess);
}

// Test dei metodi Setter
TEST(MaterialTest, SetterMethods) {
   Eng::Material material;

   // Impostiamo nuovi valori
   glm::vec4 emission(1.0f, 1.0f, 1.0f, 1.0f);
   material.setEmission(emission);
   EXPECT_EQ(material.getEmission(), emission);

   glm::vec4 ambient(0.5f, 0.5f, 0.5f, 1.0f);
   material.setAmbient(ambient);
   EXPECT_EQ(material.getAmbient(), ambient);

   glm::vec4 diffuse(0.8f, 0.8f, 0.8f, 1.0f);
   material.setDiffuse(diffuse);
   EXPECT_EQ(material.getDiffuse(), diffuse);

   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   material.setSpecular(specular);
   EXPECT_EQ(material.getSpecular(), specular);

   float shininess = 50.0f;
   material.setShininess(shininess);
   EXPECT_FLOAT_EQ(material.getShininess(), shininess);
}

// Test del metodo Render
TEST(MaterialTest, Render) {
   // Creiamo un materiale di esempio
   glm::vec4 emission(0.2f, 0.2f, 0.2f, 1.0f);
   glm::vec4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
   glm::vec4 diffuse(0.5f, 0.5f, 0.5f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   float shininess = 64.0f;
   Eng::Material material("ShinyMaterial", emission, ambient, diffuse, specular, shininess);
   material.setTexture(nullptr);
   // Creiamo una matrice di trasformazione fittizia (identità)
   glm::mat4 matrix = glm::mat4(1.0f);

   // Verifica che il rendering vada a buon fine
   EXPECT_TRUE(material.render(matrix, nullptr));
}