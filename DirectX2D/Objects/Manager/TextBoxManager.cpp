#include "Framework.h"

TextBoxManager::TextBoxManager()
{
	textBox = new Quad(L"Textures/UI/TextBox.png");
	textBox->Pos() = { CENTER_X, CENTER_Y };
	textBox->Scale() *= 5.0f;
	textBox->UpdateWorld();
	textBox->SetActive(false);
}

TextBoxManager::~TextBoxManager()
{
	delete textBox;
}

void TextBoxManager::Render()
{
	textBox->Render();
}
