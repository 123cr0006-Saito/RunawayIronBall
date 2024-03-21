#include "../../Header/Function/DrawGauge.h"

//右から反時計回り
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
        vertex[i].pos.z = 0.0f;//2ｄなので奥行はなし
        vertex[i].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
        vertex[i].dif = GetColorU8(255, 255, 255, 255);
    }
};

DrawGauge::DrawGauge(const char* name, int start_pos, float size, bool flag)
    :handle(ResourceServer::LoadGraph(_T(name),_T(name))),
    _startPosition(start_pos),
    _size(size),
    _transFlag(flag ? -1 : 1)
{
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    for (int i = 0; i < 10; i++) {
        vertex[i].pos.z = 0.0f;//2ｄなので奥行はなし
        vertex[i].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
        vertex[i].dif = GetColorU8(255, 255, 255, 255);
    }
};
DrawGauge::~DrawGauge() {
    //特になし
};

bool DrawGauge::Process(VECTOR pos, float parcent, float parcent_max) {
    vertex[0].pos = pos;
    vertex[0].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
    vertex[0].dif = GetColorU8(255, 255, 255, 255);
    vertex[0].pos.z = 0.0f;//2ｄなので奥行はなし
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    float now_parcent = parcent / parcent_max;
    for (int i = 1; i < 10; i++) {
        float value = Math::Clamp(0, 0.125f * (i - 1), now_parcent);//各頂点の最大値をクランプする
        float rad = value * TwoPI * _transFlag + PI * ((int)_startPosition * 0.5f);//割合をラジアン化

        rad = rad - TwoPI * (int)(rad / TwoPI);//ラジアンの正規化
        if (rad < 0.0f)
        {
            rad += TwoPI;
        }

        //ｙ又はｘが決まっていればtanで割り出せる
        //tan = y / x
       // 上
        if (PI * 0.25f <= rad && rad <= PI * 0.75f)
        {
            vertex[i].pos.y = 0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // 左
        else if (PI * 0.75f <= rad && rad <= PI * 1.25f)
        {
            vertex[i].pos.x = -0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }
        // 下
        else if (PI * 1.25f <= rad && rad <= PI * 1.75f)
        {
            vertex[i].pos.y = -0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // 右
        else
        {
            vertex[i].pos.x = 0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }

        //画像を張るためのuvやポジションの変更
        vertex[i].u = vertex[i].pos.x + 0.5f;
        vertex[i].v = vertex[i].pos.y + 0.5f;

        vertex[i].pos.z = 0.0f;//2ｄなので奥行はなし
        vertex[i].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
        vertex[i].dif = GetColorU8(255, 255, 255, 255);

        vertex[i].pos.x = vertex[i].pos.x * _size + vertex[0].pos.x;//大きさを適当に50倍にしている
        vertex[i].pos.y = vertex[i].pos.y * _size + vertex[0].pos.y;//大きさを適当に50倍にしている

    }
    return true;
}

bool DrawGauge::Process(VECTOR pos, float parcent, float parcent_max, float ratio) {
    vertex[0].pos = pos;
    vertex[0].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
    vertex[0].dif = GetColorU8(255, 255, 255, 255);
    vertex[0].pos.z = 0.0f;//2ｄなので奥行はなし
    vertex[0].u = 0.5f;
    vertex[0].v = 0.5f;
    float now_parcent = parcent / parcent_max;
    for (int i = 1; i < 10; i++) {
        float value = Math::Clamp(0, 0.125f * (i - 1), now_parcent);//各頂点の最大値をクランプする
        float rad = value * TwoPI * _transFlag + PI * ((int)_startPosition * 0.5f);//割合をラジアン化

        rad = rad - TwoPI * (int)(rad / TwoPI);//ラジアンの正規化
        if (rad < 0.0f)
        {
            rad += TwoPI;
        }

        //ｙ又はｘが決まっていればtanで割り出せる
        //tan = y / x
       // 上
        if (PI * 0.25f <= rad && rad <= PI * 0.75f)
        {
            vertex[i].pos.y = 0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // 左
        else if (PI * 0.75f <= rad && rad <= PI * 1.25f)
        {
            vertex[i].pos.x = -0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }
        // 下
        else if (PI * 1.25f <= rad && rad <= PI * 1.75f)
        {
            vertex[i].pos.y = -0.5f;
            vertex[i].pos.x = vertex[i].pos.y / tan(rad);
        }
        // 右
        else
        {
            vertex[i].pos.x = 0.5f;
            vertex[i].pos.y = tan(rad) * vertex[i].pos.x;
        }

        //画像を張るためのuvやポジションの変更
        vertex[i].u = vertex[i].pos.x + 0.5f;
        vertex[i].v = vertex[i].pos.y + 0.5f;

        vertex[i].pos.z = 0.0f;//2ｄなので奥行はなし
        vertex[i].rhw = 1.0f;//ここは基本的に1.0ｆでよいらしい
        vertex[i].dif = GetColorU8(255, 255, 255, 255);

        vertex[i].pos.x = vertex[i].pos.x * (_size *ratio) + vertex[0].pos.x;//大きさを適当に50倍にしている
        vertex[i].pos.y = vertex[i].pos.y * (_size * ratio) + vertex[0].pos.y;//大きさを適当に50倍にしている

    }
    return true;
};

bool DrawGauge::Draw() {

    if (_transFlag < 0) {
        DrawPrimitiveIndexed2D(vertex, 10, _leftTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    else {
        DrawPrimitiveIndexed2D(vertex, 10, _rightTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    return true;
};

bool DrawGauge::Draw(int handle) {

    if (_transFlag < 0) {
        DrawPrimitiveIndexed2D(vertex, 10, _leftTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    else {
        DrawPrimitiveIndexed2D(vertex, 10, _rightTraianglList, 24, DX_PRIMTYPE_TRIANGLELIST, handle, true);
    }
    return true;
};