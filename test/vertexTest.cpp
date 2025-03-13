#include "pch.h"
#include "engine.h"

TEST(VertexTest, ConstructorTest) {
   glm::vec3 position(1.0f, 2.0f, 3.0f);
   glm::vec3 normal(0.0f, 1.0f, 0.0f);

   // Crea il vertice
   Eng::Vertex vertex(position, normal);

   // Verifica che la posizione e la normale siano correttamente impostate
   EXPECT_EQ(vertex.getPosition(), position);
   EXPECT_EQ(vertex.getNormal(), normal);

   // Verifica che le coordinate della texture siano inizializzate a (0, 0)
   EXPECT_EQ(vertex.getTextureCoordinates(), glm::vec2(0.0f));
}

TEST(VertexTest, GetterTest) {
   glm::vec3 position(1.0f, 2.0f, 3.0f);
   glm::vec3 normal(0.0f, 1.0f, 0.0f);

   // Crea il vertice
   Eng::Vertex vertex(position, normal);

   // Verifica i metodi getter
   EXPECT_EQ(vertex.getPosition(), position);
   EXPECT_EQ(vertex.getNormal(), normal);
}

TEST(VertexTest, SetTextureCoordinatesTest) {
   glm::vec3 position(1.0f, 2.0f, 3.0f);
   glm::vec3 normal(0.0f, 1.0f, 0.0f);
   glm::vec2 newTextureCoordinates(0.5f, 0.5f);

   // Crea il vertice
   Eng::Vertex vertex(position, normal);

   // Modifica le coordinate della texture
   vertex.setTextureCoordinates(newTextureCoordinates);

   // Verifica che le coordinate siano state aggiornate correttamente
   EXPECT_EQ(vertex.getTextureCoordinates(), newTextureCoordinates);
}

TEST(VertexTest, DestructorTest) {
   glm::vec3 position(1.0f, 2.0f, 3.0f);
   glm::vec3 normal(0.0f, 1.0f, 0.0f);

   // Crea il vertice (testiamo la corretta distruzione)
   {
      Eng::Vertex vertex(position, normal);
      // Nessuna operazione specifica da fare, ma possiamo verificare che il
      // distruttore non causi problemi di memoria o accesso a risorse non valide
   } // Il vertice è distrutto al termine del blocco, non dovrebbero esserci errori
}

TEST(VertexTest, FullTest) {
   glm::vec3 position(1.0f, 2.0f, 3.0f);
   glm::vec3 normal(0.0f, 1.0f, 0.0f);
   glm::vec2 textureCoordinates(0.5f, 0.5f);

   // Crea il vertice
   Eng::Vertex vertex(position, normal);

   // Verifica che i getter restituiscano i valori corretti
   EXPECT_EQ(vertex.getPosition(), position);
   EXPECT_EQ(vertex.getNormal(), normal);
   EXPECT_EQ(vertex.getTextureCoordinates(), glm::vec2(0.0f)); // Inizialmente (0.0f)

   // Modifica le coordinate della texture
   vertex.setTextureCoordinates(textureCoordinates);

   // Verifica che le coordinate della texture siano state modificate
   EXPECT_EQ(vertex.getTextureCoordinates(), textureCoordinates);
}