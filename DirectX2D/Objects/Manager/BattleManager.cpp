#include "Framework.h"

BattleManager::BattleManager()
{
	fadeOutQuad = new Quad(L"Textures/Battle/BattleBG.png");
	fadeOutQuad->SetActive(false);
	fadeOutQuad->Pos() = { CENTER_X, CENTER_Y };
	fadeOutQuad->GetColor().w = 0.0f;
	fadeOutQuad->UpdateWorld();
}

BattleManager::~BattleManager()
{
	delete fadeOutQuad;
}

void BattleManager::Update()
{
	CheckDelay();
	DamagedEffect();
	UpdateHpGauge();
	FadeOut();
}

void BattleManager::Render()
{
	PrintSkillText();
	fadeOutQuad->Render();
}

void BattleManager::StartBattle()
{
	isBattleStart = true;
	isBattleEnd = false;

	if (MY_FIRST_POKEMON->GetBattleStat().speed >= enemy->GetBattleStat().speed)
		playerTurn = true;
	else if (MY_FIRST_POKEMON->GetBattleStat().speed < enemy->GetBattleStat().speed)
		enemyTurn = true;
}

void BattleManager::CheckDamage()
{
	if (playerTurn)
	{
		damage =(MY_FIRST_POKEMON->GetSkillPower(skillNum) * MY_FIRST_POKEMON->GetBattleStat().attack *
			(MY_FIRST_POKEMON->GetLevel() * 2 / 5 + 2) / enemy->GetBattleStat().defense / 50 + 2) *
			PokemonDataManager::Get()->GetMatchTypeRate(MY_FIRST_POKEMON->GetSkillType(skillNum), enemy->GetPokemonStat().type1) *
			PokemonDataManager::Get()->GetMatchTypeRate(MY_FIRST_POKEMON->GetSkillType(skillNum), enemy->GetPokemonStat().type2) *
			Random(85, 101) / 100;

	}
	else if (enemyTurn)
	{
		damage = (enemy->GetSkillPower(skillNum) * enemy->GetBattleStat().attack *
			(enemy->GetLevel() * 2 / 5 + 2) / MY_FIRST_POKEMON->GetBattleStat().defense / 50 + 2) *
			PokemonDataManager::Get()->GetMatchTypeRate(enemy->GetSkillType(skillNum), MY_FIRST_POKEMON->GetPokemonStat().type1) *
			PokemonDataManager::Get()->GetMatchTypeRate(enemy->GetSkillType(skillNum), MY_FIRST_POKEMON->GetPokemonStat().type2) *
			Random(85, 101) / 100;
	}
}

void BattleManager::ChangeTurn()
{
	if (playerTurn)
	{
		enemyTurn = true;
		playerTurn = false;
	}
	else if(enemyTurn)
	{
		enemyTurn = false;
		playerTurn = true;
	}
}

void BattleManager::Potion(int heal)
{
	if (heal > MY_FIRST_POKEMON->GetMaxHp() - MY_FIRST_POKEMON->GetCurHp())
		this->heal = MY_FIRST_POKEMON->GetMaxHp() - MY_FIRST_POKEMON->GetCurHp();
	else
		this->heal = heal;

	Audio::Get()->Play("Potion", 0.6f);
	isHealing = true;
}

void BattleManager::MonsterBall()
{
}

void BattleManager::PlayerAttack(int skillNum)
{
	if (isDamaging) return;

	skillPlaying = true;

	this->skillNum = skillNum;

	Audio::Get()->Play(MY_FIRST_POKEMON->GetSkillType(skillNum), 0.5f);
}

void BattleManager::EnemyAttack()
{
	if (isDamaging) return;

	skillPlaying = true;

	if (enemy->GetSkill().skill2Name == "NONE")
		skillNum = Random(1, 2);
	else if (enemy->GetSkill().skill3Name == "NONE")
		skillNum = Random(1, 3);
	else if (enemy->GetSkill().skill4Name == "NONE")
		skillNum = Random(1, 4);
	else
		skillNum = Random(1, 5);

	Audio::Get()->Play(enemy->GetSkillType(skillNum), 0.5f);
}

void BattleManager::DamagedEffect()
{
	if (damageGlow)
	{
		damageTime += DELTA;

		if (playerTurn)
		{
			if (damageTime >= 0.15f)
			{
				if (enemy->GetFront()->GetColor().w == 1.0f)
					enemy->GetFront()->GetColor().w = 0.0f;
				else if (enemy->GetFront()->GetColor().w == 0.0f)
				{
					enemy->GetFront()->GetColor().w = 1.0f;
					glowCount++;
				}

				damageTime = 0.0f;
			}
		}
		else if (enemyTurn)
		{
			if (damageTime >= 0.15f)
			{
				if (MY_FIRST_POKEMON->GetBack()->GetColor().w == 1.0f)
					MY_FIRST_POKEMON->GetBack()->GetColor().w = 0.0f;
				else if (MY_FIRST_POKEMON->GetBack()->GetColor().w == 0.0f)
				{
					MY_FIRST_POKEMON->GetBack()->GetColor().w = 1.0f;
					glowCount++;
				}

				damageTime = 0.0f;
			}
		}
	}

	if (glowCount == 3)
	{
		glowCount = 0;
		damageTime = 0.0f;
		damageGlow = false;
		CheckDamage();
	}
}

void BattleManager::CheckDelay()
{
	if (skillPlaying) // 스킬
	{
		delayTime += DELTA;

		if (delayTime >= 1.5f)
		{
			delayTime = 0.0f;
			skillPlaying = false;
			isDamaging = true;
			damageGlow = true;
			Audio::Get()->Play("Damaged", 0.6f);
		}
	}

	if (playerPokemonDead || enemyPokemonDead)
	{
		delayTime += DELTA;

		if (delayTime >= 1.5f)
		{
			delayTime = 0.0f;

			if (playerPokemonDead)
				isEnemyWin = true;
			else if (enemyPokemonDead)
				isPlayerWin = true;

			playerPokemonDead = false;
			enemyPokemonDead = false;
			playerTurn = false;
			enemyTurn = false;
		}
	}

	if (isPlayerWin || isEnemyWin)
	{
		delayTime += DELTA;

		if (delayTime >= 1.5f)
		{
			delayTime = 0.0f;
			fadeOut = true;
		}
	}

	if (isHealing)
	{
		delayTime += DELTA;

		if (delayTime >= 1.5f)
		{
			delayTime = 0.0f;
		}
	}
}

void BattleManager::PokemonDead()
{
	isBattleEnd = true;

	if (playerPokemonDead)
	{
		if (!Audio::Get()->IsPlaySound(MY_FIRST_POKEMON->GetPokemonStat().pokemonName))
		{
			if (playerPokemonSound)
			{
				MY_FIRST_POKEMON->GetBack()->SetActive(false);
			}
			else
			{
				Audio::Get()->Play(MY_FIRST_POKEMON->GetPokemonStat().pokemonName, 0.5f);
				playerPokemonSound = true;
			}
		}
	}
	else if (enemyPokemonDead)
	{

		if (!Audio::Get()->IsPlaySound(enemy->GetPokemonStat().pokemonName))
		{
			if (enemyPokemonSound)
			{
				enemy->GetFront()->SetActive(false);
			}
			else
			{
				Audio::Get()->Play(enemy->GetPokemonStat().pokemonName, 0.5f);
				enemyPokemonSound = true;
			}
		}
	}
}

void BattleManager::FadeOut()
{
	if (fadeOut)
	{
		fadeTime += DELTA;

		if (fadeTime > 0.2f)
		{
			fadeOutQuad->SetActive(true);
			fadeOutQuad->GetColor().w += 0.2f;
			fadeTime = 0.0f;
		}

		if (fadeOutQuad->GetColor().w >= 1.0f)
		{
			fadeTime = 0.0f;
			fadeOut = false;
			isPlayerWin = false;
			isEnemyWin = false;
			fadeOutQuad->SetActive(false);
			fadeOutQuad->GetColor().w = 0.0f;
			PlayerManager::Get()->GetPlayer()->SetIsBattle(false);
			SceneManager::Get()->ChangeScene("Pokemon");
		}

	}
}

void BattleManager::PrintSkillText()
{
	if (fadeOut) return;

	Font::Get()->SetStyle("PokemonFont_70");

	if (skillPlaying && !isDamaging)
	{
		if(playerTurn)
		{
			string skillText1 = MY_FIRST_POKEMON->GetPokemonStat().pokemonName + "의";
			string skillText2;

			switch (skillNum)
			{
			case 1:
				skillText2 = MY_FIRST_POKEMON->GetSkill().skill1Name + " 공격!";
				break;
			case 2:
				skillText2 = MY_FIRST_POKEMON->GetSkill().skill2Name + " 공격!";
				break;
			case 3:
				skillText2 = MY_FIRST_POKEMON->GetSkill().skill3Name + " 공격!";
				break;
			case 4:
				skillText2 = MY_FIRST_POKEMON->GetSkill().skill4Name + " 공격!";
				break;
			}

			Font::Get()->RenderText(skillText1, TEXT_POS1, TEXTPOS_SIZE);
			Font::Get()->RenderText(skillText2, TEXT_POS2, TEXTPOS_SIZE);
		}
		else if (enemyTurn)
		{
			string skillText1 = "적의 " + enemy->GetPokemonStat().pokemonName + "의";
			string skillText2;

			switch (skillNum)
			{
			case 1:
				skillText2 = enemy->GetSkill().skill1Name + "!";
				break;
			case 2:
				skillText2 = enemy->GetSkill().skill2Name + "!";
				break;
			case 3:
				skillText2 = enemy->GetSkill().skill3Name + "!";
				break;
			case 4:
				skillText2 = enemy->GetSkill().skill4Name + "!";
				break;
			}

			Font::Get()->RenderText(skillText1, TEXT_POS1, TEXTPOS_SIZE);
			Font::Get()->RenderText(skillText2, TEXT_POS2, TEXTPOS_SIZE);
		}
	}
	
	if (playerPokemonDead)
	{
		string skillText1 = MY_FIRST_POKEMON->GetPokemonStat().pokemonName + "는(은)";
		string skillText2 = "쓰러졌다!";

		Font::Get()->RenderText(skillText1, TEXT_POS1, TEXTPOS_SIZE);
		Font::Get()->RenderText(skillText2, TEXT_POS2, TEXTPOS_SIZE);
	}
	else if (enemyPokemonDead)
	{
		string skillText1 = "적의 " + enemy->GetPokemonStat().pokemonName + "는(은)";
		string skillText2 = "쓰러졌다!";

		Font::Get()->RenderText(skillText1, TEXT_POS1, TEXTPOS_SIZE);
		Font::Get()->RenderText(skillText2, TEXT_POS2, TEXTPOS_SIZE);
	}

	if (isPlayerWin)
	{
		string winningText1;
		if (battleType == WILD_BATTLE)
			winningText1 = "적과의 승부에서";
		else if (battleType == TRAINER_BATTLE)
			winningText1 = "사천왕 목호와의 승부에서";

		string winningText2 = "승리했다!";

		Font::Get()->RenderText(winningText1, TEXT_POS1, TEXTPOS_SIZE);
		Font::Get()->RenderText(winningText2, TEXT_POS2, TEXTPOS_SIZE);
	}
	else if (isEnemyWin)
	{
		string winningText1 = "플레이어의 곁에는";
		string winningText2 = "싸울 수 있는 포켓몬이 없다!";

		Font::Get()->RenderText(winningText1, TEXT_POS1, TEXTPOS_SIZE);
		Font::Get()->RenderText(winningText2, TEXT_POS2, TEXTPOS_SIZE);
	}

	if (isHealing)
	{
		string text1 = MY_FIRST_POKEMON->GetPokemonStat().pokemonName + "의 체력이";
		string text2 = to_string((int)heal) + " 회복되었다";

		Font::Get()->RenderText(text1, TEXT_POS1, TEXTPOS_SIZE);
		Font::Get()->RenderText(text2, TEXT_POS2, TEXTPOS_SIZE);
	}
}

void BattleManager::UpdateHpGauge()
{
	if (isHealing)
	{
		if (heal > 0)
		{
			gaugeTime += DELTA;

			if (gaugeTime > 0.05f)
			{
				MY_FIRST_POKEMON->GetCurHp() += 1;
				gaugeTime = 0.0f;
			}

			if (MY_FIRST_POKEMON->GetCurHp() >= MY_FIRST_POKEMON->GetMaxHp())
			{
				heal = 0.0f;
				isHealing = false;
				ChangeTurn();
			}
			else if (heal <= MY_FIRST_POKEMON->GetCurHp() - tempHp_Heal)
			{
				heal = 0.0f;
				isHealing = false;
				ChangeTurn();
			}
		}
	}
	else
	{
		tempHp_Heal = MY_FIRST_POKEMON->GetCurHp();
	}

	if (playerTurn)
	{
		if (damage > 0)
		{
			gaugeTime += DELTA;

			if (gaugeTime > 0.05f)
			{
				enemy->GetCurHp() -= 1;
				gaugeTime = 0.0f;
			}

			if ((int)damage <= tempHp - enemy->GetCurHp())
			{
				damage = 0.0f;
				isDamaging = false;
				ChangeTurn();
			}
			else if (enemy->GetCurHp() == 0.0f)
			{
				damage = 0.0f;
				isDamaging = false;
				enemyPokemonDead = true;
				PokemonDead();
			}

		}
		else
			tempHp = enemy->GetCurHp();
	}
	else if (enemyTurn)
	{
		if (damage > 0)
		{
			gaugeTime += DELTA;

			if (gaugeTime > 0.05f)
			{
				MY_FIRST_POKEMON->GetCurHp() -= 1;
				gaugeTime = 0.0f;
			}

			if ((int)damage <= tempHp - MY_FIRST_POKEMON->GetCurHp())
			{
				damage = 0.0f;
				isDamaging = false;
				ChangeTurn();
			}
			else if (MY_FIRST_POKEMON->GetCurHp() == 0.0f)
			{
				damage = 0.0f;
				isDamaging = false;
				playerPokemonDead = true;
				PokemonDead();
			}

		}
		else
			tempHp = MY_FIRST_POKEMON->GetCurHp();
	}
}
