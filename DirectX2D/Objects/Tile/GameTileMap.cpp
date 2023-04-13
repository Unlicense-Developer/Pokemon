#include "Framework.h"

GameTileMap::GameTileMap(string file)
{
    UpdateWorld();

    LoadPortalTile();
    Load(file);
}

GameTileMap::~GameTileMap()
{
    for (Tile* tile : bgTiles)
        delete tile;

    for (Tile* tile : objTiles)
        delete tile;

    for (Tile* tile : portalTiles)
        delete tile;

    for (Tile* tile : ledgeTiles)
        delete tile;
}

void GameTileMap::Render()
{
    for (Tile* tile : bgTiles)
    {
        if (CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize()))
            tile->Render();
    }

    for (Tile* tile : objTiles)
    {
        if (CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize()))
            tile->Render();
    }

    for (Tile* tile : portalTiles)
    {
        if (CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize()))
            tile->Render();
    }

    for (Tile* tile : ledgeTiles)
    {
        if (CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize()))
            tile->Render();
    }
}


Vector2 GameTileMap::LeftBottom()
{
    return Pos() - tileSize * 0.5f;
}

Vector2 GameTileMap::RightTop()
{
    return Pos() + Vector2(width, height) * tileSize - tileSize * 0.5f;
}

void GameTileMap::Load(string file)
{
    for (Tile* tile : bgTiles)
        delete tile;

    for (Tile* tile : objTiles)
        delete tile;

    for (Tile* tile : portalTiles)
        delete tile;

    for (Tile* tile : ledgeTiles)
        delete tile;

    BinaryReader* reader = new BinaryReader(file);

    width = reader->UInt();
    height = reader->UInt();

    UINT size = reader->UInt();
    bgTiles.resize(size);

    for (Tile*& tile : bgTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float() + 80.0f;
        data.pos.y = reader->Float() + 80.0f;
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
        tile->UpdateWorld();
    }

    tileSize = bgTiles[0]->GetSize();

    size = reader->UInt();
    objTiles.resize(size);

    for (Tile*& tile : objTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float() + 80.0f;
        data.pos.y = reader->Float() + 80.0f;
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
        tile->Update();
    }

    size = reader->UInt();
    portalTiles.resize(size);

    for (Tile*& tile : portalTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float() + 80.0f;
        data.pos.y = reader->Float() + 80.0f;
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        if (portalTileDatas.at(make_pair(data.coord.x, data.coord.y)).curMap == file)
        {
            data.dest = portalTileDatas.at(make_pair(data.coord.x, data.coord.y)).destMap;
            data.destCoord.x = portalTileDatas.at(make_pair(data.coord.x, data.coord.y)).destCoord_x;
            data.destCoord.y = portalTileDatas.at(make_pair(data.coord.x, data.coord.y)).destCoord_y;
            data.isReverse = portalTileDatas.at(make_pair(data.coord.x, data.coord.y)).isReverse;
        }

        tile = new Tile(data);
        tile->SetParent(this);
        tile->Scale() *= SIZE_RATE;
        tile->Update();
    }

    size = reader->UInt();
    ledgeTiles.resize(size);

    for (Tile*& tile : ledgeTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float() + 80.0f;
        data.pos.y = reader->Float() + 80.0f;
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();
    
        string dir = SplitFileName(ToString(data.textureFile), "_");

        if (dir == "left.png")
            data.ledgeDir = "left";
        else if (dir == "right.png")
            data.ledgeDir = "right";
        else if (dir == "down.png")
            data.ledgeDir = "down";

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
        tile->Update();
    }

    delete reader;

    curMapFile = file;
}

void GameTileMap::LoadPortalTile()
{
    fstream loadFile("TextData/PortalDataTable.csv");

    string temp;

    char text[256];

    bool isFirstLine = true;

    while (true)
    {
        loadFile >> temp;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");

        PortalTileData data;
        data.curMap = datas[0];
        data.keyCoord_x = stof(datas[1]);
        data.keyCoord_y = stof(datas[2]);
        data.destMap = datas[3];
        data.destCoord_x = stof(datas[4]);
        data.destCoord_y = stof(datas[5]);

        if (stoi(datas[6]) == 1)
            data.isReverse = false;
        else if (stoi(datas[6]) == 2)
            data.isReverse = true;

        portalTileDatas[make_pair(data.keyCoord_x, data.keyCoord_y)] = data;

        if (loadFile.eof())
            break;
    }
}

Tile* GameTileMap::GetCoordToTile(Vector2 coord)
{
    for (Tile* tile : objTiles)
    {
        if (tile->GetData().coord == coord)
            return tile;
    }

    for (Tile* tile : portalTiles)
    {
        if (tile->GetData().coord == coord)
            return tile;
    }

    for (Tile* tile : ledgeTiles)
    {
        if (tile->GetData().coord == coord)
            return tile;
    }

    for (Tile* tile : bgTiles)
    {
        if (tile->GetData().coord == coord)
            return tile;
    }
}
