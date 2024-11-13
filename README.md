# 🏆 Pokemon Gold
[![image](https://github.com/user-attachments/assets/b8040b5c-4bda-4f3a-b6a6-ad5a0a4eceef)](https://youtu.be/6odGXikL14s?si=1S30ZiOp6lN63BWx)
* **개발 기간** : 2023. 02. 02 ~ 2023. 02. 19 / 18일
* **게임 장르** : 턴제 RPG
* **개발 인원** : 1인
* **핵심 기술** : DirectX11, FMOD, ImGui, WinAPI
* **Notion** : https://www.notion.so/unlicense-developer/Pokemon-Gold-9d64326c0cb94a1e9dc292d05eba354d?pvs=4
<br/>

# 📕 Content
* ImGui 인터페이스를 통해 Tile을 배치하여 만든 맵을 Binary 데이터로 Save, Load할 수 있는 에디터 구현
* Tile마다 Type을 구별하여 장애물, 포탈 등을 구현
* Sprite Texture로 캐릭터의 이동 애니메이션을 구현
* 포켓몬 데이터를 가지고 있는 CSV 형식 엑셀 파일을 Load하여 인게임에서 포켓몬에 데이터 적용
* Shader 코딩을 통해 포켓몬의 체력비율로 ColorBuffer 값을 조절하여 ProgressBar의 상태를 구현
* 프레임 최적화를 위해 Frustum Culling을 통해 카메라 밖의 오브젝트를 출력하지 않게 구현
<br/>

![Untitled (2)](https://github.com/user-attachments/assets/9d85b6b5-454e-4b50-af0a-f0d8e80eeaf6)
