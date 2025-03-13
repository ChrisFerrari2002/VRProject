#include "pch.h"
#include "engine.h"
#include "GL/freeglut.h"

TEST(ListTest, AddEntry) {
   // Creiamo un oggetto List
   Eng::List list;

   // Creiamo una luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   Eng::Node* root = new Eng::Node("Root");
   Eng::Light* light = new Eng::Light("TestLight", lightNumber, ambient, diffuse, specular);
   root->addChild(light);
   // Aggiungi la luce alla lista
   list.addEntry(root);
   // Verifica che la lista contenga un solo elemento
   EXPECT_EQ(list.size(), 1);

   // Verifica che l'elemento sia quello giusto
   EXPECT_EQ(list.getObject(0), light);
}

TEST(ListTest, PopEntry) {
   // Creiamo un oggetto List
   Eng::List list;

   // Creiamo una luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   Eng::Node* root = new Eng::Node("Root");
   Eng::Light* light = new Eng::Light("TestLight", lightNumber, ambient, diffuse, specular);
   root->addChild(light);

   // Aggiungi la luce alla lista
   list.addEntry(root);

   // Verifica che la lista contenga un elemento
   EXPECT_EQ(list.size(), 1);

   // Rimuovi l'elemento dalla lista
   list.popEntry();

   // Verifica che la lista sia vuota
   EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, Render) {
   // Creiamo un oggetto List
   Eng::List list;

   // Creiamo una luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;
   
   Eng::Node* root = new Eng::Node("Root");
   Eng::Light* light = new Eng::Light("TestLight", lightNumber, ambient, diffuse, specular);
   root->addChild(light);

   // Aggiungi la luce alla lista
   list.addEntry(root);

   // Creiamo una matrice di trasformazione (cameraMatrix)
   glm::mat4 cameraMatrix = glm::mat4(1.0f);

   // Renderizza la lista
   EXPECT_TRUE(list.render(cameraMatrix, nullptr));
}

TEST(ListTest, Clear) {
   // Creiamo un oggetto List
   Eng::List list;

   // Creiamo una luce
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;
   
   Eng::Node* root = new Eng::Node("Root");
   Eng::Light* light = new Eng::Light("TestLight", lightNumber, ambient, diffuse, specular);
   root->addChild(light);

   // Aggiungi la luce alla lista
   list.addEntry(root);

   // Verifica che la lista contenga un elemento
   EXPECT_EQ(list.size(), 1);

   // Pulisci la lista
   list.clear();

   // Verifica che la lista sia vuota
   EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, GetObjectById) {
   // Creiamo un oggetto List
   Eng::List list;

   // Creiamo due luci con ID diversi
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber1 = GL_LIGHT0;
   int lightNumber2 = GL_LIGHT1;

   Eng::Node* root = new Eng::Node("Root");
   

   Eng::Light* light1 = new Eng::Light("Light1", lightNumber1, ambient, diffuse, specular);
   Eng::Light* light2 = new Eng::Light("Light2", lightNumber2, ambient, diffuse, specular);

   root->addChild(light1);
   root->addChild(light2);

   // Aggiungi le luci alla lista
   list.addEntry(root);

   // Verifica che il metodo getObjectById funzioni
   EXPECT_EQ(list.getObjectById(light1->getId()), light1);
   EXPECT_EQ(list.getObjectById(light2->getId()), light2);
}

TEST(ListTest, Size) {
   // Creiamo un oggetto List
   Eng::List list;

   // Verifica che la lista sia vuota inizialmente
   EXPECT_EQ(list.size(), 0);

   // Creiamo e aggiungi una luce alla lista
   glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
   glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
   int lightNumber = GL_LIGHT0;

   Eng::Node* root = new Eng::Node("Root");
   Eng::Light* light = new Eng::Light("TestLight", lightNumber, ambient, diffuse, specular);
   root->addChild(light);

   list.addEntry(root);

   // Verifica che la lista contenga un elemento
   EXPECT_EQ(list.size(), 1);
}