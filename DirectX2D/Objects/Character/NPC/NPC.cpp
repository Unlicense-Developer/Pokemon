#include "Framework.h"

NPC::NPC(wstring textureFile, Vector2 coord)
	:Quad(textureFile)
{
	curCoord = coord;
	Pos() = coord * 80.0f + Vector2(0,20);
	Scale() *= 5.0f;

	UpdateWorld();
}

NPC::~NPC()
{
}

void NPC::Render()
{
	Quad::Render();
}
