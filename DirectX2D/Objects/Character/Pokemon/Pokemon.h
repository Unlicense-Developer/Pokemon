#pragma once

struct PokemonStat
{
	int pokemonNum;
	string pokemonName;
	string type1;
	string type2;
	//종족값
	float h; // 체력
	float a; // 공격
	float b; // 방어
	float c; // 특공
	float d; // 특방 
	float s; // 속도

	wstring frontImage;
	wstring backImage;
	wstring iconImage;
};

struct PokemonSkill
{
	int pokemonNum;

	string skill1Name;
	string skill1Type;
	string skill1AttackType;
	int skill1Power;
	float skill1Acc;
	int skill1PP;

	string skill2Name;
	string skill2Type;
	string skill2AttackType;
	int skill2Power;
	float skill2Acc;
	int skill2PP;

	string skill3Name;
	string skill3Type;
	string skill3AttackType;
	int skill3Power;
	float skill3Acc;
	int skill3PP;

	string skill4Name;
	string skill4Type;
	string skill4AttackType;
	int skill4Power;
	float skill4Acc;
	int skill4PP;
};

class Pokemon : public GameObject
{
private:
	struct BattleStat
	{
		//실수치
		int attack;
		int block;
		int contact;
		int defense;
		int speed;
	};

public:
	Pokemon(int pokeNum);
	~Pokemon();

	void Update();
	void Render();

	Quad* GetFront() { return front; }
	Quad* GetBack() { return back; }
	Quad* GetIcon() { return icon; }
	Quad* GetListHpbar() { return listHpbar; }
	ProgressBar* GetHpGauge() { return hpGauge; }
	PokemonStat GetPokemonStat() { return stat; }
	BattleStat GetBattleStat() { return battleStat; }
	PokemonSkill GetSkill() { return skill; }
	int GetLevel() { return lv; }
	int GetExp() { return exp; }
	int& GetCurHp() { return curHp; }
	int& GetMaxHp() { return maxHp; }
	float GetHpRate();
	string GetSkillName(int skillNum);
	string GetSkillType(int skillNum);
	string GetSkillAttackType(int skillNum);
	int GetSkillPower(int skillNum);
	float GetSkillAcc(int skillNum);
	int GetSkillPP(int skillNum);

	void SetPokemonStat(int pokeNum);
	void SetPokemonSkill(int pokeNum);
	void SetPokemonTexture();
	void SetIndividualStat();
	void SetFrontOffset(Vector2 frontOffset) { this->frontOffset = frontOffset; }
	void SetBackOffset(Vector2 backOffset) { this->backOffset = backOffset; }
	void SetIconOffset(Vector2 iconOffset) { this->iconOffset = iconOffset; }
	void SetListHpBarOffset(Vector2 listHpbarOffset) { this->listHpbarOffset = listHpbarOffset; }
	void SetHpGaugeOffset(Vector2 HpGaugeOffset) { this->hpGaugeOffset = HpGaugeOffset; }

	void LevelUp();
	void UpdateHpBar();

private:
	PokemonStat stat;
	PokemonSkill skill;
	BattleStat battleStat;
	//개체값
	float indiv_h; // 체력
	float indiv_a; // 공격 
	float indiv_b; // 방어
	float indiv_c; // 특공
	float indiv_d; // 특방
	float indiv_s; // 속도

	int lv;
	int curHp;
	int maxHp;
	int exp;

	Quad* front;
	Quad* back;
	Quad* icon;
	Quad* listHpbar;

	ProgressBar* hpGauge;

	Vector2 frontOffset = { 0, 0 };
	Vector2 backOffset = { 0, 0 };
	Vector2 iconOffset = { 0, 0 };
	Vector2 listHpbarOffset = { 0, 0 };
	Vector2 hpGaugeOffset = { 0, 0 };
};