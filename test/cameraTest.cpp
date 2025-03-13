#include "pch.h"
#include "engine.h"
#include "utils.h"

class CameraTest : public ::testing::Test {
protected:
   Eng::Camera camera;

   CameraTest() : camera("Test") {}

   void SetUp() override {
      // Reset eventuali trasformazioni prima di ogni test
      camera.setTransform(glm::mat4(1.0f));
   }
};


TEST_F(CameraTest, DefaultTransformIsIdentity) {
   glm::mat4 identity = glm::mat4(1.0f);
   EXPECT_TRUE(matricesAreEqual(camera.getTransform(), identity));
}

TEST_F(CameraTest, SetUserTransformAppliesCorrectTransformations) {
   camera.setUserTransform(1.0f, 2.0f, 3.0f, 90.0f, 0.0f, 0.0f);

   glm::mat4 expectedTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
   expectedTransform = glm::rotate(expectedTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

   EXPECT_TRUE(matricesAreEqual(camera.getTransform(), expectedTransform));
}

TEST_F(CameraTest, InverseCameraMatrixIsCorrect) {
   camera.setUserTransform(1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f);

   glm::mat4 inverseTransform = camera.getInverseCameraMat();
   glm::mat4 expectedInverse = glm::inverse(camera.getTransform());

   EXPECT_TRUE(matricesAreEqual(inverseTransform, expectedInverse));
}

TEST_F(CameraTest, SetTransformCameraSetsCorrectMatrix) {
   glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 5.0f, 6.0f));
   camera.setTransformCamera(newTransform);

   EXPECT_TRUE(matricesAreEqual(camera.getTransform(), newTransform));
}
