#include "DxLib.h"
#include <math.h>

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
	int Circle_Blue, Circle_Green, Circle_Red;
	int Onpu;
} GRAPH;

typedef struct tagGRAPHSIZE
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
} GRAPHSIZE;

typedef struct tagGRAPHPOINT
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
} GRAPHPOINT;

typedef struct tagSOUND
{
	int Dice;
	int pefect, great;
} SOUND;

typedef struct tagFLAG
{
	int Title = 0;
} FLAG;

typedef struct tagBUTTONPOINT
{
	int x, y;
} BUTTONPOINT;

typedef struct tagCIRCLEPOINT
{
	int X, Y, MoveX, MoveY;
	int button, frame, flag;
} CIRCLEPOINT;

int BoxHit(int Al, int Ar, int At, int Au, int Bl, int Br, int Bt, int Bu);
int CircleHit(float Ax, float Ay, float Ar, float Bx, float By, float Br);
int Center(int GraphSize, char Tipe);
void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame);
void Struct(int MouseX, int MouseY);
void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY);

DATEDATA Date;
GRAPH Graph;
GRAPHSIZE Gs;
SOUND Sound;
GRAPHPOINT Gp;
BUTTONPOINT Bp[9];
CIRCLEPOINT Cp[64];

int UpdateKey(char Key []);

// WinMain 関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//ログがなんたら
	SetOutApplicationLogValidFlag(FALSE);
	// タイトルを変更
	SetMainWindowText("test");
	// ウインドウモードに変更
	ChangeWindowMode( TRUE ) ;
	//ウインドウの大きさ設定
	SetGraphMode(Screen_X, Screen_Y, 32);
	//ウインドウの大きさを自由に変更出来るかどうかのフラグ
	//	SetWindowSizeChangeEnableFlag( TRUE ) ;
	// 裏画面を使用
	SetDrawScreen(DX_SCREEN_BACK);
	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return -1;
	// 透過色を変更(ピンク)
//	SetTransColor(255, 0, 255);
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	int DateSum;
	int MouseX, MouseY;
	int SHandle;
	int White, Black, Gray;
//	char StrBuf[128], StrBuf2[32];
	char Key[256];
	int MemX[32], MemY[32], MoveX[32], MoveY[32];

	// 色の値を取得
	White = GetColor(255, 255, 255);
	Black = GetColor(0, 0, 0);
	Gray = GetColor(123, 123, 123);
	//乱数の初期設定(元凶)
	GetDateTime(&Date);
	DateSum = Date.Year + Date.Mon + Date.Day + Date.Hour + Date.Min + Date.Sec;

	//画像読み込み
	Graph.Circle_Blue = LoadGraph("Graph/Circle_Blue.png");
	Graph.Circle_Green = LoadGraph("Graph/Circle_Green.png");
	Graph.Circle_Red = LoadGraph("Graph/Circle_Red.png");
	Graph.Onpu = LoadGraph("Graph/Onpu.png");

	//画像のサイズを得る
	GetGraphSize(Graph.Circle_Blue, &Gs.Circle_X, &Gs.Circle_Y);
	GetGraphSize(Graph.Onpu, &Gs.Onpu_X, &Gs.Onpu_Y);

	Gp.Onpu_X = Center(Gs.Onpu_X, 'X');
	Gp.Onpu_Y = 100;

	Gp.Circle_X = Screen_X / 2;
	Gp.Circle_Y = 63 + Gs.Circle_Y / 2;

	Bp[0].x = 179;
	Bp[0].y = 164;
	Bp[1].x = 229;
	Bp[1].y = 422;
	Bp[2].x = 376;
	Bp[2].y = 640;
	Bp[3].x = 594;
	Bp[3].y = 787;
	Bp[4].x = 853;
	Bp[4].y = 839;
	Bp[5].x = 1111;
	Bp[5].y = 787;
	Bp[6].x = 1330;
	Bp[6].y = 640;
	Bp[7].x = 1477;
	Bp[7].y = 422;
	Bp[8].x = 1528;
	Bp[8].y = 164;

	for (int i = 0; i < 9; i++){
		Bp[i].x += Gs.Circle_X / 2;
		Bp[i].y += Gs.Circle_Y / 2;
	}

	for (int i = 0; i < 64; i++){
		Cp[i].flag = 0;
		Cp[i].frame = 0;
	}

	// test.mp3のメモリへの読み込みサウンドハンドルをSHandleに保存します
	Sound.Dice = LoadSoundMem("Sound/Here are Dice.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");

	// 読みこんだ音をループ再生します(『PlaySoundMem』関数使用)
//	PlaySoundMem(Sound.Dice, DX_PLAYTYPE_LOOP);


	// ゲームループ開始　エスケープキーが押されたら終了する
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面の初期化
		ClearDrawScreen();
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);
		UpdateKey(Key);

		// 画面左上の領域に四角を描き,前に描いてあった文字列を消す
		DrawBox(0, 0, Screen_X , Screen_Y, White, TRUE);

		Struct(MouseX, MouseY);

		DrawGraph(Gp.Onpu_X, Gp.Onpu_Y, Graph.Onpu, TRUE);
//		DrawGraph(Gp.Circle_X, Gp.Circle_Y, Graph.Circle_Blue, TRUE);
		DrawCirclGraph(Gp.Circle_X, Gp.Circle_Y, Graph.Circle_Blue, Gs.Circle_X, Gs.Circle_Y);


		for (int i = 0; i < 9; i++){
			DrawCirclGraph(Bp[i].x, Bp[i].y, Graph.Circle_Green, Gs.Circle_X, Gs.Circle_Y);
		}

		if (Key[KEY_INPUT_A]==1){
			for (int j = 0; j < 64; j++){
				if (Cp[j].flag == 0){
					Cp[j].flag = 1;
					Cp[j].button = j % 9;
					Cp[j].button = 4;
					break;
				}
			}
		}

		for (int i = 0; i < 64; i++){
			int BPM = 133;
			float Frame;
			int button = Cp[i].button;

			if (Cp[i].flag == 1){
				//MoveNote();
				Cp[i].X = Gp.Circle_X;
				Cp[i].Y = Gp.Circle_Y;
				Cp[i].flag = 2;
			}

			if (Cp[i].flag == 2){
				Frame = BPM / ((float)BPM / 60) - Cp[i].frame;
				Frame = 60 - Cp[i].frame;
				if (Frame < 5)	Cp[i].flag = 3;
				MovePoint(Cp[i].X, Cp[i].Y, Bp[button].x, Bp[button].y, &Cp[i].MoveX, &Cp[i].MoveY, (int)Frame);
			}
		
			if (Cp[i].flag >= 2){
				Cp[i].X += Cp[i].MoveX;
				Cp[i].Y += Cp[i].MoveY;
				Cp[i].frame++;
			}

			if (Cp[i].flag >= 1){
				DrawCirclGraph(Cp[i].X, Cp[i].Y, Graph.Circle_Red, Gs.Circle_X, Gs.Circle_Y);
				if (Cp[i].X < 0 || Screen_X < Cp[i].X || Screen_Y < Cp[i].Y){
					Cp[i].flag = 0;
					Cp[i].frame = 0;
					Cp[i].X = Gp.Circle_X;
					Cp[i].Y = Gp.Circle_Y;
				}
			}

			MemX[31]++;
			if (MemX[31] % (BPM * 10) == 0) PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);

		}

		if (Key[KEY_INPUT_SPACE] == 1){
			PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
		}
		

		ScreenFlip();
		// 一定時間待つ
//		WaitTimer(20);

		// メッセージ処理
		if (ProcessMessage() == -1)	break;	// エラーが起きたらループを抜ける
	}

	// ＤＸライブラリの使用終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

int UpdateKey(char Key[]){
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++){
		if (tmpKey[i] != 0){ // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}






int BoxHit(int Al, int Ar, int At, int Au, int Bl, int Br, int Bt, int Bu){
	int Flag;
	if (Al < Br && Ar > Bl && At < Bu && Au > Bt)	Flag = 1;
	else	Flag = 0;
	return Flag;
}

int CircleHit(float Ax, float Ay, float Ar, float Bx, float By, float Br){
	int Flag;
	if (pow(Bx - Ax, 2) + pow(By - Ay, 2) <= pow(Ar + Br, 2))	Flag = 1;
	else	Flag = 0;
	return Flag;
}

int Center(int GraphSize, char Tipe){
	if (Tipe == 'X')	return((Screen_X - GraphSize) / 2);
	if (Tipe == 'Y')	return((Screen_Y - GraphSize) / 2);
}

void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame){
	*Move_X = (After_X - Before_X) / Frame;
	*Move_Y = (After_Y - Before_Y) / Frame;
}

void Struct(int MouseX, int MouseY){	// 表示する文字列を作成
	char StrBuf[128], StrBuf2[32];

	lstrcpy(StrBuf, "座標 Ｘ"); // 文字列"座標 Ｘ"をStrBufにコピー	
	_itoa_s(MouseX, StrBuf2, 10); // MouseXの値を文字列にしてStrBuf2に格納
	lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す
	lstrcat(StrBuf, "　Ｙ "); // StrBufの内容に文字列"　Ｙ "を付け足す
	_itoa_s(MouseY, StrBuf2, 10); // MouseYの値を文字列にしてStrBuf2に格納
	lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す

	DrawString(0, 0, StrBuf, GetColor(0, 0, 0));
}

void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY){
	DrawGraph(X - GsX / 2, Y - GsY / 2, Graph, TRUE);
}