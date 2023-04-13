#pragma once

class EditTileMap : public Transform
{
private:
    const float SIZE_RATE = 5.0f;

public:
    EditTileMap(UINT width, UINT height);
    ~EditTileMap();

    void Update();
    void Render();

    void ClickTile(Quad* selectSample, Tile::Type type);
    void DeleteObjTile();
    void DeletePortalTile();
    void DeleteLedgeTile();

    void Save(string file);
    void Load(string file);

private:
    void CreateTile();

    void SetBGTile(wstring file, float angle);
    void SetOBJTile(wstring file, float angle);
    void SetPORTALTile(wstring file, float angle);
    void SetLEDGETile(wstring file, float angle);

private:
    UINT width, height;

    vector<Tile*> bgTiles;
    vector<Tile*> objTiles;
    vector<Tile*> portalTiles;
    vector<Tile*> ledgeTiles;

    Vector2 tileSize;
};