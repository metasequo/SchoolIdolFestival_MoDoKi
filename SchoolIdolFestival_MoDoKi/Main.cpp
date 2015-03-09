#include "Common.h"

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
	//透過色の設定
	SetTransColor(255, 0, 255);

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

		//Title

		if (Flag.Title == 1 && Flag.Select == 0 && Flag.Game == 0){
			Title();
		}
		
		if (Flag.Title == 2 && Flag.Select == 1 && Flag.Game == 0){
			TitleToSelect();
		}

		if (Flag.Title == 0 && Flag.Select == 1 && Flag.Game == 0){
			Select();
		}

		//ゲーム中身
		if (Flag.Title == 0 && Flag.Select == 0 && Flag.Game >= 1){
			Game();
		}

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







