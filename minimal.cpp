#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

#define NUM_OBJECTS 4

// Vertex Array attributes
#define POSITION 0
// #define COLOR 1
#define NORMAL 1
#define UV 2

// Vertex Buffer Identifiers
#define GLOBAL_MATRICES 0
#define MODEL_MATRIX 1
#define LIGHT_PROPERTIES 2
#define MATERIAL_PROPERTIES 3
#define CAMERA_PROPERTIES 4


// Vertex Array binding points
#define STREAM0 0

// GLSL Uniform indices
#define TRANSFORM0 0
#define TRANSFORM1 1
#define LIGHT 2
#define MATERIAL 3
#define CAMERA 4

typedef struct {
	GLuint vertexBufferName;
	GLuint indexBufferName;
	GLuint modelMatrixBufferName;
	GLuint vertexArrayName;
	GLuint textureName;
	GLfloat *vertices;
	GLshort *indices;
	GLfloat *modelMatrixPtr;
} Model;

Model models[NUM_OBJECTS];

typedef struct {
	GLuint bufferName;
	GLuint arrayName;
	GLuint textureName;
	GLsizei numVertices;
} Mesh;
std::vector<Mesh> meshes;

GLfloat lightProperties[]{
	// Position
	0.0f, 6.0f, 0.0f, 0.0f,
	// Ambient Color
	0.2f, 0.2f, 0.2f, 0.0f,
	// Diffuse Color
	0.2f, 0.2f, 0.2f, 0.0f,
	// Specular Color
	0.1f, 0.1f, 0.1f, 0.0f
	// coneAngle
	// 0.0f, 10.0f, 10.0f, 0.0f
};


GLfloat cameraProperties[]{
	0.0f, 1.0f, 4.0f
};

GLfloat materialProperties[] = {
	// Shininess color
	1.0f, 1.0f, 1.0f, 1.0f,
	// Shininess
	36.0f
};
	// Making the models in the list
int createModels() {
	// Vertices_1
	models[0].vertices = new GLfloat[6*4*8]{
	// Front
	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	// Back
	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	// Left
	-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	// Right
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	// Top
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	// Bottom
	-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};

	models[1].vertices = new GLfloat[6*4*8]{
		// Front
		-3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		// Back
		3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		// Left
		-3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		// Right
		3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		// Top
		-3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		// Bottom
		-3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};

	models[2].vertices = new GLfloat[6 * 4 * 8]{
		// Front
		-0.3f, 0.3f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.3f, -0.3f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.3f, -0.3f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.3f, 0.3f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		// Back
		0.3f, 0.3f, -0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		0.3f, -0.3f, -0.3f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.3f, -0.3f, -0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-0.3f, 0.3f, -0.3f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		// Left
		-0.3f, 0.3f, -0.3f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.3f, -0.3f, -0.3f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.3f, -0.3f, 0.3f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.3f, 0.3f, 0.3f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		// Right
		0.3f, 0.3f, 0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.3f, -0.3f, 0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.3f, 0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		// Top
		-0.3f, 0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.3f, 0.3f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.3f, 0.3f, 0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.3f, 0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		// Bottom
		-0.3f, -0.3f, 0.3f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.3f, -0.3f, -0.3f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		0.3f, -0.3f, -0.3f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.3f, -0.3f, 0.3f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};

	models[0].indices = new GLshort[6*6]{
		// Front
	0, 1, 2, 2, 3, 0,
	// Back
	4, 5, 6, 6, 7, 4,
	// Left
	8, 9, 10, 10, 11, 8,
	// Right
	12, 13, 14, 14, 15, 12,
	// Top
	16, 17, 18, 18, 19, 16,
	// Bottom
	20, 21, 22, 22, 23, 20
	};

	models[1].indices = new GLshort[6*6]{
		// Front
	0, 1, 2, 2, 3, 0,
	// Back
	4, 5, 6, 6, 7, 4,
	// Left
	8, 9, 10, 10, 11, 8,
	// Right
	12, 13, 14, 14, 15, 12,
	// Top
	16, 17, 18, 18, 19, 16,
	// Bottom
	20, 21, 22, 22, 23, 20
	};

	models[2].indices = new GLshort[6 * 6]{
		// Front
	0, 1, 2, 2, 3, 0,
	// Back
	4, 5, 6, 6, 7, 4,
	// Left
	8, 9, 10, 10, 11, 8,
	// Right
	12, 13, 14, 14, 15, 12,
	// Top
	16, 17, 18, 18, 19, 16,
	// Bottom
	20, 21, 22, 22, 23, 20
	};


	glCreateBuffers(1, &models[0].vertexBufferName);
	glCreateBuffers(1, &models[0].indexBufferName);
	glCreateBuffers(1, &models[0].modelMatrixBufferName);
	glCreateBuffers(1, &models[1].vertexBufferName);
	glCreateBuffers(1, &models[1].indexBufferName);
	glCreateBuffers(1, &models[1].modelMatrixBufferName);
	glCreateBuffers(1, &models[2].vertexBufferName);
	glCreateBuffers(1, &models[2].indexBufferName);
	glCreateBuffers(1, &models[2].modelMatrixBufferName);

	// Allocate storage for the vertex array buffers
	glNamedBufferStorage(models[0].vertexBufferName, 4 * 6 * 8 * sizeof(GLfloat), models[0].vertices, 0);
	glNamedBufferStorage(models[0].indexBufferName, 6 * 6 * sizeof(GLshort), models[0].indices, 0);
	glNamedBufferStorage(models[1].vertexBufferName, 4 * 6 * 8 * sizeof(GLfloat), models[1].vertices, 0);
	glNamedBufferStorage(models[1].indexBufferName, 6 * 6 * sizeof(GLshort), models[1].indices, 0);
	glNamedBufferStorage(models[2].vertexBufferName, 4 * 6 * 8 * sizeof(GLfloat), models[2].vertices, 0);
	glNamedBufferStorage(models[2].indexBufferName, 6 * 6 * sizeof(GLshort), models[2].indices, 0);

	// Allocate storage for the transformation matrices and retrieve their addresses
	glNamedBufferStorage(models[0].modelMatrixBufferName, 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(models[1].modelMatrixBufferName, 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(models[2].modelMatrixBufferName, 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	// Get a pointer to the model matrix data
	models[0].modelMatrixPtr = (GLfloat *)glMapNamedBufferRange(models[0].modelMatrixBufferName, 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	models[1].modelMatrixPtr = (GLfloat *)glMapNamedBufferRange(models[1].modelMatrixBufferName, 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	models[2].modelMatrixPtr = (GLfloat *)glMapNamedBufferRange(models[2].modelMatrixBufferName, 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	// Create and initialize a vertex array object
	glCreateVertexArrays(1, &models[0].vertexArrayName);
	glCreateVertexArrays(1, &models[1].vertexArrayName);
	glCreateVertexArrays(1, &models[2].vertexArrayName);

	// Associate attributes with binding points
	glVertexArrayAttribBinding(models[0].vertexArrayName, POSITION, STREAM0);
	// glVertexArrayAttribBinding(models[0].vertexArrayName, COLOR, STREAM0);
	glVertexArrayAttribBinding(models[0].vertexArrayName, NORMAL, STREAM0);
	glVertexArrayAttribBinding(models[0].vertexArrayName, UV, STREAM0);
	glVertexArrayAttribBinding(models[1].vertexArrayName, POSITION, STREAM0);
	// glVertexArrayAttribBinding(models[1].vertexArrayName, COLOR, STREAM0);
	glVertexArrayAttribBinding(models[1].vertexArrayName, NORMAL, STREAM0);
	glVertexArrayAttribBinding(models[1].vertexArrayName, UV, STREAM0);
	glVertexArrayAttribBinding(models[2].vertexArrayName, POSITION, STREAM0);
	// glVertexArrayAttribBinding(models[2].vertexArrayName, COLOR, STREAM0);
	glVertexArrayAttribBinding(models[2].vertexArrayName, NORMAL, STREAM0);
	glVertexArrayAttribBinding(models[2].vertexArrayName, UV, STREAM0);

	// Specify attribute format
	glVertexArrayAttribFormat(models[0].vertexArrayName, POSITION, 3, GL_FLOAT, GL_FALSE, 0);
	// glVertexArrayAttribFormat(models[0].vertexArrayName, COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[0].vertexArrayName, NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[0].vertexArrayName, UV, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[1].vertexArrayName, POSITION, 3, GL_FLOAT, GL_FALSE, 0);
	// glVertexArrayAttribFormat(models[1].vertexArrayName, COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[1].vertexArrayName, NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[1].vertexArrayName, UV, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[2].vertexArrayName, POSITION, 3, GL_FLOAT, GL_FALSE, 0);
	// glVertexArrayAttribFormat(models[2].vertexArrayName, COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[2].vertexArrayName, NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
	glVertexArrayAttribFormat(models[2].vertexArrayName, UV, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT));

	// Enable the attributes
	glEnableVertexArrayAttrib(models[0].vertexArrayName, POSITION);
	// glEnableVertexArrayAttrib(models[0].vertexArrayName, COLOR);
	glEnableVertexArrayAttrib(models[0].vertexArrayName, NORMAL);
	glEnableVertexArrayAttrib(models[0].vertexArrayName, UV);
	glEnableVertexArrayAttrib(models[1].vertexArrayName, POSITION);
	// glEnableVertexArrayAttrib(models[1].vertexArrayName, COLOR);
	glEnableVertexArrayAttrib(models[1].vertexArrayName, NORMAL);
	glEnableVertexArrayAttrib(models[1].vertexArrayName, UV);
	glEnableVertexArrayAttrib(models[2].vertexArrayName, POSITION);
	// glEnableVertexArrayAttrib(models[2].vertexArrayName, COLOR);
	glEnableVertexArrayAttrib(models[2].vertexArrayName, NORMAL);
	glEnableVertexArrayAttrib(models[2].vertexArrayName, UV);

	//Bind the indices to the vertex array
	glVertexArrayElementBuffer(models[0].vertexArrayName, models[0].indexBufferName);
	glVertexArrayElementBuffer(models[1].vertexArrayName, models[1].indexBufferName);
	glVertexArrayElementBuffer(models[2].vertexArrayName, models[2].indexBufferName);

	// Bind the buffers to the vertex array
	glVertexArrayVertexBuffer(models[0].vertexArrayName, STREAM0, models[0].vertexBufferName, 0, 8 * sizeof(GLfloat));
	glVertexArrayVertexBuffer(models[1].vertexArrayName, STREAM0, models[1].vertexBufferName, 0, 8 * sizeof(GLfloat));
	glVertexArrayVertexBuffer(models[2].vertexArrayName, STREAM0, models[2].vertexBufferName, 0, 8 * sizeof(GLfloat));

	return true;
}


	// Implementing tiny object loader
int loadObj(const char *filename) {

	// Variables for storing the data in the OBJ-data
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	// String used to return an error message from tiny_obj_loader
	std::string errorString;

	// Load the file, or return FALSE if an error occured
	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &errorString, filename, "."))
		return 0;

	// Loop through all the shapes in the OBJ-data
	for (int m = 0; m < shapes.size(); ++m) {

		// Create a new Mesh instance and store a local ponter for easy access
		meshes.push_back(Mesh());
		Mesh *mesh = &meshes[meshes.size() - 1];

		// Store a pointer to the mesh of the current shape
		tinyobj::mesh_t *objMesh = &shapes[m].mesh;

		// Get the texture of the first face in the mesh. This is used for the entire mesh.
		std::string texture_filename = materials[objMesh->material_ids[0]].diffuse_texname;
		if (texture_filename != "") {
		
			// Read the texture image
			int width, height, channels;
			GLubyte *imageData = stbi_load(texture_filename.c_str(), &width, &height, &channels, STBI_default);
			if (!imageData)
				return 0;

			// Generate a new texture name and activate it
			glGenTextures(1, &mesh->textureName);
			glBindTexture(GL_TEXTURE_2D, mesh->textureName);

			// Set sampler properties
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (channels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			else if (channels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			else
				return 0;

			// Generate mip map images
			glGenerateMipmap(GL_TEXTURE_2D);

			// Deactivate the texture and free the image data
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(imageData);
		}
		// Store the number of uique vertices and faces in the mesh
		int numVertices = mesh->numVertices = objMesh->indices.size();
		int numFaces = numVertices / 3;

		// Create a vector for storing the vector data
		std::vector<GLfloat> vertices;

		// Loop through all the faces in the mesh
		for (int f = 0; f < numFaces; ++f) {

			// Store the indices of the current triangle
			tinyobj::index_t idx1 = objMesh->indices[f * 3];
			tinyobj::index_t idx2 = objMesh->indices[f * 3 + 1];
			tinyobj::index_t idx3 = objMesh->indices[f * 3 + 2];

			// Store the first vertex (POSITION NORMAL UV)
			vertices.push_back(attributes.vertices[idx1.vertex_index * 3]);
			vertices.push_back(attributes.vertices[idx1.vertex_index * 3 + 1]);
			vertices.push_back(attributes.vertices[idx1.vertex_index * 3 + 2]);
			vertices.push_back(attributes.normals[idx1.normal_index * 3]);
			vertices.push_back(attributes.normals[idx1.normal_index * 3 + 1]);
			vertices.push_back(attributes.normals[idx1.normal_index * 3 + 2]);
			vertices.push_back(attributes.texcoords[idx1.texcoord_index * 2]);
			vertices.push_back(1.0f - attributes.texcoords[idx1.texcoord_index * 2 + 1]);
			// Store the second vertex
			vertices.push_back(attributes.vertices[idx2.vertex_index * 3]);
			vertices.push_back(attributes.vertices[idx2.vertex_index * 3 + 1]);
			vertices.push_back(attributes.vertices[idx2.vertex_index * 3 + 2]);
			vertices.push_back(attributes.normals[idx2.normal_index * 3]);
			vertices.push_back(attributes.normals[idx2.normal_index * 3 + 1]);
			vertices.push_back(attributes.normals[idx2.normal_index * 3 + 2]);
			vertices.push_back(attributes.texcoords[idx2.texcoord_index * 2]);
			vertices.push_back(1.0f - attributes.texcoords[idx2.texcoord_index * 2 + 1]);
			// Store the third vertex
			vertices.push_back(attributes.vertices[idx3.vertex_index * 3]);
			vertices.push_back(attributes.vertices[idx3.vertex_index * 3 + 1]);
			vertices.push_back(attributes.vertices[idx3.vertex_index * 3 + 2]);
			vertices.push_back(attributes.normals[idx3.normal_index * 3]);
			vertices.push_back(attributes.normals[idx3.normal_index * 3 + 1]);
			vertices.push_back(attributes.normals[idx3.normal_index * 3 + 2]);
			vertices.push_back(attributes.texcoords[idx3.texcoord_index * 2]);
			vertices.push_back(1.0f - attributes.texcoords[idx3.texcoord_index * 2 + 1]);

		}

		// Create a vertex buffer with the vertex data
		glCreateBuffers(1, &mesh->bufferName);
		glNamedBufferStorage(mesh->bufferName, vertices.size() * sizeof(GLfloat), &vertices[0], 0);

		// Create and initialize a vertex array object
		glCreateVertexArrays(1, &mesh->arrayName);

		// Associate vertex attributes with the binding point (POSITION NORMAL UV)
		glVertexArrayAttribBinding(mesh->arrayName, POSITION, STREAM0);
		glVertexArrayAttribBinding(mesh->arrayName, NORMAL, STREAM0);
		glVertexArrayAttribBinding(mesh->arrayName, UV, STREAM0);
		// Enable the attributes
		glEnableVertexArrayAttrib(mesh->arrayName, POSITION);
		glEnableVertexArrayAttrib(mesh->arrayName, NORMAL);
		glEnableVertexArrayAttrib(mesh->arrayName, UV);

		// Specify the format of the attributes
		glVertexArrayAttribFormat(mesh->arrayName, POSITION, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribFormat(mesh->arrayName, NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));
		glVertexArrayAttribFormat(mesh->arrayName, UV, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT));

		// Bind the vertex data buffer to the vertex array
		glVertexArrayVertexBuffer(mesh->arrayName, STREAM0, mesh->bufferName, 0, 8 * sizeof(GLfloat));

	}

	return 1;

}


// Pointers for updating GPU data
GLfloat *projectionMatrixPtr;
GLfloat *viewMatrixPtr;
GLfloat *modelMatrixPtr;

// Names
GLuint programName;
GLuint vertexArrayName;
GLuint vertexBufferNames[8];
GLuint textureName;
GLuint textureName1;
GLuint textureName2;
GLuint textureName3;

//Time values
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Mouse variables
float lastX = DEFAULT_WIDTH / 2, lastY = DEFAULT_HEIGHT / 2;
float pitch;
float yaw;
bool firstMouse = true;

//Initial camera values
glm::vec3 cameraPos = glm::vec3(0.0f, 8.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
/*
 * Read source file from disk
 */
char *readSourceFile(const char *filename, int *size) {

    // Open the file
    FILE *file = fopen(filename, "r");

    // Find the end of the file to know the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    // Rewind
    fseek(file, 0, SEEK_SET); 

    // Allocate memory for the source and initialize it to 0
    char *source = (char *)malloc(fileSize + 1);
    for (int i = 0; i <= fileSize; i++) source[i] = 0;

    // Read the source
    fread(source, fileSize, 1, file);

    // Close the file
    fclose(file);

    // Store the size of the file in the output variable
    *size = fileSize-1;

    // Return the shader source
    return source;

}

/*
 * Callback function for OpenGL debug messages 
 */
void glDebugCallback(GLenum sources, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *userParam) {
    printf("DEBUG: %s\n", msg);
}

/*
 * Initialize OpenGL
 */
int initGL() {

	// Register the debug callback function
	glDebugMessageCallback(glDebugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // Create and initialize 4 buffer names
    glCreateBuffers(8, vertexBufferNames);

    // Allocate storage for the transformation matrices and retrieve their addresses
    glNamedBufferStorage(vertexBufferNames[GLOBAL_MATRICES], 16 * sizeof(GLfloat) * 2, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[LIGHT_PROPERTIES], 16 * sizeof(GLfloat), lightProperties, 0);
	glNamedBufferStorage(vertexBufferNames[MATERIAL_PROPERTIES], 5 * sizeof(GLfloat), materialProperties, 0);
	glNamedBufferStorage(vertexBufferNames[CAMERA_PROPERTIES], 3 * sizeof(GLfloat), cameraProperties, 0);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    // Get a pointer to the global matrices data
    GLfloat *globalMatricesPtr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[GLOBAL_MATRICES], 0, 16 * sizeof(GLfloat) * 2, 
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrixPtr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    projectionMatrixPtr = globalMatricesPtr;
    viewMatrixPtr = globalMatricesPtr + 16;

	// Loading images and setting up textures
	// Texture 1
	stbi_set_flip_vertically_on_load(1);
	GLint width1, height1, numChannels1;
	GLubyte *imageData1 = stbi_load("earth.jpg", &width1, &height1, &numChannels1, 3);

	glGenTextures(1, &textureName1);

	glBindTexture(GL_TEXTURE_2D, textureName1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture 2
	GLint width2, height2, numChannels2;
	GLubyte *imageData2 = stbi_load("sun.jpg", &width2, &height2, &numChannels2, 3);

	glGenTextures(1, &textureName2);

	glBindTexture(GL_TEXTURE_2D, textureName2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture 3
	GLint width3, height3, numChannels3;
	GLubyte *imageData3 = stbi_load("scaryFace.jpg", &width3, &height3, &numChannels3, 3);

	glGenTextures(1, &textureName3);

	glBindTexture(GL_TEXTURE_2D, textureName3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Using the models method
	createModels();

    // Load and compile vertex shader
    GLuint vertexName = glCreateShader(GL_VERTEX_SHADER);
    int vertexLength = 0;
    char *vertexSource = readSourceFile("minimal.vert", &vertexLength);
    glShaderSource(vertexName, 1, (const char * const *)&vertexSource, &vertexLength);
    GLint compileStatus;
    glCompileShader(vertexName);
    glGetShaderiv(vertexName, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        GLint logSize = 0;
        glGetShaderiv(vertexName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetShaderInfoLog(vertexName, logSize, &logSize, errorLog);
        glDeleteShader(vertexName);
        printf("ERROR %s\n", errorLog);
        return 0;
    }
    free(vertexSource);

    // Load and compile fragment shader
    GLuint fragmentName = glCreateShader(GL_FRAGMENT_SHADER);
    int fragmentLength = 0;
    char *fragmentSource = readSourceFile("minimal.frag", &fragmentLength);
    glShaderSource(fragmentName, 1, (const char * const *)&fragmentSource, &fragmentLength);
    glCompileShader(fragmentName);
    glGetShaderiv(fragmentName, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        GLint logSize = 0;
        glGetShaderiv(fragmentName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetShaderInfoLog(fragmentName, logSize, &logSize, errorLog);
        glDeleteShader(fragmentName);

        printf("ERROR %s\n", errorLog);
        return 0;
    }
    free(fragmentSource);

    // Create and link vertex program
    programName = glCreateProgram();
    glAttachShader(programName, vertexName);
    glAttachShader(programName, fragmentName);
    glLinkProgram(programName);
    GLint linkStatus;
    glGetProgramiv(programName, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        GLint logSize = 0;
        glGetProgramiv(programName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetProgramInfoLog(programName, logSize, &logSize, errorLog);

        printf("LINK ERROR %s\n", errorLog);
        return 0;
    }

	glEnable(GL_DEPTH_TEST);

    return 1;

}

/*
 * Draw OpenGL screne
 */
void drawGLScene() {

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	// Set the view matrix


	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	memcpy(viewMatrixPtr, &view[0][0], 16 * sizeof(GLfloat));

    // Change the model matrix
	// Variables for circle movement of an object
	float radius = 15.0f;
	float radius2 = 7.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	float camG = sin(glfwGetTime()) * radius2;
	float camH = cos(glfwGetTime()) * radius2;

	// Set the model matrix
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
	// model = glm::rotate(model, (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
	memcpy(modelMatrixPtr, &model[0][0], 16 * sizeof(GLfloat));

	glm::mat4 model1 = glm::mat4(1.0);
	model1 = glm::translate(model1, glm::vec3(camX, 0.0f, camZ));
	// model1 = glm::rotate(model1, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	memcpy(models[0].modelMatrixPtr, &model1[0][0], 16 * sizeof(GLfloat));

	glm::mat4 model2 = glm::mat4(1.0);
	model2 = glm::translate(model2, glm::vec3(0.0f, -5.0f, 0.0f));
	// model2 = glm::rotate(model2, (float)glfwGetTime() * -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	memcpy(models[1].modelMatrixPtr, &model2[0][0], 16 * sizeof(GLfloat));

	glm::mat4 model3 = glm::mat4(1.0);
	model3 = glm::translate(model3, glm::vec3(camH, 0.0f, camG));
	// model3 = glm::rotate(model3, (float)glfwGetTime() * 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	memcpy(models[2].modelMatrixPtr, &model3[0][0], 16 * sizeof(GLfloat));

    // Activate the program
    glUseProgram(programName);

    // Activate the vertex array and draw the vertexes on base of models

	// Model 1
    glBindVertexArray(models[0].vertexArrayName);

	glBindTexture(GL_TEXTURE_2D, textureName1);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, models[0].modelMatrixBufferName);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	// Model 2
	glBindVertexArray(models[1].vertexArrayName);

	glBindTexture(GL_TEXTURE_2D, textureName2);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, models[1].modelMatrixBufferName);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	// Model 3
	glBindVertexArray(models[2].vertexArrayName);

	glBindTexture(GL_TEXTURE_2D, textureName3);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, models[2].modelMatrixBufferName);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    // Bind tranformation buffers to indices
    glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM0, vertexBufferNames[GLOBAL_MATRICES]);
	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX]);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT, vertexBufferNames[LIGHT_PROPERTIES]);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL, vertexBufferNames[MATERIAL_PROPERTIES]);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA, vertexBufferNames[CAMERA_PROPERTIES]);
	

	// Loop through all the meshes loaded from the OBJ-file
	for (int m = 0; m < meshes.size(); ++m) {

		// Bind the vertex array and texture of the mesh
		glBindVertexArray(meshes[m].arrayName);
		glBindTexture(GL_TEXTURE_2D, meshes[m].textureName);

		// Draw the vertex array
		glDrawArrays(GL_TRIANGLES, 0, meshes[m].numVertices);
		
		// Disable vertex array and texture
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

    // Disable
    glUseProgram(0);
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void resizeGL(int width, int height) {

    // Prevent division by zero
    if (height == 0)
        height = 1;										
  
    // Change the projection matrix

	glm::mat4 proj = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    memcpy(projectionMatrixPtr, &proj[0][0], 16 * sizeof(GLfloat));

    // Set the OpenGL viewport
    glViewport(0, 0, width, height);

}

/*
 * Error callback function for GLFW
 */
static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

	// Enabling the use of keys to move camera
static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	
	float cameraSpeed = 1000.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

	// Enabling mousemovement in scene
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
};

/*
 * Window size changed callback function for GLFW
 */
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {

    resizeGL(width, height);

}

/*
 * Program entry function
 */
int main(void) {

    // Set error callback
    glfwSetErrorCallback(glfwErrorCallback);

    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");  
        exit(EXIT_FAILURE);
    }

    // Specify minimum OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	

    // Create window
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Minimal", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");  
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	//Set curser input mode
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set input key event callback
    glfwSetKeyCallback(window, glfwKeyCallback);

    // Set window resize callback
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);

    // Make the context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");  
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make GLFW swap buffers directly 
    glfwSwapInterval(0);

    // Initialize OpenGL
    if (!initGL()) {
        printf("Failed to initialize OpenGL\n");  
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	// Load the OBJ-file
	if (!loadObj("darkcave3.obj")) {
		printf("Failed to load %s.\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

    // Initialize OpenGL view
    resizeGL(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    // Run a loop until the window is closed
    while (!glfwWindowShouldClose(window)) {

        // Draw OpenGL screne
        drawGLScene();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll fow input events
        glfwPollEvents();

    }

    // Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    // Exit
    exit(EXIT_SUCCESS);

}
