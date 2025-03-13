#include "pch.h"
#include "engine.h"

TEST(TextureTest, ConstructorTest) {
   std::string textureName = "TestTexture";

   // Crea l'oggetto Texture
   Eng::Texture texture(textureName);

   // Verifica che l'ID e il nome siano correttamente impostati
   EXPECT_EQ(texture.getName(), textureName);
   EXPECT_GT(texture.getId(), 0); // Verifica che l'ID sia maggiore di zero (impostato dal metodo statico)
}

TEST(TextureTest, GetterTextureTest) {
   Eng::Texture texture("TestTexture");

   // Imposta dimensioni della texture
   texture.setTextureSettings(512, 512);

   // Verifica che i metodi getter restituiscano i valori corretti
   EXPECT_EQ(texture.getWidth(), 512);
   EXPECT_EQ(texture.getHeight(), 512);
}