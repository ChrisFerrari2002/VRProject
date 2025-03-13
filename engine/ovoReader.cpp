/**
* @file ovoReader.cpp
* @brief OvoReader class implementation file
*
* This file contains the implementation of the OvoReader class.
*
* @details The class contains information and methods about the OvoReader.
*
* @date 2025
*
* @see Material, Node, Vertex
*
* @authors
* - Sara Bressan [sara.bressan@student.supsi.ch]
* - Chris Ferrari [chris.ferrari@student.supsi.ch]
* - Alessandro Formato [alessandro.formato@student.supsi.ch]
*/

//////////////
// #INCLUDE //
//////////////
#include <GL/freeglut.h>
#include <glm/gtc/packing.hpp>
#include "engine.h"
#include "ovoLight.h"
#include "ovoObject.h"
#include "ovoMesh.h"

#pragma warning(disable : 4996)

/**
* @brief Get the separator.
*
* Get the right separator based on different operative system.
*
*/
std::string getSeparator() {
#ifdef _WIN32
	return "\\";
#else
	return "/";
#endif
}
/**
 * @brief Constructor
 *
 * Initializes the OvoReader.
 *
 */
ENG_API Eng::OvoReader::OvoReader() {}

/**
 * @brief Destructor
 *
 * Deinitialize the OvoReader.
 */
Eng::OvoReader::~OvoReader() {
	materials.clear();
}

std::string _path; /**< Path of the file */

/**
 * @brief Read the .ovo file.
 *
 * @param filePath The file path to the .ovo.
 *
 * @return The root of the scene.
 */
Eng::Node* Eng::OvoReader::readFile(const char* path) {
	// Open file:
	FILE* dat = fopen(path, "rb");
	if (dat == nullptr)
	{
		std::cout << "ERROR: unable to open file '" << path << "'" << std::endl;
		return nullptr;
	}

	_path = path;

	Node* root = recursiveLoad(dat);
	fclose(dat);

	return root;
}

/**
 * @brief Recursively read the file.
 *
 * @param dat file data.
 *
 * @return The root of the scene.
 */
Eng::Node* Eng::OvoReader::recursiveLoad(FILE* dat)
{
	// Parse the chunk starting at buffer + position:
	unsigned int chunkId, chunkSize, position = 0;

	fread(&chunkId, sizeof(unsigned int), 1, dat);
	fread(&chunkSize, sizeof(unsigned int), 1, dat);

	// Load whole chunk into memory:
	char* data = new char[chunkSize];
	if (fread(data, sizeof(char), chunkSize, dat) != chunkSize)
	{
		std::cout << "ERROR: unable to read from file" << std::endl;
		fclose(dat);
		delete[] data;
	}

	//First Object
	if ((OvObject::Type)chunkId == OvObject::Type::O_OBJECT) {
		unsigned int versionId;
		memcpy(&versionId, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		return recursiveLoad(dat);
	}

	//Material
	if ((OvObject::Type)chunkId == OvObject::Type::O_MATERIAL) {
		// Material name:
		char materialName[FILENAME_MAX];
		strcpy(materialName, data + position);
		position += (unsigned int)strlen(materialName) + 1;
		std::string materialName_str(materialName);

		// Material term colors, starting with emissive:
		glm::vec3 emission, albedo;
		memcpy(&emission, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Albedo:
		memcpy(&albedo, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Roughness factor:
		float roughness;
		memcpy(&roughness, data + position, sizeof(float));
		position += sizeof(float);

		//Skipped values:
		// Metalness factor:
		position += sizeof(float);
		// Transparency factor:
		position += sizeof(float);
		// Albedo texture filename, or [none] if not used:
		char textureName[FILENAME_MAX];
		strcpy(textureName, data + position);
		position += (unsigned int)strlen(textureName) + 1;
		std::string textureName_str = std::string(textureName);

		//Save material on data structure
		Material* material = new Material(materialName_str, glm::vec4(emission, 1.0f), glm::vec4(albedo * 0.2f, 1.0f), glm::vec4(albedo * 0.6f, 1.0f), glm::vec4(albedo * 0.4f, 1.0f), (1 - sqrt(roughness)) * 128);
		materials.insert(std::pair<std::string, Material*>(material->getName(), material));

		Texture* texture = new Texture(textureName_str);
		material->setTexture(texture);

		if (textureName_str != "[none]") {
			texture->setTextureId(_path.substr(0, _path.find_last_of(getSeparator())) + getSeparator() + textureName_str);
		}

		return recursiveLoad(dat);
	}

	// Node name:
	char nodeName[FILENAME_MAX];
	strcpy(nodeName, data + position);
	position += (unsigned int)strlen(nodeName) + 1;
	std::string nodeName_str(nodeName);

	// Node matrix:
	glm::mat4 matrix;
	memcpy(&matrix, data + position, sizeof(glm::mat4));
	position += sizeof(glm::mat4);

	// Nr. of children nodes:
	unsigned int nrOfChildren = 0;
	memcpy(&nrOfChildren, data + position, sizeof(unsigned int));
	position += sizeof(unsigned int);

	// Parse chunk information according to its type:
	switch ((OvObject::Type)chunkId)
	{
	case OvObject::Type::O_NODE:
	{
		Node* thisNode = new Node(nodeName_str);
		thisNode->setTransform(matrix);

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren)
			while (thisNode->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisNode->addChild(childNode);
			}

		// Done:
		return thisNode;
	}

	case OvObject::Type::O_MESH:
	{
		// Optional target node, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);

		position += (unsigned int)strlen(targetName) + 1;
		std::cout << "Target name: " << nodeName_str << std::endl;

		// Mesh subtype (see OvMesh SUBTYPE enum):
		position += sizeof(unsigned char);

		// Material name, or [none] if not used:
		char materialName[FILENAME_MAX];
		strcpy(materialName, data + position);
		std::cout << "\tMaterial name: " << materialName << std::endl;

		position += (unsigned int)strlen(materialName) + 1;
		std::string materialName_str(materialName);

		Mesh* thisMesh;
		if (materialName_str != "[none]") {
			Material material = *(materials.find(materialName_str)->second);
			thisMesh = new Mesh(nodeName_str, material);
			thisMesh->setTransform(matrix);
		}
		else {
			thisMesh = new Mesh(nodeName_str, Material());
			thisMesh->setTransform(matrix);
		}

		// Mesh bounding sphere radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		position += sizeof(float);
		// Mesh bounding box minimum corner:
		glm::vec3 bBoxMin;
		memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);
		// Mesh bounding box maximum corner:
		position += sizeof(glm::vec3);
		// Optional physics properties:
		unsigned char hasPhysics;
		memcpy(&hasPhysics, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);
		if (hasPhysics)
		{
			/**
			 * Mesh physics properties.
			 */
			struct PhysProps
			{
				// Pay attention to 16 byte alignement (use padding):
				unsigned char type;
				unsigned char contCollisionDetection;
				unsigned char collideWithRBodies;
				unsigned char hullType;

				// Vector data:
				glm::vec3 massCenter;

				// Mesh properties:
				float mass;
				float staticFriction;
				float dynamicFriction;
				float bounciness;
				float linearDamping;
				float angularDamping;
				unsigned int nrOfHulls;
				unsigned int _pad;

				// Pointers:
				void* physObj;
				void* hull;
			};

			PhysProps mp;
			memcpy(&mp, data + position, sizeof(PhysProps));
			position += sizeof(PhysProps);
			// Custom hull(s) used?
			if (mp.nrOfHulls)
			{
				for (unsigned int c = 0; c < mp.nrOfHulls; c++)
				{
					// Hull number of vertices:
					unsigned int nrOfVertices;
					memcpy(&nrOfVertices, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);
					// Hull number of faces:
					unsigned int nrOfFaces;
					memcpy(&nrOfFaces, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);
					// Hull centroid:
					position += sizeof(glm::vec3);
					// Iterate through hull vertices:
					for (unsigned int c = 0; c < nrOfVertices; c++)
					{
						position += sizeof(glm::vec3);
					}
					// Iterate through hull faces:
					for (unsigned int c = 0; c < nrOfFaces; c++)
					{
						position += sizeof(unsigned int) * 3;
					}
				}
			}
		}


		// Nr. of LODs:
		unsigned int LODs;
		memcpy(&LODs, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);

		std::cout << "\tLODs: " << LODs << std::endl;

		for (unsigned int l = 0; l < LODs; l++)
		{

			// Nr. of vertices:
			unsigned int vertices, faces;

			memcpy(&vertices, data + position, sizeof(unsigned int));
			position += sizeof(unsigned int);

			std::cout << "\tNr. of vertices: " << vertices << std::endl;


			// ...and faces:
			memcpy(&faces, data + position, sizeof(unsigned int));
			position += sizeof(unsigned int);

			std::cout << "\tNr. of faces: " << faces << std::endl;

			// Interleaved and compressed vertex/normal/UV/tangent data:
			for (unsigned int c = 0; c < vertices; c++)
			{
				// Vertex coords:
				glm::vec3 vertex;
				memcpy(&vertex, data + position, sizeof(glm::vec3));
				position += sizeof(glm::vec3);
				verticesCoords.push_back(vertex);

				// Vertex normal:
				unsigned int normalData;
				memcpy(&normalData, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);

				// Texture coordinates:
				unsigned int textureData;
				memcpy(&textureData, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);

				// Tangent vector:
				unsigned int tangentData;
				memcpy(&tangentData, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);



				glm::vec3 normal = glm::unpackSnorm3x10_1x2(normalData);
				normalsCoords.push_back(normal);

				glm::vec2 uv = glm::unpackHalf2x16(textureData);
				texCoords.push_back(uv);

			}

			// Faces:
			for (unsigned int c = 0; c < faces; c++)
			{
				// Face indexes:
				unsigned int face[3];
				memcpy(face, data + position, sizeof(unsigned int) * 3);
				position += sizeof(unsigned int) * 3;
				facesArray.push_back(face[0]);
				facesArray.push_back(face[1]);
				facesArray.push_back(face[2]);

				/*for (int i = 0; i < 3; i++)
				{
					thisMesh->addVertex(tempVertices.at(face[i]), l);
				}*/
			}
		}
		thisMesh->setVertices(verticesCoords);
		thisMesh->setNormals(normalsCoords);
		thisMesh->setTexCoords(texCoords);
		thisMesh->setFaces(facesArray);
		thisMesh->setupMesh();

		verticesCoords.clear();
		normalsCoords.clear();
		texCoords.clear();
		facesArray.clear();

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren) {
			while (thisMesh->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisMesh->addChild(childNode);
			}
		}

		// Done:
		return thisMesh;
	}

	case OvObject::Type::O_LIGHT:
	{
		// Optional target node name, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		position += (unsigned int)strlen(targetName) + 1;

		// Light subtype (see OvLight SUBTYPE enum):
		unsigned char subtype;
		memcpy(&subtype, data + position, sizeof(unsigned char));
		char subtypeName[FILENAME_MAX];
		switch ((OvLight::Subtype)subtype)
		{
		case OvLight::Subtype::DIRECTIONAL: strcpy(subtypeName, "directional"); break;
		case OvLight::Subtype::OMNI: strcpy(subtypeName, "omni"); break;
		case OvLight::Subtype::SPOT: strcpy(subtypeName, "spot"); break;
		default: strcpy(subtypeName, "UNDEFINED");
		}
		position += sizeof(unsigned char);

		// Light color:
		glm::vec3 color;
		memcpy(&color, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Influence radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		position += sizeof(float);

		// Direction:
		glm::vec3 direction;
		memcpy(&direction, data + position, sizeof(glm::vec3));
		position += sizeof(glm::vec3);

		// Cutoff:
		float cutoff;
		memcpy(&cutoff, data + position, sizeof(float));
		position += sizeof(float);

		// Exponent:
		float spotExponent;
		memcpy(&spotExponent, data + position, sizeof(float));
		position += sizeof(float);

		// Cast shadow flag:
		unsigned char castShadows;
		memcpy(&castShadows, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);

		// Volumetric lighting flag:
		unsigned char isVolumetric;
		memcpy(&isVolumetric, data + position, sizeof(unsigned char));
		position += sizeof(unsigned char);

		Light* thisLight = nullptr;
		int nextLightPointer = Light::getNextLightNumber();
		switch ((OvLight::Subtype)subtype)
		{
		case OvLight::Subtype::DIRECTIONAL:
		{
			thisLight = new DirectionalLight(nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f));
		}break;
		case OvLight::Subtype::OMNI:
		{
			thisLight = new PointLight(nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f), cutoff);
		}break;
		case OvLight::Subtype::SPOT:
		{
			thisLight = new SpotLight(nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f), cutoff, direction);
		}break;
		default:
			thisLight = new Light(nodeName_str, nextLightPointer, glm::vec4(glm::vec3(color.x), 1.0f), glm::vec4(glm::vec3(color.y), 1.0f), glm::vec4(glm::vec3(color.z), 1.0f));
		}

		thisLight->setTransform(matrix);

		// Go recursive when child nodes are avaialble:
		if (nrOfChildren)
			while (thisLight->getNumberOfChildren() < nrOfChildren)
			{
				Node* childNode = recursiveLoad(dat);
				thisLight->addChild(childNode);
			}
		// Done:
		return thisLight;
	}
	}
	delete[] data;
}

