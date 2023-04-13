#pragma once

class PokemonDataManager : public Singleton<PokemonDataManager>
{
private:
	const int MAX_TYPE = 18;
	friend class Singleton;

	PokemonDataManager();
	~PokemonDataManager();

	void LoadPokemonStat();
	void LoadPokemonSkill();
	void LoadPokemonMatchType();

public:
	PokemonStat GetPokemonStat(int pokeNum);
	PokemonSkill GetPokemonSkill(int pokeNum);
	float GetMatchTypeRate(string attackType, string defenseType);
	int ChangeTypeToIndex(string type);

private:
	map<int, PokemonStat> pokemonStats;
	map<int, PokemonSkill> pokemonSkills;
	float matchTypes[18][18];

	thread* t1;
	thread* t2;
	thread* t3;
	mutex loadMutex1;
	mutex loadMutex2;
	mutex loadMutex3;
};