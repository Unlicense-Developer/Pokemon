#include "Framework.h"
#include "IntroScene.h"

IntroScene::IntroScene()
{
	introBG = new Quad(L"Textures/Intro/IntroBG.png");
	introBG->Pos() = { CENTER_X, CENTER_Y };
	introBG->UpdateWorld();

	copyRight = new Quad(L"Textures/Intro/IntroCopyRight.png");
	copyRight->Pos() = { CENTER_X, copyRight->GetSize().y * 0.5f + 5.0f };
	copyRight->UpdateWorld();

	Audio::Get()->Add("Intro", "Sounds/Intro.mp3", true);
}

IntroScene::~IntroScene()
{
	delete introBG;
	delete copyRight;
	
}

void IntroScene::Update()
{
	MoveClouds();
	hoOh->Update();
	gameFreak->Update();

	if (KEY_DOWN('Z'))
	{
		SceneManager::Get()->ChangeScene("Pokemon");
	}
}

void IntroScene::Render()
{
	introBG->Render();

	for (Quad* cloud : clouds)
		cloud->Render();

	hoOh->Render();
	copyRight->Render();

	if (!gameFreak->GetCurClip()->IsPlay())
	{
		gameFreak->SetActive(false);
	}
	else
	{
		gameFreak->Render();
	}
}

void IntroScene::Start()
{
	clouds.resize(2);

	for (int i = 0; i < clouds.size(); i++)
	{
		clouds[i] = new Quad(L"Textures/Intro/Cloud.png");
		clouds[i]->Pos() = { CENTER_X - i * WIN_WIDTH, clouds[i]->GetSize().y * 0.5f };
	}

	hoOh = new AnimObject();
	hoOh->Pos() = { CENTER_X, CENTER_Y - 80.0f };
	hoOh->UpdateWorld();

	gameFreak = new AnimObject();
	gameFreak->Pos() = { CENTER_X, CENTER_Y };
	gameFreak->Scale() *= 5.0f;
	gameFreak->UpdateWorld();

	CreatehoOhClips();
	CreateGameFreakClips();

	Audio::Get()->Play("Intro", 0.4f);
}

void IntroScene::End()
{
	Audio::Get()->Stop("Intro");

	for (Quad* cloud : clouds)
		delete cloud;

	delete hoOh;
	delete gameFreak;
}

void IntroScene::MoveClouds()
{
	for (Quad* cloud : clouds)
	{
		cloud->Pos().x += CLOUD_SPEED * DELTA;

		if (cloud->Pos().x - cloud->GetSize().x * 0.5f >= WIN_WIDTH)
		{
			cloud->Pos().x = -cloud->GetSize().x * 0.5f;
		}

		cloud->UpdateWorld();
	}
}

void IntroScene::CreatehoOhClips()
{
	wstring hoOhTexture = L"Textures/Intro/hoOh.png";
	Vector2 hoOhSize = Texture::Add(hoOhTexture)->GetSize() / Vector2(5, 1);

	vector<Frame*> frames;
	frames.push_back(new Frame(hoOhTexture, hoOhSize.x * 0, 0, hoOhSize.x, hoOhSize.y));
	frames.push_back(new Frame(hoOhTexture, hoOhSize.x * 1, 0, hoOhSize.x, hoOhSize.y));
	frames.push_back(new Frame(hoOhTexture, hoOhSize.x * 2, 0, hoOhSize.x, hoOhSize.y));
	frames.push_back(new Frame(hoOhTexture, hoOhSize.x * 3, 0, hoOhSize.x, hoOhSize.y));
	frames.push_back(new Frame(hoOhTexture, hoOhSize.x * 4, 0, hoOhSize.x, hoOhSize.y));
	hoOh->AddClip("hoOh", new Clip(frames, true, 0.5f));

	hoOh->SetClip("hoOh");
}

void IntroScene::CreateGameFreakClips()
{
	string path = "Textures/Intro/";
	string file = "GameFreak.xml";

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	document->LoadFile((path + file).c_str());

	tinyxml2::XMLElement* atlas = document->FirstChildElement();
	string textureFile = path + atlas->Attribute("imagePath");

	vector<Frame*> frames;
	tinyxml2::XMLElement* sprite = atlas->FirstChildElement();

	while (sprite != nullptr)
	{
		float x, y, w, h;
		x = sprite->FloatAttribute("x");
		y = sprite->FloatAttribute("y");
		w = sprite->FloatAttribute("w");
		h = sprite->FloatAttribute("h");

		frames.push_back(new Frame(ToWString(textureFile), x, y, w, h));

		sprite = sprite->NextSiblingElement();
	}

	Clip* clip = new Clip(frames, false, 1.4f);
	gameFreak->AddClip("GameFreak", clip);

	delete document;

	gameFreak->SetClip("GameFreak");
}

void IntroScene::PrintUIText()
{
	//Font::Get()->RenderText
}
