#pragma once

struct PortalTileData
{
    string curMap;
    float keyCoord_x = 0;
    float keyCoord_y = 0;
    string destMap;
    float destCoord_x = 0;
    float destCoord_y = 0;
    bool isReverse;
};

class GameTileMap : public Transform
{
    const float SIZE_RATE = 5.0f;
public:
    GameTileMap(string file);
    ~GameTileMap();

    void Render();

    void Load(string file);
    void LoadPortalTile();

    Tile* GetCoordToTile(Vector2 coord);

    UINT GetWidth() { return width; }
    UINT GetHeight() { return height; }
    string GetCurMapFile() { return curMapFile; }

    Vector2 LeftBottom();
    Vector2 RightTop();

private:
    UINT width, height;

    Vector2 tileSize;
    string curMapFile = "";

    vector<Tile*> bgTiles;
    vector<Tile*> objTiles;
    vector<Tile*> portalTiles;
    vector<Tile*> ledgeTiles;

    map<pair<float, float>, PortalTileData> portalTileDatas;
};