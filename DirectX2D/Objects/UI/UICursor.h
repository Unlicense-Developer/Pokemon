#pragma once
class UICursor : public Quad
{
private:
	const string MENU = "Menu";
	const string POKEMONLIST = "PokemonList";
	const string PLAYERSTATUS = "PlayerStatus";
public:
	UICursor(int menuIndexSize);
	~UICursor();

	void Update();
	void Render();

	void UpdateCursorPos();
	void ControlCursor();
	void SetMoveWidth(bool moveWidth) { this->moveWidth = moveWidth; }
	void ChangeIndexSize(int indexNum) { this->curIndexSize = indexNum; }
	void SetCursorInterval(Vector2 pos, float interval);

	int& GetCurIndex() { return curIndex; }
	int GetIndexSize() { return curIndexSize; }

private:
	int curIndex = 1;
	string curMenu;
	int curIndexSize;
	Vector2 defaultPos;
	float interval;

	bool moveWidth = false;
};