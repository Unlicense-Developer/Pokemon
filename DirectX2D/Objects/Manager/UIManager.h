#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	const string POKEMON = "포켓몬";
	const string BAG = "가방";
	const string POKEGEAR = "포켓기어";
	const string STATUS = "상태";
	const string REPORT = "레포트";
	const string SETTING = "설정";
	const string CLOSE = "닫다";
	const string QUIT = "그만두다";
	const string BACK = "돌아가다";
	const string ITEM_CHOOSE = "도구를 고르다";
	const string ITEMS = "아이템";
	const string MONSTERBALL = "몬스터볼";
	const string POTION = "상처약";
	const string SUPERPOTION = "좋은 상처약";
	const string POKEMON_DESC = "같이 있는 포켓몬의 상태";
	const string INVEN_DESC = "도구를 집어넣는 포켓 낚시배낭";
	const string GEAR_DESC = "트레이너의 여행에 도움이 되는 툴";
	const string STATUS_DESC = "현재 당신의 상태";
	const string SAVE_DESC = "잠시 쉬는 동안 상태를 기록";
	const string SETTING_DESC = "시합의 룰 등의 여러가지 변경";
	const string CLOSE_DESC = "이 메뉴를 닫는다";
	const string GO_STAT = "강한정도를 보다";
	const string CHANGE_ORDER = "순서바꾸기";
	const string GO_SKILL = "사용할 수 있는기술";
	const string TYPE = "타입";
	const string ATTACK = "공격";
	const string BLOCK = "방어";
	const string CONTACT = "특수공격";
	const string DEFENSE = "특수방어";
	const string SPEED = "스피드";
	const string FIGHT = "싸우다";
	const string ESCAPE = "도망치다";
	const string SKILL_TYPE = "기술타입";

	friend class Singleton;

	UIManager();
	~UIManager();

public:
	void Update();
	void Render();

	bool IsPanelListActive();
	void OffBattlePanel();
	Panel* GetFrontPanelTop() { return frontPanel.top(); }
	Panel* SearchFrontPanel(string key) { return totalPaneldatas.at(key); }

private:
	void CreatePanels();
	void AddPanel(string key, wstring panelTextureFile, int indexNum = 0, bool cursor = false,
		Vector2 cursorPos = Vector2(0, 0), float interval = 0.0f, Vector2 offset = Vector2(0, 0));
	void SetFrontPanel(string panelKey);
	void OffFrontPanel();
	void UpdatePanelIndex();
	void ControlPanel();
	void PrintText();
	void PrintPokemonTexture();
	void PrintUIBGTexture();

	void CreateBattlePanels();
	void AddBattlePanel(string key, wstring panelTextureFile, bool setWidth = false, int indexNum = 0, bool cursor = false,
		Vector2 cursorPos = Vector2(0, 0), float interval = 0.0f, Vector2 offset = Vector2(0, 0));
	void SetBattleFrontPanel(string panelKey);
	void ControlBattlePanel();
	void PrintBattleText();

private:
	vector<Panel*> totalPanels;
	vector<Panel*> totalBattlePanels;
	unordered_map<string, Panel*> totalPaneldatas;
	stack<Panel*> frontPanel;

	int pokemon_IndexNum = 0;
};