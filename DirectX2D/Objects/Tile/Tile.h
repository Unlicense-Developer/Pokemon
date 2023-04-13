#pragma once

class Tile : public Button
{
public:
    enum Type
    {
        BG, OBJ, PORTAL, LEDGE,
    };

    struct Data
    {
        wstring textureFile;
        Vector2 pos;
        float angle = 0.0f;
        Type type = BG;
        Vector2 coord;
        bool isReverse = false;
        string ledgeDir = "";

        string dest = "";
        Vector2 destCoord;
    };

public:
    Tile(Data data);
    ~Tile();

    void Update();

    void SetTexture(wstring file);
    void SetAngle(float angle);
    
    Data GetData() { return data; }

private:
    Data data;
};