#include "DxLib.h"
#include <math.h>

#define Screen_X 1120
#define Screen_Y 630

//構造体
typedef struct tagGRAPH
{
	int Circle_Blue, Circle_Green, Circle_Red;
} GRAPH;

typedef struct tagGRAPHSIZE
{
	int Circle_X, Circle_Y;
} GRAPHSIZE;

typedef struct tagSOUND
{
	int Dice;
	int pefect, great;
} SOUND;

typedef struct tagFLAG
{
	int Title = 0;
} FLAG;

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
}

DATEDATA Date;
GRAPH Graph;
GRAPHSIZE GraphSize;
SOUND Sound;

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
	// 画面の初期化
	ClearDrawScreen();
	// 透過色を変更(ピンク)
	SetTransColor(255, 0, 255);
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	int DateSum;
	int MouseX, MouseY;
	int SHandle;
	int White, Black, Gray;
//	char StrBuf[128], StrBuf2[32];
	char Key[256];

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

	//画像のサイズを得る
	GetGraphSize(Graph.Circle_Blue, &GraphSize.Circle_X, &GraphSize.Circle_Y);

	// test.mp3のメモリへの読み込みサウンドハンドルをSHandleに保存します
	Sound.Dice = LoadSoundMem("Sound/Here are Dice.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");

	// 読みこんだ音をループ再生します(『PlaySoundMem』関数使用)
	PlaySoundMem(Sound.Dice, DX_PLAYTYPE_LOOP);


	// ゲームループ開始　エスケープキーが押されたら終了する
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);
		UpdateKey(Key);

		// 画面左上の領域に四角を描き,前に描いてあった文字列を消す
		DrawBox(0, 0, Screen_X , Screen_Y, White, TRUE);

/*		// 表示する文字列を作成
		{
			lstrcpy(StrBuf, "座標 Ｘ"); // 文字列"座標 Ｘ"をStrBufにコピー	
			_itoa_s(MouseX, StrBuf2, 10); // MouseXの値を文字列にしてStrBuf2に格納
			lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す
			lstrcat(StrBuf, "　Ｙ "); // StrBufの内容に文字列"　Ｙ "を付け足す
			_itoa_s(MouseY, StrBuf2, 10); // MouseYの値を文字列にしてStrBuf2に格納
			lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す
		}

		// 座標文字列を描く
		DrawString(0, 0, StrBuf, Black);
*/
		Struct(MouseX, MouseY);

		if (Key[KEY_INPUT_SPACE] == 1){
			PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
		}

		// 一定時間待つ
		WaitTimer(20);

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