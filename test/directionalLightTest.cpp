#include "pch.h"
#include "engine.h"
#include "utils.h"

class DirectionalLightTest : public ::testing::Test {
protected:
   Eng::DirectionalLight light;

   DirectionalLightTest() : light("TestLight", 1, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 
      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {}

   void SetUp() override {
      // Puoi aggiungere eventuali inizializzazioni qui se necessarie
   }
};

// Test del costruttore
TEST_F(DirectionalLightTest, ConstructorInitializesCorrectly) {
   EXPECT_EQ(light.getName(), "TestLight");
   EXPECT_EQ(light.getLightNumber(), 1);
   EXPECT_EQ(light.getAmbient(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
   EXPECT_EQ(light.getDiffuse(), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
   EXPECT_EQ(light.getSpecular(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

// Test del metodo render
TEST_F(DirectionalLightTest, RenderReturnsTrue) {
   glm::mat4 transform = glm::mat4(1.0f);  // Identità
   void* ptr = nullptr;  // Può essere nullo se non usato nel rendering

   EXPECT_TRUE(light.render(transform, ptr));
}

// Test del metodo setTransform
TEST_F(DirectionalLightTest, SetTransformUpdatesMatrix) {
   glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 30.0f));
   light.setTransform(newTransform);

   EXPECT_TRUE(matricesAreEqual(light.getTransform(), newTransform));
}