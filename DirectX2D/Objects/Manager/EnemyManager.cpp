#include "Framework.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	wildPokemon = new Pokemon(1);

	lance = new EnemyTrainer(L"Textures/Character/Enemy/Lance.png", L"Textures/Character/Enemy/Lance_Front.png", Vector2(7, 14), "TextData/VioletCity_Gym.map");
	
	SetEnemyTrainerPokemons();
}

EnemyManager::~EnemyManager()
{
	delete wildPokemon;
	delete lance;
}

void EnemyManager::Update()
{
	wildPokemon->Update();
	lance->Update();
}

void EnemyManager::Render()
{
	wildPokemon->Render();
	lance->Render();
}

vector<Pokemon*> EnemyManager::GetEnemyPokemon()
{
	return lance->GetEnemyPokemon();
}

void EnemyManager::SetEnemyTrainerPokemons()
{
	lance->SetEnemyPokemon(150);
}
