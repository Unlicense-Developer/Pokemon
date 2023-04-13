#include "Framework.h"

Panel::Panel(wstring textureFile, int indexNum, bool makeCursor, Vector2 PosOffset)
    : Quad(textureFile), offset(PosOffset)
{
    Scale() *= 5.0f;
    isActive = false;

    if (makeCursor)
    {
        cursor = new UICursor(indexNum);
    }
}

Panel::~Panel()
{
    delete cursor;
}

void Panel::Update()
{
    if(cursor)
        cursor->Update();
    
    if(CUR_SCENE == SceneManager::Get()->GetScene("Pokemon"))
        Pos() = CAM->GlobalPos() + Vector2(CENTER_X, CENTER_Y) + offset;
    else if (CUR_SCENE == SceneManager::Get()->GetScene("Battle"))
        Pos() = offset;

    UpdateWorld();
}

void Panel::Render()
{
    Quad::Render();

    if (cursor)
        cursor->Render();
}

void Panel::Show()
{
    isActive = true;

    if(cursor)
        cursor->SetActive(true);
}

void Panel::Close()
{
    isActive = false;

    if (cursor)
        cursor->SetActive(false);
}

void Panel::ChangeIndexNum(int indexNum)
{
    cursor->ChangeIndexSize(indexNum);
}
