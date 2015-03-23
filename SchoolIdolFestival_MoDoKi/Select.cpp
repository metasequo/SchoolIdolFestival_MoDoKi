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

void Select()
{
	DrawBox(0, 0, Screen_X, Screen_Y, Status.White, TRUE);

	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);
//	PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_BACK);
	DrawGraph(Gp.CentrJacket_X, Gp.CentrJacket_Y, Music[Global.TargetMusic].Jacket, TRUE);

	if (CheckSoundMem(Music[Global.TargetMusic].MusicData) != 1){
		PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_LOOP);
	}

	if (Global.TargetMusic > 0){
		DrawExtendGraph(Gp.LeftJacket_X, Gp.LeftJacket_Y, Gp.LeftJacket_X + 350,
			Gp.LeftJacket_Y + 350, Music[Global.TargetMusic - 1].Jacket, TRUE);
	}
	else{
		DrawExtendGraph(Gp.LeftJacket_X, Gp.LeftJacket_Y, Gp.LeftJacket_X + 350,
			Gp.LeftJacket_Y + 350, Music[Global.MusicCnt - 1].Jacket, TRUE);
	}
	if (Global.TargetMusic < Global.MusicCnt - 1){
		DrawExtendGraph(Gp.RightJacket_X, Gp.RightJacket_Y, Gp.RightJacket_X + 350,
			Gp.RightJacket_Y + 350, Music[Global.TargetMusic + 1].Jacket, TRUE);
	}
	else{
		DrawExtendGraph(Gp.RightJacket_X, Gp.RightJacket_Y, Gp.RightJacket_X + 350,
			Gp.RightJacket_Y + 350, Music[0].Jacket, TRUE);
	}

	if (Global.Key[KEY_INPUT_V] == 1 || Global.Key[KEY_INPUT_F] == 1 ||
		Global.Key[KEY_INPUT_R] == 1 || Global.Key[KEY_INPUT_4] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		if (Global.TargetMusic > 0){
			Global.TargetMusic--;
		}
		else{
			Global.TargetMusic = Global.MusicCnt - 1;
		}
		PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_LOOP);
	}
	if (Global.Key[KEY_INPUT_N] == 1 || Global.Key[KEY_INPUT_J] == 1 ||
		Global.Key[KEY_INPUT_I] == 1 || Global.Key[KEY_INPUT_9] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		if (Global.TargetMusic < Global.MusicCnt - 1){
			Global.TargetMusic++;
		}
		else{
			Global.TargetMusic = 0;
		}
		PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_LOOP);
	}

	Global.SelectCounter++;

	if (Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		Flag.Select = 2;
		Global.SelectCounter = 0;
		Global.TargetDiff = 1;
		Global.Key[KEY_INPUT_F1] = 0;
		Global.Key[KEY_INPUT_B] = 0;
	}
}

void MusicConfirm()
{
	DrawBox(0, 0, Screen_X, Screen_Y, Status.White, TRUE);

	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);
	DrawGraph(Gp.CentrJacket_X, Gp.CentrJacket_Y, Music[Global.TargetMusic].Jacket, TRUE);

	switch (Global.TargetDiff)
	{
	case 0:
		if (Global.SelectCounter <= 15){
			DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		}
		DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		break;
	case 1:
		DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		if (Global.SelectCounter <= 15){
			DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		}
		DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		break;
	case 2:
		DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		if (Global.SelectCounter <= 15){
			DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		}
		break;
	}

	Global.SelectCounter++;
	
	if (Global.SelectCounter > 30){
		Global.SelectCounter = 0;
	}



	if (Global.Key[KEY_INPUT_V] == 1 || Global.Key[KEY_INPUT_F] == 1 ||
		Global.Key[KEY_INPUT_R] == 1 || Global.Key[KEY_INPUT_4] == 1){
		if (Global.TargetDiff > 0)
		{
			PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
			Global.TargetDiff--;
		}
	}
	if (Global.Key[KEY_INPUT_N] == 1 || Global.Key[KEY_INPUT_J] == 1 ||
		Global.Key[KEY_INPUT_I] == 1 || Global.Key[KEY_INPUT_9] == 1){
		if (Global.TargetDiff < 2)
		{
			PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
			Global.TargetDiff++;
		}
	}

	
	if (Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1){
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
		Flag.Select = 0;
		Flag.Game = 1;
		Global.SelectCounter = 0;
//		Global.TargetDiff = 1;
		
		switch (Global.TargetDiff)
		{
		case 0:
			ChartRead(Music[Global.TargetMusic].MusicName, 'E');
			break;
		case 1:
			ChartRead(Music[Global.TargetMusic].MusicName, 'N');
			break;
		case 2:
			ChartRead(Music[Global.TargetMusic].MusicName, 'H');
			break;
		}

	}
}