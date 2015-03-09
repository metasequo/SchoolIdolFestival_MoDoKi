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

void Select()
{
	DrawGraph(Gp.Title_technyan_X, Gp.Title_technyan_Y, Graph.Title_technyan, TRUE);
	DrawGraph(Gp.Title_X, Gp.Title_Y, Graph.Title, TRUE);
	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);

	if (Global.SelectCounter <= 30){
		DrawGraph(Gp.Start_X, Gp.Start_Y, Graph.Start, TRUE);
	}
	else if (Global.SelectCounter <= 60){
		DrawExtendGraph(Center(Gs.Button_X, 'X'), Gp.Start_Y + Gs.Start_Y + 15,
			Center(Gs.Button_X, 'X') + Gs.Button_X, Gp.Start_Y + Gs.Start_Y + 15 + Gs.Button_Y,
			Graph.Button_Red, TRUE);
	}
	if (Global.SelectCounter > 60){
		Global.SelectCounter = 0;
	}

	Global.SelectCounter++;

	if (Global.Key[KEY_INPUT_F1] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		Flag.Select++;
		Global.SelectCounter = 0;
	}

}