﻿#include "Common.h"

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

void Game()
{
	DrawBox(0, 0, Screen_X, Screen_Y, Status.White, TRUE);

	int i;
	DrawExtendGraph(0, 0, Screen_X, Screen_Y, Graph.Library, TRUE);
	DrawExtendGraph(0, 0, Screen_X, Screen_Y, Graph.Fade, TRUE);

	DrawCirclExtendGraph(Gp.Onpu_X, Gp.Onpu_Y, Graph.Onpu, Gs.Onpu_X / 2);
	DrawCirclExtendGraph(Gp.Circle_X, Gp.Circle_Y, Graph.Circle_Blue, Gs.Circle_X / 2);

	DrawGraph(Gp.Score_X, Gp.Score_Y, Graph.Score, TRUE);
	DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 1, Gp.Score_Y, Graph.Number[Player.Score / 10000], TRUE);
	DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 2, Gp.Score_Y, Graph.Number[(Player.Score % 10000) / 1000], TRUE);
	DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 3, Gp.Score_Y, Graph.Number[(Player.Score % 1000) / 100], TRUE);
	DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 4, Gp.Score_Y, Graph.Number[(Player.Score % 100) / 10], TRUE);
	DrawGraph(Gp.Score_X + Gs.Score_X + Gs.Number_X * 5, Gp.Score_Y, Graph.Number[Player.Score % 10], TRUE);

	DrawGraph(Gp.Combo_X, Gp.Combo_Y, Graph.Combo, TRUE);
	DrawGraph(Gp.Combo_X + Gs.Combo_X + Gs.Number_mX * 1, Gp.Combo_Y, Graph.Number_m[Player.Combo / 1000], TRUE);
	DrawGraph(Gp.Combo_X + Gs.Combo_X + Gs.Number_mX * 2, Gp.Combo_Y, Graph.Number_m[(Player.Combo % 1000) / 100], TRUE);
	DrawGraph(Gp.Combo_X + Gs.Combo_X + Gs.Number_mX * 3, Gp.Combo_Y, Graph.Number_m[(Player.Combo % 100) / 10], TRUE);
	DrawGraph(Gp.Combo_X + Gs.Combo_X + Gs.Number_mX * 4, Gp.Combo_Y, Graph.Number_m[Player.Combo % 10], TRUE);


	for (i = 0; i < 9; i++){
		DrawCirclGraph(Bp[i].x, Bp[i].y, Graph.Technyan[Global.TechRand[i]], Gs.Technyan_X, Gs.Technyan_Y);
		DrawCirclGraph(Bp[i].x, Bp[i].y, Graph.Circle_Yellow, Gs.Circle_X, Gs.Circle_Y);
	}



/*	if (Global.Key[KEY_INPUT_F1] == 1){
		Reset();
		StopSoundMem(Sound.Mizugame);
		PlaySoundMem(Sound.Mizugame, DX_PLAYTYPE_BACK);
		Status.StartTime = GetNowCount();
	}
	*/
	if (Flag.Game == 1)
	{
		Status.StartTime = GetNowCount();
		PlaySoundMem(Music[Global.TargetMusic].MusicData, DX_PLAYTYPE_BACK);
		for (i = 0; i < 9; i++){
			Global.TechRand[i] = GetRand(8);
		}
		Flag.Game++;
	}

	Status.NowTime = GetNowCount();
	Status.ElapsedTime = Status.NowTime - Status.StartTime;

	//↑ or → でBPMプラス
	if (Global.Key[KEY_INPUT_UP] != 0
	//	|| Global.Key[KEY_INPUT_RIGHT] == 1
		){
		Status.BPM += 10;
	}
	//↓ or ← でBPMマイナス
	if (Global.Key[KEY_INPUT_DOWN] != 0
	//	|| Global.Key[KEY_INPUT_LEFT] == 1
		){
		Status.BPM -= 10;
		if (Status.BPM < 60)	Status.BPM = 60;
	}

	if (Global.Key[KEY_INPUT_RIGHT] != 0){
		Status.Timing += 10;
	}
	if (Global.Key[KEY_INPUT_LEFT] != 0){
		Status.Timing -= 10;
	}


	//		if (Global.Key[KEY_INPUT_A]==1 || CheckKeyInput(KEY_INPUT_A) == 0)
	{
		for (int j = 0; j < CircleNum; j++){
			if (Cp[j].flag == 0){
				for (i = 0; i <= Player.Notes; i++)
				{
					if (Note[i].flag == 1 &&
						(Status.ElapsedTime / 60000 == Note[i].min
						&& Status.ElapsedTime % 60000 / 1000 == Note[i].sec
						&& Status.ElapsedTime % 1000 / 10 >= Note[i].mill)
						||
						(Note[i].mill >= 95 &&
						(Status.ElapsedTime / 60000 == Note[i].min
						&& Status.ElapsedTime % 60000 / 1000 == Note[i].sec + 1
						&& (Status.ElapsedTime - 5) % 1000 / 10 >= Note[i].mill)
						)
						)

					{
						Cp[j].flag = 1;
						Cp[j].button = Note[i].button - 1;
						Bp[Cp[j].button].num++ ;
						Note[i].flag++;
						break;
					}
				}
				break;
			}
		}
	}

	//ノート（サークル）動作
	for (i = 0; i < CircleNum; i++){
		int button = Cp[i].button;

		if (Cp[i].flag != 0){
			if (Cp[i].flag == 1){
				//MoveNote();
				Cp[i].X = Gp.Circle_X;
				Cp[i].Y = Gp.Circle_Y;
				Cp[i].flag = 2;
			}

			if (Cp[i].flag == 2){
				Global.Frame = 60 / (Status.BPM / 60) - Cp[i].frame;
				//Global.Frame = 60 - Cp[i].frame;
				if (Global.Frame < 5)	Cp[i].flag = 3;
				MovePoint(Cp[i].X, Cp[i].Y, Bp[button].x, Bp[button].y, &Cp[i].MoveX, &Cp[i].MoveY, (int) Global.Frame);
			}

			if (Cp[i].flag >= 2){
				Cp[i].X += Cp[i].MoveX;
				Cp[i].Y += Cp[i].MoveY;
				Cp[i].frame++;
			}

			if (Cp[i].flag >= 1){
				Cp[i].Radius = (float) Gs.Circle_X * ((Bp[Cp[i].button].r - Pythagorean((float) Cp[i].X, Cp[i].Y, (float) Bp[Cp[i].button].x, (float) Bp[Cp[i].button].y)) / (float) Bp[Cp[i].button].r);
				//DrawCirclGraph(Cp[i].X, Cp[i].Y, Graph.Circle_Red, Gs.Circle_X, Gs.Circle_Y);
				DrawCirclExtendGraph(Cp[i].X, Cp[i].Y, Graph.Circle_Red, Cp[i].Radius);

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
				*/
			}
		}
	}


	//ボタン判定
	for (i = 0; i < CircleNum; i++){
		if (Cp[i].flag && Bp[Cp[i].button].flag == 0){
			Bp[Cp[i].button].num++;
			switch (Cp[i].button){
			case 0:
				if (Global.Key[KEY_INPUT_4] == 1
					|| (CircleHit(Global.ClickX, Global.ClickY, 1, Bp[0].x, Bp[0].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 1:
				if (Global.Key[KEY_INPUT_R] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[1].x, Bp[1].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 2:
				if (Global.Key[KEY_INPUT_F] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[2].x, Bp[2].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 3:
				if (Global.Key[KEY_INPUT_V] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[3].x, Bp[3].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 4:
				if (Global.Key[KEY_INPUT_SPACE] == 1 || Global.Key[KEY_INPUT_B] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[4].x, Bp[4].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 5:
				if (Global.Key[KEY_INPUT_N] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[5].x, Bp[5].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 6:
				if (Global.Key[KEY_INPUT_J] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[6].x, Bp[6].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 7:
				if (Global.Key[KEY_INPUT_I] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[7].x, Bp[7].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			case 8:
				if (Global.Key[KEY_INPUT_9] == 1
					|| (CircleHit(Global.MouseX, Global.MouseY, 1, Bp[8].x, Bp[8].y, Cp[i].Radius) &&
					Flag.Click == 1 && (Global.Button & MOUSE_INPUT_LEFT) == 1)
					){
					Cp[i].judge = NoteHit(i, Cp[i].button);
					Bp[Cp[i].button].flag = 1;
				}
				break;
			}
		}
	}

	//判定ごとの処理
	for (i = 0; i < CircleNum; i++){
		if (Cp[i].judge != 0){
			switch (Cp[i].judge){
			case 1:
				PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
				DrawGraph(Gp.Perfect_X, Gp.Perfect_Y, Graph.Perfect, TRUE);
				Player.Combo++;
				Player.Perfect++;
				Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
				break;
			case 2:
				PlaySoundMem(Sound.great, DX_PLAYTYPE_BACK);
				DrawGraph(Gp.Great_X, Gp.Great_Y, Graph.Great, TRUE);
				Player.Combo++;
				Player.Great++;
				Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
				break;
			case 3:
				PlaySoundMem(Sound.great, DX_PLAYTYPE_BACK);
				DrawGraph(Gp.Good_X, Gp.Good_Y, Graph.Good, TRUE);
				Player.Combo = 0;
				Player.Good++;
				Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
				break;
			case 4:
				DrawGraph(Gp.Bad_X, Gp.Bad_Y, Graph.Bad, TRUE);
				Player.Combo = 0;
				Player.Bad++;
				Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);
				Player.HP--;
				break;
			case 5:
				DrawGraph(Gp.Miss_X, Gp.Miss_Y, Graph.Miss, TRUE);
				Player.Combo = 0;
				Player.Miss++;
				Player.Score += ScoreCalcu(Cp[i].judge, Player.Combo);Player.HP--;
				break;
			}
			if (Player.MostCombo < Player.Combo){
				Player.MostCombo = Player.Combo;
			}
			Bp[Cp[i].button].flag = 0;
			Bp[Cp[i].button].num--;
			if (Bp[Cp[i].button].num < 0) Bp[Cp[i].button].num = 0;
			Cp[i].flag = 0;
			Cp[i].frame = 0;
			Cp[i].judge = 0;
			Cp[i].X = Gp.Circle_X;
			Cp[i].Y = Gp.Circle_Y;
			Cp[i].MoveX = 0;
			Cp[i].MoveY = 0;
			Cp[i].Radius = 0;
			Cp[i].button = 0;

			CircleShift();
		}
	}

	if (Player.HP  < 1){
		Player.HP = 1;
/*		DrawGraph(Gp.Gameover_X, Gp.Gameover_Y, Graph.Gameover, TRUE);
		for (i = 0; i < CircleNum; i++){
			Cp[i].flag = 0;
			Cp[i].frame = 0;
			Cp[i].judge = 0;
			Cp[i].X = Gp.Circle_X;
			Cp[i].Y = Gp.Circle_Y;
		}
		
		StopSoundMem(Sound.Mizugame);
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		Reset();
		Status.StartTime = 0;
	*/
	}

	if (Player.sMin * 60000 + Player.sSec * 1000 + Player.sMill < Status.ElapsedTime - 2000){
		StopSoundMem(Music[Global.TargetMusic].MusicData);
		Status.StartTime = 0;
		Flag.Game = 0;
		Flag.Result++;
	}
}
