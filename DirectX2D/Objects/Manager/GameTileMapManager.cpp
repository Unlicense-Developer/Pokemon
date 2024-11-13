#include "Framework.h"
#include "GameTileMapManager.h"

GameTileMapManager::GameTileMapManager()
{
    AddMapBGMs();
    gameTileMap = new GameTileMap("TextData/PlayerHouse2F.map");
    Audio::Get()->Play(mapBGMs.at("TextData/PlayerHouse2F.map"), 0.5f);
}

GameTileMapManager::~GameTileMapManager()
{
    delete gameTileMap;
}

void GameTileMapManager::Update()
{
    gameTileMap->GetCurMapFile();
}

void GameTileMapManager::Render()
{
    gameTileMap->Render();
}

void GameTileMapManager::Load(string file)
{
    LoadBGM(file);

    gameTileMap->Load(file);
}

string GameTileMapManager::GetCurBGMName()
{
    return mapBGMs.at(gameTileMap->GetCurMapFile());
}


void GameTileMapManager::AddMapBGMs()
{
    mapBGMs.insert({ "TextData/PlayerHouse2F.map", "NewBarkTown" });
    mapBGMs.insert({ "TextData/PlayerHouse1F.map", "NewBarkTown" });
    mapBGMs.insert({ "TextData/NewBarkTown.map", "NewBarkTown" });
    mapBGMs.insert({ "TextData/Route29.map", "Route29" });
    mapBGMs.insert({ "TextData/Gate.map", "Route29" });
    mapBGMs.insert({ "TextData/Route30.map", "Route30" });
    mapBGMs.insert({ "TextData/VioletCity.map", "VioletCity" });
    mapBGMs.insert({ "TextData/VioletCity_Gym.map", "PokemonGym" });
}

void GameTileMapManager::LoadBGM(string file)
{
    if (mapBGMs.at(gameTileMap->GetCurMapFile()) == mapBGMs.at(file)) return;

    if (mapBGMs.at(gameTileMap->GetCurMapFile()) != mapBGMs.at(file))
    {
        Audio::Get()->Stop(mapBGMs.at(gameTileMap->GetCurMapFile()));
        Audio::Get()->Play(mapBGMs.at(file), 0.5f);
    }
}
