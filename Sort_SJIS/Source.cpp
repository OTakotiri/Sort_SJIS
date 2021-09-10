//==========================================
// C++17 , x86環境でビルド.

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include "CheckSJISCode.h"
// ソートするテキストを入れているファイルパス.
const std::string FILE_PATH = "TextData\\In\\";
// ソート後のテキストを出力するファイルパス.
const std::string OUT_FILENAME = "TextData\\EndData.txt";
// WstringをString型に変換.
const std::string WStringToString(std::wstring oWString);
// StringをWstring型に変換.
const std::wstring ConvertStringToWstring(std::string string);
// テキスト読み込み.
void InText(std::vector<std::string>& tx, std::string& Char);

int main()
{
	clock_t start = clock();    // スタート時間
	std::cout << "ファイル取得中..." << std::endl;
	std::vector<std::string> TxtFilePath;
	// txtファイル取得.
	auto eachLoad = [&](const std::filesystem::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// 拡張子.
		const std::string filePath = entry.path().string();					// ファイルパス.
		// 拡張子がtxtでなければ終了.
		if (extension != ".txt") return;

		TxtFilePath.push_back(filePath);
	};
	try {
		//指定パス(FILE_PATH)ディレクトリ内を再帰的に走査
		std::filesystem::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		// eachLoad関数を行う.
		std::for_each(dir_itr, end_itr, eachLoad);
	}
	// エラーキャッチ.
	catch (const std::filesystem::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"ファイル読み込み失敗");
	}

	std::vector<std::string> readList;	// 読みこんだテキストリスト.
	std::string Text;					// 読みこんだテキスト.

	std::cout << "テキスト取得中..." << std::endl;

	std::vector<std::string> TextVec;

	for (size_t i = 0; i < TxtFilePath.size(); i++) {

		std::fstream fileStream(TxtFilePath[i]);

		// ファイルが開けない場合.
		if (!fileStream.is_open()) {
			_ASSERT_EXPR(false, "テキストファイルが開けませんでした");
			return 0;
		}
		std::string line;	// 1行分を読み込むための文字.

		// 一行づつ読み込み.
		// テキストファイルはSJISで保存する.
		while (std::getline(fileStream, line)) {
			std::string buff = "";				// 文字を一時的に格納する.
			std::istringstream stream(line);
			// デフォルト ',' 区切りで文字を取得.
			while (std::getline(stream, buff, ',')) {
				// 読み込みリストに追加.
				readList.emplace_back(buff);
			}
		}

		for (const auto& v : readList) {
			for (int i = 0; i < static_cast<int>(v.size()); i++) {
				std::string f = v.substr(i, 1);
				if (IsDBCSLeadByte(v[i]) == TRUE) {
					f = v.substr(i++, 2);	// 全角文字.
				}
				else {
					f = v.substr(i, 1);	// 半角文字.
				}
				// @ もしくは $ ならやり直し.
				if (f == "@" || f == "$") continue;
				InText(TextVec, f);

			}
		}
		readList.clear();
		fileStream.close();
	}

	for (const auto& v : TextVec) {
		Text += v;
	}

	// wstringの生成.
	std::wstring ws = ConvertStringToWstring(Text);
	// テキストをソート.
	std::cout << "テキストソート中..." << std::endl;
	std::sort(ws.begin(), ws.end());
	Text = WStringToString(ws);

	std::vector<std::string> ReadSortList;	// ソート後の読みこんだテキストリスト.
	for (int i = 0; i < static_cast<int>(Text.size()); i++) {
		std::string f = Text.substr(i, 1);
		if (IsDBCSLeadByte(Text[i]) == TRUE) {
			f = Text.substr(i++, 2);	// 全角文字.
		}
		else {
			f = Text.substr(i, 1);	// 半角文字.
		}
		ReadSortList.emplace_back(f);
	}

	std::cout << "改行処理(SJISコードチェック)中..." << std::endl;
	std::unique_ptr<CCheckSJisMojiCode> pCheckMoji = std::make_unique<CCheckSJisMojiCode>();
	CCheckSJisMojiCode::enMojiType Type = CCheckSJisMojiCode::enMojiType::None;
	std::string f;
	int CharPage = 0;
	// 文字種事に改行を挟むため、SJISコードチェック.
	for (size_t i = 0; i < Text.size(); i++) {
		CCheckSJisMojiCode::enMojiType TmpType = pCheckMoji->CheckMojiCode(ReadSortList[CharPage]);
		std::cout << ReadSortList[CharPage];
		if (TmpType != Type) {
			if (Type != CCheckSJisMojiCode::enMojiType::None) {
				std::cout << std::endl;
				Text.insert(i, "\n");
				i++;
			}
			Type = TmpType;
		}
		if (pCheckMoji->CheckMojiIsFull(ReadSortList[CharPage]) == true) i++;
		CharPage++;
	}
	//===================================
	// ファイル作成.
	std::cout << "ファイル作成中..." << std::endl;
	std::fstream fo;
	fo.open(OUT_FILENAME, std::ios::out | std::ios::trunc);
	fo << Text;
	fo.close();
	clock_t end = clock();     // 終了時間

	std::cout << "処理時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	std::cout << "正常に終了しました。約2秒後に自動で終了" << std::endl;
	Sleep(2000);

	return 0;
}
// wstringをstirng型に変換.
const std::string WStringToString(std::wstring oWString)
{
	// wstring → SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char *)NULL, 0, NULL, NULL);
	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];
	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);
	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);
	// バッファの破棄
	delete[] cpMultiByte;

	// 変換結果を返す
	return(oRet);
}
// stringをwstirng型に変換.
const std::wstring ConvertStringToWstring(std::string string)
{
	// バッファサイズ取得.
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);
	// バッファの取得.
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];
	// SJISからwstringに.
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2
		, iBufferSize);
	// wstringの生成.
	std::wstring ws(cpUCS2, cpUCS2 + iBufferSize - 1);
	// バッファの破棄
	delete[] cpUCS2;

	return ws;
}

void InText(std::vector<std::string>& tx, std::string& Char)
{
	// 一度Wstringにしないと、日本語と英語の比較が引っかかる事があるため、一度変換.
	for (const auto& tx : tx) {
		std::wstring ws = ConvertStringToWstring(Char);
		std::wstring Text = ConvertStringToWstring(tx);

		if (Text.find(ws) != std::string::npos) {
			return;
		}
	}
	tx.emplace_back(Char);
}