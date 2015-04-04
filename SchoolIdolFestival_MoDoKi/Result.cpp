#include "Common.h"

extern DATEDATA Date;
extern GRAPH Graph;
extern GRAPHSIZE Gs;
extern SOUND Sound;
extern GRAPHPOINT Gp;
extern BUTTONPOINT Bp[9];
extern CIRCLEPOINT Cp[CircleNum];
extern NOTE Note[800];
extern FLAG Flag;
extern PLAYER Player;
extern STATUS Status;
extern GLOBAL Global;
extern MUSIC Music[MusicCntMax];

void Result(){
	int Sp_X = Center(Gs.Perfect_X, 'X') + Gs.Perfect_X / 2;
	int Np_X = Sp_X + Gs.Perfect_X;
	int Sp_Y = Gp.CentrJacket_Y;
	DrawBox(0, 0, Screen_X, Screen_Y, Status.White, TRUE);
	DrawGraph(Gp.Result_X, Gp.Result_Y, Graph.Result, TRUE);
	DrawExtendGraph(Gp.CentrJacket_X - 500, Gp.CentrJacket_Y, Gp.CentrJacket_X, Gp.CentrJacket_Y + 500, Music[Global.TargetMusic].Jacket, TRUE);
	if (Flag.Result == 1){
		PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_BACK);
		Flag.Result++;
	}

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 0, Graph.Perfect, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y, Graph.Number_m[Player.Perfect / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y, Graph.Number_m[(Player.Perfect % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y, Graph.Number_m[(Player.Perfect % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y, Graph.Number_m[Player.Perfect % 10], TRUE);

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 1, Graph.Great, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y + Gs.Number_Y * 1, Graph.Number_m[Player.Great / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y + Gs.Number_Y * 1, Graph.Number_m[(Player.Great % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y + Gs.Number_Y * 1, Graph.Number_m[(Player.Great % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y + Gs.Number_Y * 1, Graph.Number_m[Player.Great % 10], TRUE);

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 2, Graph.Good, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y + Gs.Number_Y * 2, Graph.Number_m[Player.Good / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y + Gs.Number_Y * 2, Graph.Number_m[(Player.Good % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y + Gs.Number_Y * 2, Graph.Number_m[(Player.Good % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y + Gs.Number_Y * 2, Graph.Number_m[Player.Good % 10], TRUE);

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 3, Graph.Bad, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y + Gs.Number_Y * 3, Graph.Number_m[Player.Bad / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y + Gs.Number_Y * 3, Graph.Number_m[(Player.Bad % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y + Gs.Number_Y * 3, Graph.Number_m[(Player.Bad % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y + Gs.Number_Y * 3, Graph.Number_m[Player.Bad % 10], TRUE);

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 4, Graph.Miss, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y + Gs.Number_Y * 4, Graph.Number_m[Player.Miss / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y + Gs.Number_Y * 4, Graph.Number_m[(Player.Miss % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y + Gs.Number_Y * 4, Graph.Number_m[(Player.Miss % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y + Gs.Number_Y * 4, Graph.Number_m[Player.Miss % 10], TRUE);

	DrawGraph(Sp_X, Sp_Y + Gs.Number_Y * 5, Graph.Combo, TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 1, Sp_Y + Gs.Number_Y * 5, Graph.Number_m[Player.MostCombo / 1000], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 2, Sp_Y + Gs.Number_Y * 5, Graph.Number_m[(Player.MostCombo % 1000) / 100], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 3, Sp_Y + Gs.Number_Y * 5, Graph.Number_m[(Player.MostCombo % 100) / 10], TRUE);
	DrawGraph(Np_X + Gs.Number_mX * 4, Sp_Y + Gs.Number_Y * 5, Graph.Number_m[Player.MostCombo % 10], TRUE);

	if (Global.ResultConter % 40 < 20)
	{
		DrawGraph(Gp.Score_X, Gp.CentrJacket_Y + 550, Graph.Score, TRUE);
		DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 1, Gp.CentrJacket_Y + 550, Graph.Number[Player.Score / 10000], TRUE);
		DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 2, Gp.CentrJacket_Y + 550, Graph.Number[(Player.Score % 10000) / 1000], TRUE);
		DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 3, Gp.CentrJacket_Y + 550, Graph.Number[(Player.Score % 1000) / 100], TRUE);
		DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 4, Gp.CentrJacket_Y + 550, Graph.Number[(Player.Score % 100) / 10], TRUE);
		DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 5, Gp.CentrJacket_Y + 550, Graph.Number[Player.Score % 10], TRUE);
	}

	if ((Global.Key[KEY_INPUT_F1] == 1 || Global.Key[KEY_INPUT_B] == 1) || Global.ResultConter > 500){
		PlaySoundMem(Sound.Push, DX_PLAYTYPE_NORMAL);
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		Reset();
	}

	Global.ResultConter++;
}