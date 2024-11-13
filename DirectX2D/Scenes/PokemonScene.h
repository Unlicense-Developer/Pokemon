#pragma once

class PokemonScene : public Scene
{
private:
    const Float4 ENCOUNTER_COLOR = { 0.45f, 1.0f, 0.0f, 0.785f };
    const Float4 TRAINER_ENCOUNTER_COLOR = { 1.0f, 0.21f, 0.11f, 0.785f };
    const float ENCOUNTER_RED_VALUE = 2.5f;
    const float ENCOUNTER_GREEN_VALUE = 5.5f;
public:
    PokemonScene();
    ~PokemonScene();

    virtual void Update() override;
    virtual void Render() override;
    virtual void PreRender() override;
    virtual void PostRender() override;

    virtual void Start() override;
    virtual void End() override;

private:
    void CAMSetting();
    void CreateSounds();
    void CreateWipeClips();
    void CheckEncounter();

private:
    AnimObject* encounterWipe;
    Quad* encounterGlow;

    Vector2 playerCoord = { 0, 0 };

    float redValue = 0.0f;
    float greenValue = 0.0f;
    int valueCount = 0;
};