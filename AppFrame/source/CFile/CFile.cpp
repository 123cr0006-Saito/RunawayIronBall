#include <fstream>
#include <iostream>
#include "CFile.h"
#include "../Application/UtilMacro.h"

// 読み込み
CFile::CFile(const std::string filename) {
	// メンバの初期化
	Init();
	_filename = filename;

	// ファイルを開く
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);	// 入力をバイナリ形式で
	if (!ifs) {
		// 開くの失敗
		return;
	}

	// ファイルサイズの取得
	ifs.seekg(0, std::ios_base::end);
	_size = (int)ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	// メモリを確保
	// 文字列として利用できるように、+1byte余計に確保
	_data = new char[_size + 1];

	// ファイルをロード
	ifs.read(_data, _size);

	// ファイルを閉じる
	ifs.close();

	// 文字列として利用できるように、データの末端に\0を付ける
	_data[_size] = '\0';

	// 処理成功
	_success = true;
}

// 書き込み
CFile::CFile(const std::string filename, void* data, int size) {
	// メンバの初期化
	Init();
	_filename = filename;

	// ファイルを開く
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);	// 出力をバイナリ形式で
	if (!ofs) {
		// 開くの失敗
		return;
	}

	// データの書き込み
	ofs.write((char*)data, size);

	// ファイルを閉じる
	ofs.close();

	// 処理成功
	_success = true;
}

// 書き込み


CFile::~CFile() {
	SAFE_DELETE(_data);
}

void CFile::Init() {
	_size = 0;
	_data = NULL;
	_filename = "";
	_success = false;
}

// 指定の文字を見つけるまで進める
int FindString(const char* p, const char find, const void* last)
{
	int c = 0;
	while (p[c] != find && (!last || &p[c] < last)) { c++; }
	return c;
}

// 空白やコントロールコードをスキップする
int SkipSpace(const char* p, const void* last)
{
	int c = 0;
	while (p[c] != '\0' && p[c] != '\r\n' && p[c] <= ' ' && (!last || &p[c] < last)) { c++; }
	return c;
}

// ","までの文字列を取得する
// csv用
int GetString(const char* p, std::string* out)
{
	int c = 0;
	c += GetString(p, ',', out);
	return c;
}

int GetString(const char* p, const char find, std::string* out)
{
	int c = 0;
	int len = FindString(&p[c], find, NULL);	// ","までの長さを得る
	std::string s(&p[c], &p[c + len]);	// 文字列をstring化
	*out = s;
	c += len;	// ","まで進める
	return c;
}

int GetString(const char* p, const char find, std::string* out, int maxSize)
{
	int c = 0;
	int len = FindString(&p[c], find, &p[maxSize]);	// ","までの長さを得る
	std::string s(&p[c], &p[c + len]);	// 文字列をstring化
	*out = s;
	c += len;	// ","まで進める
	return c;
}

// 10進数の整数値を取得する
int GetDecNum(const char* p, int* answer, int* digits) {
	int c = 0;
	c += SkipSpace(&p[c], NULL);	// 空白をスキップ

	// マイナス値など記号を取得
	int mul = 1;
	int dig = 1;
	if (p[c] == '-') { mul = -1; c++; }
	else if (p[c] == '+') { mul = 1; c++; }

	// 10進数を取得する
	int n = 0;
	while ('0' <= p[c] && p[c] <= '9') {
		n = n * 10 + (p[c] - '0');
		dig *= 10;
		c++;
	}

	// 記号をかけて答えに格納
	*answer = n * mul;

	if (digits != nullptr) {
		*digits = dig;
	}

	// 読み進んだ文字数を返す
	return c;
}

// 10進数の浮動小数点数を含めた値を取得する
int GetFloatNum(const char* p, float* answer) {
	int c = 0;
	c += SkipSpace(&p[c], NULL);	// 空白をスキップ

	// マイナス値など記号を取得
	int mul = 1;
	if (p[c] == '-') { mul = -1; c++; }
	else if (p[c] == '+') { mul = 1; c++; }

	// 10進数を取得する
	int n = 0;
	while ('0' <= p[c] && p[c] <= '9') {
		n = n * 10 + (p[c] - '0');
		c++;
	}

	//小数点があった場合の処理
	int decimalPoint = 0;
	int digits = 1;
	if (p[c] == '.') { c++; c += GetDecNum(&p[c], &decimalPoint, &digits); }

	// 記号をかけて答えに格納
	*answer = (n + static_cast<float>(decimalPoint) / digits) * mul;

	// 読み進んだ文字数を返す
	return c;
}

