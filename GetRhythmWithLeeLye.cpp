#include <bangtal.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <queue>
using namespace std;

const int SONG_NUM = 100; //수록곡 수
const int JUDGE_NUM = 4; //Perfect, Good, Bad, Miss
const int MAX_CIPHER = 10; //최대 자릿수
const int DECIMAL_X = 32;
const int DECIMAL_Y = 44;
const int RANK_NUM = 4; //A, B, C, D
const int NOTE_NUM = 30; //화면에 존재할 노트 수
const int CREDIT_NUM = 10; //sCredit 페이지 수
const int BUTTON_Y[5] = { 520, 420, 320, 220, 110 };
const float TICK = 0.01f; //tick 주기

int SPEED = 5; // 속도 = 5 pixel/tick

/**
* s		: Scene
* o		: Object-default
* but	: Object-Button
* t		: Timer
*/

ObjectID butReturn; /* 뒤로가기 버튼 */

SceneID sMain;
ObjectID butMain[4];
ObjectID oTitle;

SceneID sSelect;
ObjectID butSelect[5];
ObjectID oInfo;

SceneID sIngame;
ObjectID oNote[NOTE_NUM], oJudge[JUDGE_NUM], oAllPerfect, oFullCombo;
ObjectID oCombo[MAX_CIPHER];
bool bAllPerfect, bFullCombo;

SceneID sHTPlay;

SceneID sCredit;
ObjectID oCredit[CREDIT_NUM];

SceneID sResult;
ObjectID oRank[RANK_NUM];
/* use Decimal[] */

TimerID tTick, tCredit[3];

SoundID Songs[SONG_NUM];

enum class nowScene : int {
	sMain = 0,
	sSelect,
	sIngame,
	sHTPlay,
	sCredit,
	sResult
};

nowScene NowScene = nowScene::sMain;
int nowPointInMain = 0;
int nowPointInSelect = 0;
string buf;
bool bIngame = false;

void changePointOfMain(string UpDown) {
	locateObject(butMain[nowPointInMain], sMain, 880, BUTTON_Y[nowPointInMain]);
	buf = "Images/sMain/" + to_string(nowPointInMain) + "N.png";
	setObjectImage(butMain[nowPointInMain], buf.c_str());

	if (UpDown == "Up") {
		nowPointInMain--;
		if (nowPointInMain < 0) nowPointInMain = 3;
	}
	else if (UpDown == "Down") {
		nowPointInMain++;
		if (nowPointInMain > 3) nowPointInMain = 0;
	}

	locateObject(butMain[nowPointInMain], sMain, 760, BUTTON_Y[nowPointInMain]);
	buf = "Images/sMain/" + to_string(nowPointInMain) + "P.png";
	setObjectImage(butMain[nowPointInMain], buf.c_str());
}
void changePointOfSelect(string UpDown) {
	locateObject(butSelect[nowPointInSelect], sSelect, 880, BUTTON_Y[nowPointInSelect]);
	buf = "Images/sSelect/" + to_string(nowPointInSelect) + "N.png";
	setObjectImage(butSelect[nowPointInSelect], buf.c_str());

	if (UpDown == "Up") {
		nowPointInSelect--;
		if (nowPointInSelect < 0) nowPointInSelect = 4;
	}
	else if (UpDown == "Down") {
		nowPointInSelect++;
		if (nowPointInSelect > 4) nowPointInSelect = 0;
	}

	locateObject(butSelect[nowPointInSelect], sSelect, 760, BUTTON_Y[nowPointInSelect]);
	buf = "Images/sSelect/" + to_string(nowPointInSelect) + "P.png";
	setObjectImage(butSelect[nowPointInSelect], buf.c_str());
	buf = "Images/sSelect/" + to_string(nowPointInSelect) + "I.png";
	setObjectImage(oInfo, buf.c_str());
}

void keyCallback(KeyCode key, KeyState state) {
	if (state == KeyState::KEY_RELEASED)
		return;
	switch (NowScene) {
	case nowScene::sMain:
		if (key == KeyCode::KEY_UP_ARROW)
			changePointOfMain("Up");
		else if (key == KeyCode::KEY_DOWN_ARROW)
			changePointOfMain("Down");
		else if (key == KeyCode::KEY_ENTER) {
			switch (nowPointInMain) {
			case 0:
				NowScene = nowScene::sSelect;
				enterScene(sSelect);
				break;
			case 1:
				NowScene = nowScene::sHTPlay;
				enterScene(sHTPlay);
				break;
			case 2:
				startTimer(tCredit[0]);
				NowScene = nowScene::sCredit;
				enterScene(sCredit);
				break;
			case 3:
				endGame();
				break;
			}
		}
		break;

	case nowScene::sSelect:
		if (key == KeyCode::KEY_UP_ARROW)
			changePointOfSelect("Up");
		else if (key == KeyCode::KEY_DOWN_ARROW)
			changePointOfSelect("Down");
		else if (key == KeyCode::KEY_ESCAPE) {
			NowScene = nowScene::sMain;
			enterScene(sMain);
		}
		else if (key == KeyCode::KEY_ENTER) {
			NowScene = nowScene::sIngame;
			enterScene(sIngame);
			playSound(Songs[nowPointInSelect]);
			bIngame = true;
			startTimer(tTick);
		}
		break;

	case nowScene::sIngame:
		break;

	case nowScene::sHTPlay:
		if (key == KeyCode::KEY_ESCAPE) {
			NowScene = nowScene::sMain;
			enterScene(sMain);
		}
		break;

	case nowScene::sCredit:
		if (key == KeyCode::KEY_ESCAPE) {
			for (int i = 0; i < 3; i++) {
				stopTimer(tCredit[i]);
			}
			setSceneImage(sCredit, "Images/sCredit/sCredit0.png");
			NowScene = nowScene::sMain;
			enterScene(sMain);
		}
		break;

	case nowScene::sResult:
		if (key == KeyCode::KEY_ESCAPE) {
			NowScene = nowScene::sSelect;
			enterScene(sSelect);
		}
		else if (key == KeyCode::KEY_ENTER) {
			NowScene = nowScene::sIngame;
			enterScene(sIngame);
			playSound(Songs[nowPointInSelect]);
		}
		break;
	}
}

void timerCallback(TimerID timer) {
	if (timer == tTick) {


		if (bIngame)
			startTimer(tTick);
	}
	else if (timer == tCredit[0]) {
		startTimer(tCredit[1]);
		setSceneImage(sCredit, "Images/sCredit/sCredit1.png");
	}
	else if (timer == tCredit[1]) {
		startTimer(tCredit[2]);
		setSceneImage(sCredit, "Images/sCredit/sCredit2.png");
	}
	else if (timer == tCredit[2]) {
		setSceneImage(sCredit, "Images/sCredit/sCredit0.png");
		NowScene = nowScene::sMain;
		enterScene(sMain);
	}
}

/* 음악이 끝나면 콜백되는 함수 => loop가 true이면 호출 X */
void soundCallback(SoundID sound) {
	NowScene = nowScene::sResult;
	
}

/*
void printNumber(Combo target, int num, SceneID scene, int x, int y) {
	int tmp[MAX_CIPHER] = { 0 };

}
void moveObject(ObjectID object, int sx, int sy, int ex, int ey) {

}
*/

SceneID createObject(string filename, SceneID scene, int x, int y, bool shown) {
	ObjectID tmp = createObject(filename.c_str());
	locateObject(tmp, scene, x, y);
	if (shown) showObject(tmp);
	return tmp;
}

SceneID initGameSetting() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setKeyboardCallback(keyCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);

	//Create Scene
	sMain	= createScene("sMain",			"Images/sMain/sMain.png");
	sSelect	= createScene("sSelect",		"Images/sSelect/sSelect.png");
	sHTPlay	= createScene("sHTPlay",		"Images/sHTPlay/sHTPlay.png");
	sCredit	= createScene("sCredit",		"Images/sCredit/sCredit0-1.png");
	sIngame	= createScene("sIngame",		"Images/sIngame/sIngame.png");
	sResult	= createScene("sResult",		"Images/sResult/sResult.png");

	//sMain Object
	butMain[0]	= createObject("Images/sMain/0P.png",		sMain, 760, 520, true);
	butMain[1]	= createObject("Images/sMain/1N.png",		sMain, 880, 420, true);
	butMain[2]	= createObject("Images/sMain/2N.png",		sMain, 880, 320, true);
	butMain[3]	= createObject("Images/sMain/3N.png",		sMain, 880, 220, true);
	oTitle		= createObject("Images/sMain/oTitle.png",	sMain, 50,  270, true);
	
	//sSelect Object
	butSelect[0] = createObject("Images/sSelect/0P.png",	sSelect, 760, 520, true);
	butSelect[1] = createObject("Images/sSelect/1N.png",	sSelect, 880, 420, true);
	butSelect[2] = createObject("Images/sSelect/2N.png",	sSelect, 880, 320, true);
	butSelect[3] = createObject("Images/sSelect/3N.png",	sSelect, 880, 220, true);
	butSelect[4] = createObject("Images/sSelect/4N.png",	sSelect, 880, 120, true);
	oInfo		 = createObject("Images/sSelect/0I.png",	sSelect, 50,  270, true);
	
	//sHtPlay Object
	//None

	//sCredit Object
	//None

	//sIngame Object
	for (int i = 0; i < NOTE_NUM; i++)
		oNote[i] = createObject("Images/sIngame/oNote.png");
	oAllPerfect = createObject("Images/sIngame/oAllPerfect.png", sIngame, 50, 500, false);
	oFullCombo	= createObject("Images/sIngame/oFullCombo.png", sIngame, 40, 500, false); //shown=false
	for (int i = 0; i < MAX_CIPHER; i++)
		oCombo[i] = createObject("Images/Decimal/o0.png");

	//sREsult Object
	oRank[0] = createObject("Images/sResult/A.png", sResult, 50, 50, false);
	oRank[1] = createObject("Images/sResult/B.png", sResult, 50, 50, false);
	oRank[2] = createObject("Images/sResult/C.png", sResult, 50, 50, false);
	oRank[3] = createObject("Images/sResult/D.png", sResult, 50, 50, false);

	tTick = createTimer(TICK);
	tCredit[0] = createTimer(2.0f);
	tCredit[1] = createTimer(5.0f);
	tCredit[2] = createTimer(5.0f);

	//system("GetRhythmWithLeeLye.bat");
	Songs[0] = createSound("Songs/Aimai.mp3");
	Songs[1] = createSound("Songs/Balsam.mp3");
	Songs[2] = createSound("Songs/Cherry.mp3");
	Songs[3] = createSound("Songs/HEIntro.mp3");
	Songs[4] = createSound("Songs/LentLily.mp3");
	//system("GetRhythmWithLeeLye.bat");

	return sMain;
}

int main() {
	startGame(initGameSetting());
}