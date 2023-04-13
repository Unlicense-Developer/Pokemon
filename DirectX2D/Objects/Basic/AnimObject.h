#pragma once

class AnimObject : public GameObject
{
public:
    AnimObject();
    ~AnimObject();

    void Update();
    void Render();

    void AddClip(string key, Clip* clip) { clips[key] = clip; }

    void SetClip(string key);
    void RemoveCurClip() { this->curClip = nullptr; }
    Clip*& GetCurClip() { return curClip; }

protected:
    ColorBuffer* colorBuffer;

    map<string, Clip*> clips;
    Clip* curClip = nullptr;
};