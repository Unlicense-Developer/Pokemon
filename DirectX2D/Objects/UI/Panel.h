#pragma once

class Panel : public Quad
{
public:
    Panel(wstring textureFile, int indexNum = 0, bool makeCursor = false, Vector2 PosOffset = Vector2(0, 0));
    ~Panel();

    void Update();
    void Render();

    void Show();
    void Close();

    void ChangeIndexNum(int indexNum);
    UICursor* GetCursor() { return cursor; }

private:
    Vector2 offset;
    UICursor* cursor = nullptr;
    Vector2 Interval;
};