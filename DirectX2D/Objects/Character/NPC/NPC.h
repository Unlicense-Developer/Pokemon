#pragma once
class NPC : public Quad
{
public:
	NPC(wstring textureFile, Vector2 coord);
	~NPC();

	void Render();

protected:
	Vector2 curCoord;
};