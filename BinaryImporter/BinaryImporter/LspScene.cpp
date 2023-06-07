#include "pch.h"
#include <fstream>
#include <unordered_map>
#include "LspAPI.h"

//LspBone::LspBone()
//{
//}
//
//LspBone::LspBone(const LspBone& other)
//{
//	this->myIndex = other.myIndex;
//	this->parent = other.parent;
//	this->bindmodelInverseMx = other.bindmodelInverseMx;
//	this->localMx = other.localMx;
//	this->modelMx = other.modelMx;
//	this->finalMx = other.finalMx;
//}
//
//LspSkeletalMesh::LspSkeletalMesh()
//{
//}
//
//LspSkeletalMesh::LspSkeletalMesh(const LspSkeletalMesh& other)
//{
//	for (int i = 0; i < 3; i++)
//	{
//		this->worldTransform.position[i] = other.worldTransform.position[i];
//		this->worldTransform.rotation[i] = other.worldTransform.rotation[i];
//		this->worldTransform.scale[i] = other.worldTransform.scale[i];
//	}
//	this->sceneFileName = other.sceneFileName;
//
//	//Skeleton
//	for (int iJo = 0; iJo < other.skeleton.size(); iJo++)
//	{
//		LspScene::Joint newJoint;
//		//Name
//		newJoint.name = other.skeleton.at(iJo).name;
//
//		//Type
//		newJoint.type = other.skeleton.at(iJo).type;
//
//		//Animations
//		for (int iAnim = 0; iAnim < other.skeleton.at(iJo).animations.size(); iAnim++)
//		{
//			LspScene::AnimationList newAnim;
//
//			//Keys
//			for (int iKey = 0; iKey < other.skeleton.at(iJo).animations.at(iAnim).keys.size(); iKey++)
//			{
//				LspScene::AnimationKey newKey;
//
//				for (int i = 0; i < 3; i++)
//				{
//					newKey.data.position[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.position[i];
//					newKey.data.rotation[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.rotation[i];
//					newKey.data.scale[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.scale[i];
//				}
//				newKey.key = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).key;
//				newKey.name = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).name;
//
//				newAnim.keys.push_back(newKey);
//			}
//
//			newJoint.animations.push_back(newAnim);
//		}
//
//		//BindPoseInverse
//		for (int x = 0; x < 4; x++)
//		{
//			for (int y = 0; y < 4; y++)
//			{
//				newJoint.bindPoseInverse[x][y] = other.skeleton.at(iJo).bindPoseInverse[x][y];
//			}
//		}
//
//		//CustomAttrs
//		LspScene::Object::CustomAttr attrs;
//
//		//Strings
//		for (int iStr = 0; iStr < other.skeleton.at(iJo).customAttrs.strings.size(); iStr++)
//		{
//			LspScene::Object::CustomAttr::StringAttr newStr;
//			newStr.name = other.skeleton.at(iJo).customAttrs.strings.at(iStr).name;
//			newStr.value = other.skeleton.at(iJo).customAttrs.strings.at(iStr).value;
//			attrs.strings.push_back(newStr);
//		}
//
//		//Ints
//		for (int iInt = 0; iInt < other.skeleton.at(iJo).customAttrs.ints.size(); iInt++)
//		{
//			LspScene::Object::CustomAttr::IntAttr newInt;
//			newInt.name = other.skeleton.at(iJo).customAttrs.ints.at(iInt).name;
//			newInt.value = other.skeleton.at(iJo).customAttrs.ints.at(iInt).value;
//			attrs.ints.push_back(newInt);
//		}
//
//		//Bools
//		for (int iBoo = 0; iBoo < other.skeleton.at(iJo).customAttrs.bools.size(); iBoo++)
//		{
//			LspScene::Object::CustomAttr::BoolAttr boo;
//			boo.name = other.skeleton.at(iJo).customAttrs.bools.at(iBoo).name;
//			boo.value = other.skeleton.at(iJo).customAttrs.bools.at(iBoo).value;
//			attrs.bools.push_back(boo);
//		}
//
//		//Enums
//		for (int iEn = 0; iEn < other.skeleton.at(iJo).customAttrs.enums.size(); iEn++)
//		{
//			LspScene::Object::CustomAttr::EnumAttr en;
//			en.name = other.skeleton.at(iJo).customAttrs.enums.at(iEn).name;
//			en.value = other.skeleton.at(iJo).customAttrs.enums.at(iEn).value;
//			attrs.enums.push_back(en);
//		}
//
//		//Floats
//		for (int iFl = 0; iFl < other.skeleton.at(iJo).customAttrs.floats.size(); iFl++)
//		{
//			LspScene::Object::CustomAttr::FloatAttr fl;
//			fl.name = other.skeleton.at(iJo).customAttrs.floats.at(iFl).name;
//			fl.value = other.skeleton.at(iJo).customAttrs.floats.at(iFl).value;
//			attrs.floats.push_back(fl);
//		}
//
//		//Vectors
//		for (int iVec = 0; iVec < other.skeleton.at(iJo).customAttrs.vectors.size(); iVec++)
//		{
//			LspScene::Object::CustomAttr::VectorAttr vec;
//			vec.name = other.skeleton.at(iJo).customAttrs.vectors.at(iVec).name;
//			for (int i = 0; i < 3; i++)
//			{
//				vec.value[i] = other.skeleton.at(iJo).customAttrs.vectors.at(iVec).value[i];
//			}
//			attrs.vectors.push_back(vec);
//		}
//		newJoint.customAttrs = attrs;
//
//		//Transform
//		for (int i = 0; i < 3; i++)
//		{
//			newJoint.transform.position[i] = other.skeleton.at(iJo).transform.position[i];
//			newJoint.transform.rotation[i] = other.skeleton.at(iJo).transform.rotation[i];
//			newJoint.transform.scale[i] = other.skeleton.at(iJo).transform.scale[i];
//		}
//
//		this->skeleton.push_back(newJoint);
//	}
//
//	//Skeleton children & parents
//	for (int iTarget = 0; iTarget < this->skeleton.size(); iTarget++)
//	{
//		for (int iSeek = 0; iSeek < other.skeleton.size(); iSeek++)
//		{
//			//Parent
//			if (other.skeleton.at(iSeek).parent != nullptr && this->skeleton.at(iTarget).name == other.skeleton.at(iSeek).parent->name)
//			{
//				this->skeleton.at(iSeek).parent = &this->skeleton.at(iTarget);
//			}
//
//			//Children
//			else
//			{
//				for (int iChild = 0; iChild < other.skeleton.at(iSeek).children.size(); iChild++)
//				{
//					if (this->skeleton.at(iTarget).name == other.skeleton.at(iSeek).children.at(iChild)->name)
//					{
//						this->skeleton.at(iSeek).children.push_back(&this->skeleton.at(iTarget));
//					}
//				}
//			}
//
//		}
//	}
//
//	//Bones
//	for (int iBo = 0; iBo < other.bones.size(); iBo++)
//	{
//		LspBone newBone(other.bones.at(iBo));
//		this->bones.push_back(newBone);
//	}
//
//	//Mesh
//	//Name
//	this->mesh.name = other.mesh.name;
//
//	//Parent
//	this->mesh.parent = other.mesh.parent;
//
//	//Type
//	this->mesh.type = other.mesh.type;
//
//	//AnimationList
//	for (int iAnim = 0; iAnim < other.mesh.animations.size(); iAnim++)
//	{
//		LspScene::AnimationList newAnim;
//
//		for (int iKey = 0; iKey < other.mesh.animations.at(iAnim).keys.size(); iKey++)
//		{
//			LspScene::AnimationKey newKey;
//			for (int k = 0; k < 3; k++)
//			{
//				newKey.data.position[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.position[k];
//				newKey.data.rotation[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.rotation[k];
//				newKey.data.scale[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.scale[k];
//			}
//			newKey.key = other.mesh.animations.at(iAnim).keys.at(iKey).key;
//			newKey.name = other.mesh.animations.at(iAnim).keys.at(iKey).name;
//
//			newAnim.keys.push_back(newKey);
//		}
//		this->mesh.animations.push_back(newAnim);
//	}
//
//	//BlendShapes
//	for (int iBlSh = 0; iBlSh < other.mesh.blendShapes.size(); iBlSh++)
//	{
//		LspScene::Mesh::BlendShape newBlSh;
//
//		for (int iData = 0; iData < other.mesh.blendShapes.at(iBlSh).data.size(); iData++)
//		{
//			LspScene::Mesh::BlendShape::Data newData;
//			newData.frame = other.mesh.blendShapes.at(iBlSh).data.at(iData).frame;
//			newData.value = other.mesh.blendShapes.at(iBlSh).data.at(iData).value;
//
//			newBlSh.data.push_back(newData);
//		}
//		newBlSh.source = other.mesh.blendShapes.at(iBlSh).source;
//		this->mesh.blendShapes.push_back(newBlSh);
//	}
//
//	//Children
//	for (int iChildren = 0; iChildren < other.mesh.children.size(); iChildren++)
//	{
//		this->mesh.children.push_back(other.mesh.children.at(iChildren));
//	}
//
//	//CustomAttrs
//	LspScene::Object::CustomAttr attrs;
//
//	//Strings
//	for (int iStr = 0; iStr < other.mesh.customAttrs.strings.size(); iStr++)
//	{
//		LspScene::Object::CustomAttr::StringAttr newStr;
//		newStr.name = other.mesh.customAttrs.strings.at(iStr).name;
//		newStr.value = other.mesh.customAttrs.strings.at(iStr).value;
//		attrs.strings.push_back(newStr);
//	}
//
//	//Ints
//	for (int iInt = 0; iInt < other.mesh.customAttrs.ints.size(); iInt++)
//	{
//		LspScene::Object::CustomAttr::IntAttr newInt;
//		newInt.name = other.mesh.customAttrs.ints.at(iInt).name;
//		newInt.value = other.mesh.customAttrs.ints.at(iInt).value;
//		attrs.ints.push_back(newInt);
//	}
//
//	//Bools
//	for (int iBoo = 0; iBoo < other.mesh.customAttrs.bools.size(); iBoo++)
//	{
//		LspScene::Object::CustomAttr::BoolAttr boo;
//		boo.name = other.mesh.customAttrs.bools.at(iBoo).name;
//		boo.value = other.mesh.customAttrs.bools.at(iBoo).value;
//		attrs.bools.push_back(boo);
//	}
//
//	//Enums
//	for (int iEn = 0; iEn < other.mesh.customAttrs.enums.size(); iEn++)
//	{
//		LspScene::Object::CustomAttr::EnumAttr en;
//		en.name = other.mesh.customAttrs.enums.at(iEn).name;
//		en.value = other.mesh.customAttrs.enums.at(iEn).value;
//		attrs.enums.push_back(en);
//	}
//
//	//Floats
//	for (int iFl = 0; iFl < other.mesh.customAttrs.floats.size(); iFl++)
//	{
//		LspScene::Object::CustomAttr::FloatAttr fl;
//		fl.name = other.mesh.customAttrs.floats.at(iFl).name;
//		fl.value = other.mesh.customAttrs.floats.at(iFl).value;
//		attrs.floats.push_back(fl);
//	}
//
//	//Vectors
//	for (int iVec = 0; iVec < other.mesh.customAttrs.vectors.size(); iVec++)
//	{
//		LspScene::Object::CustomAttr::VectorAttr vec;
//		vec.name = other.mesh.customAttrs.vectors.at(iVec).name;
//		for (int i = 0; i < 3; i++)
//		{
//			vec.value[i] = other.mesh.customAttrs.vectors.at(iVec).value[i];
//		}
//		attrs.vectors.push_back(vec);
//	}
//	this->mesh.customAttrs = attrs;
//
//	//Material
//	this->material.name = other.material.name;
//	this->material.texureFileName = other.material.texureFileName;
//	for (int i = 0; i < 3; i++)
//	{
//		this->material.ambient[i] = other.material.ambient[i];
//		this->material.diffuse[i] = other.material.diffuse[i];
//		this->material.specular[i] = other.material.specular[i];
//		this->material.emissive[i] = other.material.emissive[i];
//	}
//	this->material.opacity = other.material.opacity;
//	this->material.specularPower = other.material.specularPower;
//	this->material.reflectivity = other.material.reflectivity;
//
//	//Mesh material reference
//	this->mesh.material = &this->material;
//
//	//Transform
//	for (int i = 0; i < 3; i++)
//	{
//		this->mesh.transform.position[i] = other.mesh.transform.position[i];
//		this->mesh.transform.rotation[i] = other.mesh.transform.rotation[i];
//		this->mesh.transform.scale[i] = other.mesh.transform.scale[i];
//	}
//
//	//Vertices
//	for (int iVx = 0; iVx < other.mesh.vertices.size(); iVx++)
//	{
//		LspScene::Mesh::Vertex vx;
//		for (int i = 0; i < 3; i++)
//		{
//			vx.position[i] = other.mesh.vertices.at(iVx).position[i];
//			vx.normal[i] = other.mesh.vertices.at(iVx).normal[i];
//			vx.biNormal[i] = other.mesh.vertices.at(iVx).biNormal[i];
//			vx.tangent[i] = other.mesh.vertices.at(iVx).tangent[i];
//			if (i < 2)
//			{
//				vx.uv[i] = other.mesh.vertices.at(iVx).uv[i];
//			}
//		}
//		this->mesh.vertices.push_back(vx);
//	}
//
//	//VertexAnimations
//	for (int iKey = 0; iKey < other.mesh.vertexAnimations.size(); iKey++)
//	{
//		LspScene::AnimationKey newKey;
//
//		for (int k = 0; k < 3; k++)
//		{
//			newKey.data.position[k] = other.mesh.vertexAnimations.at(iKey).data.position[k];
//			newKey.data.rotation[k] = other.mesh.vertexAnimations.at(iKey).data.rotation[k];
//			newKey.data.scale[k] = other.mesh.vertexAnimations.at(iKey).data.scale[k];
//
//		}
//
//		newKey.key = other.mesh.vertexAnimations.at(iKey).key;
//		newKey.name = other.mesh.vertexAnimations.at(iKey).name;
//
//		this->mesh.vertexAnimations.push_back(newKey);
//	}
//
//	//Weights
//	for (int iW = 0; iW < other.mesh.weights.size(); iW++)
//	{
//		LspScene::Mesh::Weight w;
//		w.vertexIndex = other.mesh.weights.at(iW).vertexIndex;
//		w.value = other.mesh.weights.at(iW).value;
//
//		for (int iJo = 0; iJo < this->skeleton.size(); iJo++)
//		{
//			if (other.mesh.weights.at(iW).joint->name == this->skeleton.at(iJo).name)
//			{
//				w.joint = &this->skeleton.at(iJo);
//			}
//		}
//
//		this->mesh.weights.push_back(w);
//	}
//
//	//Timeline
//	for (int iTim = 0; iTim < other.animTimeline.size(); iTim++)
//	{
//		LspScene::TimelineInfo newTimeLine;
//		newTimeLine.duration = other.animTimeline.at(iTim).duration;
//		newTimeLine.framerate = other.animTimeline.at(iTim).framerate;
//		this->animTimeline.push_back(newTimeLine);
//	}
//}

void LspScene::load(std::string path)
{

	std::ifstream in(path + ".lsp", std::ios::binary);
	if (in.is_open())
	{
#pragma region FbxHeader

		struct FbxMasterHeader
		{
			int meshes = 0;
			int materials = 0;
			int cameras = 0;
			int lights = 0;
			int groups = 0;
			int animations = 0;
			int blendshapes = 0;
			int shapes = 0;
			int weights = 0;
			int attributes = 0;
			int joints = 0;
		};

		struct FbxBasicVertices
		{
			float position[3];
			float uv[2];
			float normal[3];
		};

		struct FbxVertices
		{
			float position[3];
			float uv[2];
			float normal[3];

			float biNormal[3];
			float tangent[3];
		};

		struct FbxMaterial
		{
			char name[32];
			char textureName[32];

			float ambient[3];
			float diffuse[3];
			float specular[3];
			float emissive[3];
			float opacity;
			float specularPower;
			float reflectivity;

		};

		enum FbxVertexType
		{
			BASIC,//namn
			WITH_BI_TANGENT
		};



		struct FbxMeshHeader
		{
			char name[32];
			int polygonVertexIndex;
			FbxVertexType type;
			char matName[32];
			int vertices;
			char id[32];
		};

		struct FbxCamera
		{
			char name[32];
			float position[3];
			float up[3];
			float lookAt[3];

			// field of view
			float fov;
			float near;
			float far;
			char id[32];
		};

		struct FbxLight
		{
			char name[32];
			char type[32];
			float position[3];
			float dir[3];
			float color[3];
			float intensity;
			char id[32];
		};


		struct FbxGroup
		{
			char name[32];
			char parent[32];
			float position[3];
			float rotation[3];
			float scale[3];
		};

		struct FbxAnimationHeader
		{
			char nodeName[32];
			int keyCounts;
		};
		struct FbxAnimationData
		{
			int key;
			float translate[3];
			float rotation[3];
			float scale[3];
		};
		struct FbxJointInfo
		{
			char name[32];
			char parent[32];
			float x[4];
			float y[4];
			float z[4];
			float w[4];

		};


		struct FbxAttrHeader
		{
			int ints = 0;
			int strings = 0;
			int floats = 0;
			int bools = 0;
			int vectors = 0;
			int enums = 0;
		};
		struct FbxAttrInt
		{
			char linkedname[32];
			char name[32];
			int value;
		};
		struct FbxAttrString
		{
			char linkedname[32];
			char name[32];
			char value[32];
		};
		struct FbxAttrFloat
		{
			char linkedname[32];
			char name[32];
			float value;
		};
		struct FbxTimelineData
		{
			int duration = 0;
			int framerate;
		};
		struct FbxAttrBool
		{
			char linkedname[32];
			char name[32];
			bool value;
		};
		struct FbxAttrVector
		{
			char linkedname[32];
			char name[32];
			float value[3];
		};
		struct FbxAttrEnum
		{
			char linkedname[32];
			char name[32];
			int value;
		};
		struct FbxVertexWeightHeader
		{
			char meshName[32];
			int vertexIndex;
			int connectedJoints = 0;
		};
		struct FbxVertexWeights
		{
			char joint[32];
			float weight;
			int vertexIndex;
		};

		struct FbxBlendShapeHeader
		{
			char targetMesh[32];
			char sourceMesh[32];
			int amountOfFrames;

		};
		struct FbxBlendShapeData
		{
			int frame;
			float value;
		};

		struct FbxBlendShapeShapeHeader
		{
			char name[32];
			int controlpoints = 0;
		};

#pragma endregion
		char* offset = new char(0);

#pragma region MasterHeader

		FbxMasterHeader masterHeader;
		in.read((char*)&masterHeader, sizeof(FbxMasterHeader));
		in.read((char*)offset, 1);

#pragma endregion

#pragma region Mesh

		struct FbxMesh
		{
			std::string name;
			std::string matName;

			int vertexCount = 0;

			std::string id;

			FbxVertexType vertexType;
			std::vector<FbxVertices> vertices;
			std::vector<FbxBasicVertices> verticesBasic;
		};

		std::vector<FbxMesh> meshes;
		FbxMeshHeader meshHeader{};
		for (int i = 0; i < masterHeader.meshes; i++)
		{
			in.read((char*)&meshHeader, sizeof(FbxMeshHeader));
			in.read((char*)offset, 1);

			meshes.emplace_back();
			meshes.back().name = meshHeader.name;
			meshes.back().matName = meshHeader.matName;
			meshes.back().vertexCount = meshHeader.vertices;
			meshes.back().vertexType = meshHeader.type;
			meshes.back().id = meshHeader.id;

			for (int i = 0; i < meshHeader.polygonVertexIndex; i++)
			{
				//switch by type of vertex
				switch (meshes.back().vertexType)
				{
				case FbxVertexType::BASIC: {
					meshes.back().verticesBasic.emplace_back();
					in.read((char*)&meshes.back().verticesBasic.back(), sizeof(FbxBasicVertices));
					in.read((char*)offset, 1);
					
				}break;

				case FbxVertexType::WITH_BI_TANGENT: {
					meshes.back().vertices.emplace_back();
					in.read((char*)&meshes.back().vertices.back(), sizeof(FbxVertices));
					in.read((char*)offset, 1);
				}break;

				}

			}
		}

#pragma endregion

#pragma region Material

		//get mats, mickel lyckades inte få ut material värderna
		std::vector<FbxMaterial> materials;
		for (int i = 0; i < masterHeader.materials; i++)
		{
			//read header
			FbxMaterial temp{};
			in.read((char*)&temp, sizeof(FbxMaterial));
			in.read((char*)offset, 1);

			bool exist = false;
			for (FbxMaterial& mat : materials)
			{
				if (std::string(mat.name) == std::string(temp.name))
				{
					exist = true;
					break;
				}

			}
			if (!exist)
				materials.push_back(temp);
		}


#pragma endregion

#pragma region Camera

		//get cameras
		std::vector<FbxCamera> cameras;
		for (int i = 0; i < masterHeader.cameras; i++)
		{
			//read header
			FbxCamera temp{};
			in.read((char*)&temp, sizeof(FbxCamera));
			in.read((char*)offset, 1);
			cameras.push_back(temp);
		}

#pragma endregion

#pragma region Light

		std::vector<FbxLight> lights;
		for (int i = 0; i < masterHeader.lights; i++)
		{
			//read header
			FbxLight temp{};
			in.read((char*)&temp, sizeof(FbxLight));
			in.read((char*)offset, 1);
			lights.push_back(temp);
		}

		for (FbxLight& light : lights)
		{
			light.intensity /= 100;
		}

#pragma endregion

#pragma region Group

		std::vector<FbxGroup> groups;
		for (int i = 0; i < masterHeader.groups; i++)
		{
			//read header
			FbxGroup temp{};
			in.read((char*)&temp, sizeof(FbxGroup));
			in.read((char*)offset, 1);
			groups.push_back(temp);
		}

#pragma endregion

#pragma region Joints

		std::vector<FbxJointInfo> joints;
		for (int i = 0; i < masterHeader.joints; i++)
		{
			//read header
			FbxJointInfo temp{};
			in.read((char*)&temp, sizeof(FbxJointInfo));
			in.read((char*)offset, 1);
			joints.push_back(temp);
		}

#pragma endregion

#pragma region Weights

		struct FbxJointAnimation
		{
			std::string mesh;
			int vertexIndex;
			std::vector<FbxVertexWeights> weights;
		};

		std::vector<FbxJointAnimation> weights;
		for (int i = 0; i < masterHeader.weights; i++)
		{
			FbxVertexWeightHeader header;
			in.read((char*)&header, sizeof(FbxVertexWeightHeader));
			in.read((char*)offset, 1);

			FbxJointAnimation result;
			result.mesh = header.meshName;
			result.vertexIndex = header.vertexIndex;

			for (int i = 0; i < header.connectedJoints; i++)
			{
				FbxVertexWeights weight;
				in.read((char*)&weight, sizeof(FbxVertexWeights));
				in.read((char*)offset, 1);

				result.weights.push_back(weight);
			}
			weights.push_back(result);
		}

#pragma endregion

#pragma region Animation

		struct FbxAnimation
		{
			std::string name; //meshName
			std::vector<FbxAnimationData> data;
		};

		std::vector<FbxAnimation> animations;
		for (int i = 0; i < masterHeader.animations; i++)
		{
			FbxAnimationHeader header{};
			in.read((char*)&header, sizeof(FbxAnimationHeader));
			in.read((char*)offset, 1);

			FbxAnimation anim{};
			anim.name = header.nodeName;
			for (int k = 0; k < header.keyCounts; k++)
			{
				FbxAnimationData data{};
				in.read((char*)&data, sizeof(FbxAnimationData));
				in.read((char*)offset, 1);
				anim.data.push_back(data);
			}
			animations.push_back(anim);
		}

#pragma endregion

#pragma region Blendshapes

		struct FbxBlendShape
		{
			std::string sourceMesh;
			std::string targetMesh;
			std::vector<FbxBlendShapeData> data;
		};

		std::vector<FbxBlendShape> blendShapes;
		for (int i = 0; i < masterHeader.blendshapes; i++)
		{
			FbxBlendShapeHeader header{};
			in.read((char*)&header, sizeof(FbxBlendShapeHeader));
			in.read((char*)offset, 1);

			FbxBlendShape blendshape{};
			blendshape.targetMesh = header.targetMesh;
			blendshape.sourceMesh = header.sourceMesh;
			blendshape.data.reserve(header.amountOfFrames);
			for (int i = 0; i < header.amountOfFrames; i++)
			{
				FbxBlendShapeData data{};
				in.read((char*)&data, sizeof(FbxBlendShapeData));
				in.read((char*)offset, 1);
				blendshape.data.push_back(data);
			}
			blendShapes.push_back(blendshape);
		}


		struct Shape
		{
			struct Data
			{
				float position[3];
			};
			std::string name;
			std::vector<Data> data;
		};
		std::vector<Shape> shapes;
		for (int i = 0; i < masterHeader.shapes; i++)
		{
			FbxBlendShapeShapeHeader header{};
			in.read((char*)&header, sizeof(FbxBlendShapeShapeHeader));
			in.read((char*)offset, 1);

			Shape shape;
			shape.name = header.name;
			for (int i = 0; i < header.controlpoints; i++)
			{
				Shape::Data data{};
				in.read((char*)&data, sizeof(Shape::Data));
				in.read((char*)offset, 1);
				shape.data.push_back(data);
			}

			if (header.controlpoints != 0)
				shapes.push_back(shape);
		}


#pragma endregion

#pragma region animationData

		FbxTimelineData timelinedata;
		if (animations.size() > 0)
		{
			in.read((char*)&timelinedata, sizeof(FbxTimelineData));
			in.read((char*)offset, 1);
		}

#pragma endregion

#pragma region Attributes

		struct FbxCustomAttr
		{
			std::vector<FbxAttrString> strings;
			std::vector<FbxAttrInt> ints;
			std::vector<FbxAttrBool> bools;
			std::vector<FbxAttrEnum> enums;
			std::vector<FbxAttrFloat> floats;
			std::vector<FbxAttrVector> vectors;
		};

		FbxCustomAttr attribute;
		for (int i = 0; i < masterHeader.attributes; i++)
		{
			//read header
			FbxAttrHeader attrHeader{};
			in.read((char*)&attrHeader, sizeof(FbxAttrHeader));
			in.read((char*)offset, 1);


			//ints
			for (int i = 0; i < attrHeader.ints; i++)
			{
				FbxAttrInt temp{};
				in.read((char*)&temp, sizeof(FbxAttrInt));
				in.read((char*)offset, 1);
				attribute.ints.push_back(temp);
			}

			//strings
			for (int i = 0; i < attrHeader.strings; i++)
			{
				FbxAttrString temp{};
				in.read((char*)&temp, sizeof(FbxAttrString));
				in.read((char*)offset, 1);
				attribute.strings.push_back(temp);
			}

			//float
			for (int i = 0; i < attrHeader.floats; i++)
			{
				FbxAttrFloat temp{};
				in.read((char*)&temp, sizeof(FbxAttrFloat));
				in.read((char*)offset, 1);
				attribute.floats.push_back(temp);
			}

			//bools
			for (int i = 0; i < attrHeader.bools; i++)
			{
				FbxAttrBool temp{};
				in.read((char*)&temp, sizeof(FbxAttrBool));
				in.read((char*)offset, 1);
				attribute.bools.push_back(temp);
			}

			//vector
			for (int i = 0; i < attrHeader.vectors; i++)
			{
				FbxAttrVector temp{};
				in.read((char*)&temp, sizeof(FbxAttrVector));
				in.read((char*)offset, 1);
				attribute.vectors.push_back(temp);
			}

			//enums
			for (int i = 0; i < attrHeader.enums; i++)
			{
				FbxAttrEnum temp{};
				in.read((char*)&temp, sizeof(FbxAttrEnum));
				in.read((char*)offset, 1);
				attribute.enums.push_back(temp);
			}

		}

#pragma endregion

#pragma region Delete

		if (offset != nullptr)
			delete offset;

#pragma endregion

#pragma region iCam

		for (FbxCamera& camera : cameras)// for each camera in cameras
		{
			LspScene::Camera* cam = new LspScene::Camera();

			cam->type = LspScene::Object::Type::Camera;

			//cam->parent =
			cam->name = camera.name;

			cam->farDistance = camera.far;
			cam->fov = camera.fov;
			cam->nearDistance = camera.near;
			cam->lookAt[0] = camera.lookAt[0];
			cam->lookAt[1] = camera.lookAt[1];
			cam->lookAt[2] = camera.lookAt[2];
			cam->transform.position[0] = camera.position[0];
			cam->transform.position[1] = camera.position[1];
			cam->transform.position[2] = camera.position[2];
			cam->transform.scale[0] = 1.0f;
			cam->transform.scale[1] = 1.0f;
			cam->transform.scale[2] = 1.0f;
			cam->transform.rotation[0] = 0.0f;
			cam->transform.rotation[1] = 0.0f;
			cam->transform.rotation[2] = 0.0f;

			cam->up[0] = camera.up[0];
			cam->up[1] = camera.up[1];
			cam->up[2] = camera.up[2];



			cam->id = camera.id; //Grupp id


			for (FbxAnimation& animation : animations) ///add animation 
			{

				if (cam->name == animation.name)
				{
					cam->animations.push_back(LspScene::AnimationList());
					for (FbxAnimationData& aniData : animation.data)
					{
						LspScene::AnimationKey anime = LspScene::AnimationKey();

						anime.key = aniData.key;

						anime.data.position[0] = aniData.translate[0];
						anime.data.position[1] = aniData.translate[1];
						anime.data.position[2] = aniData.translate[2];

						anime.data.scale[0] = aniData.scale[0];
						anime.data.scale[1] = aniData.scale[1];
						anime.data.scale[2] = aniData.scale[2];

						anime.data.rotation[0] = aniData.rotation[0];
						anime.data.rotation[1] = aniData.rotation[1];
						anime.data.rotation[2] = aniData.rotation[2];

						cam->animations.back().keys.push_back(anime);
					}
				}
			}


			//cam->children = 

			this->cameras.push_back(cam);
		}

#pragma endregion

#pragma region Ilit

		for (FbxLight& light : lights)
		{

			LspScene::Light* lightObj = new LspScene::Light();

			lightObj->type = LspScene::Object::Type::Light;

			if (std::string(light.type) == ("Point"))
			{
				lightObj->lightType = LspScene::Light::Type::Point;

			}
			if (std::string(light.type) == ("Spot"))
			{
				lightObj->lightType = LspScene::Light::Type::Spot;

			}
			if (std::string(light.type) == ("Directional"))
			{
				lightObj->lightType = LspScene::Light::Type::Directional;

			}

			//lightObj->parent = ;

			lightObj->name = light.name;

			lightObj->transform.position[0] = light.position[0];
			lightObj->transform.position[1] = light.position[1];
			lightObj->transform.position[2] = light.position[2];
			lightObj->transform.scale[0] = 1.0f;
			lightObj->transform.scale[1] = 1.0f;
			lightObj->transform.scale[2] = 1.0f;
			lightObj->transform.rotation[0] = 0.0f;
			lightObj->transform.rotation[1] = 0.0f;
			lightObj->transform.rotation[2] = 0.0f;

			lightObj->dir[0] = light.dir[0];
			lightObj->dir[1] = light.dir[1];
			lightObj->dir[2] = light.dir[2];

			lightObj->color[0] = light.color[0];
			lightObj->color[1] = light.color[1];
			lightObj->color[2] = light.color[2];

			lightObj->intensity = light.intensity;

			lightObj->id = light.id;


			for (FbxAnimation& animation : animations) ///add animation 
			{
				if (lightObj->name == animation.name)
				{
					lightObj->animations.push_back(LspScene::AnimationList());
					for (FbxAnimationData& aniData : animation.data)
					{
						LspScene::AnimationKey anime = LspScene::AnimationKey();

						anime.key = aniData.key;

						anime.data.position[0] = aniData.translate[0];
						anime.data.position[1] = aniData.translate[1];
						anime.data.position[2] = aniData.translate[2];

						anime.data.scale[0] = aniData.scale[0];
						anime.data.scale[1] = aniData.scale[1];
						anime.data.scale[2] = aniData.scale[2];

						anime.data.rotation[0] = aniData.rotation[0];
						anime.data.rotation[1] = aniData.rotation[1];
						anime.data.rotation[2] = aniData.rotation[2];


						lightObj->animations.back().keys.push_back(anime);
					}
				}
			}

			//lightObj->children =  ;

			this->lights.push_back(lightObj);
		}

#pragma endregion

#pragma region iMat

		for (FbxMaterial& material : materials) //kanske klar ?
		{
			LspScene::Material* mat = new LspScene::Material();


			mat->name = material.name;

			mat->texureFileName = material.textureName;

			mat->ambient[0] = material.ambient[0];
			mat->ambient[1] = material.ambient[1];
			mat->ambient[2] = material.ambient[2];

			mat->diffuse[0] = material.diffuse[0];
			mat->diffuse[1] = material.diffuse[1];
			mat->diffuse[2] = material.diffuse[2];

			mat->specular[0] = material.specular[0];
			mat->specular[1] = material.specular[1];
			mat->specular[2] = material.specular[2];

			mat->emissive[0] = material.emissive[0];
			mat->emissive[1] = material.emissive[1];
			mat->emissive[2] = material.emissive[2];

			mat->opacity = material.opacity;
			mat->specularPower = material.specularPower;
			mat->reflectivity = material.reflectivity;


			this->materials.push_back(mat);

		}

#pragma endregion

#pragma region Ijoint
		for (FbxJointInfo& joint : joints) //?
		{
			LspScene::Joint* bone = new LspScene::Joint();

			bone->type = LspScene::Object::Type::Joint;

			bone->name = joint.name;

			bone->transform.position[0] = 0.0f;		//used as defalt, it connects values in animation-loop
			bone->transform.position[1] = 0.0f;
			bone->transform.position[2] = 0.0f;
			bone->transform.scale[0] = 1.0f;		//used as defalt, it connects values in animation-loop
			bone->transform.scale[1] = 1.0f;
			bone->transform.scale[2] = 1.0f;
			bone->transform.rotation[0] = 0.0f;		//used as defalt, it  connects values in animation-loop
			bone->transform.rotation[1] = 0.0f;
			bone->transform.rotation[2] = 0.0f;

			bone->bindPoseInverse[0][0] = joint.x[0];
			bone->bindPoseInverse[0][1] = joint.x[1];
			bone->bindPoseInverse[0][2] = joint.x[2];
			bone->bindPoseInverse[0][3] = joint.x[3];

			bone->bindPoseInverse[1][0] = joint.y[0];
			bone->bindPoseInverse[1][1] = joint.y[1];
			bone->bindPoseInverse[1][2] = joint.y[2];
			bone->bindPoseInverse[1][3] = joint.y[3];

			bone->bindPoseInverse[2][0] = joint.z[0];
			bone->bindPoseInverse[2][1] = joint.z[1];
			bone->bindPoseInverse[2][2] = joint.z[2];
			bone->bindPoseInverse[2][3] = joint.z[3];

			bone->bindPoseInverse[3][0] = joint.w[0];
			bone->bindPoseInverse[3][1] = joint.w[1];
			bone->bindPoseInverse[3][2] = joint.w[2];
			bone->bindPoseInverse[3][3] = joint.w[3];


			//bone->inverse =  //fråga Mikael vad vi fått

			for (FbxAnimation& animation : animations) ///add animation 
			{
				if (bone->name == animation.name)
				{
					bone->animations.push_back(LspScene::AnimationList());
					for (FbxAnimationData& aniData : animation.data)
					{
						LspScene::AnimationKey anime = LspScene::AnimationKey();

						anime.key = aniData.key;

						anime.data.position[0] = aniData.translate[0];
						anime.data.position[1] = aniData.translate[1];
						anime.data.position[2] = aniData.translate[2];

						anime.data.scale[0] = aniData.scale[0];
						anime.data.scale[1] = aniData.scale[1];
						anime.data.scale[2] = aniData.scale[2];

						anime.data.rotation[0] = aniData.rotation[0];
						anime.data.rotation[1] = aniData.rotation[1];
						anime.data.rotation[2] = aniData.rotation[2];


						bone->animations.back().keys.push_back(anime);
					}

				}
			}

			//bone->children =

			this->joints.push_back(bone);
		}
#pragma endregion

#pragma region Imesh

		for (FbxMesh& mesh : meshes) //inte klar
		{
			LspScene::Mesh* meshObj = new LspScene::Mesh();

			meshObj->type = LspScene::Object::Type::Mesh;

			//meshObj->parent = 

			meshObj->name = mesh.name;

			//meshObj->transform = 
			meshObj->transform.position[0] = 0.0f;		//used as defalt, it connects values in animation-loop
			meshObj->transform.position[1] = 0.0f;
			meshObj->transform.position[2] = 0.0f;
			meshObj->transform.scale[0] = 1.0f;		//used as defalt, it connects values in animation-loop
			meshObj->transform.scale[1] = 1.0f;
			meshObj->transform.scale[2] = 1.0f;
			meshObj->transform.rotation[0] = 0.0f;		//used as defalt, it  connects values in animation-loop
			meshObj->transform.rotation[1] = 0.0f;
			meshObj->transform.rotation[2] = 0.0f;


			//float position[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
			//float uv[2]{ 0.0f,0.0f };//float2 array 
			//float normal[3]{ 0.0f, 0.0f, 0.0f };//float3 array 

			//float biNormal[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
			//float tangent[3]{ 0.0f, 0.0f, 0.0f };//float3 array 
			if (mesh.vertexType == FbxVertexType::BASIC)
			{
				for (FbxBasicVertices& basic : mesh.verticesBasic)
				{
					meshObj->vertices.emplace_back();
					//LspScene::Mesh::Vertex vertex = LspScene::Mesh::Vertex();
					meshObj->vertices.back().position[0] = basic.position[0];
					meshObj->vertices.back().position[1] = basic.position[1];
					meshObj->vertices.back().position[2] = basic.position[2];
					meshObj->vertices.back().uv[0] = basic.uv[0];
					meshObj->vertices.back().uv[1] = basic.uv[1];
					meshObj->vertices.back().normal[0] = basic.normal[0];
					meshObj->vertices.back().normal[1] = basic.normal[1];
					meshObj->vertices.back().normal[2] = basic.normal[2];
				}

			}
			else if (mesh.vertexType == FbxVertexType::WITH_BI_TANGENT)
			{
				for (FbxVertices& biTang : mesh.vertices)
				{
					meshObj->vertices.emplace_back();
					//LspScene::Mesh::Vertex vertex = LspScene::Mesh::Vertex();
					meshObj->vertices.back().position[0] = biTang.position[0];
					meshObj->vertices.back().position[1] = biTang.position[1];
					meshObj->vertices.back().position[2] = biTang.position[2];
					meshObj->vertices.back().uv[0] = biTang.uv[0];
					meshObj->vertices.back().uv[1] = biTang.uv[1];
					meshObj->vertices.back().normal[0] = biTang.normal[0];
					meshObj->vertices.back().normal[1] = biTang.normal[1];
					meshObj->vertices.back().normal[2] = biTang.normal[2];

					meshObj->vertices.back().biNormal[0] = biTang.biNormal[0];
					meshObj->vertices.back().biNormal[1] = biTang.biNormal[1];
					meshObj->vertices.back().biNormal[2] = biTang.biNormal[2];

					meshObj->vertices.back().tangent[0] = biTang.tangent[0];
					meshObj->vertices.back().tangent[1] = biTang.tangent[1];
					meshObj->vertices.back().tangent[2] = biTang.tangent[2];

					//meshObj->vertices.push_back(vertex);
				}


			}


			//meshObj->animations = ???????????

			for (FbxAnimation& animation : animations) ///add animation 
			{
				if (meshObj->name == animation.name)
				{

					for (FbxAnimationData& aniData : animation.data)
					{
						LspScene::AnimationKey anime = LspScene::AnimationKey();
						anime.key = aniData.key;

						anime.data.position[0] = aniData.translate[0];
						anime.data.position[1] = aniData.translate[1];
						anime.data.position[2] = aniData.translate[2];

						anime.data.scale[0] = aniData.scale[0];
						anime.data.scale[1] = aniData.scale[1];
						anime.data.scale[2] = aniData.scale[2];

						anime.data.rotation[0] = aniData.rotation[0];
						anime.data.rotation[1] = aniData.rotation[1];
						anime.data.rotation[2] = aniData.rotation[2];


						meshObj->vertexAnimations.push_back(anime);
					}
				}
			}

			for (LspScene::Material*& material : this->materials) ///add animation 
			{
				if (material->name == std::string(mesh.matName))
				{
					meshObj->material = material;
					break;
				}
			}

			// 
			//meshObj->blendShapes =


			for (FbxJointAnimation& weight : weights) ///add animation 
			{
				if (meshObj->name == weight.mesh)
				{

					for (FbxVertexWeights& weightData : weight.weights)
					{
						LspScene::Mesh::Weight vertexWeights = LspScene::Mesh::Weight();

						vertexWeights.vertexIndex = weightData.vertexIndex;

						for (LspScene::Joint*& joint : this->joints) ///add animation 
						{
							if (joint->name == std::string(weightData.joint))
							{
								//weightData.joint;
								vertexWeights.joint = joint;
								break;
							}

						}
						vertexWeights.value = weightData.weight;

						meshObj->weights.push_back(vertexWeights);
					}
				}
			}

			//meshObj->children
			//->vertexAnimations.push_back(anime);
			this->meshes.push_back(meshObj);
		}

#pragma endregion

#pragma region Save_BlendShape_On_Heap


		for (Shape& shape : shapes)
		{
			LspScene::Shape* finalShape = new LspScene::Shape();
			finalShape->name = shape.name;

			for (Shape::Data d : shape.data)
			{
				LspScene::Shape::Data data{};
				data.position[0] = d.position[0];
				data.position[1] = d.position[1];
				data.position[2] = d.position[2];

				finalShape->data.push_back(data);
			}

			this->shapes.push_back(finalShape);
		}

		for (FbxBlendShape& blendshape : blendShapes) ///add animation 
		{
			for (LspScene::Mesh*& target : this->meshes)
			{
				if (blendshape.targetMesh == target->name)
				{
					LspScene::Mesh::BlendShape tempBlend = LspScene::Mesh::BlendShape();

					for (LspScene::Shape*& sourceShape : this->shapes)
					{
						if (blendshape.sourceMesh == sourceShape->name)
						{
							tempBlend.source = sourceShape;
							break;
						}

					}

					for (FbxBlendShapeData& blendShapeData : blendshape.data)
					{
						LspScene::Mesh::BlendShape::Data data = LspScene::Mesh::BlendShape::Data();

						data.frame = blendShapeData.frame;
						data.value = blendShapeData.value;

						tempBlend.data.push_back(data);
					}

					target->blendShapes.push_back(tempBlend);
				}

			}

		}

#pragma endregion

#pragma region Map

		std::unordered_map<std::string, LspScene::Object*> objMap;

		for (LspScene::Joint*& obj : this->joints)
		{
			objMap.insert(std::make_pair(obj->name, obj));
			allObjects.push_back(obj);
		}

		for (LspScene::Camera*& obj : this->cameras)
		{
			objMap.insert(std::make_pair(obj->name, obj));
			allObjects.push_back(obj);
		}

		for (LspScene::Light*& obj : this->lights)
		{
			objMap.insert(std::make_pair(obj->name, obj));
			allObjects.push_back(obj);
		}

		for (LspScene::Mesh*& obj : this->meshes)
		{
			objMap.insert(std::make_pair(obj->name, obj));
			allObjects.push_back(obj);
		}

#pragma endregion

#pragma region IparentChildren

		//this->root = new LspScene::Object();

		for (FbxGroup& g : groups)
		{
			std::string parent = g.parent;
			std::string name = g.name;

			if (objMap.find(name) == objMap.end())
			{
				Group* g2 = new Group();
				g2->type = Object::Type::Group;
				g2->name = name;
				this->groups.push_back(g2);
				this->allObjects.push_back(g2);

				g2->transform.position[0] = g.position[0];
				g2->transform.position[1] = g.position[1];
				g2->transform.position[2] = g.position[2];

				g2->transform.rotation[0] = g.rotation[0];
				g2->transform.rotation[1] = g.rotation[1];
				g2->transform.rotation[2] = g.rotation[2];

				g2->transform.scale[0] = g.scale[0];
				g2->transform.scale[1] = g.scale[1];
				g2->transform.scale[2] = g.scale[2];

				objMap.insert({ name, g2 });
			}

		}

		for (FbxGroup& g : groups)
		{
			std::string parent = g.parent;
			std::string name = g.name;

			if (parent != "RootNode" && (int)parent[0] >= 0 && parent != "null")
			{
				objMap[name]->parent = objMap[parent];
				objMap[parent]->children.push_back(objMap[name]);
			}
		}

		for (auto& obj : cameras)
		{
			std::string parent = obj.id;
			std::string name = obj.name;
			if (parent != "RootNode" && (int)parent[0] >= 0 && parent != "null")
			{
				objMap[name]->parent = objMap[parent];
				bool exist = false;
				for (auto& pc : objMap[parent]->children)
				{
					if (pc->name == name)
					{
						exist = true;
						break;
					}
				}
				if (!exist)
					objMap[parent]->children.push_back(objMap[name]);
			}
		}

		for (auto& obj : lights)
		{
			std::string parent = obj.id;
			std::string name = obj.name;
			if (parent != "RootNode" && (int)parent[0] >= 0 && parent != "null")
			{
				objMap[name]->parent = objMap[parent];
				bool exist = false;
				for (auto& pc : objMap[parent]->children)
				{
					if (pc->name == name)
					{
						exist = true;
						break;
					}
				}
				if (!exist)
				{
					objMap[parent]->children.push_back(objMap[name]);
				}
			}
		}

		for (auto& obj : meshes)
		{
			std::string parent = obj.id;
			std::string name = obj.name;
			if (parent != "RootNode" && (int)parent[0] >= 0 && parent != "null")
			{
				objMap[name]->parent = objMap[parent];
				bool exist = false;
				for (auto& pc : objMap[parent]->children)
				{
					if (pc->name == name)
					{
						exist = true;
						break;
					}
				}
				if (!exist)
				{
					objMap[parent]->children.push_back(objMap[name]);
				}
			}
		}


		for (auto& obj : joints)
		{
			std::string parent = obj.parent;
			std::string name = obj.name;
			if (parent != "RootNode" && (int)parent[0] >= 0 && parent != "null")
			{
				objMap[name]->parent = objMap[parent];

				bool exist = false;
				for (auto& pc : objMap[parent]->children)
				{
					if (pc->name == name)
					{
						exist = true;
						break;
					}
				}
				if (!exist)
				{
					objMap[parent]->children.push_back(objMap[name]);
				}
			}
		}

		root = new LspScene::Object();
		for (LspScene::Object*& obj : this->allObjects)
		{
			if (obj->parent == nullptr)
			{
				root->children.push_back(obj);
			}
		}

#pragma endregion

#pragma region IAttr

		for (FbxAttrString& attr : attribute.strings)
		{
			LspScene::Object::CustomAttr::StringAttr strAttr;
			strAttr.name = attr.name;
			strAttr.value = attr.value;
			objMap[std::string(attr.linkedname)]->customAttrs.strings.push_back(strAttr);
		}

		for (FbxAttrBool& attr : attribute.bools)
		{
			LspScene::Object::CustomAttr::BoolAttr tempAttr;
			tempAttr.name = attr.name;
			tempAttr.value = attr.value;
			objMap[std::string(attr.linkedname)]->customAttrs.bools.push_back(tempAttr);
		}

		for (FbxAttrInt& attr : attribute.ints)
		{
			LspScene::Object::CustomAttr::IntAttr tempAttr;
			tempAttr.name = attr.name;
			tempAttr.value = attr.value;
			objMap[std::string(attr.linkedname)]->customAttrs.ints.push_back(tempAttr);
		}

		for (FbxAttrFloat& attr : attribute.floats)
		{
			LspScene::Object::CustomAttr::FloatAttr tempAttr;
			tempAttr.name = attr.name;
			tempAttr.value = attr.value;
			objMap[std::string(attr.linkedname)]->customAttrs.floats.push_back(tempAttr);
		}

		for (FbxAttrEnum& attr : attribute.enums)
		{
			LspScene::Object::CustomAttr::EnumAttr tempAttr;
			tempAttr.name = attr.name;
			tempAttr.value = attr.value;
			objMap[std::string(attr.linkedname)]->customAttrs.enums.push_back(tempAttr);
		}

		for (FbxAttrVector& attr : attribute.vectors)
		{
			LspScene::Object::CustomAttr::VectorAttr tempAttr;
			tempAttr.name = attr.name;
			tempAttr.value[0] = attr.value[0];
			tempAttr.value[1] = attr.value[1];
			tempAttr.value[2] = attr.value[2];
			objMap[std::string(attr.linkedname)]->customAttrs.vectors.push_back(tempAttr);
		}

#pragma endregion

#pragma region TimeLine

		timelineInfo.duration = timelinedata.duration;
		timelineInfo.framerate = timelinedata.framerate;

#pragma endregion

		in.close();
	}
}

void LspScene::clear()
{
	for (Group*& g : groups)
	{
		if (g != nullptr)
		{
			delete g;
		}
	}
	groups.clear();
	for (Shape*& shape : shapes)
	{
		if (shape != nullptr)
		{
			delete shape;
		}
	}
	shapes.clear();

	for (Material*& material : materials)
	{
		if (material != nullptr)
		{
			delete material;
		}
	}
	materials.clear();

	for (Joint*& joint : joints)
	{
		if (joint != nullptr)
		{
			delete joint;
		}
	}
	joints.clear();

	for (Light*& light : lights)
	{
		if (light != nullptr)
		{
			delete light;
		}
	}
	lights.clear();

	for (Camera*& camera : cameras)
	{
		if (camera != nullptr)
		{
			delete camera;
		}
	}
	cameras.clear();

	for (Mesh*& mesh : meshes)
	{
		if (mesh != nullptr)
		{
			delete mesh;
		}
	}
	meshes.clear();

	allObjects.clear();

	if (root != nullptr)
	{
		delete root;
	}
}

LspScene::LspScene(std::string path)
{
	load(path);
}

LspScene::~LspScene()
{
	clear();
}

//LspSkeletalMesh::LspSkeletalMesh(const LspSkeletalMesh& other)
/* {
	for (int i = 0; i < 3; i++)
	{
		this->worldTransform.position[i] = other.worldTransform.position[i];
		this->worldTransform.rotation[i] = other.worldTransform.rotation[i];
		this->worldTransform.scale[i] = other.worldTransform.scale[i];
	}
	this->sceneFileName = other.sceneFileName;

	//Skeleton
	for (int iJo = 0; iJo < other.skeleton.size(); iJo++)
	{
		LspScene::Joint newJoint;
		//Name
		newJoint.name = other.skeleton.at(iJo).name;

		//Type
		newJoint.type = other.skeleton.at(iJo).type;

		//Animations
		for (int iAnim = 0; iAnim < other.skeleton.at(iJo).animations.size(); iAnim++)
		{
			LspScene::AnimationList newAnim;

			//Keys
			for (int iKey = 0; iKey < other.skeleton.at(iJo).animations.at(iAnim).keys.size(); iKey++)
			{
				LspScene::AnimationKey newKey;

				for (int i = 0; i < 3; i++)
				{
					newKey.data.position[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.position[i];
					newKey.data.rotation[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.rotation[i];
					newKey.data.scale[i] = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).data.scale[i];
				}
				newKey.key = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).key;
				newKey.name = other.skeleton.at(iJo).animations.at(iAnim).keys.at(iKey).name;

				newAnim.keys.push_back(newKey);
			}

			newJoint.animations.push_back(newAnim);
		}

		//BindPoseInverse
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				newJoint.bindPoseInverse[x][y] = other.skeleton.at(iJo).bindPoseInverse[x][y];
			}
		}

		//CustomAttrs
		LspScene::Object::CustomAttr attrs;

		//Strings
		for (int iStr = 0; iStr < other.skeleton.at(iJo).customAttrs.strings.size(); iStr++)
		{
			LspScene::Object::CustomAttr::StringAttr newStr;
			newStr.name = other.skeleton.at(iJo).customAttrs.strings.at(iStr).name;
			newStr.value = other.skeleton.at(iJo).customAttrs.strings.at(iStr).value;
			attrs.strings.push_back(newStr);
		}

		//Ints
		for (int iInt = 0; iInt < other.skeleton.at(iJo).customAttrs.ints.size(); iInt++)
		{
			LspScene::Object::CustomAttr::IntAttr newInt;
			newInt.name = other.skeleton.at(iJo).customAttrs.ints.at(iInt).name;
			newInt.value = other.skeleton.at(iJo).customAttrs.ints.at(iInt).value;
			attrs.ints.push_back(newInt);
		}

		//Bools
		for (int iBoo = 0; iBoo < other.skeleton.at(iJo).customAttrs.bools.size(); iBoo++)
		{
			LspScene::Object::CustomAttr::BoolAttr boo;
			boo.name = other.skeleton.at(iJo).customAttrs.bools.at(iBoo).name;
			boo.value = other.skeleton.at(iJo).customAttrs.bools.at(iBoo).value;
			attrs.bools.push_back(boo);
		}

		//Enums
		for (int iEn = 0; iEn < other.skeleton.at(iJo).customAttrs.enums.size(); iEn++)
		{
			LspScene::Object::CustomAttr::EnumAttr en;
			en.name = other.skeleton.at(iJo).customAttrs.enums.at(iEn).name;
			en.value = other.skeleton.at(iJo).customAttrs.enums.at(iEn).value;
			attrs.enums.push_back(en);
		}

		//Floats
		for (int iFl = 0; iFl < other.skeleton.at(iJo).customAttrs.floats.size(); iFl++)
		{
			LspScene::Object::CustomAttr::FloatAttr fl;
			fl.name = other.skeleton.at(iJo).customAttrs.floats.at(iFl).name;
			fl.value = other.skeleton.at(iJo).customAttrs.floats.at(iFl).value;
			attrs.floats.push_back(fl);
		}

		//Vectors
		for (int iVec = 0; iVec < other.skeleton.at(iJo).customAttrs.vectors.size(); iVec++)
		{
			LspScene::Object::CustomAttr::VectorAttr vec;
			vec.name = other.skeleton.at(iJo).customAttrs.vectors.at(iVec).name;
			for (int i = 0; i < 3; i++)
			{
				vec.value[i] = other.skeleton.at(iJo).customAttrs.vectors.at(iVec).value[i];
			}
			attrs.vectors.push_back(vec);
		}
		newJoint.customAttrs = attrs;

		//Transform
		for (int i = 0; i < 3; i++)
		{
			newJoint.transform.position[i] = other.skeleton.at(iJo).transform.position[i];
			newJoint.transform.rotation[i] = other.skeleton.at(iJo).transform.rotation[i];
			newJoint.transform.scale[i] = other.skeleton.at(iJo).transform.scale[i];
		}

		this->skeleton.push_back(newJoint);
	}

	//Skeleton children & parents
	for (int iTarget = 0; iTarget < this->skeleton.size(); iTarget++)
	{
		for (int iSeek = 0; iSeek < other.skeleton.size(); iSeek++)
		{
			//Parent
			if (other.skeleton.at(iSeek).parent != nullptr && this->skeleton.at(iTarget).name == other.skeleton.at(iSeek).parent->name)
			{
				this->skeleton.at(iSeek).parent = &this->skeleton.at(iTarget);
			}

			//Children
			else
			{
				for (int iChild = 0; iChild < other.skeleton.at(iSeek).children.size(); iChild++)
				{
					if (this->skeleton.at(iTarget).name == other.skeleton.at(iSeek).children.at(iChild)->name)
					{
						this->skeleton.at(iSeek).children.push_back(&this->skeleton.at(iTarget));
					}
				}
			}

		}
	}

	//Bones
	for (int iBo = 0; iBo < other.bones.size(); iBo++)
	{
		LspBone newBone(other.bones.at(iBo));
		this->bones.push_back(newBone);
	}

	//Mesh
	//Name
	this->mesh.name = other.mesh.name;

	//Parent
	this->mesh.parent = other.mesh.parent;

	//Type
	this->mesh.type = other.mesh.type;

	//AnimationList
	for (int iAnim = 0; iAnim < other.mesh.animations.size(); iAnim++)
	{
		LspScene::AnimationList newAnim;

		for (int iKey = 0; iKey < other.mesh.animations.at(iAnim).keys.size(); iKey++)
		{
			LspScene::AnimationKey newKey;
			for (int k = 0; k < 3; k++)
			{
				newKey.data.position[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.position[k];
				newKey.data.rotation[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.rotation[k];
				newKey.data.scale[k] = other.mesh.animations.at(iAnim).keys.at(iKey).data.scale[k];
			}
			newKey.key = other.mesh.animations.at(iAnim).keys.at(iKey).key;
			newKey.name = other.mesh.animations.at(iAnim).keys.at(iKey).name;

			newAnim.keys.push_back(newKey);
		}
		this->mesh.animations.push_back(newAnim);
	}

	//BlendShapes
	for (int iBlSh = 0; iBlSh < other.mesh.blendShapes.size(); iBlSh++)
	{
		LspScene::Mesh::BlendShape newBlSh;

		for (int iData = 0; iData < other.mesh.blendShapes.at(iBlSh).data.size(); iData++)
		{
			LspScene::Mesh::BlendShape::Data newData;
			newData.frame = other.mesh.blendShapes.at(iBlSh).data.at(iData).frame;
			newData.value = other.mesh.blendShapes.at(iBlSh).data.at(iData).value;

			newBlSh.data.push_back(newData);
		}
		newBlSh.source = other.mesh.blendShapes.at(iBlSh).source;
		this->mesh.blendShapes.push_back(newBlSh);
	}

	//Children
	for (int iChildren = 0; iChildren < other.mesh.children.size(); iChildren++)
	{
		this->mesh.children.push_back(other.mesh.children.at(iChildren));
	}

	//CustomAttrs
	LspScene::Object::CustomAttr attrs;

	//Strings
	for (int iStr = 0; iStr < other.mesh.customAttrs.strings.size(); iStr++)
	{
		LspScene::Object::CustomAttr::StringAttr newStr;
		newStr.name = other.mesh.customAttrs.strings.at(iStr).name;
		newStr.value = other.mesh.customAttrs.strings.at(iStr).value;
		attrs.strings.push_back(newStr);
	}

	//Ints
	for (int iInt = 0; iInt < other.mesh.customAttrs.ints.size(); iInt++)
	{
		LspScene::Object::CustomAttr::IntAttr newInt;
		newInt.name = other.mesh.customAttrs.ints.at(iInt).name;
		newInt.value = other.mesh.customAttrs.ints.at(iInt).value;
		attrs.ints.push_back(newInt);
	}

	//Bools
	for (int iBoo = 0; iBoo < other.mesh.customAttrs.bools.size(); iBoo++)
	{
		LspScene::Object::CustomAttr::BoolAttr boo;
		boo.name = other.mesh.customAttrs.bools.at(iBoo).name;
		boo.value = other.mesh.customAttrs.bools.at(iBoo).value;
		attrs.bools.push_back(boo);
	}

	//Enums
	for (int iEn = 0; iEn < other.mesh.customAttrs.enums.size(); iEn++)
	{
		LspScene::Object::CustomAttr::EnumAttr en;
		en.name = other.mesh.customAttrs.enums.at(iEn).name;
		en.value = other.mesh.customAttrs.enums.at(iEn).value;
		attrs.enums.push_back(en);
	}

	//Floats
	for (int iFl = 0; iFl < other.mesh.customAttrs.floats.size(); iFl++)
	{
		LspScene::Object::CustomAttr::FloatAttr fl;
		fl.name = other.mesh.customAttrs.floats.at(iFl).name;
		fl.value = other.mesh.customAttrs.floats.at(iFl).value;
		attrs.floats.push_back(fl);
	}

	//Vectors
	for (int iVec = 0; iVec < other.mesh.customAttrs.vectors.size(); iVec++)
	{
		LspScene::Object::CustomAttr::VectorAttr vec;
		vec.name = other.mesh.customAttrs.vectors.at(iVec).name;
		for (int i = 0; i < 3; i++)
		{
			vec.value[i] = other.mesh.customAttrs.vectors.at(iVec).value[i];
		}
		attrs.vectors.push_back(vec);
	}
	this->mesh.customAttrs = attrs;

	//Material
	this->material.name = other.material.name;
	this->material.texureFileName = other.material.texureFileName;
	for (int i = 0; i < 3; i++)
	{
		this->material.ambient[i] = other.material.ambient[i];
		this->material.diffuse[i] = other.material.diffuse[i];
		this->material.specular[i] = other.material.specular[i];
		this->material.emissive[i] = other.material.emissive[i];
	}
	this->material.opacity = other.material.opacity;
	this->material.specularPower = other.material.specularPower;
	this->material.reflectivity = other.material.reflectivity;

	//Mesh material reference
	this->mesh.material = &this->material;

	//Transform
	for (int i = 0; i < 3; i++)
	{
		this->mesh.transform.position[i] = other.mesh.transform.position[i];
		this->mesh.transform.rotation[i] = other.mesh.transform.rotation[i];
		this->mesh.transform.scale[i] = other.mesh.transform.scale[i];
	}

	//Vertices
	for (int iVx = 0; iVx < other.mesh.vertices.size(); iVx++)
	{
		LspScene::Mesh::Vertex vx;
		for (int i = 0; i < 3; i++)
		{
			vx.position[i] = other.mesh.vertices.at(iVx).position[i];
			vx.normal[i] = other.mesh.vertices.at(iVx).normal[i];
			vx.biNormal[i] = other.mesh.vertices.at(iVx).biNormal[i];
			vx.tangent[i] = other.mesh.vertices.at(iVx).tangent[i];
			if (i < 2)
			{
				vx.uv[i] = other.mesh.vertices.at(iVx).uv[i];
			}
		}
		this->mesh.vertices.push_back(vx);
	}

	//VertexAnimations
	for (int iKey = 0; iKey < other.mesh.vertexAnimations.size(); iKey++)
	{
		LspScene::AnimationKey newKey;

		for (int k = 0; k < 3; k++)
		{
			newKey.data.position[k] = other.mesh.vertexAnimations.at(iKey).data.position[k];
			newKey.data.rotation[k] = other.mesh.vertexAnimations.at(iKey).data.rotation[k];
			newKey.data.scale[k] = other.mesh.vertexAnimations.at(iKey).data.scale[k];

		}

		newKey.key = other.mesh.vertexAnimations.at(iKey).key;
		newKey.name = other.mesh.vertexAnimations.at(iKey).name;

		this->mesh.vertexAnimations.push_back(newKey);
	}

	//Weights
	for (int iW = 0; iW < other.mesh.weights.size(); iW++)
	{
		LspScene::Mesh::Weight w;
		w.vertexIndex = other.mesh.weights.at(iW).vertexIndex;
		w.value = other.mesh.weights.at(iW).value;

		for (int iJo = 0; iJo < this->skeleton.size(); iJo++)
		{
			if (other.mesh.weights.at(iW).joint->name == this->skeleton.at(iJo).name)
			{
				w.joint = &this->skeleton.at(iJo);
			}
		}

		this->mesh.weights.push_back(w);
	}

	//Timeline
	for (int iTim = 0; iTim < other.animTimeline.size(); iTim++)
	{
		LspScene::TimelineInfo newTimeLine;
		newTimeLine.duration = other.animTimeline.at(iTim).duration;
		newTimeLine.framerate = other.animTimeline.at(iTim).framerate;
		this->animTimeline.push_back(newTimeLine);
	}
}
*/
//XMMATRIX Inverse(const XMMATRIX& mx)
//{
//	return XMMatrixInverse(nullptr, mx);
//}
//
//XMMATRIX CreateLocalMx(const LspSkeletalMesh& mesh, int index)
//{
//	XMMATRIX myScaleMx = XMMatrixScaling(mesh.skeleton.at(index).transform.scale[0], mesh.skeleton.at(index).transform.scale[1], mesh.skeleton.at(index).transform.scale[2]);
//	XMMATRIX myRotXMx = XMMatrixRotationX((mesh.skeleton.at(index).transform.rotation[0] / 180) * XM_PI);
//	XMMATRIX myRotYMx = XMMatrixRotationY((mesh.skeleton.at(index).transform.rotation[1] / 180) * XM_PI);
//	XMMATRIX myRotZMx = XMMatrixRotationZ((mesh.skeleton.at(index).transform.rotation[2] / 180) * XM_PI);
//	XMMATRIX myposMx = XMMatrixTranslation(mesh.skeleton.at(index).transform.position[0], mesh.skeleton.at(index).transform.position[1], mesh.skeleton.at(index).transform.position[2]);
//
//	XMMATRIX myRotMx = XMMatrixMultiply(myRotXMx, myRotYMx);
//	myRotMx = XMMatrixMultiply(myRotMx, myRotZMx);
//
//	XMMATRIX localMx = XMMatrixMultiply(myScaleMx, myRotMx);
//	localMx = XMMatrixMultiply(localMx, myposMx);
//
//	return localMx;
//}
//
//XMMATRIX CreateLocalMx(const std::vector<LspScene::Joint>& skeleton, int index)
//{
//	XMMATRIX myScaleMx = XMMatrixScaling(skeleton.at(index).transform.scale[0], skeleton.at(index).transform.scale[1], skeleton.at(index).transform.scale[2]);
//	XMMATRIX myRotXMx = XMMatrixRotationX(skeleton.at(index).transform.rotation[0]);
//	XMMATRIX myRotYMx = XMMatrixRotationY(skeleton.at(index).transform.rotation[1]);
//	XMMATRIX myRotZMx = XMMatrixRotationZ(skeleton.at(index).transform.rotation[2]);
//	XMMATRIX myposMx = XMMatrixTranslation(skeleton.at(index).transform.position[0], skeleton.at(index).transform.position[1], skeleton.at(index).transform.position[2]);
//
//	XMMATRIX myRotMx = XMMatrixMultiply(myRotXMx, myRotYMx);
//	myRotMx = XMMatrixMultiply(myRotMx, myRotZMx);
//
//	XMMATRIX localMx = XMMatrixMultiply(myScaleMx, myRotMx);
//	localMx = XMMatrixMultiply(localMx, myposMx);
//
//	return localMx;
//}
//
////Calculation to be made per frame
//XMMATRIX CalcModelMx(const LspSkeletalMesh& mesh, LspBone& targetBone)
//{
//	XMMATRIX parentModelMx;
//	targetBone.localMx = CreateLocalMx(mesh, targetBone.myIndex);
//
//	if (targetBone.parent == -1)
//	{
//		parentModelMx = targetBone.localMx;
//	}
//	else
//	{
//		parentModelMx = mesh.bones.at(targetBone.parent).modelMx;
//	}
//
//	return XMMatrixMultiply(parentModelMx, targetBone.localMx);
//}
//
////vvvvv Do we even need this??? vvvvv
////Calculation to be made at startup
//XMMATRIX CalcBindModelMx(const XMMATRIX& parentBindModelMx, const XMMATRIX& bindLocalMx)
//{
//	return XMMatrixMultiply(parentBindModelMx, bindLocalMx);
//}
//
////Calculation to be made per frame
//XMMATRIX CalcFinalMx(const XMMATRIX& modelMx, const XMMATRIX& bindmodelInverseMx)
//{
//	XMMATRIX tmp = XMMatrixMultiply(modelMx, bindmodelInverseMx);
//	XMMATRIX rot = XMMatrixRotationZ(0.5 * XM_PI);
//	return XMMatrixMultiply(tmp, rot);
//}
//
////Convert from float[4][4] to XMMATRIX
//XMMATRIX ConvMx(const float a[4][4])
//{
//	XMMATRIX mx
//	{
//		a[0][0], a[1][0], a[2][0], a[3][0],
//		a[0][1], a[1][1], a[2][1], a[3][1],
//		a[0][2], a[1][2], a[2][2], a[3][2],
//		a[0][3], a[1][3], a[2][3], a[3][3]
//	};
//	return mx;
//}
//
//void CalcMxPerFrame(LspBone& bone, const LspSkeletalMesh& myMesh)
//{
//	bone.modelMx = CalcModelMx(myMesh, bone);
//	bone.finalMx = CalcFinalMx(bone.modelMx, bone.bindmodelInverseMx);
//}

void LoadScene(LspScene& myScene, std::string fileName)
{
	myScene.load(fileName);
}

//Recursive function to order joints, using depth-first traversion.
//void OrderSkeleton(std::vector<LspScene::Joint>& target, const LspScene::Joint& root, std::vector<LspScene::Joint*> source)
//{
//	target.push_back(root);
//	for (LspScene::Object* child : root.children)
//	{
//		//small problem, parents and children are Object pointers, not Joints.
//		//Therefore I have to compare them in a roundabout way. Not sure how well this will work later when animating...
//		for (LspScene::Joint* joint : source)
//		{
//			if (child->name == joint->name)
//			{
//				//Multiply bindposeinverse? is it already multiplied?
//				OrderSkeleton(target, *joint, source);
//				break;
//			}
//		}
//	}
//}
//
////Recursive function to order joints, using depth-first traversion. Also orders a bone vector connecting the joints.
//void OrderSkeleton(std::vector<LspScene::Joint>& targetSkeleton, std::vector<LspBone>& targetBones, const LspScene::Joint& root, std::vector<LspScene::Joint*> source, int index = 0)
//{
//	targetSkeleton.push_back(root);
//	if (root.parent == nullptr)
//	{
//		LspBone bone;
//		bone.parent = -1;
//		bone.myIndex = index;
//		bone.bindmodelInverseMx = ConvMx(root.bindPoseInverse);
//		bone.localMx = CreateLocalMx(targetSkeleton, bone.myIndex);
//		bone.modelMx = bone.localMx;
//		targetBones.push_back(bone);
//
//	}
//	else
//	{
//		LspBone bone;
//		bone.parent = -1;
//		bone.myIndex = index;
//		for (int i = 0; i < targetSkeleton.size() && bone.parent == -1; i++)
//		{
//			if (root.parent->name == targetSkeleton[i].name)
//			{
//				bone.parent = i;
//				break;
//			}
//		}
//		bone.bindmodelInverseMx = ConvMx(root.bindPoseInverse);
//		bone.localMx = CreateLocalMx(targetSkeleton, bone.myIndex);
//		targetBones.push_back(bone);
//	}
//
//	for (LspScene::Object* child : root.children)
//	{
//		for (LspScene::Joint* joint : source)
//		{
//			if (child->name == joint->name)
//			{
//				OrderSkeleton(targetSkeleton, targetBones, *joint, source, index + 1);
//				break;
//			}
//		}
//	}
//}
//
//void LoadSkeleton(std::string fileName, std::vector<LspScene::Joint>& target)
//{
//	LspScene myScene{ fileName };
//
//	for (LspScene::Joint* joint : myScene.joints)
//	{
//		if (joint->parent == nullptr)
//		{
//			OrderSkeleton(target, *joint, myScene.joints);
//			break;
//		}
//	}
//}
//
//void LoadSkeleton(const LspScene& myScene, std::vector<LspScene::Joint>& target)
//{
//	for (LspScene::Joint* joint : myScene.joints)
//	{
//		if (joint->parent == nullptr)
//		{
//			OrderSkeleton(target, *joint, myScene.joints);
//			break;
//		}
//	}
//}
//
//void LoadSkeleton(const LspScene& myScene, std::vector<LspScene::Joint>& targetSkeleton, std::vector<LspBone>& targetBones)
//{
//	for (LspScene::Joint* joint : myScene.joints)
//	{
//		if (joint->parent == nullptr)
//		{
//			OrderSkeleton(targetSkeleton, targetBones, *joint, myScene.joints);
//			break;
//		}
//	}
//}
//
//void LoadMesh(LspSkeletalMesh& target, int index, std::string meshName)
//{
//	bool meshLoaded = false;
//	bool materialLoaded = false;
//	bool meshIndex = index;
//
//	LspScene myScene{ target.sceneFileName };
//	if (myScene.meshes.size() > 0)
//	{
//		if (index > -1 && index < myScene.meshes.size())
//		{
//			target.mesh = *myScene.meshes[index];
//			meshLoaded = true;
//		}
//
//		if (!meshLoaded && meshName != "§")
//		{
//			for (int i = 0; i < myScene.meshes.size(); i++)
//			{
//				if (myScene.meshes[i]->name == meshName)
//				{
//					target.mesh = *myScene.meshes[i];
//					meshIndex = i;
//					meshLoaded = true;
//				}
//			}
//		}
//
//		else if (!meshLoaded && meshName == "§" && index == -1)
//		{
//			target.mesh = *myScene.meshes[0];
//			meshIndex = 0;
//			meshLoaded = true;
//		}
//	}
//
//	if (meshLoaded && target.skeleton.size() == 0)
//	{
//		LoadSkeleton(myScene, target.skeleton, target.bones);
//
//		for (LspScene::Joint& joint : target.skeleton)
//		{
//			for (int i = 0; i < target.mesh.weights.size(); i++)
//			{
//				if (target.mesh.weights[i].joint != nullptr && target.mesh.weights[i].joint->name == joint.name)
//				{
//					target.mesh.weights[i].joint = &joint;
//				}
//				if (target.mesh.weights[i].joint == nullptr)
//				{
//					target.mesh.weights[i].joint = &target.skeleton[0];
//				}
//			}
//		}
//
//	}
//
//	if (meshLoaded)
//	{
//		target.material = *myScene.meshes[meshIndex]->material;
//		materialLoaded = true;
//
//		target.worldTransform = myScene.root->transform;
//	}
//
//	if (!meshLoaded || !materialLoaded)
//	{
//		LspScene errorScene{ "../LspMeshes/ERROR" };
//
//		if (!meshLoaded)
//		{
//			for (int i = 0; i < errorScene.meshes.size(); i++)
//			{
//				if (errorScene.meshes[i]->name == "error")
//				{
//					target.mesh = *errorScene.meshes[i];
//				}
//			}
//			target.worldTransform = errorScene.root->transform;
//		}
//		if (!materialLoaded)
//		{
//			for (int i = 0; i < errorScene.meshes.size(); i++)
//			{
//				if (errorScene.meshes[i]->name == "error")
//				{
//					target.material = *errorScene.meshes[i]->material;
//				}
//			}
//		}
//
//	}
//
//	target.mesh.material = &target.material;
//	target.animTimeline.push_back(myScene.timelineInfo);
//
//}

void LoadMesh(LspMesh& target, int index, std::string meshName)
{
	bool meshLoaded = false;
	bool materialLoaded = false;
	bool meshIndex = index;

	LspScene myScene{ target.sceneFileName };
	if (myScene.meshes.size() > 0)
	{
		if (index > -1 && index < myScene.meshes.size())
		{
			target.mesh = *myScene.meshes[index];
			meshLoaded = true;
		}

		if (!meshLoaded && meshName != "§")
		{
			for (int i = 0; i < myScene.meshes.size(); i++)
			{
				if (myScene.meshes[i]->name == meshName)
				{
					target.mesh = *myScene.meshes[i];
					meshIndex = i;
					meshLoaded = true;
				}
			}
		}

		else if (!meshLoaded && meshName == "§" && index == -1)
		{
			target.mesh = *myScene.meshes[0];
			meshIndex = 0;
			meshLoaded = true;
		}
	}

	if (meshLoaded)
	{
		target.material = *myScene.meshes[meshIndex]->material;
		materialLoaded = true;

		target.worldTransform = myScene.root->transform;
	}

	if (!meshLoaded || !materialLoaded)
	{
		LspScene errorScene{ "../LspMeshes/ERROR" };

		if (!meshLoaded)
		{
			for (int i = 0; i < errorScene.meshes.size(); i++)
			{
				if (errorScene.meshes[i]->name == "error")
				{
					target.mesh = *errorScene.meshes[i];
				}
			}
			target.worldTransform = errorScene.root->transform;
		}
		if (!materialLoaded)
		{
			for (int i = 0; i < errorScene.meshes.size(); i++)
			{
				if (errorScene.meshes[i]->name == "error")
				{
					target.material = *errorScene.meshes[i]->material;
				}
			}
		}

	}

	target.mesh.material = &target.material;
}

//bool LoadAnimation(LspSkeletalMesh& targetMesh, std::string fileName)
//{
//	std::vector<LspScene::Joint> otherSkeleton;
//	LspScene myScene(fileName);
//	LoadSkeleton(myScene, otherSkeleton);
//	targetMesh.animTimeline.push_back(myScene.timelineInfo);
//
//	if (otherSkeleton.size() == targetMesh.skeleton.size())
//	{
//		for (int i = 0; i < otherSkeleton.size(); i++)
//		{
//			LspScene::AnimationList animList;
//			if (otherSkeleton[i].animations.size() > 0)
//			{
//				for (int j = 0; j < otherSkeleton[i].animations[0].keys.size(); j++)
//				{
//					LspScene::AnimationKey key = otherSkeleton[i].animations[0].keys[j];
//					animList.keys.push_back(key);
//				}
//				targetMesh.skeleton[i].animations.push_back(animList);
//			}
//		}
//
//		return true;
//	}
//
//	return false;
//}

const std::vector<LspScene::Camera*> LoadAllCameras(LspScene& scene)
{
	std::vector<LspScene::Camera*> allCameras;
	for (LspScene::Camera* cam : scene.cameras)
	{
		allCameras.push_back(cam);
	}
	return allCameras;
}

const std::vector <LspScene::AnimationList> GetCameraAnimations(LspScene& scene, int cameraIndex)
{
	return scene.cameras.at(cameraIndex)->animations;
}

const LspScene::Camera GetCamera(LspScene& scene, int cameraIndex)
{
	return *scene.cameras.at(cameraIndex);
}

const std::vector<LspScene::Light*> LoadAllLights(LspScene& scene)
{
	std::vector<LspScene::Light*> allLights;
	for (LspScene::Light* light : scene.lights)
	{
		allLights.push_back(light);
	}
	return allLights;
}

const LspScene::Light* GetLight(LspScene& scene, int lightIndex)
{
	return scene.lights.at(lightIndex);
}

const std::vector<std::string> GetNamesObjectByType(LspScene& scene, LspScene::Object::Type objectType)
{
	std::vector<std::string> names;

	// Mesh, Joint, Light, Camera
	if (objectType == LspScene::Object::Type::Mesh)
	{
		for (int i = 0; i < scene.meshes.size(); i++)
		{
			names.push_back(scene.meshes.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Joint)
	{
		for (int i = 0; i < scene.joints.size(); i++)
		{
			names.push_back(scene.joints.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Light)
	{
		for (int i = 0; i < scene.lights.size(); i++)
		{
			names.push_back(scene.lights.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Camera)
	{
		for (int i = 0; i < scene.cameras.size(); i++)
		{
			names.push_back(scene.cameras.at(i)->name);
		}
	}
	return names;
}

const int GetNrOfObjectByType(LspScene& scene, LspScene::Object::Type objectType)
{
	int nrOf = 0;

	// Mesh, Joint, Light, Camera
	if (objectType == LspScene::Object::Type::Mesh)
	{
		nrOf = scene.meshes.size();
	}
	else if (objectType == LspScene::Object::Type::Joint)
	{
		nrOf = scene.joints.size();
	}
	else if (objectType == LspScene::Object::Type::Light)
	{
		nrOf = scene.lights.size();

	}
	else if (objectType == LspScene::Object::Type::Camera)
	{
		nrOf = scene.cameras.size();
	}
	return nrOf;
}

const std::vector<std::string> GetNamesOfChildrenPerObject(LspScene& scene, LspScene::Object::Type objectType, int objectIndex)
{
	std::vector<std::string> names;

	// Mesh, Joint, Light, Camera
	if (objectType == LspScene::Object::Type::Mesh)
	{
		for (int i = 0; i < scene.meshes.at(objectIndex)->children.size(); i++)
		{
			names.push_back(scene.meshes.at(objectIndex)->children.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Joint)
	{
		for (int i = 0; i < scene.joints.at(objectIndex)->children.size(); i++)
		{
			names.push_back(scene.joints.at(objectIndex)->children.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Light)
	{
		for (int i = 0; i < scene.lights.at(objectIndex)->children.size(); i++)
		{
			names.push_back(scene.lights.at(objectIndex)->children.at(i)->name);
		}
	}
	else if (objectType == LspScene::Object::Type::Camera)
	{
		for (int i = 0; i < scene.cameras.at(objectIndex)->children.size(); i++)
		{
			names.push_back(scene.cameras.at(objectIndex)->children.at(i)->name);
		}
	}
	return names;
}

const std::vector<std::string> GetNamesOfChildrenPerObject(LspScene& scene, LspScene::Object::Type objectType, std::string objectName)
{
	std::vector<std::string> names;

	// Mesh, Joint, Light, Camera
	if (objectType == LspScene::Object::Type::Mesh)
	{
		for (int j = 0; j < scene.meshes.size(); j++)
		{
			if (scene.meshes.at(j)->name == objectName)
			{
				for (int i = 0; i < scene.meshes.at(j)->children.size(); i++)
				{
					names.push_back(scene.meshes.at(j)->children.at(i)->name);
				}
			}
		}
	}
	else if (objectType == LspScene::Object::Type::Joint)
	{
		for (int j = 0; j < scene.joints.size(); j++)
		{
			if (scene.joints.at(j)->name == objectName)
			{
				for (int i = 0; i < scene.joints.at(j)->children.size(); i++)
				{
					names.push_back(scene.joints.at(j)->children.at(i)->name);
				}
			}
		}
	}
	else if (objectType == LspScene::Object::Type::Light)
	{
		for (int j = 0; j < scene.lights.size(); j++)
		{
			if (scene.lights.at(j)->name == objectName)
			{
				for (int i = 0; i < scene.lights.at(j)->children.size(); i++)
				{
					names.push_back(scene.lights.at(j)->children.at(i)->name);
				}
			}
		}
	}
	else if (objectType == LspScene::Object::Type::Camera)
	{
		for (int j = 0; j < scene.cameras.size(); j++)
		{
			if (scene.cameras.at(j)->name == objectName)
			{
				for (int i = 0; i < scene.cameras.at(j)->children.size(); i++)
				{
					names.push_back(scene.cameras.at(j)->children.at(i)->name);
				}
			}
		}
	}
	return names;
}

void printLspInfo(std::string filename)
{
	LspScene myScene{ filename };

	std::ofstream print;
	print.open(filename + ".txt", std::fstream::out | std::fstream::trunc);
	if (print.is_open())
	{
		print << "Opened: " << filename << std::endl;

		print << std::endl << "Meshes:" << std::endl;
		if (myScene.meshes.size() > 0)
		{
			for (int i = 0; i < myScene.meshes.size(); i++)
			{
				print << "	Mesh " << i << ":" << std::endl;
				print << "		name: " << myScene.meshes[i]->name << std::endl;
				int id = (int)myScene.meshes[i]->type;
				print << "		object type: " << id << std::endl;
				print << "		position: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->transform.position[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		rotation: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->transform.rotation[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		scale: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->transform.scale[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		custom attributes:" << std::endl;
				bool empty = true;
				if (myScene.meshes[i]->customAttrs.strings.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.strings.size(); k++)
					{
						print << "			string " << k << ": n: " << myScene.meshes[i]->customAttrs.strings[k].name << " v: " << myScene.meshes[i]->customAttrs.strings[k].value << std::endl;
					}
				}
				if (myScene.meshes[i]->customAttrs.ints.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.ints.size(); k++)
					{
						print << "			int " << k << ": n: " << myScene.meshes[i]->customAttrs.ints[k].name << " v: " << myScene.meshes[i]->customAttrs.ints[k].value << std::endl;
					}
				}
				if (myScene.meshes[i]->customAttrs.bools.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.bools.size(); k++)
					{
						print << "			bool " << k << ": n: " << myScene.meshes[i]->customAttrs.bools[k].name << " v: " << myScene.meshes[i]->customAttrs.bools[k].value << std::endl;
					}
				}
				if (myScene.meshes[i]->customAttrs.enums.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.enums.size(); k++)
					{
						print << "			enum " << k << ": n: " << myScene.meshes[i]->customAttrs.enums[k].name << " v: " << myScene.meshes[i]->customAttrs.enums[k].value << std::endl;
					}
				}
				if (myScene.meshes[i]->customAttrs.floats.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.floats.size(); k++)
					{
						print << "			float " << k << ": n: " << myScene.meshes[i]->customAttrs.floats[k].name << " v: " << myScene.meshes[i]->customAttrs.floats[k].value << std::endl;
					}
				}
				if (myScene.meshes[i]->customAttrs.vectors.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.meshes[i]->customAttrs.vectors.size(); k++)
					{
						print << "			vector " << k << ": n: " << myScene.meshes[i]->customAttrs.vectors[k].name << " v: " << myScene.meshes[i]->customAttrs.vectors[k].value[0] << ", " << myScene.meshes[i]->customAttrs.vectors[k].value[1] << ", " << myScene.meshes[i]->customAttrs.vectors[k].value[2] << std::endl;
					}
				}
				if (empty)
				{
					print << "			--none--" << std::endl;
				}
				print << "		animations" << std::endl;
				if (myScene.meshes[i]->animations.size() > 0)
				{
					for (int k = 0; k < myScene.meshes[i]->animations.size(); k++)
					{
						print << "			anim " << k << ":" << std::endl;
						for (int m = 0; m < myScene.meshes[i]->animations[k].keys.size(); m++)
						{
							print << "				key " << m << ": n: " << myScene.meshes[i]->animations[k].keys[m].name << " nr: " << myScene.meshes[i]->animations[k].keys[m].key << " tr: p: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.meshes[i]->animations[k].keys[m].data.position[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " r: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.meshes[i]->animations[k].keys[m].data.rotation[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " s: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.meshes[i]->animations[k].keys[m].data.scale[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		(parent and children not relevant in print)" << std::endl;
				print << "		material:" << std::endl;
				print << "			n: " << myScene.meshes[i]->material->name << " tx: " << myScene.meshes[i]->material->texureFileName << std::endl;
				print << "			a: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->material->ambient[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl << "			d: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->material->diffuse[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl << "			s: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->material->specular[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl << "			e: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.meshes[i]->material->emissive[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "			o: " << myScene.meshes[i]->material->opacity << " r: " << myScene.meshes[i]->material->reflectivity << " sp: " << myScene.meshes[i]->material->specularPower << std::endl;
				print << "		vertices:" << std::endl;
				if (myScene.meshes[i]->vertices.size() > 0)
				{
					for (int k = 0; k < myScene.meshes[i]->vertices.size(); k++)
					{
						print << "			vertex " << k << ": p: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertices[k].position[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << " n: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertices[k].normal[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << " uv: ";
						for (int m = 0; m < 2; m++)
						{
							print << myScene.meshes[i]->vertices[k].uv[m];
							if (m != 1)
							{
								print << ", ";
							}
						}
						print << " bn: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertices[k].biNormal[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << " t: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertices[k].tangent[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << std::endl;
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		blendshapes:" << std::endl;
				if (myScene.meshes[i]->blendShapes.size() > 0)
				{
					for (int k = 0; k < myScene.meshes[i]->blendShapes.size(); k++)
					{
						print << "			blend " << k << ": " << std::endl;
						print << "			(source not relevant in print)" << std::endl;
						for (int m = 0; m < myScene.meshes[i]->blendShapes[k].data.size(); m++)
						{
							print << "				data " << m << ": f: " << myScene.meshes[i]->blendShapes[k].data[m].frame << " v: " << myScene.meshes[i]->blendShapes[k].data[m].value << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		vertex animations:" << std::endl;
				if (myScene.meshes[i]->vertexAnimations.size() > 0)
				{
					for (int k = 0; k < myScene.meshes[i]->vertexAnimations.size(); k++)
					{
						print << "			key " << k << ": (meshname not relevant for print) k: " << myScene.meshes[i]->vertexAnimations[k].key << " p: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertexAnimations[k].data.position[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << " r: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertexAnimations[k].data.rotation[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
						print << " s: ";
						for (int m = 0; m < 3; m++)
						{
							print << myScene.meshes[i]->vertexAnimations[k].data.scale[m];
							if (m != 2)
							{
								print << ", ";
							}
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		weights:" << std::endl;
				if (myScene.meshes[i]->weights.size() > 0)
				{
					for (int k = 0; k < myScene.meshes[i]->weights.size(); k++)
					{
						if (myScene.meshes[i]->weights[k].joint != nullptr)
						{
							print << "			w " << k << " j: " << myScene.meshes[i]->weights[k].joint->name << " i: " << myScene.meshes[i]->weights[k].vertexIndex << " v: " << myScene.meshes[i]->weights[k].value << std::endl;
						}
						else
						{
							print << "			w " << k << " j: (has no joint) i: " << myScene.meshes[i]->weights[k].vertexIndex << " v: " << myScene.meshes[i]->weights[k].value << std::endl;
						}
						
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Shapes:" << std::endl;
		if (myScene.shapes.size() > 0)
		{
			for (int i = 0; i < myScene.shapes.size(); i++)
			{
				print << "	Shape " << i << ":" << std::endl;
				print << "		name: " << myScene.shapes[i]->name << std::endl;
				print << "		points:" << std::endl;
				for (int k = 0; k < myScene.shapes[i]->data.size(); k++)
				{
					print << "			point " << k << ": p: ";
					for (int m = 0; m < 3; m++)
					{
						print << myScene.shapes[i]->data[k].position[m];
						if (m != 2)
						{
							print << ", ";
						}
					}
					print << std::endl;
				}
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Joints:" << std::endl;
		if (myScene.joints.size() > 0)
		{
			for (int i = 0; i < myScene.joints.size(); i++)
			{
				print << "	Joint " << i << ":" << std::endl;
				print << "		name: " << myScene.joints[i]->name << std::endl;
				int id = (int)myScene.joints[i]->type;
				print << "		object type: " << id << std::endl;
				print << "		position: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.joints[i]->transform.position[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		rotation: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.joints[i]->transform.rotation[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		scale: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.joints[i]->transform.scale[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		custom attributes:" << std::endl;
				bool empty = true;
				if (myScene.joints[i]->customAttrs.strings.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.strings.size(); k++)
					{
						print << "			string " << k << ": n: " << myScene.joints[i]->customAttrs.strings[k].name << " v: " << myScene.joints[i]->customAttrs.strings[k].value << std::endl;
					}
				}
				if (myScene.joints[i]->customAttrs.ints.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.ints.size(); k++)
					{
						print << "			int " << k << ": n: " << myScene.joints[i]->customAttrs.ints[k].name << " v: " << myScene.joints[i]->customAttrs.ints[k].value << std::endl;
					}
				}
				if (myScene.joints[i]->customAttrs.bools.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.bools.size(); k++)
					{
						print << "			bool " << k << ": n: " << myScene.joints[i]->customAttrs.bools[k].name << " v: " << myScene.joints[i]->customAttrs.bools[k].value << std::endl;
					}
				}
				if (myScene.joints[i]->customAttrs.enums.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.enums.size(); k++)
					{
						print << "			enum " << k << ": n: " << myScene.joints[i]->customAttrs.enums[k].name << " v: " << myScene.joints[i]->customAttrs.enums[k].value << std::endl;
					}
				}
				if (myScene.joints[i]->customAttrs.floats.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.floats.size(); k++)
					{
						print << "			float " << k << ": n: " << myScene.joints[i]->customAttrs.floats[k].name << " v: " << myScene.joints[i]->customAttrs.floats[k].value << std::endl;
					}
				}
				if (myScene.joints[i]->customAttrs.vectors.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.joints[i]->customAttrs.vectors.size(); k++)
					{
						print << "			vector " << k << ": n: " << myScene.joints[i]->customAttrs.vectors[k].name << " v: " << myScene.joints[i]->customAttrs.vectors[k].value[0] << ", " << myScene.joints[i]->customAttrs.vectors[k].value[1] << ", " << myScene.meshes[i]->customAttrs.vectors[k].value[2] << std::endl;
					}
				}
				if (empty)
				{
					print << "			--none--" << std::endl;
				}
				print << "		animations" << std::endl;
				if (myScene.joints[i]->animations.size() > 0)
				{
					for (int k = 0; k < myScene.joints[i]->animations.size(); k++)
					{
						print << "			anim " << k << ":" << std::endl;
						for (int m = 0; m < myScene.joints[i]->animations[k].keys.size(); m++)
						{
							print << "				key " << m << ": n: " << myScene.joints[i]->animations[k].keys[m].name << " nr: " << myScene.joints[i]->animations[k].keys[m].key << " tr: p: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.joints[i]->animations[k].keys[m].data.position[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " r: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.joints[i]->animations[k].keys[m].data.rotation[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " s: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.joints[i]->animations[k].keys[m].data.scale[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		(parent and children not relevant in print)" << std::endl;

				print << "		bind pose inverse:" << std::endl;
				print << "			";
				for (int k = 0; k < 4; k++)
				{
					for (int m = 0; m < 4; m++)
					{
						print << myScene.joints[i]->bindPoseInverse[m][k];
						if (m != 3)
						{
							print << " ";
						}
						else
						{
							print << std::endl;
							if (k != 3)
							{
								print << "			";
							}
						}
					}
				}
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Lights:" << std::endl;
		if (myScene.lights.size() > 0)
		{
			for (int i = 0; i < myScene.lights.size(); i++)
			{
				print << "	Light " << i << ":" << std::endl;
				print << "		name: " << myScene.lights[i]->name << std::endl;
				int id = (int)myScene.lights[i]->type;
				print << "		object type: " << id << std::endl;
				print << "		position: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.lights[i]->transform.position[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		rotation: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.lights[i]->transform.rotation[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		scale: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.lights[i]->transform.scale[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		custom attributes:" << std::endl;
				bool empty = true;
				if (myScene.lights[i]->customAttrs.strings.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.strings.size(); k++)
					{
						print << "			string " << k << ": n: " << myScene.lights[i]->customAttrs.strings[k].name << " v: " << myScene.lights[i]->customAttrs.strings[k].value << std::endl;
					}
				}
				if (myScene.lights[i]->customAttrs.ints.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.ints.size(); k++)
					{
						print << "			int " << k << ": n: " << myScene.lights[i]->customAttrs.ints[k].name << " v: " << myScene.lights[i]->customAttrs.ints[k].value << std::endl;
					}
				}
				if (myScene.lights[i]->customAttrs.bools.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.bools.size(); k++)
					{
						print << "			bool " << k << ": n: " << myScene.lights[i]->customAttrs.bools[k].name << " v: " << myScene.lights[i]->customAttrs.bools[k].value << std::endl;
					}
				}
				if (myScene.lights[i]->customAttrs.enums.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.enums.size(); k++)
					{
						print << "			enum " << k << ": n: " << myScene.lights[i]->customAttrs.enums[k].name << " v: " << myScene.lights[i]->customAttrs.enums[k].value << std::endl;
					}
				}
				if (myScene.lights[i]->customAttrs.floats.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.floats.size(); k++)
					{
						print << "			float " << k << ": n: " << myScene.lights[i]->customAttrs.floats[k].name << " v: " << myScene.lights[i]->customAttrs.floats[k].value << std::endl;
					}
				}
				if (myScene.lights[i]->customAttrs.vectors.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.lights[i]->customAttrs.vectors.size(); k++)
					{
						print << "			vector " << k << ": n: " << myScene.lights[i]->customAttrs.vectors[k].name << " v: " << myScene.lights[i]->customAttrs.vectors[k].value[0] << ", " << myScene.lights[i]->customAttrs.vectors[k].value[1] << ", " << myScene.lights[i]->customAttrs.vectors[k].value[2] << std::endl;
					}
				}
				if (empty)
				{
					print << "			--none--" << std::endl;
				}
				print << "		animations" << std::endl;
				if (myScene.lights[i]->animations.size() > 0)
				{
					for (int k = 0; k < myScene.lights[i]->animations.size(); k++)
					{
						print << "			anim " << k << ":" << std::endl;
						for (int m = 0; m < myScene.lights[i]->animations[k].keys.size(); m++)
						{
							print << "				key " << m << ": n: " << myScene.lights[i]->animations[k].keys[m].name << " nr: " << myScene.lights[i]->animations[k].keys[m].key << " tr: p: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.lights[i]->animations[k].keys[m].data.position[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " r: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.lights[i]->animations[k].keys[m].data.rotation[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " s: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.lights[i]->animations[k].keys[m].data.scale[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		(parent and children not relevant in print)" << std::endl;
				id = (int)myScene.lights[i]->lightType;
				print << "		light type: " << id << std::endl;
				print << "		direction: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.lights[i]->dir[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		color: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.lights[i]->color[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		intensity: " << myScene.lights[i]->intensity << std::endl;
				print << "		id: " << myScene.lights[i]->id << std::endl;
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Cameras:" << std::endl;
		if (myScene.cameras.size() > 0)
		{
			for (int i = 0; i < myScene.cameras.size(); i++)
			{
				print << "	Camera" << i << ":" << std::endl;
				print << "		name: " << myScene.cameras[i]->name << std::endl;
				int id = (int)myScene.cameras[i]->type;
				print << "		object type: " << id << std::endl;
				print << "		position: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.cameras[i]->transform.position[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		rotation: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.cameras[i]->transform.rotation[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		scale: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.cameras[i]->transform.scale[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		custom attributes:" << std::endl;
				bool empty = true;
				if (myScene.cameras[i]->customAttrs.strings.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.strings.size(); k++)
					{
						print << "			string " << k << ": n: " << myScene.cameras[i]->customAttrs.strings[k].name << " v: " << myScene.cameras[i]->customAttrs.strings[k].value << std::endl;
					}
				}
				if (myScene.cameras[i]->customAttrs.ints.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.ints.size(); k++)
					{
						print << "			int " << k << ": n: " << myScene.cameras[i]->customAttrs.ints[k].name << " v: " << myScene.cameras[i]->customAttrs.ints[k].value << std::endl;
					}
				}
				if (myScene.cameras[i]->customAttrs.bools.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.bools.size(); k++)
					{
						print << "			bool " << k << ": n: " << myScene.cameras[i]->customAttrs.bools[k].name << " v: " << myScene.cameras[i]->customAttrs.bools[k].value << std::endl;
					}
				}
				if (myScene.cameras[i]->customAttrs.enums.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.enums.size(); k++)
					{
						print << "			enum " << k << ": n: " << myScene.cameras[i]->customAttrs.enums[k].name << " v: " << myScene.cameras[i]->customAttrs.enums[k].value << std::endl;
					}
				}
				if (myScene.cameras[i]->customAttrs.floats.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.floats.size(); k++)
					{
						print << "			float " << k << ": n: " << myScene.cameras[i]->customAttrs.floats[k].name << " v: " << myScene.cameras[i]->customAttrs.floats[k].value << std::endl;
					}
				}
				if (myScene.cameras[i]->customAttrs.vectors.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.cameras[i]->customAttrs.vectors.size(); k++)
					{
						print << "			vector " << k << ": n: " << myScene.cameras[i]->customAttrs.vectors[k].name << " v: " << myScene.cameras[i]->customAttrs.vectors[k].value[0] << ", " << myScene.cameras[i]->customAttrs.vectors[k].value[1] << ", " << myScene.cameras[i]->customAttrs.vectors[k].value[2] << std::endl;
					}
				}
				if (empty)
				{
					print << "			--none--" << std::endl;
				}
				print << "		animations" << std::endl;
				if (myScene.cameras[i]->animations.size() > 0)
				{
					for (int k = 0; k < myScene.cameras[i]->animations.size(); k++)
					{
						print << "			anim " << k << ":" << std::endl;
						for (int m = 0; m < myScene.cameras[i]->animations[k].keys.size(); m++)
						{
							print << "				key " << m << ": n: " << myScene.cameras[i]->animations[k].keys[m].name << " nr: " << myScene.cameras[i]->animations[k].keys[m].key << " tr: p: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.cameras[i]->animations[k].keys[m].data.position[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " r: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.cameras[i]->animations[k].keys[m].data.rotation[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " s: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.cameras[i]->animations[k].keys[m].data.scale[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		(parent and children not relevant in print)" << std::endl;
				print << "		up vector: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.cameras[i]->up[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		look at: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.cameras[i]->lookAt[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		id: " << myScene.cameras[i]->id << std::endl;
				print << "		fov: " << myScene.cameras[i]->fov << std::endl;
				print << "		near distance: " << myScene.cameras[i]->nearDistance << std::endl;
				print << "		far distance: " << myScene.cameras[i]->farDistance << std::endl;
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Materials:" << std::endl;
		if (myScene.materials.size() > 0)
		{
			for (int i = 0; i < myScene.materials.size(); i++)
			{
				print << "	Material " << i << ":" << std::endl;
				print << "		name: " << myScene.materials[i]->name << std::endl;
				print << "		texture: " << myScene.materials[i]->texureFileName << std::endl;
				print << "			a: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.materials[i]->ambient[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "			d: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.materials[i]->diffuse[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "			s: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.materials[i]->specular[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "			e: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.materials[i]->emissive[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "			o: " << myScene.materials[i]->opacity << std::endl;
				print << "			r: " << myScene.materials[i]->reflectivity << std::endl;
				print << "			sp: " << myScene.materials[i]->specularPower << std::endl;
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}
		print << std::endl << "Groups:" << std::endl;
		if (myScene.groups.size() > 0)
		{
			for (int i = 0; i < myScene.groups.size(); i++)
			{
				print << "	Group " << i << ":" << std::endl;
				print << "		name: " << myScene.groups[i]->name << std::endl;
				int id = (int)myScene.groups[i]->type;
				print << "		object type: " << id << std::endl;
				print << "		position: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.groups[i]->transform.position[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		rotation: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.groups[i]->transform.rotation[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		scale: ";
				for (int k = 0; k < 3; k++)
				{
					print << myScene.groups[i]->transform.scale[k];
					if (k != 2)
					{
						print << ", ";
					}
				}
				print << std::endl;
				print << "		custom attributes:" << std::endl;
				bool empty = true;
				if (myScene.groups[i]->customAttrs.strings.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.strings.size(); k++)
					{
						print << "			string " << k << ": n: " << myScene.groups[i]->customAttrs.strings[k].name << " v: " << myScene.groups[i]->customAttrs.strings[k].value << std::endl;
					}
				}
				if (myScene.groups[i]->customAttrs.ints.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.ints.size(); k++)
					{
						print << "			int " << k << ": n: " << myScene.groups[i]->customAttrs.ints[k].name << " v: " << myScene.groups[i]->customAttrs.ints[k].value << std::endl;
					}
				}
				if (myScene.groups[i]->customAttrs.bools.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.bools.size(); k++)
					{
						print << "			bool " << k << ": n: " << myScene.groups[i]->customAttrs.bools[k].name << " v: " << myScene.groups[i]->customAttrs.bools[k].value << std::endl;
					}
				}
				if (myScene.groups[i]->customAttrs.enums.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.enums.size(); k++)
					{
						print << "			enum " << k << ": n: " << myScene.groups[i]->customAttrs.enums[k].name << " v: " << myScene.groups[i]->customAttrs.enums[k].value << std::endl;
					}
				}
				if (myScene.groups[i]->customAttrs.floats.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.floats.size(); k++)
					{
						print << "			float " << k << ": n: " << myScene.groups[i]->customAttrs.floats[k].name << " v: " << myScene.groups[i]->customAttrs.floats[k].value << std::endl;
					}
				}
				if (myScene.groups[i]->customAttrs.vectors.size() > 0)
				{
					empty = false;
					for (int k = 0; k < myScene.groups[i]->customAttrs.vectors.size(); k++)
					{
						print << "			vector " << k << ": n: " << myScene.groups[i]->customAttrs.vectors[k].name << " v: " << myScene.groups[i]->customAttrs.vectors[k].value[0] << ", " << myScene.groups[i]->customAttrs.vectors[k].value[1] << ", " << myScene.groups[i]->customAttrs.vectors[k].value[2] << std::endl;
					}
				}
				if (empty)
				{
					print << "			--none--" << std::endl;
				}
				print << "		animations" << std::endl;
				if (myScene.groups[i]->animations.size() > 0)
				{
					for (int k = 0; k < myScene.groups[i]->animations.size(); k++)
					{
						print << "			anim " << k << ":" << std::endl;
						for (int m = 0; m < myScene.groups[i]->animations[k].keys.size(); m++)
						{
							print << "				key " << m << ": n: " << myScene.groups[i]->animations[k].keys[m].name << " nr: " << myScene.groups[i]->animations[k].keys[m].key << " tr: p: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.groups[i]->animations[k].keys[m].data.position[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " r: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.groups[i]->animations[k].keys[m].data.rotation[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << " s: ";
							for (int o = 0; o < 3; o++)
							{
								print << myScene.groups[i]->animations[k].keys[m].data.scale[o];
								if (o != 2)
								{
									print << ", ";
								}
							}
							print << std::endl;
						}
					}
				}
				else
				{
					print << "			--none--" << std::endl;
				}
				print << "		(parent and children not relevant in print)" << std::endl;
			}
		}
		else
		{
			print << "	--none--" << std::endl;
		}

		print << std::endl << "Timeline Info:" << std::endl;
		print << "	duration: " << myScene.timelineInfo.duration << std::endl;
		print << "	framerate: " << myScene.timelineInfo.framerate << std::endl;

		print.close();
	}

}