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
extern MUSIC Music[MusicCntMax];

void Title()
{
	DrawBox(0, 0, Screen_X, Screen_Y, Status.White, TRUE);

	DrawGraph(Gp.Title_technyan_X, Gp.Title_technyan_Y, Graph.Title_technyan, TRUE);
	DrawGraph(Gp.Title_X, Gp.Title_Y, Graph.Title, TRUE);

	if (Global.StartCounter <= 30){
		DrawGraph(Gp.Start_X, Gp.Start_Y, Graph.Start, TRUE);
	}
	else if (Global.StartCounter <= 60){
		DrawExtendGraph(Center(Gs.Button_X, 'X'), Gp.Start_Y + Gs.Start_Y + 15,
			Center(Gs.Button_X, 'X') + Gs.Button_X, Gp.Start_Y + Gs.Start_Y + 15 + Gs.Button_Y,
			Graph.Button_Red, TRUE);
	}
	if (Global.StartCounter > 60){
		Global.StartCounter = 0;
	}

	Global.StartCounter++;

	if (Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		Flag.Title++;
		Flag.Select++;
		Global.StartCounter = 0;
	}
}

void TitleToSelect()
{
	if (Global.StartCounter == 0){
		MovePoint(Gp.Title_technyan_X, Gp.Title_technyan_Y, Gp.Title_technyan_X,
			Gp.Title_technyan_Y + 450, &Global.Move_X[0], &Global.Move_Y[0], 30);
		MovePoint(Gp.Title_X, Gp.Title_Y, Gp.Title_X, -Gs.Title_Y,
			&Global.Move_X[1], &Global.Move_Y[1], 30);

		Gp.Select_Y = -Gs.Select_Y;
		MovePoint(Gp.Select_X, Gp.Select_Y, Gp.Select_X, 25,
			&Global.Move_X[2], &Global.Move_Y[2], 30);
	}
	if (Global.StartCounter < 30){
		Gp.Title_technyan_X += Global.Move_X[0];
		Gp.Title_technyan_Y += Global.Move_Y[0];
		Gp.Title_X += Global.Move_X[1];
		Gp.Title_Y += Global.Move_Y[1];
	}
	if (Global.StartCounter < 60){
		Gp.Select_X += Global.Move_X[2];
		Gp.Select_Y += Global.Move_Y[2];
	}

	DrawGraph(Gp.Title_technyan_X, Gp.Title_technyan_Y, Graph.Title_technyan, TRUE);
	DrawGraph(Gp.Title_X, Gp.Title_Y, Graph.Title, TRUE);
	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);

	Global.StartCounter++;
	if (Global.StartCounter == 60){
		Global.StartCounter = 0;
		Flag.Title = 0;
		Flag.Select = 1;
	}

}