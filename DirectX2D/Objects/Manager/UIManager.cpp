#include "Framework.h"

UIManager::UIManager()
{
	CreatePanels();
	CreateBattlePanels();
}

UIManager::~UIManager()
{
	for (pair<string, Panel*> panel : totalPaneldatas)
		delete panel.second;

	for (int i = 0; i < frontPanel.size(); i++)
		frontPanel.pop();
}

void UIManager::Update()
{
	UpdatePanelIndex();

	if (frontPanel.size() >= 1)
	{
		frontPanel.top()->Update();
	}
	
	if(CUR_SCENE == SceneManager::Get()->GetScene("Pokemon"))
		ControlPanel();
	else if(CUR_SCENE == SceneManager::Get()->GetScene("Battle"))
		ControlBattlePanel();
}

void UIManager::Render()
{
	if (frontPanel.size() == 0) return;

	if (CUR_SCENE == SceneManager::Get()->GetScene("Pokemon"))
	{
		for (Panel* panel : totalPanels)
		{
			if (panel == totalPaneldatas.at("PokemonOption") &&
				FRONT_PANEL("PokemonOption"))
			{
				PrintText();
				PrintUIBGTexture();
			}
			panel->Render();
		}

		PrintPokemonTexture();
		PrintText();
	}
	else if (CUR_SCENE == SceneManager::Get()->GetScene("Battle"))
	{
		for (Panel* panel : totalBattlePanels)
		{
			panel->Render();
		}

		PrintBattleText();
	}
}

void UIManager::CreatePanels()
{
	AddPanel("Menu", L"Textures/UI/menu.png", 7, true, Vector2(450.0f, 630.0f), 90.0f);
	AddPanel("PokemonList", L"Textures/UI/PokemonList.png", 1, true, Vector2(20.0f, 680.0f), 83.0f);
	AddPanel("PokemonOption", L"Textures/UI/PokemonOption.png", 4, true, Vector2(310.0f, 380.0f), 100.0f, 
		Vector2(132.5f, -130.0f));
	AddPanel("Pokemon_Type", L"Textures/UI/Pokemon_Type.png");
	AddPanel("Pokemon_Skill", L"Textures/UI/Pokemon_Skill.png");
	AddPanel("Pokemon_Stat", L"Textures/UI/Pokemon_Stat.png");
	AddPanel("Pokemon_Skilldesc", L"Textures/UI/Pokemon_Skilldesc.png");
	AddPanel("Bag_Items", L"Textures/UI/Bag_Items.png", 4, true, Vector2(270, 600), 100.0f);
	AddPanel("Pokegear", L"Textures/UI/Pokegear.png");
	AddPanel("Status", L"Textures/UI/Status.png");
	AddPanel("Save", L"Textures/UI/Save.png", 2, true);
}

void UIManager::CreateBattlePanels()
{
	AddBattlePanel("Battle_Select", L"Textures/UI/Battle_Select.png", true, 4, true, Vector2(380, 150), 200.0f, Vector2(CENTER_X + 157, 115));
	AddBattlePanel("Battle_SkillSelect", L"Textures/UI/Battle_SkillSelect.png", false, 4, true, Vector2(60, 315), 85.0f, Vector2(400.0f, 200.0f));
	AddBattlePanel("Battle_Bag_Items", L"Textures/UI/Bag_Items.png", false, 4, true, Vector2(270, 600), 100.0f, Vector2(400, 360));
	AddBattlePanel("Battle_PokemonList", L"Textures/UI/PokemonList.png", false, 1, true, Vector2(20.0f, 680.0f), 83.0f);
}

void UIManager::AddBattlePanel(string key, wstring panelTextureFile, bool setWidth, int indexNum, bool cursor, Vector2 cursorPos, float interval, Vector2 offset)
{
	Panel* panel = new Panel(panelTextureFile, indexNum, cursor, offset);

	if (cursor)
		panel->GetCursor()->SetCursorInterval(cursorPos, interval);

	if (setWidth)
		panel->GetCursor()->SetMoveWidth(true);

	totalPaneldatas[key] = panel;
	totalBattlePanels.push_back(panel);
}

void UIManager::AddPanel(string key, wstring panelTextureFile, int indexNum, bool cursor, Vector2 cursorPos, float interval, Vector2 offset)
{
	Panel* panel = new Panel(panelTextureFile, indexNum, cursor, offset);

	if(cursor)
		panel->GetCursor()->SetCursorInterval(cursorPos, interval);

	totalPaneldatas[key] = panel;
	totalPanels.push_back(panel);
}

bool UIManager::IsPanelListActive()
{
	if (frontPanel.size() >= 1)
		return true;

	return false;
}

void UIManager::SetFrontPanel(string panelKey)
{
	totalPaneldatas.at(panelKey)->Show();
	frontPanel.push(totalPaneldatas.at(panelKey));
}

void UIManager::SetBattleFrontPanel(string panelKey)
{
	totalPaneldatas.at(panelKey)->Show();
	frontPanel.push(totalPaneldatas.at(panelKey));
}

void UIManager::OffFrontPanel()
{
	if(CUR_PANEL->GetCursor())
		FRONT_CURSOR_INDEX = 1;
	
	if (FRONT_PANEL("PokemonList"))
	{
		for (int i = 0; i < MY_POKEMONS.size(); i++)
		{
			MY_POKEMONS[i]->GetListHpbar()->SetActive(false);
			MY_POKEMONS[i]->GetIcon()->SetActive(false);
			MY_POKEMONS[i]->GetHpGauge()->SetActive(false);
		}
	}
	else if (FRONT_PANEL("Pokemon_Type") ||
		FRONT_PANEL("Pokemon_Skill") || 
		FRONT_PANEL("Pokemon_Stat"))
	{
		MY_POKEMONS[pokemon_IndexNum]->GetFront()->SetActive(false);
		MY_POKEMONS[pokemon_IndexNum]->GetHpGauge()->SetActive(false);
	}
		

	frontPanel.top()->Close();
	frontPanel.pop();
}

void UIManager::OffBattlePanel()
{
	for (Panel* panel : totalBattlePanels)
		panel->Close();

	int panelSize = frontPanel.size();

	for(int i = 0; i < panelSize; i++)
	{
		frontPanel.pop();
	}
}

void UIManager::UpdatePanelIndex()
{
	totalPaneldatas.at("PokemonList")->ChangeIndexNum(MY_POKEMONS.size() + 1);
}

void UIManager::ControlPanel()
{
	if (PlayerManager::Get()->GetPlayer()->GetIsMove()) return;

	if (KEY_DOWN(VK_RETURN))
	{
		if (frontPanel.size() == 0)
		{
			SetFrontPanel("Menu");
		}
	}

	if (frontPanel.size() == 0) return;

	if (FRONT_PANEL("Menu"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 1:
				SetFrontPanel("PokemonList");
				break;
			case 2:
				SetFrontPanel("Bag_Items");
				break;
			case 3:
				SetFrontPanel("Pokegear");
				break;
			case 4:
				SetFrontPanel("Status");
				break;
			case 5:
				SetFrontPanel("Save");
				break;
			case 7:
				OffFrontPanel();
				break;

			}
		}

	}
	else if(FRONT_PANEL("PokemonList"))
	{
		if (KEY_DOWN('Z'))
		{
			if (FRONT_CURSOR_INDEX == FRONT_INDEX_SIZE)
			{
				OffFrontPanel();
				return;
			}

			pokemon_IndexNum = FRONT_CURSOR_INDEX - 1;
			SetFrontPanel("PokemonOption");
		}

	}
	else if (FRONT_PANEL("PokemonOption"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 1:
				SetFrontPanel("Pokemon_Type");
				break;
			case 2:
				OffFrontPanel();
				break;
			case 3:
				SetFrontPanel("Pokemon_Skilldesc");
				break;
			case 4:
				OffFrontPanel();
				break;
			}

		}

	}
	else if (FRONT_PANEL("Pokemon_Type"))
	{
		if (KEY_DOWN(VK_LEFT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Stat");
		}
		else if (KEY_DOWN(VK_RIGHT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Skill");
		}
	}
	else if (FRONT_PANEL("Pokemon_Skill"))
	{
		if (KEY_DOWN(VK_LEFT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Type");
		}
		else if (KEY_DOWN(VK_RIGHT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Stat");
		}
	}
	else if (FRONT_PANEL("Pokemon_Stat"))
	{
		if (KEY_DOWN(VK_LEFT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Skill");
		}
		else if (KEY_DOWN(VK_RIGHT))
		{
			OffFrontPanel();
			SetFrontPanel("Pokemon_Type");
		}
	}
	else if (FRONT_PANEL("Bag_Items"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 4:
				OffFrontPanel();
				break;
			}

		}
	}

	if (KEY_DOWN('X'))
	{
		if (frontPanel.size() >= 1)
		{
			OffFrontPanel();
		}
	}
}

void UIManager::PrintText()
{
	if (FRONT_PANEL("Menu"))
	{
		Font::Get()->SetColor("Black");
		Font::Get()->SetStyle("PokemonFont_90");

		Font::Get()->RenderText(POKEMON, { 600, 635 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(BAG, { 600, 545 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(POKEGEAR, { 600, 455 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(STATUS, { 600, 365 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(REPORT, { 600, 275 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(SETTING, { 600, 185 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(CLOSE, { 600, 95 }, { 200.0f, 100.0f });

		Font::Get()->SetStyle("PokemonFont_75");

		switch (FRONT_CURSOR_INDEX)
		{
		case 1:
			Font::Get()->RenderText(POKEMON_DESC, { 145, 150 }, { 235.0f, 100.0f });
			break;
		case 2:
			Font::Get()->RenderText(INVEN_DESC, { 175, 150 }, { 300.0f, 100.0f });
			break;
		case 3:
			Font::Get()->RenderText(GEAR_DESC, { 190, 150 }, { 330.0f, 100.0f });
			break;
		case 4:
			Font::Get()->RenderText(STATUS_DESC, { 150, 150 }, { 235.0f, 100.0f });
			break;
		case 5:
			Font::Get()->RenderText(SAVE_DESC, { 180, 150 }, { 300.0f, 100.0f });
			break;
		case 6:
			Font::Get()->RenderText(SETTING_DESC, { 170, 150 }, { 280.0f, 100.0f });
			break;
		case 7:
			Font::Get()->RenderText(CLOSE_DESC, { 200, 150 }, { 340.0f, 100.0f });
			break;

		}
	}
	else if (FRONT_PANEL("PokemonList")|| FRONT_PANEL("PokemonOption"))
	{
		for (int i = 0; i < MY_POKEMONS.size() + 1; i++)
		{
			Font::Get()->SetStyle("PokemonFont_75");

			if (i == MY_POKEMONS.size())
			{
				Font::Get()->RenderText(QUIT, { 240, 680 - MY_POKEMONS.size() * 83.0f}, {340.0f, 100.0f});
			}
			else
			{
				Font::Get()->RenderText(MY_POKEMONS[i]->GetPokemonStat().pokemonName, { 330, 670 - i * 80.0f }, { 340.0f, 100.0f });

				if (FRONT_PANEL("PokemonOption") && i >= 3)
					continue;

				Font::Get()->SetStyle("PokemonFont_60");

				Font::Get()->RenderText(to_string(MY_POKEMONS[i]->GetLevel()), { 408, 649 - i * 80.0f }, { 100.0f, 100.0f });

				if (MY_POKEMONS[i]->GetCurHp() >= 100)
					Font::Get()->RenderText(to_string(MY_POKEMONS[i]->GetCurHp()), { 580, 691 - i * 80.0f }, { 100.0f, 100.0f });
				else
					Font::Get()->RenderText(to_string(MY_POKEMONS[i]->GetCurHp()), { 610, 691 - i * 80.0f }, { 100.0f, 100.0f });

				if (MY_POKEMONS[i]->GetMaxHp() >= 100)
					Font::Get()->RenderText(to_string(MY_POKEMONS[i]->GetMaxHp()), { 710, 691 - i * 80.0f }, { 100.0f, 100.0f });
				else
					Font::Get()->RenderText(to_string(MY_POKEMONS[i]->GetMaxHp()), { 740, 691 - i * 80.0f }, { 100.0f, 100.0f });
			}
		}

		if (FRONT_PANEL("PokemonOption"))
		{
			Font::Get()->SetStyle("PokemonFont_75");

			Font::Get()->RenderText(GO_STAT, { 490, 380 }, { 300.0f, 100.0f });
			Font::Get()->RenderText(CHANGE_ORDER, { 490, 280 }, { 300.0f, 100.0f });
			Font::Get()->RenderText(GO_SKILL, { 540, 180 }, { 400.0f, 100.0f });
			Font::Get()->RenderText(BACK, { 490, 80 }, { 300.0f, 100.0f });
		}

	}
	else if (FRONT_PANEL("Pokemon_Type") ||
		FRONT_PANEL("Pokemon_Skill") ||
		FRONT_PANEL("Pokemon_Stat"))
	{
		Font::Get()->SetStyle("PokemonFont_60");
		
		if(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonNum < 10)
			Font::Get()->RenderText("00"+ to_string(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonNum), {450, 691}, {100.0f, 100.0f});
		else if (MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonNum < 100)
			Font::Get()->RenderText("0" + to_string(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonNum), { 450, 691 }, { 100.0f, 100.0f });
		else
			Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonNum), { 450, 691 }, { 100.0f, 100.0f });
			
		Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetLevel()), { 654, 691 }, { 100.0f, 100.0f });
		
		Font::Get()->SetStyle("PokemonFont_75");
		Font::Get()->RenderText(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonName, { 450, 620 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().pokemonName, { 520, 550 }, { 200.0f, 100.0f });
			
		if (FRONT_PANEL("Pokemon_Type"))
		{
			Font::Get()->SetStyle("PokemonFont_60");
			
			if(MY_POKEMONS[pokemon_IndexNum]->GetCurHp() < 100)
				Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetCurHp()), { 150, 291 }, { 100.0f, 100.0f });
			else	
				Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetCurHp()), { 130, 291 }, { 100.0f, 100.0f });
			
			if (MY_POKEMONS[pokemon_IndexNum]->GetMaxHp() < 100)
				Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetMaxHp()), { 310, 291 }, { 100.0f, 100.0f });
			else
				Font::Get()->RenderText(to_string(MY_POKEMONS[pokemon_IndexNum]->GetMaxHp()), { 290, 291 }, { 100.0f, 100.0f });
		
			Font::Get()->SetStyle("PokemonFont_75");
			Font::Get()->RenderText(TYPE, { 130, 146 }, { 100.0f, 100.0f });
			Font::Get()->RenderText(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().type1, {331, 146}, {200.0f, 100.0f});
			
			if(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().type2 != "NONE")
				Font::Get()->RenderText(MY_POKEMONS[pokemon_IndexNum]->GetPokemonStat().type2, {281, 66}, {100.0f, 100.0f});
			
		}
		else if (FRONT_PANEL("Pokemon_Skill"))
		{

		}
		else if (FRONT_PANEL("Pokemon_Stat"))
		{
			Font::Get()->SetStyle("PokemonFont_75");
			Font::Get()->RenderText(ATTACK, { 480, 360 }, { 100.0f, 100.0f });
			Font::Get()->RenderText(BLOCK, { 480, 280 }, { 100.0f, 100.0f });
			Font::Get()->RenderText(CONTACT, { 580, 200 }, { 300.0f, 100.0f });
			Font::Get()->RenderText(DEFENSE, { 580, 120 }, { 300.0f, 100.0f });
			Font::Get()->RenderText(SPEED, { 530, 40 }, { 200.0f, 100.0f });

			Font::Get()->SetStyle("PokemonFont_60");
			Font::Get()->RenderText(to_string(int(MY_POKEMONS[pokemon_IndexNum]->GetBattleStat().attack)), {750, 345}, {100.0f, 100.0f});
			Font::Get()->RenderText(to_string(int(MY_POKEMONS[pokemon_IndexNum]->GetBattleStat().block)), {750, 265}, {100.0f, 100.0f});
			Font::Get()->RenderText(to_string(int(MY_POKEMONS[pokemon_IndexNum]->GetBattleStat().contact)), {750, 185}, {100.0f, 100.0f});
			Font::Get()->RenderText(to_string(int(MY_POKEMONS[pokemon_IndexNum]->GetBattleStat().defense)), {750, 105}, {100.0f, 100.0f});
			Font::Get()->RenderText(to_string(int(MY_POKEMONS[pokemon_IndexNum]->GetBattleStat().speed)), {750, 25}, {100.0f, 100.0f});
		}
	}
	else if (FRONT_PANEL("Bag_Items"))
	{
		Font::Get()->SetColor("Magenta");
		Font::Get()->SetStyle("PokemonFont_50");

		Font::Get()->RenderText(ITEM_CHOOSE, { 400, 682 }, { 300.0f, 100.0f });

		Font::Get()->SetStyle("PokemonFont_75");
		Font::Get()->SetColor("White");

		Font::Get()->RenderText(ITEMS, { 145, 380 }, { 200.0f, 100.0f });

		Font::Get()->SetColor("Black");

		Font::Get()->RenderText(MONSTERBALL, { 450, 600 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(POTION, { 450, 500 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(SUPERPOTION, { 450, 400 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(QUIT, { 450, 300}, { 300.0f, 100.0f});

		Font::Get()->SetStyle("PokemonFont_60");
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetMonsterball()), { 720, 590}, {200.0f, 100.0f});
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetPotion()), { 720, 490}, {200.0f, 100.0f});
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetSuperPotion()), { 720, 390}, {200.0f, 100.0f});

		switch (FRONT_CURSOR_INDEX)
		{
		case 1:
			Font::Get()->RenderText("야생 포켓몬에게 던져서 잡기 위한 볼", {390, 100}, {700.0f, 200.0f});
			Font::Get()->RenderText("캡슐식으로 되어 있다", {390, 20}, {700.0f, 200.0f});
			break;
		case 2:
			Font::Get()->RenderText("체력을 20 회복시킨다", { 390, 100 }, { 700.0f, 200.0f });
			break;
		case 3:
			Font::Get()->RenderText("체력을 50 회복시킨다", { 390, 100 }, { 700.0f, 200.0f });
			break;
		}
	}
	else if (FRONT_PANEL("Pokegear"))
	{
		time_t curTime = time(NULL);
		struct tm* local = localtime(&curTime);

		Font::Get()->SetStyle("PokemonFont_60");

		Font::Get()->RenderText(to_string(local->tm_year - 100) + "/" + to_string(local->tm_mon + 1) + "/" + to_string(local->tm_mday), { 430, 450 }, { 340.0f, 100.0f });

		int day = local->tm_wday;

		switch (day)
		{
		case 0:
			Font::Get()->RenderText("일요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 1:
			Font::Get()->RenderText("월요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 2:
			Font::Get()->RenderText("화요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 3:
			Font::Get()->RenderText("수요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 4:
			Font::Get()->RenderText("목요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 5:
			Font::Get()->RenderText("금요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		case 6:
			Font::Get()->RenderText("토요일", { 650, 450 }, { 340.0f, 100.0f });
			break;
		}

		Font::Get()->RenderText(to_string(local->tm_hour), { 430, 372 }, { 340.0f, 100.0f });

		if (local->tm_min < 10)
			Font::Get()->RenderText("0" + to_string(local->tm_min), { 535, 372 }, { 340.0f, 100.0f });
		else
			Font::Get()->RenderText(to_string(local->tm_min), { 535, 372 }, { 340.0f, 100.0f });

		if (local->tm_sec < 10)
			Font::Get()->RenderText("0" + to_string(local->tm_sec), { 645, 372 }, { 340.0f, 100.0f });
		else
			Font::Get()->RenderText(to_string(local->tm_sec), { 645, 372 }, { 340.0f, 100.0f });

		Font::Get()->SetStyle("PokemonFont_75");

		Font::Get()->RenderText("버튼을 누르면 포켓기어를 종료합니다", { 240, 150 }, { 360.0f, 100.0f });
	}
}

void UIManager::PrintBattleText()
{
	if (BattleManager::Get()->IsBattleEnd()) return;

	if (FRONT_PANEL("Battle_Select") )
	{
		Font::Get()->SetStyle("PokemonFont_75");
		Font::Get()->RenderText(FIGHT, { 500, 150 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(BAG, { 700, 150 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(POKEMON, { 500, 70 }, { 200.0f, 100.0f });
		Font::Get()->RenderText(ESCAPE, { 700, 70 }, { 200.0f, 100.0f });
	}
	else if (FRONT_PANEL("Battle_SkillSelect"))
	{
		Font::Get()->SetStyle("PokemonFont_75");
		Font::Get()->RenderText(MY_FIRST_POKEMON->GetSkill().skill1Name, { 230, 319}, {300.0f, 100.0f});
		Font::Get()->RenderText(MY_FIRST_POKEMON->GetSkill().skill2Name, { 230, 234}, {300.0f, 100.0f});

		if(MY_FIRST_POKEMON->GetSkill().skill3Name == "NONE")
			Font::Get()->RenderText("-", { 230, 149}, {300.0f, 100.0f});
		else
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetSkill().skill3Name, { 230, 149}, {300.0f, 100.0f});

		if (MY_FIRST_POKEMON->GetSkill().skill4Name == "NONE")
			Font::Get()->RenderText("-", { 230, 64}, {300.0f, 100.0f});
		else
			Font::Get()->RenderText(MY_FIRST_POKEMON->GetSkill().skill4Name, { 230, 64}, {300.0f, 100.0f});
			
		Font::Get()->RenderText(SKILL_TYPE, { 570, 100 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(MY_FIRST_POKEMON->GetSkillType(FRONT_CURSOR_INDEX), { 720, 65 }, {200.0f, 100.0f});

		Font::Get()->SetStyle("PokemonFont_60");
		Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetSkillPP(FRONT_CURSOR_INDEX)), { 640, 171 }, { 100.0f, 100.0f});
		Font::Get()->RenderText(to_string(MY_FIRST_POKEMON->GetSkillPP(FRONT_CURSOR_INDEX)), { 735, 171 }, { 100.0f, 100.0f });
	}
	else if (FRONT_PANEL("Battle_Bag_Items"))
	{
		Font::Get()->SetColor("Magenta");
		Font::Get()->SetStyle("PokemonFont_50");

		Font::Get()->RenderText(ITEM_CHOOSE, { 400, 682 }, { 300.0f, 100.0f });

		Font::Get()->SetStyle("PokemonFont_75");
		Font::Get()->SetColor("White");

		Font::Get()->RenderText(ITEMS, { 145, 380 }, { 200.0f, 100.0f });

		Font::Get()->SetColor("Black");

		Font::Get()->RenderText(MONSTERBALL, { 450, 600 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(POTION, { 450, 500 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(SUPERPOTION, { 450, 400 }, { 300.0f, 100.0f });
		Font::Get()->RenderText(QUIT, { 450, 300 }, { 300.0f, 100.0f });

		Font::Get()->SetStyle("PokemonFont_60");
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetMonsterball()), { 720, 590 }, { 200.0f, 100.0f });
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetPotion()), { 720, 490 }, { 200.0f, 100.0f });
		Font::Get()->RenderText("X   " + to_string(PlayerManager::Get()->GetSuperPotion()), { 720, 390 }, { 200.0f, 100.0f });

		switch (FRONT_CURSOR_INDEX)
		{
		case 1:
			Font::Get()->RenderText("야생 포켓몬에게 던져서 잡기 위한 볼", { 390, 100 }, { 700.0f, 200.0f });
			Font::Get()->RenderText("캡슐식으로 되어 있다", { 390, 20 }, { 700.0f, 200.0f });
			break;
		case 2:
			Font::Get()->RenderText("체력을 20 회복시킨다", { 390, 100 }, { 700.0f, 200.0f });
			break;
		case 3:
			Font::Get()->RenderText("체력을 50 회복시킨다", { 390, 100 }, { 700.0f, 200.0f });
			break;
		}
	}
}

void UIManager::ControlBattlePanel()
{
	if (frontPanel.size() == 0 && BattleManager::Get()->IsBattle() && !BattleManager::Get()->IsSkillPlaying() &&
		!BattleManager::Get()->IsBattleEnd() && !BattleManager::Get()->GetIsHealing())
	{
		if (BattleManager::Get()->IsPlayerTurn() && !BattleManager::Get()->IsDamaging())
		{
			SetBattleFrontPanel("Battle_Select");
		}
		else if (BattleManager::Get()->IsEnemyTurn())
		{
			BattleManager::Get()->EnemyAttack();
		}
	}

	if (MY_FIRST_POKEMON->GetBack()->Active() && KEY_DOWN('Z') && frontPanel.size() == 0 )
	{
		BattleManager::Get()->StartBattle();
	}
	else if (frontPanel.size() > 0 && FRONT_PANEL("Battle_Select"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 1:
				SetBattleFrontPanel("Battle_SkillSelect");
				break;
			case 2:
				SetBattleFrontPanel("Battle_Bag_Items");
				break;
			}
		}
	}
	else if (frontPanel.size() > 0 && FRONT_PANEL("Battle_SkillSelect"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 1:
				BattleManager::Get()->PlayerAttack(1);
				break;
			case 2:
				BattleManager::Get()->PlayerAttack(2);
				break;
			case 3:
				if (MY_FIRST_POKEMON->GetSkill().skill3Name != "NONE")
				{
					BattleManager::Get()->PlayerAttack(3);
					break;
				}
			case 4:
				if (MY_FIRST_POKEMON->GetSkill().skill4Name != "NONE")
				{
					BattleManager::Get()->PlayerAttack(4);
					break;
				}
			}

			OffBattlePanel();
		}
	}
	else if (frontPanel.size() > 0 && FRONT_PANEL("Battle_Bag_Items"))
	{
		if (KEY_DOWN('Z'))
		{
			switch (FRONT_CURSOR_INDEX)
			{
			case 1:
				BattleManager::Get()->MonsterBall();
				break;
			case 2:
				BattleManager::Get()->Potion(20);
				break;
			case 3:
				BattleManager::Get()->Potion(50);
				break;
			case 4:
				OffBattlePanel();
				break;
			}

			OffBattlePanel();
		}
	}

	if (KEY_DOWN('X'))
	{
		if (frontPanel.size() >= 1)
		{
			OffFrontPanel();
		}
	}
}

void UIManager::PrintPokemonTexture()
{
	if (FRONT_PANEL("PokemonList"))
	{
		for (int i = 0; i < MY_POKEMONS.size(); i++)
		{
			MY_POKEMONS[i]->GetListHpbar()->SetActive(true);
			MY_POKEMONS[i]->GetIcon()->SetActive(true);
			MY_POKEMONS[i]->GetHpGauge()->SetActive(true);


			if(FRONT_CURSOR_INDEX == i + 1)
				MY_POKEMONS[i]->SetIconOffset(Vector2(100, 680 - i * 80.0f));
			else
				MY_POKEMONS[i]->SetIconOffset(Vector2(50, 680 - i * 80.0f));

			MY_POKEMONS[i]->SetListHpBarOffset(Vector2(545, 680 - i * 80.0f));
			MY_POKEMONS[i]->SetHpGaugeOffset(Vector2(630, 660 - i * 80.0f));

			MY_POKEMONS[i]->GetListHpbar()->Render();
			MY_POKEMONS[i]->GetIcon()->Render();
			MY_POKEMONS[i]->GetHpGauge()->Render();
		}
	}
	else if (FRONT_PANEL("Pokemon_Type")||
		FRONT_PANEL("Pokemon_Skill") || 
		FRONT_PANEL("Pokemon_Stat"))
	{
		if (FRONT_PANEL("Pokemon_Type"))
		{
			MY_POKEMONS[pokemon_IndexNum]->GetHpGauge()->SetActive(true);
			MY_POKEMONS[pokemon_IndexNum]->SetHpGaugeOffset(Vector2(200, 340));
			MY_POKEMONS[pokemon_IndexNum]->GetHpGauge()->Render();
			MY_POKEMONS[pokemon_IndexNum]->GetHpGauge()->UpdateWorld();
		}

		MY_POKEMONS[pokemon_IndexNum]->GetFront()->SetActive(true);

		MY_POKEMONS[pokemon_IndexNum]->Pos() = CAM->GlobalPos() + Vector2(150, 580);

		MY_POKEMONS[pokemon_IndexNum]->GetFront()->Rot().y = XM_PI;

		MY_POKEMONS[pokemon_IndexNum]->GetFront()->Render();
		MY_POKEMONS[pokemon_IndexNum]->GetHpGauge()->Render();
	}
}

void UIManager::PrintUIBGTexture()
{
	if (FRONT_PANEL("PokemonOption"))
	{
		for (int i = 0; i < MY_POKEMONS.size(); i++)
		{
			MY_POKEMONS[i]->GetListHpbar()->SetActive(true);
			MY_POKEMONS[i]->GetIcon()->SetActive(true);
			MY_POKEMONS[i]->GetHpGauge()->SetActive(true);

			MY_POKEMONS[i]->SetIconOffset(Vector2(50, 680 - i * 80.0f));
			MY_POKEMONS[i]->SetListHpBarOffset(Vector2(545, 680 - i * 80.0f));
			MY_POKEMONS[i]->SetHpGaugeOffset(Vector2(630, 660 - i * 80.0f));

			MY_POKEMONS[i]->GetListHpbar()->Render();
			MY_POKEMONS[i]->GetIcon()->Render();
			MY_POKEMONS[i]->GetHpGauge()->Render();
		}
	}
}