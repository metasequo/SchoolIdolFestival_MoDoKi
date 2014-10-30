#include "DxLib.h"
#include <math.h>
#include <string.h>

#define FULLHD

#ifdef FULLHD
#define Screen_X 1920
#define Screen_Y 1080
#else
#define Screen_X 1280
#define Screen_Y 720
#endif

//�\����
typedef struct tagGRAPH
{
	int Circle_Blue, Circle_Green, Circle_Red;
	int Onpu;
	int Number[10], combo;
	int Perfect, Great, Good, Bad, Miss;
	int Gameover;
} GRAPH;

typedef struct tagGRAPHSIZE
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
	int Number_X, Number_Y;
	int combo_X, combo_Y;
	int Perfect_X, Great_X, Good_X, Bad_X, Miss_X;
	int Perfect_Y, Great_Y, Good_Y, Bad_Y, Miss_Y;
	int Gameover_X, Gameover_Y;
} GRAPHSIZE;

typedef struct tagGRAPHPOINT
{
	int Circle_X, Circle_Y;
	int Onpu_X, Onpu_Y;
	int combo_X, combo_Y;
	int Perfect_X, Great_X, Good_X, Bad_X, Miss_X;
	int Perfect_Y, Great_Y, Good_Y, Bad_Y, Miss_Y;
	int Gameover_X, Gameover_Y;
} GRAPHPOINT;

typedef struct tagSOUND
{
	int Dice, Mizugame;
	int pefect, great;
} SOUND;

typedef struct tagBUTTONPOINT
{
	int x, y;
	int flag;
} BUTTONPOINT;

typedef struct tagCIRCLEPOINT
{
	int X, Y, MoveX, MoveY;
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
} FLAG;

typedef struct tagPLAYER
{
	int Music, Level;
	int pMin, pSec, pMill;
	int sMin, sSec, sMill;
	int Score, Combo, HP;
	int Perfect, Great, Good, Bad, Miss;
	int jPerfect, jGreat, jGood, jBad, jMiss;
} PLAYER;


int BoxHit(int Al, int Ar, int At, int Au, int Bl, int Br, int Bt, int Bu);
int CircleHit(float Ax, float Ay, float Ar, float Bx, float By, float Br);
int Center(int GraphSize, char Tipe);
void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame);
void Struct(int MouseX, int MouseY);
void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY);
int UpdateKey(char Key []);
int NoteHit(int circle, int button);
int ScoreCalcu(int judge, int combo);
void ChartRead(int *min, int *sec, int *mill);

DATEDATA Date;
GRAPH Graph;
GRAPHSIZE Gs;
SOUND Sound;
GRAPHPOINT Gp;
BUTTONPOINT Bp[9];
CIRCLEPOINT Cp[64];
NOTE Note[800];
PLAYER Player;


// WinMain �֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//���O���Ȃ񂽂�
	SetOutApplicationLogValidFlag(FALSE);
	// �^�C�g����ύX
	SetMainWindowText("test");
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode( TRUE ) ;
	//�E�C���h�E�̑傫���ݒ�
	SetGraphMode(Screen_X, Screen_Y, 32);
	//�E�C���h�E�̑傫�������R�ɕύX�o���邩�ǂ����̃t���O
	//	SetWindowSizeChangeEnableFlag( TRUE ) ;
	// ����ʂ��g�p
	SetDrawScreen(DX_SCREEN_BACK);
	// �c�w���C�u�����̏�����
	if (DxLib_Init() == -1) return -1;
	// ���ߐF��ύX(�s���N)
//	SetTransColor(255, 0, 255);
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	int DateSum;
	int MouseX, MouseY;
	int White, Black, Gray;
	int i, j, k;
//	char StrBuf[128], StrBuf2[32];
	char Key[256];
	int MemX[32] = {0}, MemY[32], MoveX[32], MoveY[32];
	float Frame;
	int BPM;
	int StartTime;
	StartTime = GetNowCount();

	// �F�̒l���擾
	White = GetColor(255, 255, 255);
	Black = GetColor(0, 0, 0);
	Gray = GetColor(123, 123, 123);
	//�����̏����ݒ�(����)
	GetDateTime(&Date);
	DateSum = Date.Year + Date.Mon + Date.Day + Date.Hour + Date.Min + Date.Sec;
	SRand(DateSum);

	//�摜�ǂݍ���
	Graph.Circle_Blue = LoadGraph("Graph/Circle_Blue.png");
	Graph.Circle_Green = LoadGraph("Graph/Circle_Green.png");
	Graph.Circle_Red = LoadGraph("Graph/Circle_Red.png");
	Graph.Onpu = LoadGraph("Graph/Onpu.png");
	LoadDivGraph("Graph/Number.png", 10, 5, 2, 90, 86, Graph.Number);
	Graph.combo = LoadGraph("Graph/combo.png");
	Graph.Perfect = LoadGraph("Graph/Perfect.png");
	Graph.Great = LoadGraph("Graph/Great.png");
	Graph.Good = LoadGraph("Graph/Good.png");
	Graph.Bad = LoadGraph("Graph/Bad.png");
	Graph.Miss = LoadGraph("Graph/Miss.png");
	Graph.Gameover = LoadGraph("Graph/Gameover.png");

	//�摜�̃T�C�Y�𓾂�
	GetGraphSize(Graph.Circle_Blue, &Gs.Circle_X, &Gs.Circle_Y);
	GetGraphSize(Graph.Onpu, &Gs.Onpu_X, &Gs.Onpu_Y);
	GetGraphSize(Graph.Number[0], &Gs.Number_X, &Gs.Number_Y);
	GetGraphSize(Graph.combo, &Gs.combo_X, &Gs.combo_Y);
	GetGraphSize(Graph.Perfect, &Gs.Perfect_X, &Gs.Perfect_Y);
	GetGraphSize(Graph.Great, &Gs.Great_X, &Gs.Great_Y);
	GetGraphSize(Graph.Good, &Gs.Good_X, &Gs.Good_Y);
	GetGraphSize(Graph.Bad, &Gs.Bad_X, &Gs.Bad_Y);
	GetGraphSize(Graph.Miss, &Gs.Miss_X, &Gs.Miss_Y);
	GetGraphSize(Graph.Gameover, &Gs.Gameover_X, &Gs.Gameover_Y);

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
	}

	for (i = 0; i < 64; i++){
		Cp[i].flag = 0;
		Cp[i].frame = 0;
		Cp[i].judge = 0;
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


	// test.mp3�̃������ւ̓ǂݍ��݃T�E���h�n���h����SHandle�ɕۑ����܂�
	Sound.Mizugame = LoadSoundMem("Sound/Mizugame_short.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");

	ChartRead(&Player.sSec, &Player.sMin, &Player.sMill);
	

	// �ǂ݂��񂾉������[�v�Đ����܂�(�wPlaySoundMem�x�֐��g�p)
//	PlaySoundMem(Sound.Dice, DX_PLAYTYPE_LOOP);


	// �Q�[�����[�v�J�n�@�G�X�P�[�v�L�[�������ꂽ��I������
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ̏�����
		ClearDrawScreen();
		// �}�E�X�̈ʒu���擾
		GetMousePoint(&MouseX, &MouseY);
		UpdateKey(Key);

		// ��ʍ���̗̈�Ɏl�p��`��,�O�ɕ`���Ă����������������
		DrawBox(0, 0, Screen_X , Screen_Y, White, TRUE);

		Struct(MouseX, MouseY);

		DrawCirclGraph(Gp.Onpu_X, Gp.Onpu_Y, Graph.Onpu, Gs.Onpu_X, Gs.Onpu_Y);
		DrawCirclGraph(Gp.Circle_X, Gp.Circle_Y, Graph.Circle_Blue, Gs.Circle_X, Gs.Circle_Y);

		for (i = 0; i < 9; i++){
			DrawCirclGraph(Bp[i].x, Bp[i].y, Graph.Circle_Green, Gs.Circle_X, Gs.Circle_Y);
		}

		if (Key[KEY_INPUT_A]==1 || CheckKeyInput(KEY_INPUT_A) == 0){
			for (int j = 0; j < 64; j++){
				if (Cp[j].flag == 0){
					Cp[j].flag = 1;
					Cp[j].button = j % 9;
//					Cp[j].button = GetRand(9);
//					Cp[j].button = j % 2 + 2;
					break;
				}
			}
		}

		//�m�[�g�i�T�[�N���j����
		for (i = 0; i < 64; i++){
			BPM = 60;
			int button = Cp[i].button;

			if (Cp[i].flag != 0){
				if (Cp[i].flag == 1){
					//MoveNote();
					Cp[i].X = Gp.Circle_X;
					Cp[i].Y = Gp.Circle_Y;
					Cp[i].flag = 2;
				}

				if (Cp[i].flag == 2){
					Frame = 60 / (BPM / 60) - Cp[i].frame;
//					Frame = 60 - Cp[i].frame;
					if (Frame < 5)	Cp[i].flag = 3;
					MovePoint(Cp[i].X, Cp[i].Y, Bp[button].x, Bp[button].y, &Cp[i].MoveX, &Cp[i].MoveY, (int) Frame);
				}

				if (Cp[i].flag >= 2){
					Cp[i].X += Cp[i].MoveX;
					Cp[i].Y += Cp[i].MoveY;
					Cp[i].frame++;
				}

				if (Cp[i].flag >= 1){
					DrawCirclGraph(Cp[i].X, Cp[i].Y, Graph.Circle_Red, Gs.Circle_X, Gs.Circle_Y);

					if (Cp[i].button == 0 || Cp[i].button == 1 || Cp[i].button == 2 || Cp[i].button == 3){
						if (Cp[i].X < Bp[Cp[i].button].x){
							if (NoteHit(i, Cp[i].button) == 5){
								Cp[i].judge = 5;
							}
						}
					}
					if (Cp[i].button == 5 || Cp[i].button == 6 || Cp[i].button == 7 || Cp[i].button == 8){
						if (Bp[Cp[i].button].x < Cp[i].X){
							if (NoteHit(i, Cp[i].button) == 5){
								Cp[i].judge = 5;
							}
						}
					}
					if (Cp[i].button == 4){
						if (Bp[Cp[i].button].y < Cp[i].Y){
							if (NoteHit(i, Cp[i].button) == 5){
								Cp[i].judge = 5;
							}
						}
					}

/*					if (NoteHit(i, Cp[i].button) == 5){
						Cp[i].judge = 5;
					}
	*/			}
			}
		}

		//�{�^������
		for (i = 0; i < 64; i++){
			if (Cp[i].flag && Bp[Cp[i].button].flag == 0){
				switch (Cp[i].button){
				case 0:
					if (Key[KEY_INPUT_4] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 1:
					if (Key[KEY_INPUT_R] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 2:
					if (Key[KEY_INPUT_F] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 3:
					if (Key[KEY_INPUT_V] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 4:
					if (Key[KEY_INPUT_SPACE] == 1 || Key[KEY_INPUT_B] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 5:
					if (Key[KEY_INPUT_N] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 6:
					if (Key[KEY_INPUT_J] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 7:
					if (Key[KEY_INPUT_I] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				case 8:
					if (Key[KEY_INPUT_9] == 1){
						Cp[i].judge = NoteHit(i, Cp[i].button);
						Bp[Cp[i].button].flag = 1;
					}
					break;
				}
			}
		}
		
		//���育�Ƃ̏���
		for (i = 0; i < 64; i++){
			if (Cp[i].judge != 0){
				switch (Cp[i].judge){
				case 1:
					PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
					DrawGraph(Gp.Perfect_X, Gp.Perfect_Y, Graph.Perfect, TRUE);
					Player.Combo++;
					Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
					Bp[Cp[i].button].flag = 0;
					break;
				case 2:
					PlaySoundMem(Sound.great, DX_PLAYTYPE_BACK);
					DrawGraph(Gp.Great_X, Gp.Great_Y, Graph.Great, TRUE);
					Player.Combo++;
					Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
					Bp[Cp[i].button].flag = 0;
					break;
				case 3:
					PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
					DrawGraph(Gp.Good_X, Gp.Good_Y, Graph.Good, TRUE);
					Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
					Player.Combo = 0;
					Bp[Cp[i].button].flag = 0;
					break;
				case 4:
					DrawGraph(Gp.Bad_X, Gp.Bad_Y, Graph.Bad, TRUE);
					Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
					Player.Combo = 0;
					Player.HP--;
					Bp[Cp[i].button].flag = 0;
					break;
				case 5:
					DrawGraph(Gp.Miss_X, Gp.Miss_Y, Graph.Miss, TRUE);
					Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
					Player.Combo = 0;
					Player.HP--;
					Bp[Cp[i].button].flag = 0;
					break;
				}
				Cp[i].flag = 0;
				Cp[i].frame = 0;
				Cp[i].judge = 0;
				Cp[i].X = Gp.Circle_X;
				Cp[i].Y = Gp.Circle_Y;
			}
		}

		if (Player.HP <= 0){
			Player.HP = 0;
			DrawGraph(Gp.Gameover_X, Gp.Gameover_Y, Graph.Gameover, TRUE);
			for (i = 0; i < 64; i++){
				Cp[i].flag = 0;
				Cp[i].frame = 0;
				Cp[i].judge = 0;
				Cp[i].X = Gp.Circle_X;
				Cp[i].Y = Gp.Circle_Y;
			}
		}

/*		if ((GetNowCount() - StartTime) % (60000 / BPM) <= 50){
			if (CheckSoundMem(Sound.pefect) == 0)
				PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
			StartTime = GetNowCount();
		}
		*/

		if (Key[KEY_INPUT_SPACE] == 1){
			PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
		}

		//����ʕ`��
		ScreenFlip();

		// ���b�Z�[�W����
		if (ProcessMessage() == -1)	break;	// �G���[���N�����烋�[�v�𔲂���
	}

	// �c�w���C�u�����̎g�p�I��
	DxLib_End();

	// �\�t�g�̏I��
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
	else return 0;
}

void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame){
	*Move_X = (After_X - Before_X) / Frame;
	*Move_Y = (After_Y - Before_Y) / Frame;
}

void Struct(int MouseX, int MouseY){	// �\�����镶������쐬
	char StrBuf[128], StrBuf2[32];

	lstrcpy(StrBuf, "���W �w"); // ������"���W �w"��StrBuf�ɃR�s�[	
	_itoa_s(MouseX, StrBuf2, 10); // MouseX�̒l�𕶎���ɂ���StrBuf2�Ɋi�[
	lstrcat(StrBuf, StrBuf2); // StrBuf�̓��e��StrBuf2�̓��e��t������
	lstrcat(StrBuf, "�@�x "); // StrBuf�̓��e�ɕ�����"�@�x "��t������
	_itoa_s(MouseY, StrBuf2, 10); // MouseY�̒l�𕶎���ɂ���StrBuf2�Ɋi�[
	lstrcat(StrBuf, StrBuf2); // StrBuf�̓��e��StrBuf2�̓��e��t������

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

	DrawString(0, 15, StrBuf, GetColor(0, 0, 0));

	for (int i = 0; i < 64; i++){
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

	for (int i = 0; i < 800; i++){
		if (Note[i].flag != 0){
			lstrcpy(StrBuf, "flag ");
			_itoa_s(i, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " : ");
			_itoa_s(Note[i].flag, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " min : ");
			_itoa_s(Note[i].min, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " sec : ");
			_itoa_s(Note[i].sec, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			lstrcat(StrBuf, " mill : ");
			_itoa_s(Note[i].mill, StrBuf2, 10);
			lstrcat(StrBuf, StrBuf2);
			DrawString(1800, 30 + i * 15, StrBuf, GetColor(0, 0, 0));
		}
	}

}

int UpdateKey(char Key[]){
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i<256; i++){
		if (tmpKey[i] != 0)		Key[i]++;
		else	Key[i] = 0;
	}
	return 0;
}

void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY){
	DrawGraph(X - GsX / 2, Y - GsY / 2, Graph, TRUE);
}

int NoteHit(int circle, int button){
	double X, Y, Z;
	int judge[5] , i;

	X = Bp[button].x - Cp[circle].X;
	Y = Bp[button].y - Cp[circle].Y;

	Z = (int) sqrt(pow(X, 2) + pow(Y, 2));

	judge[0] = Player.jPerfect;
	judge[1] = Player.jGreat;
	judge[2] = Player.jGood;
	judge[3] = Gs.Circle_X / 2;
	judge[4] = Gs.Circle_X;

	for (i = 0; i < 4; i++)
		if (Z <= judge[i] && Z <= judge[i])		return i + 1;
	if (Z > judge[4] && Z > judge[4])	return 5;
}

int ScoreCalcu(int judge, int combo){
	float score = 200;

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

	if (combo <= 50)	score *= 1.0;
	else if (combo <= 100)	score *= 1.1;
	else if (combo <= 200)	score *= 1.15;
	else if (combo <= 400)	score *= 1.2;
	else if (combo <= 600)	score *= 1.25;
	else if (combo <= 800)	score *= 1.3;
	else if (combo <= 1000)	score *= 1.35;

	return score;
}

void ChartRead(int *min, int *sec, int *mill){
	int i, Chart, notes, times[8];
	char read[256], *mem;
	char cut[] = "[:.] ";

	Chart = FileRead_open("Chart/Mizugame_short.txt");

	FileRead_gets(read, 256, Chart);
	notes = atoi(read);

	i = 0;
	FileRead_gets(read, 256, Chart);
	mem = strtok(read, cut);
	*min = atoi(mem);
	mem = strtok(read, cut);
	*sec = atoi(mem);
	mem = strtok(read, cut);
	*mill = atoi(mem);
	mem = strtok(read, cut);


	for (i = 0; i < notes; i++){
		FileRead_gets(read, 256, Chart);
		mem = strtok(read, cut);
		Note[i].sec = atoi(mem);
		mem = strtok(read, cut);
		Note[i].min = atoi(mem);
		mem = strtok(read, cut);
		Note[i].mill = atoi(mem);
		mem = strtok(read, cut);
		Note[i].button = atoi(mem);
		Note[i].flag = 1;
	}

	FileRead_close(Chart);
}