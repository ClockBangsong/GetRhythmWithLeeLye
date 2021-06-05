#include <bangtal.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
using namespace std;
const int SONG_NUM = 100; //수록곡 수
const int JUDGE_NUM = 4; //Perfect, Good, Bad, Miss
const int MAX_CIPHER = 10; //최대 자릿수
const int DECIMAL_X = 32;
const int DECIMAL_Y = 44;
const int RANK_NUM = 4; //A, B, C, D
const int NOTE_NUM = 30; //화면에 존재할 노트 수
const int CREDIT_NUM = 10; //sCredit 페이지 수
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
ObjectID butPlay, butRecord, butSetting, butCredit, butExit;
ObjectID oTitle;

SceneID sSelect;
ObjectID butAimai, butBalsam, butHEIntro, butLentLily, butCherry;

SceneID sIngame;
ObjectID oNote[NOTE_NUM], oJudge[JUDGE_NUM], oAllPerfect, oFullCombo;
ObjectID oCombo[MAX_CIPHER];
bool bAllPerfect = true, bFullCombo = true;

SceneID sRecord;
ObjectID oScore[MAX_CIPHER];
int Score = 0;

SceneID sHowToPlay;

SceneID sCredit;
ObjectID oCredit[CREDIT_NUM];

SceneID sResult;
ObjectID oRank[RANK_NUM];
/* use Decimal[] */

TimerID tTick;

SoundID Sound[SONG_NUM];

bool bIngame = false;
bool bSync = false;

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	playSound(Sound[0], false);
}

void keyCallback(KeyCode key, KeyState state) {
	if (key == KeyCode::KEY_ESCAPE && state == KeyState::KEY_PRESSED) {
		playSound(Sound[0], false);
	}
}

void timerCallback(TimerID timer) {
	if (bIngame) {

	}
	else if (bSync) {


	}

	startTimer(tTick);
}

/* 음악이 끝나면 콜백되는 함수 => loop가 true이면 호출 X */
void soundCallback(SoundID sound) {
	
}

//void printNumber(int num, SceneID scene, int x, int y, char align) {

//}

void moveObject(ObjectID object, int sx, int sy, int ex, int ey) {

}

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
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);

	//Create Scene
	sMain		= createScene("sMain",			"Images/sMain/sMain.png");
	sSelect		= createScene("sSelect",		"Images/sSelect/sSelect.png");
	sRecord		= createScene("sRecord",		"Images/sRecord/sRecord.png");
	sHowToPlay	= createScene("sHowToPlay",		"Images/sHowToPlay/sHowToPlay.png");
	sCredit		= createScene("sCredit",		"Images/sCredit/sCredit.png");
	sIngame		= createScene("sIngame",		"Images/sIngame/sIngame.png");
	sResult		= createScene("sResult",		"Images/sResult/sResult.png");

	//sMain Object
	butPlay		= createObject("Images/sMain/butPlayPoint.png",			sMain, 760, 520, true);
	butRecord	= createObject("Images/sMain/butRecordNotPoint.png",	sMain, 880, 420, true);
	butSetting	= createObject("Images/sMain/butSettingNotPoint.png",	sMain, 880, 320, true);
	butCredit	= createObject("Images/sMain/butCreditNotPoint.png",	sMain, 880, 220, true);
	butExit		= createObject("Images/sMain/butExitNotPoint.png",		sMain, 880, 120, true);
	oTitle		= createObject("Images/sMain/oTitle.png",				sMain, 50,  270, true);
	
	//sSelect Object
	butAimai	= createObject("Images/sSelect/butAimai.png",	sSelect, 880, 520, true);
	butBalsam	= createObject("Images/sSelect/butBalsam.png",	sSelect, 880, 420, true);
	butCherry	= createObject("Images/sSelect/butCherry.png",	sSelect, 880, 320, true);
	butHEIntro	= createObject("Images/sSelect/butHEIntro.png",	sSelect, 880, 220, true);
	butLentLily = createObject("Images/sSelect/butLentLily.png",sSelect, 880, 120, true);
	
	//sIngame Object
	for (int i = 0; i < NOTE_NUM; i++)
		oNote[i] = createObject("Images/sIngame/oNote.png");
	oAllPerfect = createObject("Images/sIngame/oAllPerfect.png", sIngame, 50, 500, true);
	oFullCombo	= createObject("Images/sIngame/oFullCombo.png", sIngame, 40, 500, false); //shown=false
	for (int i = 0; i < MAX_CIPHER; i++)
		oCombo[i] = createObject("Images/Decimal/o0.png");
	
	//sRecord Object
	for (int i = 0; i < MAX_CIPHER; i++)
		oScore[i] = createObject("Images/Decimal/o0.png");

	//sHowtoPlay Object
	//None
	
	//sCredit Object
	string buf;
	for (int i = 0; i < CREDIT_NUM; i++) {
		buf = "Images/sCredit/credit" + to_string(i) + ".png";
		oCredit[i] = createObject(buf.c_str());
	}
	
	//sREsult Object
	oRank[0] = createObject("Images/sResult/A.png", sResult, 50, 50, false);
	oRank[1] = createObject("Images/sResult/B.png", sResult, 50, 50, false);
	oRank[2] = createObject("Images/sResult/C.png", sResult, 50, 50, false);
	oRank[3] = createObject("Images/sResult/D.png", sResult, 50, 50, false);

	tTick = createTimer(TICK);

	//system("GetRhythmWithLeeLye.bat");
	Sound[0] = createSound("Songs/Aimai.mp3");
	Sound[1] = createSound("Songs/Balsam.mp3");
	Sound[2] = createSound("Songs/Cherry.mp3");
	Sound[3] = createSound("Songs/HEIntro.mp3");
	Sound[4] = createSound("Songs/LentLily.mp3");
	//system("GetRhythmWithLeeLye.bat");

	return sMain;
	//return sIngame;
}

int main() {
	startGame(initGameSetting());
}