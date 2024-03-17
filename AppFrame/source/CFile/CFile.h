#pragma once

#include <string>

class CFile
{
public:
	// 読み込み
	CFile(const std::string filename);
	// 書き込み
	CFile(const std::string filename, void* data, int size);		// バイナリデータ
	CFile(const std::string filename, const std::string writestr) : CFile(filename, (void*)writestr.c_str(), (int)writestr.size()) {		// 文字列データ
		// 委任コンストラクタで、バイナリデータ書き込みを呼び出す
	}

	~CFile();
	std::string	Filename() { return _filename; }
	void* Data() { return _data; }
	std::string DataStr() { return std::string(_data); }
	int Size() { return _size; }
	bool Success() { return _success; }

private:
	void Init();

	std::string		_filename;

	int		_size;
	char* _data;
	bool	_success;
};


// char型の文字列に対する操作

// 指定の文字を見つけるまで進める
int FindString(const char* p, const char find, const void* last);
// 空白やコントロールコードをスキップする
int SkipSpace(const char* p, const void* last);
// ","までの文字列を取得する
// csv用
int GetString(const char* p, std::string* out);
int GetString(const char* p,const char find, std::string* out);
int GetString(const char* p, const char find, std::string* out,int maxSize);
// 10進数の整数値を取得する
int GetDecNum(const char* p, int* answer, int* digits = nullptr);
// 10進数の浮動小数点数を含めた値を取得する
int GetFloatNum(const char* p, float* answer);