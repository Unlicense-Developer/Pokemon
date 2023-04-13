#pragma once
class IntroScene : public Scene
{
private:
	const float CLOUD_SPEED = 22.0f;
public:
	IntroScene();
	~IntroScene();

	virtual void Update() override;
	virtual void Render() override;

	virtual void Start() override;
	virtual void End() override;

	void MoveClouds();
	void CreatehoOhClips();
	void CreateGameFreakClips();
	void CheckGameStart();
	void CreateStartUI();
	
	void PrintUIText();

private:
	Quad* introBG;
	Quad* copyRight;
	vector<Quad*> clouds;
	AnimObject* hoOh;
	AnimObject* gameFreak;

	Quad* startUI;
};