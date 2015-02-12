#include "Common.h"

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







