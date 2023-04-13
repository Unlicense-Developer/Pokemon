#include "Framework.h"

UICursor::UICursor(int menuIndexSize)
	:Quad(L"Textures/UI/UICursor.png")
{
	isActive = false;
	curIndexSize = menuIndexSize;
}

UICursor::~UICursor()
{
}

void UICursor::Update()
{
	ControlCursor();
	UpdateCursorPos();
}

void UICursor::Render()
{
	Quad::Render();
}

void UICursor::UpdateCursorPos()
{
	if (CUR_SCENE == SceneManager::Get()->GetScene("Pokemon"))
	{
		Pos() = CAM->Pos() + Vector2(defaultPos.x, defaultPos.y - (curIndex - 1) * interval);
	}
	else if (CUR_SCENE == SceneManager::Get()->GetScene("Battle"))
	{
		if (moveWidth)
		{
			if (curIndex == 1 || curIndex == 2)
				Pos() = Vector2(defaultPos.x + (curIndex - 1) * interval, defaultPos.y);
			else if (curIndex == 3 || curIndex == 4)
				Pos() = Vector2(defaultPos.x + (curIndex - 3) * interval, defaultPos.y - 80.0f);
		}
		else
			Pos() = Vector2(defaultPos.x, defaultPos.y - (curIndex - 1) * interval);
	}

	UpdateWorld();
}

void UICursor::ControlCursor()
{
	if (!isActive) return;

	if (moveWidth)
	{
		if (KEY_DOWN(VK_DOWN))
		{
			if (curIndex == 1 || curIndex == 2)
				curIndex += 2;		
		}
		else if (KEY_DOWN(VK_UP))
		{
			if (curIndex == 3 || curIndex == 4)
				curIndex -= 2;
		}
		else if (KEY_DOWN(VK_LEFT))
		{
			if (curIndex == 2 || curIndex == 4)
				curIndex -= 1;
		}
		else if (KEY_DOWN(VK_RIGHT))
		{
			if (curIndex == 1 || curIndex == 3)
				curIndex += 1;
		}
	}
	else
	{
		if (KEY_DOWN(VK_DOWN))
		{
			if (curIndex == curIndexSize)
				curIndex = 1;
			else
				curIndex++;
		}
		else if (KEY_DOWN(VK_UP))
		{
			if (curIndex == 1)
				curIndex = curIndexSize;
			else
				curIndex--;
		}
	}
}

void UICursor::SetCursorInterval(Vector2 pos, float interval)
{
	defaultPos = pos;
	this->interval = interval;
}
