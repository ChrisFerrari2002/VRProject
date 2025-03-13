#include "pch.h"
#include "engine.h"

TEST(MeshTest, ConstructorTest) {
   // Crea un materiale di test
   Eng::Material mat("TestMaterial", glm::vec4(0.1f), glm::vec4(0.1f), glm::vec4(0.1f), glm::vec4(0.1f), 10.0f);

   // Crea la mesh
   Eng::Mesh mesh("TestMesh", mat);

   // Verifica che la mesh sia correttamente inizializzata
   EXPECT_EQ(mesh.getName(), "TestMesh");
   EXPECT_EQ(mesh.getMaterial()->getShininess(), 10.0f);  // Verifica la proprietà shininess del materiale
}

TEST(MeshTest, AddVertexTest) {
   // Crea materiale e mesh
   Eng::Material mat("TestMaterial", glm::vec4(0.2f), glm::vec4(0.2f), glm::vec4(0.2f), glm::vec4(0.2f), 5.0f);
   Eng::Mesh mesh("TestMesh", mat);

   // Crea un vertice
   Eng::Vertex* vertex = new Eng::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

   // Aggiungi il vertice a LOD 0
   mesh.addVertex(vertex, 0);

   // Verifica che il vertice sia stato aggiunto
   EXPECT_EQ(mesh.getVertices(0).size(), 1);
}

TEST(MeshTest, GetVerticesTest) {
   // Crea materiale e mesh
   Eng::Material mat("TestMaterial", glm::vec4(0.3f), glm::vec4(0.3f), glm::vec4(0.3f), glm::vec4(0.3f), 15.0f);
   Eng::Mesh mesh("TestMesh", mat);

   // Crea un vertice
   Eng::Vertex* vertex = new Eng::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

   // Aggiungi il vertice a LOD 0
   mesh.addVertex(vertex, 0);

   // Recupera i vertici per LOD 0
   std::vector<Eng::Vertex*> vertices = mesh.getVertices(0);

   // Verifica che i vertici siano correttamente recuperati
   EXPECT_EQ(vertices.size(), 1);
   EXPECT_EQ(vertices[0], vertex);
}

TEST(MeshTest, RenderTest) {
   // Crea materiale e mesh
   Eng::Material mat("TestMaterial", glm::vec4(0.4f), glm::vec4(0.4f), glm::vec4(0.4f), glm::vec4(0.4f), 20.0f);
   mat.setTexture(nullptr);
   Eng::Mesh mesh("TestMesh", mat);

   // Crea un vertice
   Eng::Vertex* vertex = new Eng::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
   mesh.addVertex(vertex, 0);
   // Esegui il rendering (test per verificare che non fallisca)
   glm::mat4 matrix(1.0f);  // matrice identità
   EXPECT_TRUE(mesh.render(matrix, nullptr));
}

TEST(MeshTest, GetMaterialTest) {
   // Crea materiale e mesh
   Eng::Material mat("TestMaterial", glm::vec4(0.5f), glm::vec4(0.5f), glm::vec4(0.5f), glm::vec4(0.5f), 30.0f);
   Eng::Mesh mesh("TestMesh", mat);

   // Verifica che il materiale venga recuperato correttamente
   EXPECT_EQ(mesh.getMaterial()->getShininess(), 30.0f);
}

TEST(MeshTest, GetColorBasedOnIdTest) {
   Eng::Mesh mesh("TestMesh", Eng::Material());

   // Supponiamo che l'ID sia 0x123456
   glm::vec4 color = mesh.getColorBasedOnId(0x123456);

   // Verifica che i valori RGB siano corretti
   EXPECT_FLOAT_EQ(color.r, (float)((0x123456 >> 16) & 0xFF) / 255.0f);
   EXPECT_FLOAT_EQ(color.g, (float)((0x123456 >> 8) & 0xFF) / 255.0f);
   EXPECT_FLOAT_EQ(color.b, (float)((0x123456 >> 0) & 0xFF) / 255.0f);
}

TEST(MeshTest, AddVerticesToDifferentLODTest) {
   // Crea materiale e mesh
   Eng::Material mat("TestMaterial", glm::vec4(0.6f), glm::vec4(0.6f), glm::vec4(0.6f), glm::vec4(0.6f), 25.0f);
   Eng::Mesh mesh("TestMesh", mat);

   // Crea vertici
   Eng::Vertex* vertex1 = new Eng::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
   Eng::Vertex* vertex2 = new Eng::Vertex(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

   // Aggiungi il primo vertice a LOD 0
   mesh.addVertex(vertex1, 0);
   // Aggiungi il secondo vertice a LOD 1
   mesh.addVertex(vertex2, 1);

   // Verifica che i vertici siano aggiunti ai rispettivi LOD
   EXPECT_EQ(mesh.getVertices(0).size(), 1);
   EXPECT_EQ(mesh.getVertices(1).size(), 1);
}

