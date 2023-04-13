#pragma once

enum BattleType
{
	WILD_BATTLE, TRAINER_BATTLE,
};

class BattleManager : public Singleton<BattleManager>
{
private:
	const Float2 TEXT_POS1 = { CENTER_X - 10.0f , CENTER_Y - 260.0f };
	const Float2 TEXT_POS2 = { CENTER_X - 10.0f , CENTER_Y - 340.0f };
	const Float2 TEXTPOS_SIZE = { 700.0f, 200.0f };

	friend class Singleton;

	BattleManager();
	~BattleManager();

public:
	void Update();
	void Render();
	
	void StartBattle();
	bool& IsBattle() {return isBattleStart;}
	bool& IsPlayerTurn() { return playerTurn; }
	bool& IsEnemyTurn() { return enemyTurn; }
	bool& IsSkillPlaying() { return skillPlaying; }
	bool& IsDamaging() { return isDamaging; }

	BattleType GetBattleType() { return battleType; }
	Pokemon* GetEnemy() { return enemy; }
	bool GetPlayerPokemonIsDead() { return playerPokemonDead; }
	bool GetEnemyPokemonIsDead() { return enemyPokemonDead; }
	bool GetIsHealing() { return isHealing; }
	bool GetIsMonsterBall() { return isMonsterBall; }
	bool IsBattleEnd() { return isBattleEnd; }
	bool isFadeOut() { return fadeOut; }

	void SetEnemy(Pokemon* enemy) { this->enemy =  enemy; }
	void SetBattleType(BattleType type) { this->battleType = type; }
	
	void CheckDamage();
	void ChangeTurn();
	void Potion(int heal);
	void MonsterBall();
	void PlayerAttack(int skillNum);
	void EnemyAttack();
	void DamagedEffect();
	void CheckDelay();
	void PokemonDead();
	void FadeOut();
	void PrintSkillText();
	void UpdateHpGauge();

private:
	BattleType battleType;

	Pokemon* enemy;
	Quad* fadeOutQuad;

	int skillNum = 0;
	int glowCount = 0;
	float tempHp_Heal = 0.0f;
	float tempHp = 0.0f;
	float delayTime = 0.0f;
	float fadeTime = 0.0f;
	float damageTime = 0.0f;
	float gaugeTime = 0.0f;
	float damage = 0;
	float heal = 0;

	bool isBattleStart = false;
	bool isBattleEnd = false;
	bool playerTurn = false;
	bool enemyTurn = false;
	bool isMonsterBall = false;
	bool isHealing = false;
	bool playerPokemonDead = false;
	bool enemyPokemonDead = false;
	bool playerPokemonSound = false;
	bool enemyPokemonSound = false;
	bool isPlayerWin = false;
	bool isEnemyWin = false;
	bool skillPlaying = false;
	bool damageGlow = false;
	bool isDamaging = false;
	bool fadeOut = false;
};