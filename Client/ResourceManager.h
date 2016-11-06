#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_



#include "stdafx.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"



class CResource{
public:
	CResource() : m_MeshId(-1), m_TextureId(-1), m_MaterialId(-1), m_ShaderId(-1) {}
	~CResource() {}

	/*	========================================================================
	*	해당 리소스가 어떤 부품?을 사용할 지 저장한다
	*	\remarks 사용하지 않는다면 -1		*/
	void SetIDs(BYTE meshID, BYTE textureID, BYTE materialID, BYTE shaderID);

	BYTE GetMeshID() { return m_MeshId; }
	BYTE GetTextureID() { return m_TextureId; }
	BYTE GetMaterialID() { return m_MaterialId; }
	BYTE GetShaderID() { return m_ShaderId; }

private:
	BYTE m_MeshId;
	BYTE m_TextureId;
	BYTE m_MaterialId;
	BYTE m_ShaderId;
};



enum class eResourceType : BYTE{
	START = 0,
	User = 0,
	Monster1,
	Item_HP,
	Item_Buff,
	Floor,
	Tree,
	Wall1,
	grass,
	END
};



class CResourceManager
{
public:
	enum class eShaderType : BYTE{
		START = 0,
		IlluminatedTextured = 0,
		Player,
		END
	};

	static CResourceManager* GetSingleton();
	~CResourceManager();

	CMesh* GetMesh(eResourceType type);
	CTexture* GetTexture(eResourceType type);
	CMaterial* GetMaterial(eResourceType type);
	CShader* GetShaderByResourceType(eResourceType type);
	CShader* GetShaderByShaderType(eShaderType type);

	bool IsTextureEnable(eResourceType type);
	bool IsMaterialEnable(eResourceType type);

private:
	CResourceManager();

	std::vector<CResource*> m_vResources;

	std::map<BYTE, CMesh*> m_mMesh;
	std::map<BYTE, CTexture*> m_mTexture;
	std::map<BYTE, CMaterial*> m_mMaterial;
	std::map<BYTE, CShader*> m_mShader;

	void _LoadMesh();
	void _LoadTextures();
	void _LoadMaterials();
	void _CreateShaders();
};





#endif