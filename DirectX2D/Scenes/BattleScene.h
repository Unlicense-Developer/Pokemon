#pragma once
class BattleScene : public Scene
{
private:
    const Float4 ORIGINAL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Float4 DARKER_COLOR = { 0.1f, 0.1, 0.1f, 1.0f };
    const Vector2 PLAYER_POS = { 200.0f, 360.0f };
    const Vector2 ENEMY_POS = { 620.0f, 580.0f };
    const Float2 TEXT_POS1 = { CENTER_X - 10.0f , CENTER_Y - 260.0f };
    const Float2 TEXT_POS2 = { CENTER_X - 10.0f , CENTER_Y - 340.0f };
    const Float2 TEXTPOS_SIZE = { 700.0f, 200.0f };

public:
	BattleScene();
	~BattleScene();

    virtual void Update() override;
    virtual void Render() override;

    virtual void Start() override;
    virtual void End() override;

private:
    void CreateSounds();
    void CreateClips();
    void CheckBGMStart();
    void CheckPokemonSoundPlay();
    void PrintText();

    void ControlPlayerImage();
    void CheckEnemyImagePos();

    void UpdateHpBar();

private:
    Quad* battleBG;
    Quad* textBox;
    Quad* enemyBallUI;
    Quad* playerBallUI;
    Quad* enemyTrainerBallUI;
    Quad* playerHpBar;
    Quad* enemyHpBar;
    AnimObject* appearEffect;

    bool isEnemyPokemonSet = false;
    bool isEnemyPokemonCanSet = false;
    bool isEnemyTrainerSet = false;
    bool isPlayerPokemonSet = false;
    bool isPlayerPokemonCanSet = false;
    bool playerPokemonSoundPlay = false;
    bool enemyPokemonSoundPlay = false;
};