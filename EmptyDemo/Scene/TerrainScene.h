#pragma once

#include <Engine/scene.h>

class TerrainScene : public Scene
{
public:
	TerrainScene();
	virtual~TerrainScene();

	void InitScene() override;
	void UpdateScene(float delta) override;
	void RenderScene() override;

	void RenderText();

	Camera* mCamera;

	UniformBuffer* mUbos[3];

	Shader*		mShader;
	Model*		mModel;
	Texture*	mTexture;
	Texture*	mGroundTexture;
	Terrain* mTerrain {nullptr};
	Environment* m_sky{};
};