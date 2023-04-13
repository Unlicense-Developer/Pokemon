#pragma once
class TextBoxManager : public Singleton<TextBoxManager>
{
private:
	friend class Singleton;

	TextBoxManager();
	~TextBoxManager();

public:
	void Render();

	Quad* GetTextBox() { return textBox; }

private:
	Quad* textBox;
};