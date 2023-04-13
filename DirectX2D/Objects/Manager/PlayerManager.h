#pragma once
class PlayerManager : public Singleton<PlayerManager>
{
private:
	friend class Singleton;

	PlayerManager();
	~PlayerManager();

public:
	void Update();
	void Render();

	void SetPlayerCoord(Vector2 coord);

	int& GetMonsterball() { return monsterball; }
	int& GetPotion() { return potion; }
	int& GetSuperPotion() { return superPotion; }
	Trainer* GetPlayer() { return trainer; }
	vector<Pokemon*> GetMyPokemons() { return myPokemons; }

private:
	Trainer* trainer;
	vector<Pokemon*> myPokemons;
	
	int monsterball = 3;
	int potion = 4;
	int superPotion = 2;
};