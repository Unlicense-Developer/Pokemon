#pragma once

class GameObject : public Transform
{
public:
    GameObject(wstring shaderFile = L"UV.hlsl");
    ~GameObject();

    virtual void SetRender();

    void SetPixelShader(wstring shaderFile);
    void SetVertexShader(wstring shaderFile);

protected:
    VertexShader* vertexShader;
    PixelShader* pixelShader;    
    MatrixBuffer* worldBuffer;

    Vector2 size;
};