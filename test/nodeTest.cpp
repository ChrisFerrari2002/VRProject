#include "pch.h"
#include "engine.h"

TEST(NodeTest, ConstructorTest) {
   // Crea un nodo con un nome
   Eng::Node node("TestNode");

   // Verifica che il nome del nodo sia quello fornito
   EXPECT_EQ(node.getName(), "TestNode");

   // Verifica che la scala di default sia 1.0
   EXPECT_FLOAT_EQ(node.getScale(), 1.0f);
}

TEST(NodeTest, AddChildTest) {
   // Crea un nodo genitore e un nodo figlio
   Eng::Node parent("ParentNode");
   Eng::Node child("ChildNode");

   // Aggiungi il figlio al nodo genitore
   parent.addChild(&child);

   // Verifica che il figlio sia stato aggiunto alla lista dei figli del nodo genitore
   EXPECT_EQ(parent.getNumberOfChildren(), 1);
   EXPECT_EQ(parent.getChildAt(0)->getName(), "ChildNode");

   // Verifica che il genitore del figlio sia impostato correttamente
   EXPECT_EQ(child.getParent()->getName(), "ParentNode");
}

TEST(NodeTest, RemoveChildTest) {
   // Crea un nodo genitore e due nodi figli
   Eng::Node parent("ParentNode");
   Eng::Node child1("ChildNode1");
   Eng::Node child2("ChildNode2");

   // Aggiungi i figli al nodo genitore
   parent.addChild(&child1);
   parent.addChild(&child2);

   // Rimuovi un figlio
   parent.removeChild(&child1);

   // Verifica che il figlio sia stato rimosso
   EXPECT_EQ(parent.getNumberOfChildren(), 1);
   EXPECT_EQ(parent.getChildAt(0)->getName(), "ChildNode2");
}

TEST(NodeTest, FinalMatrixTest) {
   // Crea un nodo genitore e un nodo figlio
   Eng::Node parent("ParentNode");
   Eng::Node child("ChildNode");

   // Imposta una trasformazione di esempio per il genitore e il figlio
   parent.setTransform(glm::mat4(1.0f));
   child.setTransform(glm::mat4(2.0f));

   // Aggiungi il figlio al nodo genitore
   parent.addChild(&child);

   // Calcola la matrice finale per il figlio
   glm::mat4 finalMatrix = child.getFinalMatrix();

   // Verifica che la matrice finale del figlio sia corretta
   EXPECT_EQ(finalMatrix, glm::mat4(2.0f));  // La matrice finale del figlio dovrebbe essere quella della trasformazione del figlio
}

TEST(NodeTest, WorldPositionTest) {
   // Crea un nodo genitore e un nodo figlio
   Eng::Node parent("ParentNode");
   Eng::Node child("ChildNode");

   // Imposta la posizione mondiale per il genitore
   parent.setWorldPosition(glm::vec3(1.0f, 0.0f, 0.0f));

   // Imposta la posizione per il figlio
   child.setWorldPosition(glm::vec3(0.0f, 1.0f, 0.0f));

   // Aggiungi il figlio al nodo genitore
   parent.addChild(&child);

   // Verifica che la posizione mondiale del figlio sia corretta (somma della posizione del genitore e del figlio)
   glm::vec3 worldPosition = child.getWorldPosition();
   EXPECT_EQ(worldPosition, glm::vec3(1.0f, 1.0f, 0.0f));
}

TEST(NodeTest, ScaleTest) {
   // Crea un nodo
   Eng::Node node("TestNode");

   // Imposta una scala
   node.setScale(2.0f);

   // Verifica che la scala sia correttamente impostata
   EXPECT_FLOAT_EQ(node.getScale(), 2.0f);
}

TEST(NodeTest, RenderTest) {
   // Crea un nodo
   Eng::Node node("TestNode");

   // Crea una matrice di trasformazione
   glm::mat4 matrix = glm::mat4(1.0f);  // matrice identità

   // Verifica che il metodo di renderizzazione non sollevi eccezioni
   EXPECT_FALSE(node.render(matrix, nullptr));
}

TEST(NodeTest, SetParentTest) {
   // Crea un nodo genitore e un nodo figlio
   Eng::Node parent("ParentNode");
   Eng::Node child("ChildNode");

   // Aggiungi il figlio al nodo genitore
   parent.addChild(&child);

   // Verifica che il genitore del figlio sia stato correttamente impostato
   EXPECT_EQ(child.getParent()->getName(), "ParentNode");
}

