#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Update() = 0;

    virtual void PreRender() {}
    virtual void Render() = 0;
    virtual void PostRender() {}

    virtual void Start() {}
    virtual void End() {}
};