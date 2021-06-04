#include <bangtal.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
using namespace std;
const int SELECT_LIMIT = 100; //수록곡 수
const int JUDGE_NUM = 4; //Perfect, Good, Bad, Miss
const int DECIMAL_NUM = 10; //0 ~ 10
const int RANK_NUM = 4; //A, B, C, D
const int NOTE_NUM = 30; //화면에 존재할 노트 수
const float TICK = 0.01f;

/**
* s		: Scene
* o		: Object-default
* but	: Object-Button
* t		: Timer
*/

ObjectID Decimal[DECIMAL_NUM];

ObjectID butReturn; /* 뒤로가기 버튼 */

SceneID sMain;
ObjectID butPlay, butRecord, butSetting, butCredit, butExit;

SceneID sSelect;
ObjectID oTitle[SELECT_LIMIT];

SceneID sIngame;
ObjectID oNote[100], oJudge[JUDGE_NUM], oAllperfect, oFullCombo;
/* use Decimal[] */

SceneID sRecord;
/* use Decimal[] */

SceneID sSetting;
ObjectID butKeySoundOn, butKeySoundOff, butKeyBinding, butSynk;

SceneID sKeyBinding;
struct SBinding {
	ObjectID A, B, C, D;
}struBinding;

SceneID sCredit;
ObjectID oCredit;

SceneID sResult;
ObjectID oRank[RANK_NUM], butGotoSelect, butGotoIngame;

SceneID sSynk;
ObjectID oSynkNote;
ObjectID butPlus, butMinus;

TimerID tTick;

bool bIngame = false;
bool bSync = false;

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

}

void keyCallback(KeyCode key, KeyState state) {
	endGame();
}

void timerCallback(TimerID timer) {
	if (bIngame) {

	}
	else if (bSync) {


	}

	startTimer(tTick);
}

/* 과연 음악이 끝나면 콜백되는 함수인가? */
void soundCallback(SoundID sound) {

}

void printNumber(int num, int x, int y, char align) {

}

SceneID initGameSetting() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setKeyboardCallback(keyCallback);
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);

	system("GetRhythmWithLeeLye.bat");
	sMain = createScene("sMain", "Images/sMain.png");
	system("GetRhythmWithLeeLye.bat");

	/*
	sSelect		= createScene("sSelect",		"Images/sSelect.png");
	sRecord		= createScene("sRecord",		"Images/sRecord.png");
	sSetting	= createScene("sSetting",		"Images/sSetting.png");
	sCredit		= createScene("sCredit",		"Images/sCredit.png");
	sIngame		= createScene("sIngame",		"Images/sIngame.png");
	sKeyBinding	= createScene("sKeyBinding",	"Images/sKeyBinding.png");
	sSynk		= createScene("sSynk",			"Images/sSynk.png");
	sResult		= createScene("sResult",		"Images/sResult.png");
	*/


	tTick = createTimer(TICK);

	return sMain;
}

int main() {
	startGame(initGameSetting());
}