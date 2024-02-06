
#include "appframe.h"

class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SelectGameStart();//�Q�[�����J�n���鍀�ڂ̏���
	void SelectOption();//�I�v�V������ʂɕψڂ��鍀�ڂ̏���
	void SelectGameEnd();//�Q�[�����I�����鍀�ڂ̏���
protected:
	XInput* _input;
	int _modeCount;//���I������Ă��鍀�ڂ̔ԍ�
	int _titleLogo;//�^�C�g�����S�̉摜�n���h��
	int _comandHandlle[6];//�^�C�g���̍��ڂŎg�p����摜�n���h��
}; 
