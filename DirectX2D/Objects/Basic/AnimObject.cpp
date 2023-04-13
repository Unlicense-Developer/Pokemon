#include "Framework.h"

AnimObject::AnimObject()
{
    colorBuffer = new ColorBuffer();
}

AnimObject::~AnimObject()
{
    for (pair<string, Clip*> clip : clips)
        delete clip.second;

    delete colorBuffer;
}

void AnimObject::Update()
{
    if (curClip == nullptr) return;

    curClip->Update();
    UpdateWorld();
}

void AnimObject::Render()
{
    if (!isActive) return;
    if (curClip == nullptr) return;

    colorBuffer->SetPS(0);
    GameObject::SetRender();

    curClip->Render();
}

void AnimObject::SetClip(string key)
{
    if (curClip != clips[key])
    {
        curClip = clips[key];
        curClip->Play();
    }
}


