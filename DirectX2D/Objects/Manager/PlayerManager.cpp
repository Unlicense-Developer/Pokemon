#include "Framework.h"

PlayerManager::PlayerManager()
{
	trainer = new Trainer();

	myPokemons.push_back(new Pokemon(249));
	myPokemons.push_back(new Pokemon(3));
}

PlayerManager::~PlayerManager()
{
	delete trainer;

	for (Pokemon* pokemon : myPokemons)
		delete pokemon;
}

void PlayerManager::Update()
{
	if (SceneManager::Get()->GetCurScene() == SceneManager::Get()->GetScene("Pokemon"))
		trainer->Update();

	for (Pokemon* pokemon : myPokemons)
		pokemon->Update();
}

void PlayerManager::Render()
{
	if(SceneManager::Get()->GetCurScene() == SceneManager::Get()->GetScene("Pokemon"))
		trainer->Render();

	for (Pokemon* pokemon : myPokemons)
		pokemon->Render();
}

void PlayerManager::SetPlayerCoord(Vector2 coord)
{
	trainer->SetCurCoord(coord);
}
