#pragma once
#include "LspScene.h"
#include <DirectXMath.h>

using namespace DirectX;

//struct LspBone
//{
//	int myIndex;
//	int parent;
//	XMMATRIX bindmodelInverseMx;		//Calculated at start. Bind pose transform related to origin.
//	XMMATRIX localMx;					//Calculated per frame. Transform related to parent.
//	XMMATRIX modelMx;					//Calculated per frame. Transform related to model origin.
//	XMMATRIX finalMx;					//Calculated per frame. Position related to bind pose. (Transformation)
//
//	//LspBone() {}
//	//LspBone(const LspBone& other)
//	//{
//	//	this->myIndex = other.myIndex;
//	//	this->parent = other.parent;
//	//	this->bindmodelInverseMx = other.bindmodelInverseMx;
//	//	this->localMx = other.localMx;
//	//	this->modelMx = other.modelMx;
//	//	this->finalMx = other.finalMx;
//	//}
//};

//struct LspSkeletalMesh
//{
//	LspScene::Transform worldTransform;
//	std::string sceneFileName;
//	LspScene::Mesh mesh;
//	LspScene::Material material;
//	std::vector<LspScene::Joint> skeleton;
//	std::vector<LspBone> bones;
//	std::vector<LspScene::TimelineInfo> animTimeline;
//
//	//LspSkeletalMesh() {};
//	//LspSkeletalMesh(const LspSkeletalMesh& other);
//};

struct LspMesh
{
	LspScene::Transform worldTransform;
	std::string sceneFileName;
	LspScene::Mesh mesh;
	LspScene::Material material;
};

//Create local matrix based on transform

/// <summary>
/// create a local transform matrix based on position, rotation, scale
/// </summary>
/// <param name="mesh: ">the mesh containing joint</param>
/// <param name="index: ">the index in mesh::skeleton containing transform</param>
/// <returns></returns>
//XMMATRIX CreateLocalMx(const LspSkeletalMesh& mesh, int index);

//Calculations per frame for animation

/// <summary>
/// calculate new transform matrices for each animated frame
/// </summary>
/// <param name="bone: ">target for calculations</param>
/// <param name="myMesh: ">target containing bone</param>
//void CalcMxPerFrame(LspBone& bone, const LspSkeletalMesh& myMesh);

/// <summary>
/// loads the info from an lsp scene and fills it
/// into the scene sent into the function
/// </summary>
/// <param name="myScene: ">the scene to fill</param>
/// <param name="fileName: ">file path and name from which to load</param>
void LoadScene(LspScene& myScene, std::string fileName);

//Load a skeleton based on a filename
/// <summary>
/// loads a skeleton from an lsp scene and fills it
/// into the skeleton sent into the function
/// </summary>
/// <param name="fileName: ">file path and name from which to load</param>
/// <param name="target: ">the skeleton to fill</param>
//void LoadSkeleton(std::string fileName, std::vector<LspScene::Joint>& target);

//Load skeleton from existing LspScene
/// <summary>
/// loads a skeleton from an existing lsp scene and fills it
/// into the skeleton sent into the function
/// </summary>
/// <param name="myScene: ">the scene to load from</param>
/// <param name="targetSkeleton: ">the skeleton to fill</param>
/// <param name="targetBones: ">the bones to fill</param>
//void LoadSkeleton(const LspScene& myScene, std::vector<LspScene::Joint>& targetSkeleton, std::vector<LspBone>& targetBones);

//Load skeletal mesh containing mesh, material and joint vector
/// <summary>
/// loads a mesh with skeleton from an lsp scene
/// and fills it into the mesh sent into the function
/// </summary>
/// <param name="target: ">the mesh to fill</param>
/// <param name="index: ">index of the mesh to use in lsp scene</param>
/// <param name="meshName: ">the name of the mesh to use in lsp scene</param>
//void LoadMesh(LspSkeletalMesh& target, int index = -1, std::string meshName = "§");

//Load mesh with material
/// <summary>
/// loads a mesh without a skeleton from an lsp scene
/// and fills it into the mesh sent into the function
/// </summary>
/// <param name="target: ">the mesh to fill</param>
/// <param name="index: ">index of the mesh to use in lsp scene</param>
/// <param name="meshName: ">the name of the mesh to use in lsp scene</param>
void LoadMesh(LspMesh& target, int index = -1, std::string meshName = "§");

//Load animaton from scene and apply to target
/// <summary>
/// loads animation from an lsp scene and applies it
/// to the mesh that is sent into the function
/// </summary>
/// <param name="targetMesh: ">the mesh to fill</param>
/// <param name="fileName: ">file path and name from which to load</param>
/// <returns>if succeeded</returns>
//bool LoadAnimation(LspSkeletalMesh& targetMesh, std::string fileName);

//Load all cameras in fbx file
/// <summary>
/// loads all cameras in the lsp scene
/// </summary>
/// <param name="scene: ">scene to load from</param>
/// <returns>vector with pointers to all cameras</returns>
const std::vector<LspScene::Camera*> LoadAllCameras(LspScene& scene);

//Gets animations tied to camera at specific index
/// <summary>
/// gets the animation on a camera in an lsp scene
/// </summary>
/// <param name="scene: ">scene from which to load</param>
/// <param name="cameraIndex: ">the index of the camera to load from</param>
/// <returns>the animation</returns>
const std::vector <LspScene::AnimationList> GetCameraAnimations(LspScene& scene, int cameraIndex);

//Gets camera from specific index
/// <summary>
/// gets the camera of a specific index in an lsp scene
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="cameraIndex: ">the index of the camera</param>
/// <returns>the camera</returns>
const LspScene::Camera GetCamera(LspScene& scene, int cameraIndex);

//Load all lights in fbx file
/// <summary>
/// gets all the lights in an lsp scene
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <returns>vector with pointers to all the lights</returns>
const std::vector<LspScene::Light*> LoadAllLights(LspScene& scene);

//Returns specific light from index
/// <summary>
/// gets a light from an lsp scene
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="lightIndex: ">index of the light</param>
/// <returns>pointer to the light</returns>
const LspScene::Light* GetLight(LspScene& scene, int lightIndex);

//Returns all names within a object type
/// <summary>
/// get the names of all objects of a specified type
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="objectType: ">type to look for</param>
/// <returns>vector of the names of the objects</returns>
const std::vector<std::string> GetNamesObjectByType(LspScene& scene, LspScene::Object::Type objectType);

//Gets objects nr of type sent to function
/// <summary>
/// gets the number of the type of object sent into the function
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="objectType: ">type of object to look for</param>
/// <returns>the number of objects</returns>
const int GetNrOfObjectByType(LspScene& scene, LspScene::Object::Type objectType);

//Gets names of children of specific object specified by object index
/// <summary>
/// get the names of the children from an object in an lsp scene by index
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="objectType: ">type of object</param>
/// <param name="objectIndex: ">index of object</param>
/// <returns>vector of the names of the children</returns>
const std::vector<std::string> GetNamesOfChildrenPerObject(LspScene& scene, LspScene::Object::Type objectType, int objectIndex);

//Gets names of children of specific object specified by object name
/// <summary>
/// get the names of the children from an object in an lsp scene by name
/// </summary>
/// <param name="scene: ">the scene from which to load</param>
/// <param name="objectType: ">type of object</param>
/// <param name="objectName: ">name of object</param>
/// <returns>vector of the names of the children</returns>
const std::vector<std::string> GetNamesOfChildrenPerObject(LspScene& scene, LspScene::Object::Type objectType, std::string objectName);

/// <summary>
/// prints all the info in an lsp file
/// </summary>
/// <param name="filename: ">file path and name from which to load</param>
void printLspInfo(std::string filename);
