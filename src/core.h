#pragma once

#include <cglm/cglm.h>
#include <stdint.h>

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
