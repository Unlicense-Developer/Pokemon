#pragma once
class EnemyTrainer : public NPC
{
public:
	EnemyTrainer(wstring textureFile, wstring battleTexture, Vector2 coord, string appearMap);
	~EnemyTrainer();

	void Update();
	void Render();

	void CheckTrainerThru();
	void Encounter();

	void SetIsFight(bool isFight) { this->isFight = isFight; }
	void SetEnemyPokemon(int pokeNum);

	vector<Pokemon*> GetEnemyPokemon() { return pokemons; }
	Quad* GetBattleImage() { return battleImage; }

private:
	Quad* exclamation;
	Quad* battleImage;

	string appearMap;

	float encounterDelay = 0.0f;
	
	bool isFight = false;

	vector<Pokemon*> pokemons;
};