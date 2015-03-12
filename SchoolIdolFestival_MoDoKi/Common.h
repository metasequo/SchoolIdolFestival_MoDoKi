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

#define MusicNum 5

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
	int Title_technyan;
	int Title, Start, Select;
	int Button_Red, Button_Blue, Button_Green, Button_Yellow, Button_White;
	int Medal_Gold, Medal_Silver, Medal_Bronze;
	int Easy, Normal, Hard, Diff[3];
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
	int Title_X, Title_Y, Start_X, Start_Y, Select_X, Select_Y;
	int Button_X, Button_Y;
	int Medal_X, Medal_Y;
	int CentrJacket_X, CentrJacket_Y;
	int LeftJacket_X, LeftJacket_Y;
	int RightJacket_X, RightJacket_Y;
	int Easy_X, Easy_Y, Normal_X, Normal_Y, Hard_X, Hard_Y;
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
	int Title_technyan_X, Title_technyan_Y;
	int Title_X, Title_Y, Start_X, Start_Y, Select_X, Select_Y;
	int CentrJacket_X, CentrJacket_Y;
	int LeftJacket_X, LeftJacket_Y;
	int RightJacket_X, RightJacket_Y;
	int Easy_X, Easy_Y, Normal_X, Normal_Y, Hard_X, Hard_Y;
} GRAPHPOINT;

typedef struct tagSOUND
{
	int Dice, Mizugame;
	int pefect, great;
	int Push;
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
	int Title, Select, Game, End;
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
	int StartCounter;
	int SelectCounter;
	int TargetSong;
	int TargetDiff;
	int TechRand[9];
	int Mem_X[16], Mem_Y[16];
	int Move_X[16], Move_Y[16];
} GLOBAL;

typedef struct tagMUSIC
{
	char MusicName[64];
	char Artist[64];
	int MusicData;
	int Notes;
	int MMin, MSec, MMill;
	int Ranking[5];
	int Jacket;
	int Easy, Normal, Hard;
} MUSIC;

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
void ListRead();
void ChartRead(char *MusicName, char Type); 
void Struct(int MouseX, int MouseY);
void Format();
void Reset();

// Game.cpp //
void Game();

// Title.cpp //
void Title();
void TitleToSelect();

// Select.cpp //
void Select();
void MusicConfirm();

#include "Function.h"
#include "Game.h"
#include "Title.h"
#include "Select.h"