#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	const string POKEMON = "���ϸ�";
	const string BAG = "����";
	const string POKEGEAR = "���ϱ��";
	const string STATUS = "����";
	const string REPORT = "����Ʈ";
	const string SETTING = "����";
	const string CLOSE = "�ݴ�";
	const string QUIT = "�׸��δ�";
	const string BACK = "���ư���";
	const string ITEM_CHOOSE = "������ ����";
	const string ITEMS = "������";
	const string MONSTERBALL = "���ͺ�";
	const string POTION = "��ó��";
	const string SUPERPOTION = "���� ��ó��";
	const string POKEMON_DESC = "���� �ִ� ���ϸ��� ����";
	const string INVEN_DESC = "������ ����ִ� ���� ���ù賶";
	const string GEAR_DESC = "Ʈ���̳��� ���࿡ ������ �Ǵ� ��";
	const string STATUS_DESC = "���� ����� ����";
	const string SAVE_DESC = "��� ���� ���� ���¸� ���";
	const string SETTING_DESC = "������ �� ���� �������� ����";
	const string CLOSE_DESC = "�� �޴��� �ݴ´�";
	const string GO_STAT = "���������� ����";
	const string CHANGE_ORDER = "�����ٲٱ�";
	const string GO_SKILL = "����� �� �ִ±��";
	const string TYPE = "Ÿ��";
	const string ATTACK = "����";
	const string BLOCK = "���";
	const string CONTACT = "Ư������";
	const string DEFENSE = "Ư�����";
	const string SPEED = "���ǵ�";
	const string FIGHT = "�ο��";
	const string ESCAPE = "����ġ��";
	const string SKILL_TYPE = "���Ÿ��";

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