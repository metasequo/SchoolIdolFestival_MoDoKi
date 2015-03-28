#include "Common.h"

extern DATEDATA Date;
extern GRAPH Graph;
extern GRAPHSIZE Gs;
extern SOUND Sound;
extern GRAPHPOINT Gp;
extern BUTTONPOINT Bp[9];
extern CIRCLEPOINT Cp[128];
extern NOTE Note[800];
extern FLAG Flag;
extern PLAYER Player;
extern STATUS Status;
extern GLOBAL Global;
extern MUSIC Music[MusicCntMax];

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
		if (Z <= judge[i])	return i + 1;
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

	if (Combo <= 20)	score *= 1.0;
	else if (Combo <= 40)	score *= 1.1;
	else if (Combo <= 60)	score *= 1.15;
	else if (Combo <= 80)	score *= 1.2;
	else if (Combo <= 100)	score *= 1.25;
	else if (Combo <= 120)	score *= 1.3;
	else if (Combo <= 140)	score *= 1.35;
	else if (Combo <= 160)	score *= 1.4;
	else if (Combo <= 180)	score *= 1.45;
	else score *= 1.5;

	return score;
}

void ListRead()
{
	int i = 0, List;
	int len;
	char read[256], *token, *nexttoken;
	char cut [] = "[:.]; \n";
	char Add [] = "Chart/MusicList.txt";

	List = FileRead_open(Add);
	Global.MusicCnt = 0;

	while (FileRead_gets(read, 255, List) != NULL){
		token = strtok_s(read, cut, &nexttoken);
		strcpy_s(Music[i].MusicName, token);
		token = strtok_s(NULL, cut, &nexttoken);
		Music[i].Easy = atoi(token);
		token = strtok_s(NULL, cut, &nexttoken);
		Music[i].Normal = atoi(token);
		token = strtok_s(NULL, cut, &nexttoken);
		Music[i].Hard = atoi(token);
		i++;
		Global.MusicCnt++;
	}

	FileRead_close(List);
}

void ChartRead(char *MusicName, char Type){
	int i, j,  Chart, fullnotes, times[8];
	char read[256], *token, *nexttoken;
	char cut [] = "[:.]; \n";
	char Add[64];
//	char Musicname [] = "Mizugame_short";

	switch (Type)
	{
	case 'E':
		sprintf_s(Add, "Chart/Easy/%s.txt", MusicName);
		break;
	case 'N':
		sprintf_s(Add, "Chart/Normal/%s.txt", MusicName);
		break;
	case 'H':
		sprintf_s(Add, "Chart/Hard/%s.txt", MusicName);
		break;
	}
	puts(Add);
	Chart = FileRead_open(Add);

	i = 0;
	while (FileRead_gets(read, 255, Chart) != NULL){
		j = 0;

		if (i){
			token = strtok_s(read, cut, &nexttoken);
			Note[i].min = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Note[i].sec = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Note[i].mill = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			if (Note[i - 1].min == Note[i].min && Note[i - 1].sec == Note[i].sec && Note[i - 1].mill == Note[i].mill){
				Global.Simultaneously++;
			}
			else{
				Global.Simultaneously = 0;
			}

			Note[i].button = atoi(token);


			if (Type == 'E'){
				switch (Music[Global.TargetMusic].Easy)
				{
				case 0:
					if (Note[i].button == 1 || Note[i].button == 2 || Note[i].button == 8 || Note[i].button == 9){
						do{
							Note[i].button = GetRand(5) + 2;
							Simultaneous(i, 5);
						} while (Note[i].button == 1 || Note[i].button == 2 || Note[i].button == 8 || Note[i].button == 9);
					}
					break;
				}
			}
			else if (Type == 'N'){
				switch (Music[Global.TargetMusic].Normal)
				{
				case 0:
					if (Note[i].button == 1 || Note[i].button == 9){
						do{
							Note[i].button = GetRand(7) + 1;
							Simultaneous(i, 7);
						} while (Note[i].button == 1 || Note[i].button == 9);
					}
					break;
				}
			}
			else if (Type == 'H'){
				switch (Music[Global.TargetMusic].Hard)
				{
				case 2:
					Note[i].button = GetRand(8) + 1; 
					Simultaneous(i, 9);
					break;
				case 3:
					Note[i].button = 5;
					Simultaneous(i, 9);
					break;
				}
			}



			Note[i].flag = 1;
		}
		else{
			token = strtok_s(read, cut, &nexttoken);
			Player.sMin = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Player.sSec = atoi(token);
			token = strtok_s(NULL, cut, &nexttoken);
			Player.sMill = atoi(token);
//			token = strtok_s(NULL, cut, &nexttoken);
//			Player.Notes = atoi(token);
		}
		i++;
	}
	Player.Notes = i - 1;

	for (i = 0; i < Player.Notes; i++){
		Note[i].mill += Status.Timing / 10;
		while (Note[i].mill >= 100){
			Note[i].mill -= 100;
			Note[i].sec += 1;
			if (Note[i].sec >= 60){
				Note[i].sec -= 60;
				Note[i].min += 1;
			}
		}
		while (Note[i].mill < 0){
			Note[i].mill += 100;
			Note[i].sec -= 1;
			if (Note[i].sec < 0){
				Note[i].sec += 60;
				Note[i].min -= 1;
			}
		}
	}

	FileRead_close(Chart);
}

void Simultaneous(int num, int ButtonNum){
	int j = 0;
	for (int cnt = 1; cnt <= Global.Simultaneously; cnt++){
		while (Note[num - cnt].button == Note[num].button){
			if (ButtonNum == 5)	Note[num].button = GetRand(ButtonNum) + 2;
			if (ButtonNum == 7)	Note[num].button = GetRand(ButtonNum) + 1;
			if (ButtonNum == 9)	Note[num].button = GetRand(8) + 1;
			j++;
			if (j == ButtonNum)	break;
		}
	}
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

	for (i = 0; i < 128; i++){
		if (Cp[i].flag != 0){
			lstrcpy(StrBuf, "judge ");
			_itoa_s(i, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " : ");
			_itoa_s(Cp[i].flag, StrBuf2, 10);
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

	
	for (i = 0; i < Global.MusicCnt; i++){
		lstrcpy(StrBuf, "Name :");
		strcpy_s(StrBuf2, Music[i].MusicName);
		lstrcat(StrBuf, StrBuf2);
		lstrcat(StrBuf, " , data :");
		_itoa_s(Music[i].MusicData, StrBuf2, 10);
		lstrcat(StrBuf, StrBuf2);
		DrawString(0, 60 + i * 15, StrBuf, GetColor(0, 0, 0));
	}
	

}

void Format(){
	int i, j;
	double X, Y;

	// 色の値を取得
	Status.White = GetColor(239, 239, 239);
	Status.Black = GetColor(0, 0, 0);
	Status.Gray = GetColor(123, 123, 123);
	//乱数の初期設定(元凶)
	GetDateTime(&Date);
	Status.DateSum = Date.Year + Date.Mon + Date.Day + Date.Hour + Date.Min + Date.Sec;
	SRand(Status.DateSum);

	//音声読み込み
	Sound.Mizugame = LoadSoundMem("Sound/Mizugame.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");
	Sound.Push = LoadSoundMem("Sound/botan_b25.mp3");

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
	Graph.Title_technyan = LoadGraph("Graph/Title_technyan.png");
	Graph.Title = LoadGraph("Graph/Title.png");
	Graph.Start = LoadGraph("Graph/Start.png");
	Graph.Select = LoadGraph("Graph/Select.png");
	Graph.Button_Red = LoadGraph("Graph/button_red.png");
	Graph.Button_Blue = LoadGraph("Graph/button_blue.png");
	Graph.Button_Green = LoadGraph("Graph/button_green.png");
	Graph.Button_Yellow = LoadGraph("Graph/button_yellow.png");
	Graph.Button_White = LoadGraph("Graph/button_white.png");
	Graph.Medal_Gold = LoadGraph("Graph/medal_gold.png");
	Graph.Medal_Silver = LoadGraph("Graph/medal_silver.png");
	Graph.Medal_Bronze = LoadGraph("Graph/medal_bronze.png");
	Graph.Easy = LoadGraph("Graph/Easy.png");
	Graph.Normal = LoadGraph("Graph/Normal.png");
	Graph.Hard = LoadGraph("Graph/Hard.png");


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
	GetGraphSize(Graph.Title, &Gs.Title_X, &Gs.Title_Y);
	GetGraphSize(Graph.Start, &Gs.Start_X, &Gs.Start_Y);
	GetGraphSize(Graph.Select, &Gs.Select_X, &Gs.Select_Y);
	GetGraphSize(Graph.Button_Red, &Gs.Button_X, &Gs.Button_Y);
	GetGraphSize(Graph.Medal_Gold, &Gs.Medal_X, &Gs.Medal_Y);
	GetGraphSize(Graph.Easy, &Gs.Easy_X, &Gs.Easy_Y);
	GetGraphSize(Graph.Normal, &Gs.Normal_X, &Gs.Normal_Y);
	GetGraphSize(Graph.Hard, &Gs.Hard_X, &Gs.Hard_Y);

	Gs.Button_X /= 3;
	Gs.Button_Y /= 3;

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

	Gp.CentrJacket_X = Center(500, 'X');
	Gp.CentrJacket_Y = Center(500, 'Y');
	Gp.LeftJacket_X = Center(500, 'X') - 600;
	Gp.LeftJacket_Y = Gp.CentrJacket_Y + 150;
	Gp.RightJacket_X = Center(500, 'X') + 750;
	Gp.RightJacket_Y = Gp.CentrJacket_Y + 150;

	for (i = 0; i < 9; i++){
		Bp[i].x += Gs.Circle_X / 2;
		Bp[i].y += Gs.Circle_Y / 2;
		Bp[i].flag = 0;
		Bp[i].num = 0;
		Bp[i].r = Pythagorean(Gp.Circle_X, Gp.Circle_Y, Bp[i].x, Bp[i].y);
	}

	Status.Timing = -1000;
	Global.TargetMusic = 0;

	//楽曲リスト読み取り
	ListRead();

	for (i = 0; i < Global.MusicCnt; i++){
		char Add[64];
		sprintf_s(Add, "Graph/Jacket/%s.jpg", Music[i].MusicName);
		Music[i].Jacket = LoadGraph(Add);

		sprintf_s(Add, "Sound/%s.mp3", Music[i].MusicName);
		Music[i].MusicData = LoadSoundMem(Add);
	}

	Reset();

}

void Reset(){
	int i, j;

	for (i = 0; i < 128; i++){
		Cp[i].flag = 0;
		Cp[i].frame = 0;
		Cp[i].judge = 0;
		Cp[i].Radius = 76;
		Cp[i].button = 0;
	}

	for (i = 0; i < 800; i++){
		Note[i].min = 0;
		Note[i].sec = 0;
		Note[i].mill = 0;
		Note[i].button = 0;
		Note[i].flag = 0;
	}

	Flag.Title = 1;
	Flag.Select = 0;
	Flag.Game = 0;

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

	Global.TargetDiff = 1;
	Global.Simultaneously = 0;

	Status.BPM = 80;

	//楽曲リスト読み取り
	ListRead();

	Gp.Title_technyan_X = 0;
	Gp.Title_technyan_Y = 0;
	Gp.Title_X = Center(Gs.Title_X, 'X');
	Gp.Title_Y = 150;
	Gp.Start_X = Center(Gs.Start_X, 'X');
	Gp.Start_Y = Gp.Title_Y + Gs.Title_Y + Gs.Start_Y * 2;
	Gp.Select_X = Center(Gs.Select_X, 'X');
	Gp.Select_Y = 100;


}