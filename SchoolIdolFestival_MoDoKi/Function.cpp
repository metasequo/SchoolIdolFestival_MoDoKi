#include "Common.h"

extern DATEDATA Date;
extern GRAPH Graph;
extern GRAPHSIZE Gs;
extern SOUND Sound;
extern GRAPHPOINT Gp;
extern BUTTONPOINT Bp[9];
extern CIRCLEPOINT Cp[64];
extern NOTE Note[800];
extern FLAG Flag;
extern PLAYER Player;
extern STATUS Status;
extern GLOBAL Global;

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
	else return 0;
}

void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame){
	*Move_X = (After_X - Before_X) / Frame;
	*Move_Y = (After_Y - Before_Y) / Frame;
}

int UpdateKey(char Key []){
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++){
		if (tmpKey[i] != 0)		Key[i]++;
		else	Key[i] = 0;
	}
	return 0;
}

void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY){
	DrawGraph(X - GsX / 2, Y - GsY / 2, Graph, TRUE);
}

void DrawCirclExtendGraph(int X, int Y, int Graph, int Radius){
	DrawExtendGraph(X - (Radius / 2), Y - (Radius / 2), X + (Radius / 2), Y + (Radius / 2), Graph, TRUE);
}

int Pythagorean(int Ax, int Ay, int Bx, int By){
	return sqrt(pow(((double) Ax - (double) Bx), 2) + pow(((double) Ay - (double) By), 2));
}

int NoteHit(int circle, int button){
	int judge[5], Z, i;

	Z = Pythagorean(Bp[button].x, Bp[button].y, Cp[circle].X, Cp[circle].Y);

	judge[0] = Player.jPerfect;
	judge[1] = Player.jGreat;
	judge[2] = Player.jGood;
	judge[3] = Gs.Circle_X / 2;
	judge[4] = Gs.Circle_X;

	for (i = 0; i < 4; i++)
		if (Z <= judge[i] && Z <= judge[i])		return i + 1;
	if (Z > judge[4] && Z > judge[4])	return 5;
}

int ScoreCalcu(int judge, int Combo){
	float score = 221;

	switch (judge)
	{
	case 1:
		score *= 1.0;
		break;
	case 2:
		score *= 0.88;
		break;
	case 3:
		score *= 0.8;
		break;
	case 4:
		score *= 0.4;
		break;
	case 5:
		score *= 0.0;
		break;
	}

	if (Combo <= 50)	score *= 1.0;
	else if (Combo <= 100)	score *= 1.1;
	else if (Combo <= 200)	score *= 1.15;
	else if (Combo <= 400)	score *= 1.2;
	else if (Combo <= 600)	score *= 1.25;
	else if (Combo <= 800)	score *= 1.3;
	else if (Combo <= 1000)	score *= 1.35;

	return score;
}

void ChartRead(){
	int i, Chart, fullnotes, times[8];
	char read[256], *token, *nexttoken;
	char cut [] = "[:.]; \n";
	char Add[64];
	char Songname [] = "Mizugame_short";

	sprintf_s(Add, "Chart/%s.txt", Songname);
	puts(Add);
	Chart = FileRead_open(Add);

	i = 0;
	while (FileRead_gets(read, 255, Chart) != NULL){
		if (i){
			token = strtok_s(read, cut, &nexttoken);
			Note[i].min = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Note[i].sec = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Note[i].mill = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Note[i].button = atoi(token);
			Note[i].flag = 1;
		}
		else{
			token = strtok_s(read, cut, &nexttoken);
			//			Player.sMin = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Player.sMin = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Player.sSec = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Player.sMill = atoi(token);
		}
		i++;
	}
	Player.Notes = i - 1;

	FileRead_close(Chart);
}

void Struct(int MouseX, int MouseY){	// 表示する文字列を作成
	char StrBuf[128], StrBuf2[32];
	int i, j;

	lstrcpy(StrBuf, "座標 Ｘ"); // 文字列"座標 Ｘ"をStrBufにコピー
	_itoa_s(MouseX, StrBuf2, 10); // MouseXの値を文字列にしてStrBuf2に格納
	lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す
	lstrcat(StrBuf, "　Ｙ "); // StrBufの内容に文字列"　Ｙ "を付け足す
	_itoa_s(MouseY, StrBuf2, 10); // MouseYの値を文字列にしてStrBuf2に格納
	lstrcat(StrBuf, StrBuf2); // StrBufの内容にStrBuf2の内容を付け足す

	DrawString(0, 0, StrBuf, GetColor(0, 0, 0));

	lstrcpy(StrBuf, "HP : ");
	_itoa_s(Player.HP, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, " Score : ");
	_itoa_s(Player.Score, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, " Combo : ");
	_itoa_s(Player.Combo, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, " Timing : ");
	_itoa_s(Status.Timing, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, " Time : ");
	_itoa_s((GetNowCount() - Status.StartTime) / 60000, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, ":");
	_itoa_s((GetNowCount() - Status.StartTime) % 60000 / 1000, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, ":");
	_itoa_s((GetNowCount() - Status.StartTime) % 1000 / 10, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);

	DrawString(0, 15, StrBuf, GetColor(0, 0, 0));

	for (i = 0; i < 64; i++){
		if (Cp[i].flag != 0){
			lstrcpy(StrBuf, "judge ");
			_itoa_s(i, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " : ");
			_itoa_s(Cp[i].judge, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			DrawString(0, 30 + i * 15, StrBuf, GetColor(0, 0, 0));
		}
	}

	lstrcpy(StrBuf, "Notes : ");
	_itoa_s(Player.Notes, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, " sTime ");
	_itoa_s(Player.sMin, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, ":");
	_itoa_s(Player.sSec, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	lstrcat(StrBuf, ":");
	_itoa_s(Player.sMill, StrBuf2, 10);
	lstrcat(StrBuf, StrBuf2);
	DrawString(300, 0, StrBuf, GetColor(0, 0, 0));

	j = 0;
	for (i = 0; i < 800; i++){
		if (Note[i].flag == 1){
			lstrcpy(StrBuf, "");
			_itoa_s(i, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " : Flag : ");
			_itoa_s(Note[i].flag, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " NoteTime ");
			_itoa_s(Note[i].min, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, ":");
			_itoa_s(Note[i].sec, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, ":");
			_itoa_s(Note[i].mill, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " Button :");
			_itoa_s(Note[i].button, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			DrawString(300, 30 + j * 15, StrBuf, GetColor(0, 0, 0));
			j++;
		}
	}
}

void Format(){
	int i, j;
	double X, Y;
	// 色の値を取得
	Status.White = GetColor(255, 255, 255);
	Status.Black = GetColor(0, 0, 0);
	Status.Gray = GetColor(123, 123, 123);
	//乱数の初期設定(元凶)
	GetDateTime(&Date);
	Status.DateSum = Date.Year + Date.Mon + Date.Day + Date.Hour + Date.Min + Date.Sec;
	SRand(Status.DateSum);

	//音声読み込み
	Sound.Mizugame = LoadSoundMem("Sound/Mizugame_short.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");

	//画像読み込み
	Graph.Circle_Blue = LoadGraph("Graph/Circle_Blue.png");
	Graph.Circle_Green = LoadGraph("Graph/Circle_Green.png");
	Graph.Circle_Red = LoadGraph("Graph/Circle_Red.png");
	Graph.Circle_Yellow = LoadGraph("Graph/Circle_Yellow.png");
	Graph.Onpu = LoadGraph("Graph/Onpu.png");
	LoadDivGraph("Graph/Number.png", 10, 5, 2, 90, 86, Graph.Number);
	LoadDivGraph("Graph/Number_mini.png", 10, 5, 2, 59, 57, Graph.Number_m);
	Graph.Combo = LoadGraph("Graph/Combo.png");
	Graph.Perfect = LoadGraph("Graph/Perfect.png");
	Graph.Great = LoadGraph("Graph/Great.png");
	Graph.Good = LoadGraph("Graph/Good.png");
	Graph.Bad = LoadGraph("Graph/Bad.png");
	Graph.Miss = LoadGraph("Graph/Miss.png");
	Graph.Gameover = LoadGraph("Graph/Gameover.png");
	Graph.Scare = LoadGraph("Graph/Score.png");
	Graph.Library = LoadGraph("Graph/library.jpg");
	Graph.Fade = LoadGraph("Graph/Fade_All.png");
	LoadDivGraph("Graph/Technyan_icon.png", 9, 3, 3, 200, 200, Graph.Technyan);


	//画像のサイズを得る
	GetGraphSize(Graph.Circle_Blue, &Gs.Circle_X, &Gs.Circle_Y);
	GetGraphSize(Graph.Onpu, &Gs.Onpu_X, &Gs.Onpu_Y);
	GetGraphSize(Graph.Number[0], &Gs.Number_X, &Gs.Number_Y);
	GetGraphSize(Graph.Number_m[0], &Gs.Number_mX, &Gs.Number_mY);
	GetGraphSize(Graph.Combo, &Gs.Combo_X, &Gs.Combo_Y);
	GetGraphSize(Graph.Perfect, &Gs.Perfect_X, &Gs.Perfect_Y);
	GetGraphSize(Graph.Great, &Gs.Great_X, &Gs.Great_Y);
	GetGraphSize(Graph.Good, &Gs.Good_X, &Gs.Good_Y);
	GetGraphSize(Graph.Bad, &Gs.Bad_X, &Gs.Bad_Y);
	GetGraphSize(Graph.Miss, &Gs.Miss_X, &Gs.Miss_Y);
	GetGraphSize(Graph.Gameover, &Gs.Gameover_X, &Gs.Gameover_Y);
	GetGraphSize(Graph.Scare, &Gs.Score_X, &Gs.Score_Y);
	Gs.Radius = Gs.Circle_X / 2;
	GetGraphSize(Graph.Technyan[6], &Gs.Technyan_X, &Gs.Technyan_Y);


	Gp.Circle_X = Screen_X / 2;
	Gp.Circle_Y = 164 + Gs.Circle_X / 2;
	Gp.Onpu_X = Screen_X / 2;
	Gp.Onpu_Y = Gp.Circle_Y;
	Gp.Perfect_X = Center(Gs.Perfect_X, 'X');
	Gp.Perfect_Y = 523;
	Gp.Great_X = Center(Gs.Great_X, 'X');
	Gp.Great_Y = 523;
	Gp.Good_X = Center(Gs.Good_X, 'X');
	Gp.Good_Y = 523;
	Gp.Bad_X = Center(Gs.Bad_X, 'X');
	Gp.Bad_Y = 523;
	Gp.Miss_X = Center(Gs.Miss_X, 'X');
	Gp.Miss_Y = 523;
	Gp.Gameover_X = Center(Gs.Gameover_X, 'X');
	Gp.Gameover_Y = 523;
	Gp.Score_X = Center(Gs.Score_X, 'X') - Gs.Number_X * 3;
	Gp.Score_Y = 30;
	Gp.Combo_X = Center(Gs.Combo_X, 'X') - Gs.Number_mX * 2;
	Gp.Combo_Y = Gp.Perfect_Y - Gs.Combo_Y - 15;

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

	for (i = 0; i < 9; i++){
		Bp[i].x += Gs.Circle_X / 2;
		Bp[i].y += Gs.Circle_Y / 2;
		Bp[i].flag = 0;
		Bp[i].r = Pythagorean(Gp.Circle_X, Gp.Circle_Y, Bp[i].x, Bp[i].y);
	}

	Status.Timing = 1000;

	Reset();
}

void Reset(){
	int i, j;

	for (i = 0; i < 64; i++){
		Cp[i].flag = 0;
		Cp[i].frame = 0;
		Cp[i].judge = 0;
		Cp[i].Radius = 76;
	}

	Player.Score = 0;
	Player.Combo = 0;
	Player.HP = 20;
	Player.Perfect = 0;
	Player.Great = 0;
	Player.Good = 0;
	Player.Bad = 0;
	Player.Miss = 0;
	Player.jPerfect = 50;
	Player.jGreat = 100;
	Player.jGood = 150;
	Player.pSec = 0;
	Player.pMin = 0;
	Player.pMill = 0;

	Status.BPM = 120;

	ChartRead();
	for (i = 0; i < Player.Notes; i++){
		Note[i].mill += Status.Timing / 10;
		if (Note[i].mill >= 100){
			Note[i].mill = Note[i].mill % 100;
			Note[i].sec + 1;
			if (Note[i].sec >= 60){
				Note[i].sec %= 60;
				Note[i].min + 1;
			}
		}
	}

}