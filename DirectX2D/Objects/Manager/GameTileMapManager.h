#pragma once
class GameTileMapManager : public Singleton<GameTileMapManager>
{
private:
	friend class Singleton;

	GameTileMapManager();
	~GameTileMapManager();

public:

	void Update();
	void Render();

	void Load(string file);

	string GetCurBGMName();
	GameTileMap* GetTileMap() { return gameTileMap; }

private:
	void AddMapBGMs();
	void LoadBGM(string file);

private:
	GameTileMap* gameTileMap;
	map<string, string> mapBGMs;
};