#include "DxLib.h"
#include "../../Header/Function/Fps.h"

Fps::Fps()
{
    fps_count = 0;
    count0t = 0;
    for (int i = 0; i < FLAME; i++)
    {
        f[i] = 0;
    }
    ave = 0;
}

Fps::~Fps()
{

}

//FLAME fps になるようにfpsを計算・制御
bool Fps::WaitFps() {
    int term, i, gnt;
    static int t = 0;
    if (fps_count == 0) {//60フレームの1回目なら
        if (t == 0)//完全に最初ならまたない
            term = 0;
        else//前回記録した時間を元に計算
            term = count0t + 1000 - GetNowCount();
    }
    else    //待つべき時間=現在あるべき時刻-現在の時刻
        term = (int)(count0t + fps_count * (1000.0 / FLAME)) - GetNowCount();

    if (term > 0)//待つべき時間だけ待つ
        Sleep(term);

    gnt = GetNowCount();

    if (fps_count == 0)//60フレームに1度基準を作る
        count0t = gnt;
    f[fps_count] = gnt - t;//１周した時間を記録
    t = gnt;
    //平均計算
    if (fps_count == FLAME - 1) {
        ave = 0;
        for (i = 0; i < FLAME; i++)
            ave += f[i];
        ave /= FLAME;
    }
    fps_count = (++fps_count) % FLAME;

    return true;
}

bool Fps::DrawFps(int x, int y) {
    if (ave != 0) {
        DrawBox(0, 0, 120, 20, GetColor(128, 128, 128), true);
        DrawFormatString(x, y, GetColor(255, 255, 255), "fps:%lf", 1000 / ave); //double型は%lf
    }
    return true;
}