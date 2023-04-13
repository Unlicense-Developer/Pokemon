#include "Framework.h"

Tile::Tile(Data data)
    : Button(data.textureFile), data(data)
{
    localPosition = data.pos;
    localRotation.z = data.angle;
}

Tile::~Tile()
{
}

void Tile::Update()
{
    if (!isActive) return;

    UpdateWorld();
    collider->UpdateWorld();
}

void Tile::SetTexture(wstring file)
{
    Quad::SetTexture(file);
    data.textureFile = file;
}

void Tile::SetAngle(float angle)
{
    localRotation.z = angle;
    data.angle = angle;
}
