
#include "appframe.h"

class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//���ڂ�I�����鏈��
	void SelectGameStart();//�Q�[�����J�n���鍀�ڂ̏���
	void SelectOption();//�I�v�V������ʂɕψڂ��鍀�ڂ̏���
	void SelectGameEnd();//�Q�[�����I�����鍀�ڂ̏���

	void UpdateSelectItems();//�I������Ă��鍀�ڂ̍X�V
	void  UpdateSelectToGameStart();//�Q�[�����J�n���鍀�ڂ̍X�V
	void UpdateCrackedScreen();//��ʂ�����鏈��
	void DrawTitleItems();//�^�C�g���̕`��
	void DrawCrackedScreen();//��ʂ�����`��


protected:
	XInput* _input;
	int _modeCount;//���I������Ă��鍀�ڂ̔ԍ�
	std::unordered_map<std::string, int> _handleMap;//�^�C�g���Ŏg�p����摜�n���h���̃}�b�v
	bool _IsGameStart;//�Q�[�����J�n�������ǂ���
	//-----------------
	//�^�C�g��������鏈���̕ϐ�
	int _modelHandle;//����郂�f���̃n���h��
	int _frameSize;//����郂�f���̃t���[����
	int _currentTime;//���݂̎���
	bool _IsBreak;//����鏈�����n�܂������ǂ���
	VECTOR* _MoveVec;//�ړ��x�N�g��
	VECTOR* _rotVec;//��]�x�N�g�� radian�l

}; 
