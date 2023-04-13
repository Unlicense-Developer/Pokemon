#pragma once
class EnemyManager : public Singleton<EnemyManager>
{
private:
	enum EnemyType
	{
		
	};
private:
	friend class Singleton;

	EnemyManager();
	~EnemyManager();

public:
	void Update();
	void Render();

	Pokemon* GetWildPokemon() { return wildPokemon; }
	EnemyTrainer* GetEnemyTrainer() { return lance; }
	vector<Pokemon*> GetEnemyPokemon();
	void SetEnemyTrainerPokemons();

private:
	Pokemon* wildPokemon;
	EnemyTrainer* lance;
};