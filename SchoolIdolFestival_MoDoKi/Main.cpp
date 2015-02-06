#include "DxLib.h"
#include <math.h>
#include <string.h>
#include "Function.h"
#include "Game.h"

#define FULLHD

#ifdef FULLHD
#define Screen_X 1920
#define Screen_Y 1080
#else
#define Screen_X 1280
#define Screen_Y 720
#endif

//構造体
typedef struct tagGRAPH
{
	int Circle_Blue, Circle_Green, Circle_Red, Circle_Yellow;
	int Onpu;
	int Number[10], Number_m[10], Combo;
	int Perfect, Great, Good, Bad, Miss;
	int Gameover;
	int Scare;
	int Library;
	int Fade;
	int Technyan[9];
} GRAPH;

typedef struct tagGRAPHSIZE
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
	int Number_X, Number_Y;
	int Number_mX, Number_mY;
	int Combo_X, Combo_Y;
	int Perfect_X, Great_X, Good_X, Bad_X, Miss_X;
	int Perfect_Y, Great_Y, Good_Y, Bad_Y, Miss_Y;
	int Gameover_X, Gameover_Y;
	int Score_X, Score_Y;
	int Radius;
	int Technyan_X, Technyan_Y;
} GRAPHSIZE;

typedef struct tagGRAPHPOINT
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
	int Combo_X, Combo_Y;
	int Perfect_X, Great_X, Good_X, Bad_X, Miss_X;
	int Perfect_Y, Great_Y, Good_Y, Bad_Y, Miss_Y;
	int Gameover_X, Gameover_Y;
	int Score_X, Score_Y;
} GRAPHPOINT;

typedef struct tagSOUND
{
	int Dice, Mizugame;
	int pefect, great;
} SOUND;

typedef struct tagBUTTONPOINT
{
	int x, y, r;
	int flag;
} BUTTONPOINT;

typedef struct tagCIRCLEPOINT
{
	int X, Y, MoveX, MoveY, Radius;
	int button, frame, flag, judge;
} CIRCLEPOINT;

typedef struct tagNOTE
{
	int min, sec, mill, button;
	int flag;
} NOTE;

typedef struct tagFLAG
{
	int Title , Select, Game, End;
	int Click;
} FLAG;

typedef struct tagPLAYER
{
	int Music, Level, Notes;
	int pMin, pSec, pMill;
	int sMin, sSec, sMill;
	int Score, Combo, HP;
	int Perfect, Great, Good, Bad, Miss;
	int jPerfect, jGreat, jGood, jBad, jMiss;
	char SongName[64];
} PLAYER;

typedef struct tagSTATUS
{
	int StartTime, NowTime, ElapsedTime;
	int Timing;
	int DateSum;
	int White, Black, Gray;
	int BPM;
} STATUS;

typedef struct tagGLOBAL
{
	int MouseX, MouseY;
	int ClickX, ClickY, Button;
	char Key[256];
	int Frame;
} GLOBAL;

DATEDATA Date;
GRAPH Graph;
GRAPHSIZE Gs;
SOUND Sound;
GRAPHPOINT Gp;
BUTTONPOINT Bp[9];
CIRCLEPOINT Cp[64];
NOTE Note[800];
FLAG Flag;
PLAYER Player;
STATUS Status;
GLOBAL Global;

// Function.cpp //
int BoxHit(int Al, int Ar, int At, int Au, int Bl, int Br, int Bt, int Bu);
int CircleHit(float Ax, float Ay, float Ar, float Bx, float By, float Br);
int Center(int GraphSize, char Tipe);
void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame);
int UpdateKey(char Key []);
void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY);
void DrawCirclExtendGraph(int X, int Y, int Graph, int Radius);
int Pythagorean(int Ax, int Ay, int Bx, int By);
int NoteHit(int circle, int button);
int ScoreCalcu(int judge, int Combo);
void ChartRead();
void Struct(int MouseX, int MouseY);
void Format();
void Reset();

// Game.cpp //
void Game();



// WinMain 関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//ログがなんたら
	SetOutApplicationLogValidFlag(FALSE);
	// タイトルを変更
	SetMainWindowText("test");
	// ウインドウモードに変更
//	ChangeWindowMode( TRUE ) ;
	//ウインドウの大きさ設定
	SetGraphMode(Screen_X, Screen_Y, 32);
	//ウインドウの大きさを自由に変更出来るかどうかのフラグ
	SetWindowSizeChangeEnableFlag( TRUE ) ;
	// 裏画面を使用
	SetDrawScreen(DX_SCREEN_BACK);
	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return -1;
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	int i, j, k;

	Format();

	// 読みこんだ音をループ再生します(『PlaySoundMem』関数使用)
//	PlaySoundMem(Sound.Mizugame, DX_PLAYTYPE_LOOP);


	// ゲームループ開始　エスケープキーが押されたら終了する
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面の初期化
		ClearDrawScreen();
		// マウスの位置を取得
		GetMousePoint(&Global.MouseX, &Global.MouseY);
		//マウスのクリック情報
		if (GetMouseInputLog(&Global.Button, &Global.ClickX, &Global.ClickY, TRUE) == 0){
			Flag.Click = 1;
		}
		else{
			Flag.Click = 0;
		}
		//ボタンの押下情報
		UpdateKey(Global.Key);

		// 画面左上の領域に四角を描き,前に描いてあった文字列を消す
		DrawBox(0, 0, Screen_X , Screen_Y, Status.White, TRUE);
		DrawExtendGraph(0, 0, Screen_X, Screen_Y, Graph.Library, TRUE);
		DrawGraph(0, 0, Graph.Fade, TRUE);

		//文字表示
		Struct(Global.MouseX, Global.MouseY);

		//ゲーム中身
		Game();

		//裏画面描画
		ScreenFlip();

		// メッセージ処理
		if (ProcessMessage() == -1)	break;	// エラーが起きたらループを抜ける
	}

	// ＤＸライブラリの使用終了
	DxLib_End();

	// ソフトの終了
	return 0;
}







