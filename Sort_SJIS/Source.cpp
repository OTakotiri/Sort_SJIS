//==========================================
// C++17 , x86���Ńr���h.

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include "CheckSJISCode.h"
// �\�[�g����e�L�X�g�����Ă���t�@�C���p�X.
const std::string FILE_PATH = "TextData\\In\\";
// �\�[�g��̃e�L�X�g���o�͂���t�@�C���p�X.
const std::string OUT_FILENAME = "TextData\\EndData.txt";
// Wstring��String�^�ɕϊ�.
const std::string WStringToString(std::wstring oWString);
// String��Wstring�^�ɕϊ�.
const std::wstring ConvertStringToWstring(std::string string);
// �e�L�X�g�ǂݍ���.
void InText(std::vector<std::string>& tx, std::string& Char);

int main()
{
	clock_t start = clock();    // �X�^�[�g����
	std::cout << "�t�@�C���擾��..." << std::endl;
	std::vector<std::string> TxtFilePath;
	// txt�t�@�C���擾.
	auto eachLoad = [&](const std::filesystem::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// �g���q.
		const std::string filePath = entry.path().string();					// �t�@�C���p�X.
		// �g���q��txt�łȂ���ΏI��.
		if (extension != ".txt") return;

		TxtFilePath.push_back(filePath);
	};
	try {
		//�w��p�X(FILE_PATH)�f�B���N�g�������ċA�I�ɑ���
		std::filesystem::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		// eachLoad�֐����s��.
		std::for_each(dir_itr, end_itr, eachLoad);
	}
	// �G���[�L���b�`.
	catch (const std::filesystem::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"�t�@�C���ǂݍ��ݎ��s");
	}

	std::vector<std::string> readList;	// �ǂ݂��񂾃e�L�X�g���X�g.
	std::string Text;					// �ǂ݂��񂾃e�L�X�g.

	std::cout << "�e�L�X�g�擾��..." << std::endl;

	std::vector<std::string> TextVec;

	for (size_t i = 0; i < TxtFilePath.size(); i++) {

		std::fstream fileStream(TxtFilePath[i]);

		// �t�@�C�����J���Ȃ��ꍇ.
		if (!fileStream.is_open()) {
			_ASSERT_EXPR(false, "�e�L�X�g�t�@�C�����J���܂���ł���");
			return 0;
		}
		std::string line;	// 1�s����ǂݍ��ނ��߂̕���.

		// ��s�Âǂݍ���.
		// �e�L�X�g�t�@�C����SJIS�ŕۑ�����.
		while (std::getline(fileStream, line)) {
			std::string buff = "";				// �������ꎞ�I�Ɋi�[����.
			std::istringstream stream(line);
			// �f�t�H���g ',' ��؂�ŕ������擾.
			while (std::getline(stream, buff, ',')) {
				// �ǂݍ��݃��X�g�ɒǉ�.
				readList.emplace_back(buff);
			}
		}

		for (const auto& v : readList) {
			for (int i = 0; i < static_cast<int>(v.size()); i++) {
				std::string f = v.substr(i, 1);
				if (IsDBCSLeadByte(v[i]) == TRUE) {
					f = v.substr(i++, 2);	// �S�p����.
				}
				else {
					f = v.substr(i, 1);	// ���p����.
				}
				// @ �������� $ �Ȃ��蒼��.
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

	// wstring�̐���.
	std::wstring ws = ConvertStringToWstring(Text);
	// �e�L�X�g���\�[�g.
	std::cout << "�e�L�X�g�\�[�g��..." << std::endl;
	std::sort(ws.begin(), ws.end());
	Text = WStringToString(ws);

	std::vector<std::string> ReadSortList;	// �\�[�g��̓ǂ݂��񂾃e�L�X�g���X�g.
	for (int i = 0; i < static_cast<int>(Text.size()); i++) {
		std::string f = Text.substr(i, 1);
		if (IsDBCSLeadByte(Text[i]) == TRUE) {
			f = Text.substr(i++, 2);	// �S�p����.
		}
		else {
			f = Text.substr(i, 1);	// ���p����.
		}
		ReadSortList.emplace_back(f);
	}

	std::cout << "���s����(SJIS�R�[�h�`�F�b�N)��..." << std::endl;
	std::unique_ptr<CCheckSJisMojiCode> pCheckMoji = std::make_unique<CCheckSJisMojiCode>();
	CCheckSJisMojiCode::enMojiType Type = CCheckSJisMojiCode::enMojiType::None;
	std::string f;
	int CharPage = 0;
	// �����펖�ɉ��s�����ނ��߁ASJIS�R�[�h�`�F�b�N.
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
	// �t�@�C���쐬.
	std::cout << "�t�@�C���쐬��..." << std::endl;
	std::fstream fo;
	fo.open(OUT_FILENAME, std::ios::out | std::ios::trunc);
	fo << Text;
	fo.close();
	clock_t end = clock();     // �I������

	std::cout << "�������� = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	std::cout << "����ɏI�����܂����B��2�b��Ɏ����ŏI��" << std::endl;
	Sleep(2000);

	return 0;
}
// wstring��stirng�^�ɕϊ�.
const std::string WStringToString(std::wstring oWString)
{
	// wstring �� SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char *)NULL, 0, NULL, NULL);
	// �o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];
	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);
	// string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);
	// �o�b�t�@�̔j��
	delete[] cpMultiByte;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}
// string��wstirng�^�ɕϊ�.
const std::wstring ConvertStringToWstring(std::string string)
{
	// �o�b�t�@�T�C�Y�擾.
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);
	// �o�b�t�@�̎擾.
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];
	// SJIS����wstring��.
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2
		, iBufferSize);
	// wstring�̐���.
	std::wstring ws(cpUCS2, cpUCS2 + iBufferSize - 1);
	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	return ws;
}

void InText(std::vector<std::string>& tx, std::string& Char)
{
	// ��xWstring�ɂ��Ȃ��ƁA���{��Ɖp��̔�r�����������鎖�����邽�߁A��x�ϊ�.
	for (const auto& tx : tx) {
		std::wstring ws = ConvertStringToWstring(Char);
		std::wstring Text = ConvertStringToWstring(tx);

		if (Text.find(ws) != std::string::npos) {
			return;
		}
	}
	tx.emplace_back(Char);
}