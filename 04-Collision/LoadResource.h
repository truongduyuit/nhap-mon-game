#pragma once

class CLoadResourcesHelper
{
public:

	void LoadAllTextures();
	void LoadSprites(string filePath, int tex_id);

	CLoadResourcesHelper() { ; }
	~CLoadResourcesHelper() { ; }
};