#include "Framework.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	EnemyManager::Get();
	BattleManager::Get();

	battleBG = new Quad(L"Textures/Battle/BattleBG.png");
	battleBG->Pos() = { CENTER_X, CENTER_Y };
	battleBG->UpdateWorld();

	enemyBallUI = new Quad(L"Textures/Battle/EnemyPokemon_UI.png");
	enemyBallUI->Pos() = { 240, 640 };
	enemyBallUI->SetActive(false);
	enemyBallUI->UpdateWorld();

	playerBallUI = new Quad(L"Textures/Battle/PlayerPokemon_UI.png");
	playerBallUI->Pos() = { 560, 300 };
	playerBallUI->SetActive(false);
	playerBallUI->UpdateWorld();

	enemyHpBar = new Quad(L"Textures/Battle/EnemyHP_UI.png");
	enemyHpBar->Pos() = Vector2( 0, WIN_HEIGHT ) + 
		Vector2(enemyHpBar->GetSize().x * 0.5f, -enemyHpBar->GetSize().y * 0.5f) + Vector2(40.0f, -40.0f);
	enemyHpBar->SetActive(false);
	enemyHpBar->UpdateWorld();

	playerHpBar = new Quad(L"Textures/Battle/PlayerHP_UI.png"); 
	playerHpBar->Pos() = { 560, 340 };
	playerHpBar->SetActive(false);
	playerHpBar->UpdateWorld();

	appearEffect = new AnimObject();
	appearEffect->Scale() *= 5.0f;
	appearEffect->SetActive(false);

	CreateSounds();
	CreateClips();
}

BattleScene::~BattleScene()
{
	BattleManager::Delete();
	delete battleBG; 
	delete appearEffect;
	delete playerHpBar;
	delete playerBallUI;
	delete enemyHpBar;

}

void BattleScene::Update()
{
	CheckBGMStart();
	CheckPokemonSoundPlay();
	CheckEnemyImagePos();
	ControlPlayerImage();
		
	EnemyManager::Get()->Update();
	PlayerManager::Get()->Update();
	UIManager::Get()->Update();
	appearEffect->Update();
	BattleManager::Get()->Update();
	UpdateHpBar();
}

void BattleScene::Render()
{
	battleBG->Render();
	TextBoxManager::Get()->GetTextBox()->Render();

	enemyBallUI->Render();
	playerBallUI->Render();
	playerHpBar->Render();
	enemyHpBar->Render();
	MY_FIRST_POKEMON->GetHpGauge()->Render();
	ENEMY_POKEMON->GetHpGauge()->Render();

	PrintText();

	EnemyManager::Get()->Render();
	PlayerManager::Get()->Render();
	MY_FIRST_POKEMON->Render();
	ENEMY_POKEMON->Render();
	appearEffect->Render();
	MY_TRAINER->GetBattleImage()->Render();
	UIManager::Get()->Render();
	BattleManager::Get()->Render();
}

void BattleScene::Start()
{
	TextBoxManager::Get()->GetTextBox()->SetActive(true);

	if (BattleManager::Get()->GetBattleType() == WILD_BATTLE)
		BattleManager::Get()->SetEnemy(WILD_POKEMON);
	else if (BattleManager::Get()->GetBattleType() == TRAINER_BATTLE)
		BattleManager::Get()->SetEnemy(ENEMY_TRAINER->GetEnemyPokemon()[0]);

	MY_TRAINER->GetBattleImage()->Pos() = { WIN_WIDTH + MY_TRAINER->GetBattleImage()->GetSize().x, PLAYER_POS.y };
	MY_TRAINER->GetBattleImage()->SetActive(true);
	MY_FIRST_POKEMON->GetBack()->SetActive(false);

	if (BattleManager::Get()->GetBattleType() == WILD_BATTLE)
	{
		ENEMY_TRAINER->GetBattleImage()->SetActive(false);
		ENEMY_POKEMON->GetFront()->SetActive(true);
		ENEMY_POKEMON->GetFront()->GetColor() = DARKER_COLOR;
		ENEMY_POKEMON->SetPokemonStat(Random(1, 32));
		ENEMY_POKEMON->SetPokemonSkill(ENEMY_POKEMON->GetPokemonStat().pokemonNum);
		ENEMY_POKEMON->SetPokemonTexture();
		ENEMY_POKEMON->Pos() = { ENEMY_POKEMON->GetFront()->GetSize().x * 0.7f, ENEMY_POS.y };
	}
	else if (BattleManager::Get()->GetBattleType() == TRAINER_BATTLE)
	{
		ENEMY_POKEMON->GetFront()->SetActive(false);

		ENEMY_TRAINER->GetBattleImage()->SetActive(true);
		ENEMY_TRAINER->GetBattleImage()->GetColor() = DARKER_COLOR;
		ENEMY_TRAINER->GetBattleImage()->Pos() = { ENEMY_POKEMON->GetFront()->GetSize().x * 0.7f, ENEMY_POS.y };
	}
}

void BattleScene::End()
{
	TextBoxManager::Get()->GetTextBox()->SetActive(false);
	UIManager::Get()->OffBattlePanel();

	Audio::Get()->Stop("Wild_Pokemon_Appear");
	Audio::Get()->Stop("Battle_Loop");
	Audio::Get()->Play(GameTileMapManager::Get()->GetCurBGMName());
	MY_FIRST_POKEMON->GetBack()->SetActive(false);
	MY_TRAINER->GetBattleImage()->Pos() = { WIN_WIDTH + MY_TRAINER->GetBattleImage()->GetSize().x, PLAYER_POS.y };
	playerHpBar->SetActive(false);
	playerBallUI->SetActive(false);
	MY_FIRST_POKEMON->GetHpGauge()->SetActive(false);
	enemyHpBar->SetActive(false);
	ENEMY_POKEMON->GetHpGauge()->SetActive(false);
	appearEffect->SetActive(false);
	appearEffect->GetCurClip() = nullptr;
	isEnemyPokemonSet = false;
	isEnemyPokemonCanSet = false;
	isEnemyTrainerSet = false;
	isPlayerPokemonSet = false;
	isPlayerPokemonCanSet = false;
	isPlayerPokemonSet = false;
	isEnemyPokemonSet = false;
	BattleManager::Get()->IsBattle() = false;
	BattleManager::Get()->IsPlayerTurn() = false;
	BattleManager::Get()->IsSkillPlaying() = false;
}

void BattleScene::CreateSounds()
{
	//BGM
	Audio::Get()->Add("Battle_Loop","Sounds/Wild_Pokemon_Battle_Loop.mp3", true);
	Audio::Get()->Add("Trainer_Battle_Loop","Sounds/Lance_Loop.wav", true);

	//EFFECT
	Audio::Get()->Add("노말","Sounds/Attack/Normal.wav");
	Audio::Get()->Add("불꽃","Sounds/Attack/Fire.wav");
	Audio::Get()->Add("물","Sounds/Attack/Water.wav");
	Audio::Get()->Add("풀","Sounds/Attack/Grass.wav");
	Audio::Get()->Add("전기","Sounds/Attack/Electric.wav");
	Audio::Get()->Add("얼음","Sounds/Attack/Ice.wav");
	Audio::Get()->Add("격투","Sounds/Attack/Fighting.wav");
	Audio::Get()->Add("독","Sounds/Attack/Poison.wav");
	Audio::Get()->Add("땅","Sounds/Attack/Ground.wav");
	Audio::Get()->Add("비행","Sounds/Attack/Flying.wav");
	Audio::Get()->Add("에스퍼","Sounds/Attack/Psychic.wav");
	Audio::Get()->Add("벌레","Sounds/Attack/Bug.wav");
	Audio::Get()->Add("바위","Sounds/Attack/Rock.wav");
	Audio::Get()->Add("고스트","Sounds/Attack/Ghost.wav");
	Audio::Get()->Add("드래곤","Sounds/Attack/Dragon.wav");
	Audio::Get()->Add("악","Sounds/Attack/Dark.wav");
	Audio::Get()->Add("강철","Sounds/Attack/Metal.wav");
	Audio::Get()->Add("페어리","Sounds/Attack/Fairy.wav");
	Audio::Get()->Add("Potion","Sounds/Potion.wav");
	Audio::Get()->Add("Damaged","Sounds/Damaged.wav");
}

void BattleScene::CreateClips()
{
	vector<Frame*> frames;
	wstring textureFile = L"Textures/Battle/AppearEffect.png";
	Vector2 cutSize = Texture::Add(textureFile)->GetSize() / Vector2(4, 1);

	frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 3, 0, cutSize.x, cutSize.y));
	appearEffect->AddClip("APPEAR", new Clip(frames, false));
	frames.clear();
}

void BattleScene::CheckBGMStart()
{
	if (Audio::Get()->IsPlaySound("Battle_Loop")) return;
	if (Audio::Get()->IsPlaySound("Trainer_Battle_Loop")) return;

	if (!Audio::Get()->IsPlaySound("Wild_Pokemon_Appear") && BattleManager::Get()->GetBattleType() == WILD_BATTLE)
	{
		Audio::Get()->Play("Battle_Loop", 0.5f);
	}

	if (!Audio::Get()->IsPlaySound("Trainer_Encounter") && BattleManager::Get()->GetBattleType() == TRAINER_BATTLE)
	{
		Audio::Get()->Play("Trainer_Battle_Loop", 0.5f);
	}
}

void BattleScene::CheckPokemonSoundPlay()
{
	if (isPlayerPokemonSet)
	{
		if (!playerPokemonSoundPlay && !Audio::Get()->IsPlaySound(MY_FIRST_POKEMON->GetPokemonStat().pokemonName))
		{
			Audio::Get()->Play(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, 0.5f);
			MY_FIRST_POKEMON->GetBack()->SetActive(true);
			playerPokemonSoundPlay = true;
		}
	}
	if (isEnemyPokemonSet)
	{
		if (!enemyPokemonSoundPlay && !Audio::Get()->IsPlaySound(ENEMY_POKEMON->GetPokemonStat().pokemonName))
		{
			Audio::Get()->Play(ENEMY_POKEMON->GetPokemonStat().pokemonName, 0.5f);
			ENEMY_POKEMON->GetFront()->SetActive(true);
			enemyPokemonSoundPlay = true;
		}
	}
}

void BattleScene::PrintText()
{
	if (BattleManager::Get()->isFadeOut()) return;
	if (UIManager::Get()->IsPanelListActive() &&
		UIManager::Get()->GetFrontPanelTop() == UIManager::Get()->SearchFrontPanel("Battle_Bag_Items")) return;

	if (isEnemyPokemonSet)
	{
		Font::Get()->SetColor("Black");
		Font::Get()->SetStyle("PokemonFont_70");

		if (ENEMY_POKEMON->GetPokemonStat().pokemonName.size() == 10)
		{
			Font::Get()->RenderText(ENEMY_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 70.0f, CENTER_Y + 320.0f });
		}
		else if (ENEMY_POKEMON->GetPokemonStat().pokemonName.size() == 8)
		{
			Font::Get()->RenderText(ENEMY_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 40.0f, CENTER_Y + 320.0f });
		}
		else if (ENEMY_POKEMON->GetPokemonStat().pokemonName.size() == 6)
		{
			Font::Get()->RenderText(ENEMY_POKEMON->GetPokemonStat().pokemonName, { CENTER_X - 30.0f, CENTER_Y + 320.0f });
		}
		else if (ENEMY_POKEMON->GetPokemonStat().pokemonName.size() == 4)
		{
			Font::Get()->RenderText(ENEMY_POKEMON->GetPokemonStat().pokemonName, { CENTER_X - 100.0f, CENTER_Y + 320.0f });
		}

		Font::Get()->SetStyle("PokemonFont_60");
		if (ENEMY_POKEMON->GetLevel() >= 10)
			Font::Get()->RenderText(to_string(ENEMY_POKEMON->GetLevel()), { CENTER_X + 4.0f, CENTER_Y + 315.0f });
		else
			Font::Get()->RenderText(to_string(ENEMY_POKEMON->GetLevel()), { CENTER_X - 26.0f, CENTER_Y + 315.0f });
	}

	if (!isPlayerPokemonSet && !isPlayerPokemonCanSet && !isEnemyTrainerSet && isEnemyPokemonSet)
	{
		Font::Get()->SetStyle("PokemonFont_70");
		//if(WILD_POKEMON->GetPokemonStat().pokemonName.substr(WILD_POKEMON->GetPokemonStat().pokemonName.size() - 1), )
		string appear = "야생의 " + ENEMY_POKEMON->GetPokemonStat().pokemonName + "(이)가 나타났다!";

		Font::Get()->RenderText(appear, TEXT_POS1, TEXTPOS_SIZE);
	}

	if (!isPlayerPokemonSet && !isPlayerPokemonCanSet && isEnemyTrainerSet)
	{
		if (!isEnemyPokemonSet)
		{
			Font::Get()->SetStyle("PokemonFont_70");
			Font::Get()->SetColor("Black");

			string appear1 = "사천왕 목호(이)가";
			string appear2 = "승부를 걸어왔다!";

			Font::Get()->RenderText(appear1, TEXT_POS1, TEXTPOS_SIZE);
			Font::Get()->RenderText(appear2, TEXT_POS2, TEXTPOS_SIZE);
		}
		else
		{
			Font::Get()->SetStyle("PokemonFont_70");
			Font::Get()->SetColor("Black");

			string appear1 = "사천왕 목호(이)는";
			string appear2 = ENEMY_POKEMON->GetPokemonStat().pokemonName + "를(을) 꺼냈다!";

			Font::Get()->RenderText(appear1, TEXT_POS1, TEXTPOS_SIZE);
			Font::Get()->RenderText(appear2, TEXT_POS2, TEXTPOS_SIZE);
		}
	}

	if (MY_TRAINER->GetBattleImage()->Pos().x < -400.f && !BattleManager::Get()->IsBattle())
	{
		Font::Get()->SetStyle("PokemonFont_70");
		string appear = "가랏! " + MY_FIRST_POKEMON->GetPokemonStat().pokemonName + "!";

		Font::Get()->RenderText(appear, TEXT_POS1, TEXTPOS_SIZE);
	}


	if (isPlayerPokemonSet)
	{
		Font::Get()->SetStyle("PokemonFont_70");

		if (MY_FIRST_POKEMON->GetPokemonStat().pokemonName.size() == 10)
		{
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 400.0f, CENTER_Y + 55.0f });
		}
		else if (MY_FIRST_POKEMON->GetPokemonStat().pokemonName.size() == 8)
		{
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 360.0f, CENTER_Y + 55.0f });
		}
		else if (MY_FIRST_POKEMON->GetPokemonStat().pokemonName.size() == 6)
		{
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 290.0f, CENTER_Y + 55.0f });
		}
		else if (MY_FIRST_POKEMON->GetPokemonStat().pokemonName.size() == 4)
		{
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, { CENTER_X + 222.0f, CENTER_Y + 55.0f });
		}

		Font::Get()->SetStyle("PokemonFont_60");
		
		Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetCurHp()), { CENTER_X + 160.0f, CENTER_Y - 28.0f});
		Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetMaxHp()), { CENTER_X + 310.0f, CENTER_Y - 28.0f});

		if (MY_FIRST_POKEMON->GetLevel() >= 10)
			Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetLevel()), { CENTER_X + 332.0f, CENTER_Y + 51.0f });
		else
			Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetLevel()), { CENTER_X + 302.0f, CENTER_Y + 51.0f });
		
	}
}

void BattleScene::ControlPlayerImage()
{
	if (!isPlayerPokemonCanSet)
	{
		if (MY_TRAINER->GetBattleImage()->Pos().x >= PLAYER_POS.x)
		{
			MY_TRAINER->GetBattleImage()->GetColor() = DARKER_COLOR;
			MY_TRAINER->GetBattleImage()->Pos().x -= 520.0f * DELTA;
		}
		else
		{
			playerBallUI->SetActive(true);
			MY_TRAINER->GetBattleImage()->GetColor() = ORIGINAL_COLOR;
		}
	}

	if (KEY_DOWN('Z') && isEnemyPokemonSet)
	{
		isPlayerPokemonCanSet = true;
		playerBallUI->SetActive(false);
	}

	if (isPlayerPokemonCanSet)
	{
		if (MY_TRAINER->GetBattleImage()->Pos().x >= -400.f)
		{
			MY_TRAINER->GetBattleImage()->Pos().x -= 520.0f * DELTA;
			appearEffect->RemoveCurClip();
		}
		else
		{
			if (appearEffect->GetCurClip() == nullptr)
			{
				appearEffect->Pos() = PLAYER_POS;
				appearEffect->SetActive(true);
				appearEffect->SetClip("APPEAR");
			}
		}
	}

	if (appearEffect->GetCurClip() != nullptr &&
		!appearEffect->GetCurClip()->IsPlay() &&
		isPlayerPokemonCanSet) // 내 포켓몬 등장
	{
		appearEffect->SetActive(false);
		playerHpBar->SetActive(true);
		MY_FIRST_POKEMON->GetHpGauge()->SetActive(true);
		isPlayerPokemonSet = true;
		MY_FIRST_POKEMON->Pos() = PLAYER_POS;
	}

	MY_TRAINER->GetBattleImage()->UpdateWorld();
	appearEffect->UpdateWorld();
}

void BattleScene::CheckEnemyImagePos()
{
	if (BattleManager::Get()->IsBattle()) return;

	if (BattleManager::Get()->GetBattleType() == WILD_BATTLE)
	{
		if (ENEMY_POKEMON->Pos().x <= ENEMY_POS.x)
			ENEMY_POKEMON->Pos().x += 460.0f * DELTA;
		else
		{
			isEnemyPokemonSet = true;
			ENEMY_POKEMON->GetFront()->GetColor() = ORIGINAL_COLOR;
		}

		if (isEnemyPokemonSet)
		{
			enemyHpBar->SetActive(true);
			ENEMY_POKEMON->GetHpGauge()->SetActive(true);
		}
	}
	else if (BattleManager::Get()->GetBattleType() == TRAINER_BATTLE)
	{
		if (ENEMY_TRAINER->GetBattleImage()->Pos().x <= ENEMY_POS.x)
			ENEMY_TRAINER->GetBattleImage()->Pos().x += 460.0f * DELTA;
		else
		{
			isEnemyTrainerSet = true;
			ENEMY_TRAINER->GetBattleImage()->GetColor() = ORIGINAL_COLOR;
		}

		if (isEnemyTrainerSet && !isEnemyPokemonCanSet)
		{
			enemyBallUI->SetActive(true);
		}

		if (KEY_DOWN('Z') && isEnemyTrainerSet)
		{
			isEnemyPokemonCanSet = true;
			enemyBallUI->SetActive(false);
		}

		if (isEnemyPokemonCanSet)
		{
			if (ENEMY_TRAINER->GetBattleImage()->Pos().x <= WIN_WIDTH + 400.0f)
				ENEMY_TRAINER->GetBattleImage()->Pos().x += 520.0f * DELTA;
			else
			{
				if (appearEffect->GetCurClip() == nullptr &&
					!isEnemyPokemonSet)
				{
					appearEffect->Pos() = ENEMY_POS;
					appearEffect->SetActive(true);
					appearEffect->SetClip("APPEAR");
				}
			}
		}

		if (appearEffect->GetCurClip() != nullptr &&
			!appearEffect->GetCurClip()->IsPlay() &&
			isEnemyPokemonCanSet) // 적 포켓몬 등장
		{
			appearEffect->SetActive(false);
			enemyHpBar->SetActive(true);
			ENEMY_POKEMON->GetHpGauge()->SetActive(true);
			isEnemyPokemonSet = true;
			ENEMY_POKEMON->Pos() = ENEMY_POS;
		}

		ENEMY_TRAINER->GetBattleImage()->UpdateWorld();
	}
}

void BattleScene::UpdateHpBar()
{
	MY_FIRST_POKEMON->GetHpGauge()->Pos() = playerHpBar->Pos() + Vector2(40, 20);
	ENEMY_POKEMON->GetHpGauge()->Pos() = enemyHpBar->Pos() + Vector2(35, 5);

	MY_FIRST_POKEMON->GetHpGauge()->UpdateWorld();
	ENEMY_POKEMON->GetHpGauge()->UpdateWorld();
}
