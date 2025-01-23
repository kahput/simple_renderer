#include <cglm/cglm.h>
#include <cglm/types.h>

// // Mesh, vertex data and vao/vbo
// typedef struct Mesh {
// 	int vertexCount; // Number of vertices stored in arrays
// 	int triangleCount; // Number of triangles stored (indexed or not)
//
// 	// Vertex attributes data
// 	float* vertices; // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
// 	float* texcoords; // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
// 	float* texcoords2; // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
// 	float* normals; // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
// 	float* tangents; // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
// 	unsigned char* colors; // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
// 	unsigned short* indices; // Vertex indices (in case vertex data comes indexed)
//
// 	// Animation vertex data
// 	float* animVertices; // Animated vertex positions (after bones transformations)
// 	float* animNormals; // Animated normals (after bones transformations)
// 	unsigned char* boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning) (shader-location = 6)
// 	float* boneWeights; // Vertex bone weight, up to 4 bones influence by vertex (skinning) (shader-location = 7)
// 	mat4* boneMatrices; // Bones animated transformation matrices
// 	int boneCount; // Number of bones
//
// 	// OpenGL identifiers
// 	unsigned int vaoId; // OpenGL Vertex Array Object id
// 	unsigned int* vboId; // OpenGL Vertex Buffer Objects id (default vertex data)
// } Mesh;
// // Texture, tex data stored in GPU memory (VRAM)
// typedef struct Texture {
// 	unsigned int id; // OpenGL texture id
// 	int width; // Texture base width
// 	int height; // Texture base height
// 	int mipmaps; // Mipmap levels, 1 by default
// 	int format; // Data format (PixelFormat type)
// } Texture;
//
// // Texture2D, same as Texture
// typedef Texture Texture2D;
//
// // Color, 4 components, R8G8B8A8 (32bit)
// typedef struct Color {
// 	unsigned char r; // Color red value
// 	unsigned char g; // Color green value
// 	unsigned char b; // Color blue value
// 	unsigned char a; // Color alpha value
// } Color;
//
// // MaterialMap
// typedef struct MaterialMap {
// 	Texture2D texture; // Material map texture
// 	Color color; // Material map color
// 	float value; // Material map value
// } MaterialMap;
//
// // Material, includes shader and maps
// typedef struct Material {
// 	Shader shader; // Material shader
// 	MaterialMap* maps; // Material maps array (MAX_MATERIAL_MAPS)
// 	float params[4]; // Material generic parameters (if required)
// } Material;
//
// // Bone, skeletal animation bone
// typedef struct BoneInfo {
// 	char name[32]; // Bone name
// 	int parent; // Bone parent
// } BoneInfo;
//
// typedef vec4 Quaternion;
//
// typedef struct Transform {
// 	vec3 translation; // Translation
// 	vec4 rotation; // Rotation
// 	vec3 scale; // Scale
// } Transform;
//
// typedef struct Model {
// 	mat4 transform; // Local transform matrix
//
// 	int meshCount; // Number of meshes
// 	int materialCount; // Number of materials
// 	Mesh* meshes; // Meshes array
// 	Material* materials; // Materials array
// 	int* meshMaterial; // Mesh material number
//
// 	// Animation data
// 	int boneCount; // Number of bones
// 	BoneInfo* bones; // Bones information (skeleton)
// 	Transform* bindPose; // Bones base transformation (pose)
// } Model;
//
// typedef struct Camera3D {
// 	vec3 position; // Camera position
// 	vec3 target; // Camera target it looks-at
// 	vec3 up; // Camera up vector (rotation over its axis)
// 	float fovy; // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
// 	int projection; // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
// } Camera3D;
