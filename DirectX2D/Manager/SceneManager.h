#pragma once

class SceneManager : public Singleton<SceneManager>
{
private:
    friend class Singleton;

    SceneManager() = default;
    ~SceneManager();

public:
    void Update();
    void Render();

    void PreRender();
    void PostRender();

    void Add(string key, Scene* scene);

    Scene* GetCurScene() { return curScene; }
    Scene* GetScene(string key);
    void ChangeScene(string key);

private:
    map<string, Scene*> scenes;

    Scene* curScene = nullptr;
};