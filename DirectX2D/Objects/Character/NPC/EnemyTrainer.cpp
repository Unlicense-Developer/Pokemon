#include "Framework.h"

EnemyTrainer::EnemyTrainer(wstring textureFile, wstring battleTexture, Vector2 coord, string appearMap)
	:NPC(textureFile, coord)
{
	this->appearMap = appearMap;
	exclamation = new Quad(L"Textures/Character/Enemy/Exclamation.png");

	exclamation->SetActive(false);
	exclamation->Pos() = Pos() + Vector2(0, 80);
	exclamation->Scale() *= 5.0f;
	exclamation->UpdateWorld();

	battleImage = new Quad(battleTexture);
	battleImage->Scale() *= 5.0f;
	battleImage->SetActive(false);

	UpdateWorld();
}

EnemyTrainer::~EnemyTrainer()
{
	delete exclamation;
}

void EnemyTrainer::Update()
{
	if (GameTileMapManager::Get()->GetTileMap()->GetCurMapFile() == appearMap)
	{
		CheckTrainerThru();
		Encounter();
	}

	for (Pokemon* pokemon : pokemons)
		pokemon->Update();
}

void EnemyTrainer::Render()
{
	if (GameTileMapManager::Get()->GetTileMap()->GetCurMapFile() == appearMap)
	{
		NPC::Render();
		exclamation->Render();
	}

	battleImage->Render();
}

void EnemyTrainer::CheckTrainerThru()
{
	if (MY_TRAINER->GetCurCoord().x == curCoord.x &&
		MY_TRAINER->GetCurCoord().y >= curCoord.y - 2 &&
		MY_TRAINER->GetCurCoord().y < curCoord.y &&
		!MY_TRAINER->GetIsBattle() && !isFight)
	{
		exclamation->SetActive(true);
		MY_TRAINER->SetIsBattle(true);
		isFight = true;
	}
}

void EnemyTrainer::Encounter()
{
	if (exclamation->Active())
	{
		encounterDelay += DELTA;

		if (encounterDelay > 1.0f)
		{
			encounterDelay = 0.0f;
			exclamation->SetActive(false);
			Audio::Get()->Stop(GameTileMapManager::Get()->GetCurBGMName());
			Audio::Get()->Play("Trainer_Encounter", 0.5f);
			BattleManager::Get()->SetBattleType(TRAINER_BATTLE);
			MY_TRAINER->SetIsMove(false);
			MY_TRAINER->SetIsBattle(true);
			MY_TRAINER->SetEncounter(true);
		}
	}
}

void EnemyTrainer::SetEnemyPokemon(int pokeNum)
{
	pokemons.push_back(new Pokemon(pokeNum));
}