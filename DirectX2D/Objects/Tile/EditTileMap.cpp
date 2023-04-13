#include "Framework.h"

EditTileMap::EditTileMap(UINT width, UINT height)
    : width(width), height(height)
{
    CreateTile();

    Vector2 rightTop = Vector2(width, height) * tileSize + Vector2(80, 80);

    CAM->SetRightTop(rightTop.x + tileSize.x * 10, rightTop.y);
}

EditTileMap::~EditTileMap()
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

void EditTileMap::Update()
{
    for (Tile* tile : bgTiles)
        tile->Update();

    for (Tile* tile : objTiles)
        tile->Update();

    for (Tile* tile : portalTiles)
        tile->Update();

    for (Tile* tile : ledgeTiles)
        tile->Update();

    UpdateWorld();
}

void EditTileMap::Render()
{
    for (Tile* tile : bgTiles)
        tile->Render();

    for (Tile* tile : objTiles)
        tile->Render();

    for (Tile* tile : portalTiles)
        tile->Render();

    for (Tile* tile : ledgeTiles)
        tile->Render();
}

void EditTileMap::ClickTile(Quad* selectSample, Tile::Type type)
{
    switch (type)
    {
    case Tile::BG:
        SetBGTile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
        break;
    case Tile::OBJ:
        SetOBJTile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
        break;
    case Tile::PORTAL:
        SetPORTALTile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
        break;
    case Tile::LEDGE:
        SetLEDGETile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
        break;
    }
}

void EditTileMap::DeleteObjTile()
{
    vector<Tile*>::iterator iter = objTiles.begin();

    for (; iter != objTiles.end(); )
    {
        if ((*iter)->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            delete *iter;
            iter = objTiles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void EditTileMap::DeletePortalTile()
{
    vector<Tile*>::iterator iter = portalTiles.begin();

    for (; iter != portalTiles.end(); )
    {
        if ((*iter)->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            delete* iter;
            iter = portalTiles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void EditTileMap::DeleteLedgeTile()
{
    vector<Tile*>::iterator iter = ledgeTiles.begin();

    for (; iter != ledgeTiles.end(); )
    {
        if ((*iter)->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            delete* iter;
            iter = ledgeTiles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void EditTileMap::Save(string file)
{
    BinaryWriter* writer = new BinaryWriter(file);

    writer->UInt(width);
    writer->UInt(height);

    writer->UInt(bgTiles.size());

    for (Tile* tile : bgTiles)
    {
        Tile::Data data = tile->GetData();

        writer->WString(data.textureFile);
        writer->Float(data.pos.x);
        writer->Float(data.pos.y);
        writer->Float(data.angle);
        writer->Int(data.type);
        writer->Float(data.coord.x);
        writer->Float(data.coord.y);
    }

    writer->UInt(objTiles.size());

    for (Tile* tile : objTiles)
    {
        Tile::Data data = tile->GetData();

        writer->WString(data.textureFile);
        writer->Float(data.pos.x);
        writer->Float(data.pos.y);
        writer->Float(data.angle);
        writer->Int(data.type);
        writer->Float(data.coord.x);
        writer->Float(data.coord.y);
    }

    writer->UInt(portalTiles.size());

    for (Tile* tile : portalTiles)
    {
        Tile::Data data = tile->GetData();

        writer->WString(data.textureFile);
        writer->Float(data.pos.x);
        writer->Float(data.pos.y);
        writer->Float(data.angle);
        writer->Int(data.type);
        writer->Float(data.coord.x);
        writer->Float(data.coord.y);
    }

    writer->UInt(ledgeTiles.size());

    for (Tile* tile : ledgeTiles)
    {
        Tile::Data data = tile->GetData();

        writer->WString(data.textureFile);
        writer->Float(data.pos.x);
        writer->Float(data.pos.y);
        writer->Float(data.angle);
        writer->Int(data.type);
        writer->Float(data.coord.x);
        writer->Float(data.coord.y);
    }

    delete writer;
}

void EditTileMap::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    width = reader->UInt();
    height = reader->UInt();

    UINT size = reader->UInt();

    for (Tile* tile : bgTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float();
        data.pos.y = reader->Float();
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile->SetTexture(data.textureFile);
        tile->Pos() = data.pos;
        tile->Rot().z = data.angle;
    }

    size = reader->UInt();

    for (Tile* tile : objTiles)
        delete tile;

    objTiles.resize(size);

    for (Tile*& tile : objTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float();
        data.pos.y = reader->Float();
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
    }

    size = reader->UInt();

    for (Tile* tile : portalTiles)
        delete tile;

    portalTiles.resize(size);

    for (Tile*& tile : portalTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float();
        data.pos.y = reader->Float();
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
    }

    size = reader->UInt();

    for (Tile* tile : ledgeTiles)
        delete tile;

    ledgeTiles.resize(size);

    for (Tile*& tile : ledgeTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos.x = reader->Float();
        data.pos.y = reader->Float();
        data.angle = reader->Float();
        data.type = (Tile::Type)reader->Int();
        data.coord.x = reader->Float();
        data.coord.y = reader->Float();

        tile = new Tile(data);
        tile->Scale() *= SIZE_RATE;
        tile->SetParent(this);
    }

    delete reader;
}

void EditTileMap::CreateTile()
{
    wstring baseTile = L"Textures/Tile/basetile.png";
    Texture* texture = Texture::Add(baseTile);

    tileSize = texture->GetSize() * 5.0f;

    bgTiles.reserve(width * height);

    for (float y = 0; y < height; y++)
    {
        for (float x = 0; x < width; x++)
        {
            Tile::Data data = {};
            data.textureFile = baseTile;
            data.pos = Vector2(tileSize.x * x, tileSize.y * y);
            data.coord = { x + 1, y + 1};

            Tile* tile = new Tile(data);
            tile->Scale() *= 5.0f;
            tile->SetParent(this);
            bgTiles.push_back(tile);
        }
    }
}

void EditTileMap::SetBGTile(wstring file, float angle)
{
    for (Tile* tile : bgTiles)
    {
        if (tile->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            tile->SetTexture(file);
            tile->SetAngle(angle);
        }
    }
}

void EditTileMap::SetOBJTile(wstring file, float angle)
{
    DeleteObjTile();

    for (Tile* tile : bgTiles)
    {
        if (tile->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            Tile::Data data = tile->GetData();
            data.angle = angle;
            data.textureFile = file;
            data.type = Tile::OBJ;

            Tile* tile = new Tile(data);
            tile->Scale() *= 5.0f;
            tile->SetParent(this);
            tile->UpdateWorld();

            objTiles.push_back(tile);
            return;
        }
    }
}

void EditTileMap::SetPORTALTile(wstring file, float angle)
{
    DeletePortalTile();

    for (Tile* tile : bgTiles)
    {
        if (tile->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            Tile::Data data = tile->GetData();
            data.angle = angle;
            data.textureFile = file;
            data.type = Tile::PORTAL;

            Tile* tile = new Tile(data);
            tile->Scale() *= 5.0f;
            tile->SetParent(this);
            tile->UpdateWorld();

            portalTiles.push_back(tile);
            return;
        }
    }
}

void EditTileMap::SetLEDGETile(wstring file, float angle)
{
    DeleteLedgeTile();

    for (Tile* tile : bgTiles)
    {
        if (tile->GetCollider()->IsPointCollision(CAM->ScreenToWorld(mousePos)))
        {
            Tile::Data data = tile->GetData();
            data.angle = angle;
            data.textureFile = file;
            data.type = Tile::LEDGE;

            Tile* tile = new Tile(data);
            tile->Scale() *= 5.0f;
            tile->SetParent(this);
            tile->UpdateWorld();

            ledgeTiles.push_back(tile);
            return;
        }
    }
}
