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
extern MUSIC Music[MusicNum];

void Select()
{
	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);
//	PlaySoundMem(Music[Global.TargetSong].MusicData, DX_PLAYTYPE_BACK);
	/*
	DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
	DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
	DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
	*/
	DrawGraph(Gp.CentrJacket_X, Gp.CentrJacket_Y, Music[Global.TargetSong].Jacket, TRUE);
	if (Global.TargetSong > 0)
		DrawExtendGraph(Gp.LeftJacket_X, Gp.LeftJacket_Y, Gp.LeftJacket_X + 350,
			Gp.LeftJacket_Y + 350, Music[Global.TargetSong - 1].Jacket, TRUE);
	if (Global.TargetSong < MusicNum - 1)
		DrawExtendGraph(Gp.RightJacket_X, Gp.RightJacket_Y, Gp.RightJacket_X + 350,
			Gp.RightJacket_Y + 350, Music[Global.TargetSong + 1].Jacket, TRUE);


	if (Global.Key[KEY_INPUT_V] == 1 || Global.Key[KEY_INPUT_F] == 1 ||
		Global.Key[KEY_INPUT_R] == 1 || Global.Key[KEY_INPUT_4] == 1){
		if (Global.TargetSong > 0)
		{
			PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
			StopSoundMem(Music[Global.TargetSong].MusicData);
			Global.TargetSong--;
		}
	}
	if (Global.Key[KEY_INPUT_N] == 1 || Global.Key[KEY_INPUT_J] == 1 ||
		Global.Key[KEY_INPUT_I] == 1 || Global.Key[KEY_INPUT_9] == 1){
		if (Global.TargetSong < MusicNum - 1)
		{
			PlaySoundMem(Sound.Push, DX_PLAYTYPE_BACK);
			StopSoundMem(Music[Global.TargetSong].MusicData);
			Global.TargetSong++;
		}
	}

	Global.SelectCounter++;

	if (Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		Flag.Select++;
		Global.SelectCounter = 0;
		Global.TargetDiff = 1;
	}
}

void MusicConfirm()
{
	DrawGraph(Gp.Select_X, Gp.Select_Y, Graph.Select, TRUE);
	DrawGraph(Gp.CentrJacket_X, Gp.CentrJacket_Y, Music[Global.TargetSong].Jacket, TRUE);

	/*
	switch (Global.TargetDiff)
	{
	case 0:
		if (Global.SelectCounter <= 30){
			DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		}
		DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		break;
	case 1:
		DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		if (Global.SelectCounter <= 30){
			DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		}
		DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		break;
	case 2:
		DrawGraph((Screen_X / 2 - Gs.Easy_X) / 2, Gp.CentrJacket_Y + 600, Graph.Easy, TRUE);
		DrawGraph((Screen_X - Gs.Normal_X) / 2, Gp.CentrJacket_Y + 600, Graph.Normal, TRUE);
		if (Global.SelectCounter <= 30){
			DrawGraph((Screen_X + Screen_X / 2 - Gs.Hard_X) / 2, Gp.CentrJacket_Y + 600, Graph.Hard, TRUE);
		}
		break;
	}
*/
	Global.SelectCounter++;
	
	if (Global.SelectCounter > 60){
		Global.SelectCounter = 0;
	}



	if (Global.TargetSong != 0){
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
	}

	
	if (Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		Flag.Select = 0;
		Flag.Game = 1;
		Global.SelectCounter = 0;
		Global.TargetDiff = 1;
		
		switch (Global.TargetDiff)
		{
		case 0:
			ChartRead(Music[Global.TargetSong].MusicName, 'E');
			break;
		case 1:
			ChartRead(Music[Global.TargetSong].MusicName, 'N');
			break;
		case 2:
			ChartRead(Music[Global.TargetSong].MusicName, 'H');
			break;
		}

	}
}