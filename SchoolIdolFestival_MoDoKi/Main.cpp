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

//�\����
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



// WinMain �֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//���O���Ȃ񂽂�
	SetOutApplicationLogValidFlag(FALSE);
	// �^�C�g����ύX
	SetMainWindowText("test");
	// �E�C���h�E���[�h�ɕύX
//	ChangeWindowMode( TRUE ) ;
	//�E�C���h�E�̑傫���ݒ�
	SetGraphMode(Screen_X, Screen_Y, 32);
	//�E�C���h�E�̑傫�������R�ɕύX�o���邩�ǂ����̃t���O
	SetWindowSizeChangeEnableFlag( TRUE ) ;
	// ����ʂ��g�p
	SetDrawScreen(DX_SCREEN_BACK);
	// �c�w���C�u�����̏�����
	if (DxLib_Init() == -1) return -1;
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	int i, j, k;

	Format();

	// �ǂ݂��񂾉������[�v�Đ����܂�(�wPlaySoundMem�x�֐��g�p)
//	PlaySoundMem(Sound.Mizugame, DX_PLAYTYPE_LOOP);


	// �Q�[�����[�v�J�n�@�G�X�P�[�v�L�[�������ꂽ��I������
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ̏�����
		ClearDrawScreen();
		// �}�E�X�̈ʒu���擾
		GetMousePoint(&Global.MouseX, &Global.MouseY);
		//�}�E�X�̃N���b�N���
		if (GetMouseInputLog(&Global.Button, &Global.ClickX, &Global.ClickY, TRUE) == 0){
			Flag.Click = 1;
		}
		else{
			Flag.Click = 0;
		}
		//�{�^���̉������
		UpdateKey(Global.Key);

		// ��ʍ���̗̈�Ɏl�p��`��,�O�ɕ`���Ă����������������
		DrawBox(0, 0, Screen_X , Screen_Y, Status.White, TRUE);
		DrawExtendGraph(0, 0, Screen_X, Screen_Y, Graph.Library, TRUE);
		DrawGraph(0, 0, Graph.Fade, TRUE);

		//�����\��
		Struct(Global.MouseX, Global.MouseY);

		//�Q�[�����g
		Game();

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







