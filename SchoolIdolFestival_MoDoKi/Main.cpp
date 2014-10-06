#include "DxLib.h"
#include <math.h>

#define Screen_X 1120
#define Screen_Y 630

//�\����
typedef struct tagGRAPH
{
	int Circle_Blue, Circle_Green, Circle_Red;
} GRAPH;

typedef struct tagGRAPHSIZE
{
	int Circle_X, Circle_Y;
} GRAPHSIZE;

typedef struct tagSOUND
{
	int Dice;
	int pefect, great;
} SOUND;

typedef struct tagFLAG
{
	int Title = 0;
} FLAG;

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
}

DATEDATA Date;
GRAPH Graph;
GRAPHSIZE GraphSize;
SOUND Sound;

int UpdateKey(char Key []);

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
	// ��ʂ̏�����
	ClearDrawScreen();
	// ���ߐF��ύX(�s���N)
	SetTransColor(255, 0, 255);
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	int DateSum;
	int MouseX, MouseY;
	int SHandle;
	int White, Black, Gray;
//	char StrBuf[128], StrBuf2[32];
	char Key[256];

	// �F�̒l���擾
	White = GetColor(255, 255, 255);
	Black = GetColor(0, 0, 0);
	Gray = GetColor(123, 123, 123);
	//�����̏����ݒ�(����)
	GetDateTime(&Date);
	DateSum = Date.Year + Date.Mon + Date.Day + Date.Hour + Date.Min + Date.Sec;

	//�摜�ǂݍ���
	Graph.Circle_Blue = LoadGraph("Graph/Circle_Blue.png");
	Graph.Circle_Green = LoadGraph("Graph/Circle_Green.png");
	Graph.Circle_Red = LoadGraph("Graph/Circle_Red.png");

	//�摜�̃T�C�Y�𓾂�
	GetGraphSize(Graph.Circle_Blue, &GraphSize.Circle_X, &GraphSize.Circle_Y);

	// test.mp3�̃������ւ̓ǂݍ��݃T�E���h�n���h����SHandle�ɕۑ����܂�
	Sound.Dice = LoadSoundMem("Sound/Here are Dice.mp3");
	Sound.pefect = LoadSoundMem("Sound/perfect.mp3");
	Sound.great = LoadSoundMem("Sound/great.mp3");

	// �ǂ݂��񂾉������[�v�Đ����܂�(�wPlaySoundMem�x�֐��g�p)
	PlaySoundMem(Sound.Dice, DX_PLAYTYPE_LOOP);


	// �Q�[�����[�v�J�n�@�G�X�P�[�v�L�[�������ꂽ��I������
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �}�E�X�̈ʒu���擾
		GetMousePoint(&MouseX, &MouseY);
		UpdateKey(Key);

		// ��ʍ���̗̈�Ɏl�p��`��,�O�ɕ`���Ă����������������
		DrawBox(0, 0, Screen_X , Screen_Y, White, TRUE);

/*		// �\�����镶������쐬
		{
			lstrcpy(StrBuf, "���W �w"); // ������"���W �w"��StrBuf�ɃR�s�[	
			_itoa_s(MouseX, StrBuf2, 10); // MouseX�̒l�𕶎���ɂ���StrBuf2�Ɋi�[
			lstrcat(StrBuf, StrBuf2); // StrBuf�̓��e��StrBuf2�̓��e��t������
			lstrcat(StrBuf, "�@�x "); // StrBuf�̓��e�ɕ�����"�@�x "��t������
			_itoa_s(MouseY, StrBuf2, 10); // MouseY�̒l�𕶎���ɂ���StrBuf2�Ɋi�[
			lstrcat(StrBuf, StrBuf2); // StrBuf�̓��e��StrBuf2�̓��e��t������
		}

		// ���W�������`��
		DrawString(0, 0, StrBuf, Black);
*/
		Struct(MouseX, MouseY);

		if (Key[KEY_INPUT_SPACE] == 1){
			PlaySoundMem(Sound.pefect, DX_PLAYTYPE_BACK);
		}

		// ��莞�ԑ҂�
		WaitTimer(20);

		// ���b�Z�[�W����
		if (ProcessMessage() == -1)	break;	// �G���[���N�����烋�[�v�𔲂���
	}

	// �c�w���C�u�����̎g�p�I��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

int UpdateKey(char Key[]){
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i<256; i++){
		if (tmpKey[i] != 0){ // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}