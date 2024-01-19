#include "../../Header/Function/DrawGauge.h"

//�E���甽���v���
const unsigned short DrawGauge::_leftTraianglList[24] =
{
 4,3,5,  5,3,0,
 3,2,0,  0,2,1,
 5,0,6,  6,0,7,
 0,9,7,  7,9,8,
};

const unsigned short DrawGauge::_rightTraianglList[24] =
{
2, 3, 1, 1, 3, 0,
3, 4, 0, 0, 4, 5,
9, 0, 8, 8, 0, 7,
0, 5, 7, 7, 5, 6,
};

DrawGauge::DrawGauge(int image, int start_pos, float size, bool flag)
    :handle(image),
    _startPosition(start_pos),
    _size(size),
    _transFlag(flag ? -1 : 1)
{
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    for (int i = 0; i < 10; i++) {
        vertex[i].pos.z = 0.0f;//2���Ȃ̂ŉ��s�͂Ȃ�
        vertex[i].rhw = 1.0f;//�����͊�{�I��1.0���ł悢�炵��
        vertex[i].dif = GetColorU8(255, 255, 255, 255);
    }
};

DrawGauge::DrawGauge(const char* name, int start_pos, float size, bool flag)
    :handle(ResourceServer::LoadGraph(_T(name))),
    _startPosition(start_pos),
    _size(size),
    _transFlag(flag ? -1 : 1)
{
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    for (int i = 0; i < 10; i++) {
        vertex[i].pos.z = 0.0f;//2���Ȃ̂ŉ��s�͂Ȃ�
        vertex[i].rhw = 1.0f;//�����͊�{�I��1.0���ł悢�炵��
        vertex[i].dif = GetColorU8(255, 255, 255, 255);
    }
};

bool DrawGauge::Process(VECTOR pos, float parcent, float parcent_max) {
    vertex[0].pos = pos;
    vertex[0].rhw = 1.0f;//�����͊�{�I��1.0���ł悢�炵��
    vertex[0].dif = GetColorU8(255, 255, 255, 255);
    vertex[0].pos.z = 0.0f;//2���Ȃ̂ŉ��s�͂Ȃ�
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    float now_parcent = parcent / parcent_max;
    for (int i = 1; i < 10; i++) {
        float value = Math::Clamp(0, 0.125f * (i - 1), now_parcent);//�e���_�̍ő�l���N�����v����
        float rad = value * TwoPI * _transFlag + PI * ((int)_startPosition * 0.5f);//���������W�A����

        rad = rad - TwoPI * (int)(rad / TwoPI);//���W�A���̐��K��
        if (rad < 0.0f)
        {
            rad += TwoPI;
        }

        //�����͂������܂��Ă����tan�Ŋ���o����
        //tan = y / x
       // ��
        if (PI * 0.25f <= rad && rad <= PI * 0.75f)
        {
            vertex[i].pos.y = 0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // ��
        else if (PI * 0.75f <= rad && rad <= PI * 1.25f)
        {
            vertex[i].pos.x = -0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }
        // ��
        else if (PI * 1.25f <= rad && rad <= PI * 1.75f)
        {
            vertex[i].pos.y = -0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // �E
        else
        {
            vertex[i].pos.x = 0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }

        //�摜�𒣂邽�߂�uv��|�W�V�����̕ύX
        vertex[i].u = vertex[i].pos.x + 0.5f;
        vertex[i].v = vertex[i].pos.y + 0.5f;

        vertex[i].pos.z = 0.0f;//2���Ȃ̂ŉ��s�͂Ȃ�
        vertex[i].rhw = 1.0f;//�����͊�{�I��1.0���ł悢�炵��
        vertex[i].dif = GetColorU8(255, 255, 255, 255);

        vertex[i].pos.x = vertex[i].pos.x * _size + vertex[0].pos.x;//�傫����K����50�{�ɂ��Ă���
        vertex[i].pos.y = vertex[i].pos.y * _size + vertex[0].pos.y;//�傫����K����50�{�ɂ��Ă���

    }
    return true;
}

bool DrawGauge::Draw() {

    if (_transFlag < 0) {
        DrawPrimitiveIndexed2D(vertex, 10, _leftTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    else {
        DrawPrimitiveIndexed2D(vertex, 10, _rightTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    return true;
};