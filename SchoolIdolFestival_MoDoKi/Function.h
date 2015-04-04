#include "DxLib.h"
#include <math.h>
#include <string.h>

int BoxHit(int Al, int Ar, int At, int Au, int Bl, int Br, int Bt, int Bu);
int CircleHit(float Ax, float Ay, float Ar, float Bx, float By, float Br);
int Center(int GraphSize, char Tipe);
void MovePoint(int Before_X, int Before_Y, int After_X, int After_Y, int *Move_X, int *Move_Y, int Frame);
int UpdateKey(char Key []);
void DrawCirclGraph(int X, int Y, int Graph, int GsX, int GsY);
void DrawCirclExtendGraph(int X, int Y, int Graph, int Radius);
int Pythagorean(int Ax, int Ay, int Bx, int By);
int NoteHit(int circle, int button);
void CircleShift();
int ScoreCalcu(int judge, int Combo);
void ListRead();
void ChartRead(char *MusicName, char Type);
void CheckAllMusic();
void Simultaneous(int num, int ButtonNum);
void Struct(int MouseX, int MouseY);
void Format();
void Reset();