#include <Core/PCH.h>
#include <Scene/TerrainScene.h>
#include <Graphics/Context.h>

#include <Engine/Camera.h>
#include <Core/Input.h>

#include <Cache/BufferCache.h>
#include <Cache/ModelCache.h>
#include <Cache/ShaderCache.h>
#include <Cache/TextureCache.h>

#include <Engine/Terrain.h>
#include <Renderer/Batch/FontBatch.h>

#include <Renderer/Environment.h>

TerrainScene::TerrainScene()
{
}

TerrainScene::~TerrainScene()
{
	SAFE_DELETE(mTerrain);
}

void TerrainScene::InitScene()
{

	mCamera = CreateSceneCamera("main", 45.f, GContext->GetWidth() / (float)GContext->GetHeight());

	BufferCreateInfo bufferInfo{};
	bufferInfo.binding = BufferBinding::BIND_UNIFORM;
	bufferInfo.pData = nullptr;
	bufferInfo.size = sizeof(Matrix4x4) * 2;
	bufferInfo.type = BufferType::BUFFER_DYNAMIC;

	mUbos[0] = BufferCache::CreateUniformBuffer(&bufferInfo);
	bufferInfo.size = sizeof(Matrix4x4);
	mUbos[1] = BufferCache::CreateUniformBuffer(&bufferInfo);

	mShader = ShaderCache::CreateShader("ForwardVS", "ForwardPS");
	mModel = ModelCache::LoadModel("../data/model/knot.obj");
	mTexture = TextureCache::LoadTexture("../data/texture/concrete.dds");
	mGroundTexture = TextureCache::LoadTexture("../data/texture/ground01.jpg");
	
	TerrainCreateInfo terrainInfo{};
	terrainInfo.Filename = "../data/texture/256fix.bmp";
	terrainInfo.CellScale = 2;
	terrainInfo.HeightScale = 100;
	terrainInfo.nSmooth = 2;
	terrainInfo.nTiles = 10;

	mTerrain = new Terrain();
	mTerrain->Init(terrainInfo);

	m_sky = new SkyCube;

}

void TerrainScene::UpdateScene(float delta)
{
	mCamera->ProcessFirstPerson(GInput, delta);

	Matrix4x4 pv[2] = { mCamera->GetProjection(), mCamera->GetView() };

	mUbos[0]->Update(pv);

	Matrix4x4 m;
	mUbos[1]->Update(&m);
}

void TerrainScene::RenderScene()
{
	mUbos[0]->BindVS(0);
	mUbos[1]->BindVS(1);

	mShader->Bind();

	mTexture->Bind(0);

	mModel->Render();

	Frustum frustum = mCamera->GetExtractedFrustum();

	mGroundTexture->Bind(0);
	mTerrain->Render(&frustum);

	m_sky->Render();
}

void TerrainScene::RenderText()
{
	char buffer[64];
	sprintf(buffer, "visible node : %d", mTerrain->result.renderedVertices);
	G_FontBatch->Render(20, 20, buffer, vec4f(1));
}
