#include "Framework.h"

Pokemon::Pokemon(int pokeNum)
{
	front = new Quad(Vector2(56, 56));
	front->Scale() *= 5.0f;
	front->SetActive(false);
	front->SetParent(this);

	back = new Quad(Vector2(48, 48));
	back->Scale() *= 5.0f;
	back->SetActive(false);
	back->SetParent(this);

	icon = new Quad(Vector2(16, 16));
	icon->Scale() *= 5.0f;
	icon->SetActive(false);
	icon->SetParent(this);

	listHpbar = new Quad(L"Textures/UI/PokemonList_HPBar.png");
	listHpbar->Scale() *= 5.0f;
	listHpbar->SetActive(false);

	hpGauge = new ProgressBar();
	hpGauge->SetActive(false);

	SetIndividualStat();
	SetPokemonStat(pokeNum);
	SetPokemonSkill(pokeNum);
	SetPokemonTexture();
}

Pokemon::~Pokemon()
{
	delete front;
	delete back;
	delete icon;
}

void Pokemon::Update()
{
	if (curHp < 0)
		curHp = 0;

	front->Pos() = Pos();
	back->Pos() = Pos();
	icon->Pos() = CAM->GlobalPos() + iconOffset;
	listHpbar->Pos() = CAM->GlobalPos() + listHpbarOffset;
	hpGauge->Pos() = CAM->GlobalPos() + hpGaugeOffset;

	front->UpdateWorld();
	back->UpdateWorld();
	icon->UpdateWorld();
	listHpbar->UpdateWorld();
	hpGauge->UpdateWorld();

	UpdateHpBar();
}

void Pokemon::Render()
{
	front->Render();
	back->Render();
}

float Pokemon::GetHpRate()
{
	return float(curHp) / float(maxHp);
}

string Pokemon::GetSkillName(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1Name;
	case 2:
		return skill.skill2Name;
	case 3:
		return skill.skill3Name;
	case 4:
		return skill.skill4Name;
	}
}

string Pokemon::GetSkillType(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1Type;
	case 2:
		return skill.skill2Type;
	case 3:
		return skill.skill3Type;
	case 4:
		return skill.skill4Type;
	}
}

string Pokemon::GetSkillAttackType(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1AttackType;
	case 2:
		return skill.skill2AttackType;
	case 3:
		return skill.skill3AttackType;
	case 4:
		return skill.skill4AttackType;
	}
}

int Pokemon::GetSkillPower(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1Power;
	case 2:
		return skill.skill2Power;
	case 3:
		return skill.skill3Power;
	case 4:
		return skill.skill4Power;
	}
}

float Pokemon::GetSkillAcc(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1Acc;
	case 2:
		return skill.skill2Acc;
	case 3:
		return skill.skill3Acc;
	case 4:
		return skill.skill4Acc;
	}
}

int Pokemon::GetSkillPP(int skillNum)
{
	switch (skillNum)
	{
	case 1:
		return skill.skill1PP;
	case 2:
		return skill.skill2PP;
	case 3:
		return skill.skill3PP;
	case 4:
		return skill.skill4PP;
	}
}

void Pokemon::SetPokemonStat(int pokeNum)
{
	stat = PokemonDataManager::Get()->GetPokemonStat(pokeNum);

	//lv = Random(5, 51);
	lv = 30;

	maxHp = (int)((stat.h * 2 + indiv_h) / 2 * lv / 50) + 10 + lv;
	battleStat.attack = (int)((stat.a * 2 + indiv_a) / 2 * lv / 50) + 5;
	battleStat.block = (int)((stat.b * 2 + indiv_b) / 2 * lv / 50) + 5;
	battleStat.contact = (int)((stat.c * 2 + indiv_c) / 2 * lv / 50) + 5;
	battleStat.defense = (int)((stat.d * 2 + indiv_d) / 2 * lv / 50) + 5;
	battleStat.speed = (int)((stat.s * 2 + indiv_s) / 2 * lv / 50) + 5;
	
	curHp = maxHp;

	exp = 0;
}

void Pokemon::SetPokemonSkill(int pokeNum)
{
	skill = PokemonDataManager::Get()->GetPokemonSkill(pokeNum);
}

void Pokemon::SetPokemonTexture()
{
	front->SetTexture(stat.frontImage);
	back->SetTexture(stat.backImage);
	icon->SetTexture(stat.iconImage);
}

void Pokemon::SetIndividualStat()
{
	indiv_h = Random(0, 32);
	indiv_a = Random(0, 32);
	indiv_b = Random(0, 32);
	indiv_c = Random(0, 32);
	indiv_d = Random(0, 32);
	indiv_s = Random(0, 32);
}

void Pokemon::LevelUp()
{
	lv++;

	curHp += (int)((stat.h * 2 + indiv_h) / 2 * lv / 50) + 10 + lv - maxHp;

	maxHp = (int)((stat.h * 2 + indiv_h) / 2 * lv / 50) + 10 + lv;
	battleStat.attack = (int)((stat.a * 2 + indiv_a) / 2 * lv / 50) + 5;
	battleStat.block = (int)((stat.b * 2 + indiv_b) / 2 * lv / 50) + 5;
	battleStat.contact = (int)((stat.c * 2 + indiv_c) / 2 * lv / 50) + 5;
	battleStat.defense = (int)((stat.d * 2 + indiv_d) / 2 * lv / 50) + 5;
	battleStat.speed = (int)((stat.s * 2 + indiv_s) / 2 * lv / 50) + 5;


}

void Pokemon::UpdateHpBar()
{
	if (GetHpRate() >= 0.6f)
		hpGauge->GetColor() = { 0.0f, 0.72f, 0.0f, 1.0f };
	else if (GetHpRate() >= 0.25f)
		hpGauge->GetColor() = { 0.97f, 0.69f, 0.0f, 1.0f };
	else if (GetHpRate() > 0.0f)
		hpGauge->GetColor() = { 0.97f, 0.0f, 0.0f, 1.0f };


	hpGauge->SetAmount(GetHpRate());
}
