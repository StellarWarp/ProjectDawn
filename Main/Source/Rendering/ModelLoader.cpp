#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelLoader.h"
#include "Libraries/OpenGL.h"
#include "Libraries/Libs.h"

#include "Utility/Utility.h"

GameObject* ModelLoader::LoadModelFromFile(const std::wstring_view path) {
	using namespace Assimp;
	Importer importer;
	// 去掉里面的点、线图元
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, 
		aiPrimitiveType_LINE | 
		aiPrimitiveType_POINT);
	const aiScene* scene = importer.ReadFile(Utility::WStringToUTF8(path),
		aiProcess_Triangulate |// 将多边形拆分
		aiProcess_ImproveCacheLocality |    // 改善缓存局部性
		aiProcess_GenNormals |    // 生成平滑法线
		aiProcess_FlipUVs); // 翻转UV
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || !scene->HasMeshes()) {
		std::cerr << "Error: " << importer.GetErrorString();
		return nullptr;
	} 

	aiNode* rootNode = scene->mRootNode;
	GameObject* rootObject{};

	for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* pAiMesh = scene->mMeshes[i];

		std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();
		if (scene->mNumMeshes == 1)
		{
			rootObject = new SceneObject();
			dynamic_cast<SceneObject*>(rootObject)->renderer.meshData = meshData;
		}
		else
		{
			rootObject = new GameObject();
			//create sub-object for each mesh
			std::unique_ptr<SceneObject> child = std::make_unique<SceneObject>();
			child->renderer.meshData = meshData;
			rootObject->AddChild(std::move(child));
		}

		auto AiVector2Attribute = [&](const auto& aiVec, auto& attribute) {
			attribute.resize(pAiMesh->mNumVertices);
			size_t size = attribute.byte_size();
			memcpy_s(attribute, size,
				aiVec, size);
		};
		//position
		if (pAiMesh->mNumVertices > 0) {
			AiVector2Attribute(pAiMesh->mVertices,
				meshData->vertices.position);
		}

		//normal
		if (pAiMesh->HasNormals()) {
			AiVector2Attribute(pAiMesh->mNormals,
				meshData->vertices.normal);
		}

		//tangent
		if (pAiMesh->HasTangentsAndBitangents()) {
			AiVector2Attribute(pAiMesh->mTangents,
				meshData->vertices.tangent);
			AiVector2Attribute(pAiMesh->mBitangents,
				meshData->vertices.bitangent);

		}

		//uv
		if (pAiMesh->HasTextureCoords(0)) {
			AiVector2Attribute(pAiMesh->mTextureCoords[0],
				meshData->vertices.texCoords);
		}
		//indices
		meshData->indices.reserve(pAiMesh->mNumFaces * 3);
		for (uint32_t i = 0; i < pAiMesh->mNumFaces; i++)
		{
			aiFace face = pAiMesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				meshData->indices.push_back(face.mIndices[j]);
		}

		meshData->LoadToGPU();

	}

	return rootObject;
}

//auto LoadMesh = [&scene](aiNode* node, GameObject* gameObject) {
//	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
//		aiMesh* pAiMesh = scene->mMeshes[node->mMeshes[i]];
//
//		std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();
//		if (node->mNumMeshes == 1)
//		{
//			gameObject->renderer.meshData = meshData;
//		}
//		else
//		{
//			//create sub-object for each mesh
//			std::unique_ptr<SceneObject> child = std::make_unique<SceneObject>();
//			child->renderer.meshData = meshData;
//			gameObject->AddChild(std::move(child));
//		}
//
//		auto AiVector2Attribute = [&](const auto& aiVec, auto& attribute) {
//			attribute.resize(pAiMesh->mNumVertices);
//			size_t size = attribute.size();
//			memcpy_s(attribute, size,
//				aiVec, size);
//		};
//		//position
//		if (pAiMesh->mNumVertices > 0) {
//			AiVector2Attribute(pAiMesh->mVertices,
//				meshData->vertices.position);
//		}
//
//		//normal
//		if (pAiMesh->HasNormals()) {
//			AiVector2Attribute(pAiMesh->mNormals,
//				meshData->vertices.normal);
//		}
//
//		//tangent
//		if (pAiMesh->HasTangentsAndBitangents()) {
//			AiVector2Attribute(pAiMesh->mTangents,
//				meshData->vertices.tangent);
//			AiVector2Attribute(pAiMesh->mBitangents,
//				meshData->vertices.bitangent);
//
//		}
//
//		//uv
//		if (pAiMesh->HasTextureCoords(0)) {
//			AiVector2Attribute(pAiMesh->mTextureCoords[0],
//				meshData->vertices.texCoords);
//		}
//
//		//indices
//		meshData->indices.resize(pAiMesh->mNumFaces * 3);
//		size_t size = meshData->indices.size();
//		memcpy_s(meshData->indices.data(), size,
//			pAiMesh->mFaces, size);
//
//		meshData->LoadToGPU();
//
//	}
//};
//auto ProcessNode = [&](aiNode* node, GameObject* gameObject) {
//
//};

