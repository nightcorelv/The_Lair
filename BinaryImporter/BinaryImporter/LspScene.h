#pragma once

#include <string>
#include <vector>

struct LspScene
{
	struct Transform final
	{
		float position[3];//float3 array 
		float rotation[3];//float3 array 
		float scale[3];//float3 array 
	};

	struct AnimationKey final
	{
		std::string name; //MeshName
		int key;
		Transform data;
	};
	struct AnimationList
	{
		std::vector<AnimationKey> keys;
	};
	struct Object
	{
		struct CustomAttr
		{
			struct StringAttr
			{
				std::string name;
				std::string value;
			};
			struct IntAttr
			{
				std::string name;
				int value;
			};
			struct BoolAttr
			{
				std::string name;
				bool value;
			};
			struct EnumAttr
			{
				std::string name;
				int value;
			};
			struct FloatAttr
			{
				std::string name;
				float value;
			};
			struct VectorAttr
			{
				std::string name;
				float value[3];//float3 array
			};

			std::vector<StringAttr> strings;
			std::vector<IntAttr> ints;
			std::vector<BoolAttr> bools;
			std::vector<EnumAttr> enums;
			std::vector<FloatAttr> floats;
			std::vector<VectorAttr> vectors;
		};
		enum struct Type { Mesh, Joint, Light, Camera, Group };
		Type type;
		Transform transform;
		std::string name;
		Object* parent;   //Gruppen eller joint etc
		std::vector<Object*> children;
		std::vector<AnimationList> animations;
		CustomAttr customAttrs;
	};
	struct Group : Object
	{

	};
	struct Camera : Object														//Initsiera!!!
	{
		float up[3];//float3 array 
		float lookAt[3];//float3 array 
		std::string id;
		float fov;
		float nearDistance;
		float farDistance;
	};
	struct Light : Object
	{
		enum struct Type { Point, Spot, Directional };
		Type lightType;
		float dir[3];//float3 array 
		float color[3];//float3 array 
		float intensity;
		std::string id;
	};
	struct Material final
	{
		std::string name;

		std::string texureFileName;

		float ambient[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
		float diffuse[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
		float specular[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
		float emissive[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
		float opacity = 0.0f;
		float specularPower = 0.0f;
		float reflectivity = 0.0f;
	};
	struct Joint : Object
	{
		float bindPoseInverse[4][4];//4x4 two dimensional array
	};
	struct Shape
	{
		struct Data
		{
			float position[3];
		};
		std::string name;
		std::vector<Data> data;
	};
	struct Mesh : Object
	{
		struct BlendShape
		{
			struct Data
			{
				int frame;
				float value;
			};
			Shape* source;

			std::vector<Data> data;
		};
		struct Vertex final
		{
			float position[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
			float uv[2]{ 0.0f,0.0f };//float2 array 
			float normal[3]{ 0.0f, 0.0f, 0.0f };//float3 array 

			float biNormal[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
			float tangent[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
		};
		struct Weight
		{
			int vertexIndex;
			Joint* joint;
			float value;
		};

		std::vector<Vertex> vertices;
		Material* material;

		std::vector<BlendShape> blendShapes;
		std::vector<AnimationKey> vertexAnimations;
		std::vector<Weight> weights;
	};
	struct TimelineInfo
	{
		int duration = 0;//duration of timeline
		int framerate = 0; //framerate, 30fps 60fps ...
	};

	std::vector<Material*> materials;//all materials in scene, done

	Object* root = nullptr;//root node, tree structure, done

	std::vector<Object*> allObjects;//all Objects in scene, done

	std::vector<Shape*> shapes;//all shapes in scene, done
	std::vector<Mesh*> meshes;//all meshes in scene, done
	std::vector<Joint*> joints;//all joints in scene, done
	std::vector<Light*> lights;//all lights in scene, done
	std::vector<Camera*> cameras;//all cameras in scene, done
	std::vector<Group*> groups;//all grups in scene, done

	TimelineInfo timelineInfo;

	void load(std::string path);// load scene
	void clear();// free scene from memory

	LspScene(){} // create empty scene
	LspScene(std::string path);// load scene

	~LspScene();// free scene from memory
};