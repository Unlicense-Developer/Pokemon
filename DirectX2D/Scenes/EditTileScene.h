#pragma once

class EditTileScene : public Scene
{
private:
    const float SAMPLE_SIZE = 48.0f;

public:
    EditTileScene();
    ~EditTileScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;

private:
    void ClickSampleBtn(void* sampleBtn);

    void CreateSample();

    void Save();
    void Load();

private:
    vector<Button*> sampleBtns;

    EditTileMap* editTileMap;

    Quad* selectSample = nullptr;

    string projectPath;

    Tile::Type selectType = Tile::BG;
};