#include "CheckSJISCode.h"

// �����R�[�h�`�F�b�N.
const CCheckSJisMojiCode::enMojiType CCheckSJisMojiCode::CheckMojiCode(std::string str)
{
	unsigned int moji_code;					// �����R�[�h(16bit�R�[�h).
	unsigned char first_byte, second_byte;	// ��1byte�A��2byte(8bit�R�[�h).
	enMojiType TmpType = enMojiType::NotSJis;			// ������.
	first_byte = (unsigned char)*str.c_str();			// ��1�o�C�g�ɕϊ�.
	second_byte = (unsigned char)*(str.c_str() + 1);	// ��2�o�C�g�ɕϊ�.
	moji_code = first_byte * 256 + second_byte;			// �����R�[�h�ɕϊ�.
	// �����u���v����.
	// 0x889f�̈ʒu�Ɂu���v�͂����āA��P�o�C�g�� 0x88 (10�i��136)�A��2�o�C�g��0x9f(10�i��159)�ƂȂ�.
	do
	{
		// ���p�L��.
		if (((first_byte >= 0x21) && (first_byte <= 0x2f)) || ((first_byte >= 0x3a)) && (first_byte <= 0x40) ||
			((first_byte >= 0x5b) && (first_byte <= 0x60)) || ((first_byte >= 0x7b)) && (first_byte <= 0x7e) ||
			((first_byte >= 0xa1) && (first_byte <= 0xa5)) || (first_byte == 0xb0 ) || ((first_byte >= 0xde)) && (first_byte <= 0xdf))
		{
			TmpType = enMojiType::HalfSymbol;
			break;
		}
		// ���p����.
		else if (((first_byte >= 0x30) && (first_byte <= 0x39)))
		{
			TmpType = enMojiType::HalfNumber;
			break;
		}
		// ���p�p��.
		else if (((first_byte >= 0x41) && (first_byte <= 0x5a)) || ((first_byte >= 0x61)) && (first_byte <= 0x7a))
		{
			TmpType = enMojiType::HalfEnglish;
			break;
		}
		// ���p�J�^�J�i.
		else if (((first_byte >= 0xa6) && (first_byte <= 0xdd)))
		{
			TmpType = enMojiType::HalfKatakana;
			break;
		}
		// �S�p�L��.
		else if (((moji_code >= 0x8141) && (moji_code <= 0x8156)) || ((moji_code >= 0x8159) && (moji_code <= 0x81ac)) ||
				((moji_code >= 0x81b8) && (moji_code <= 0x81bf)) || ((moji_code >= 0x81c8) && (moji_code <= 0x81ce)) ||
				((moji_code >= 0x81da) && (moji_code <= 0x81e8)) || ((moji_code >= 0x81f0) && (moji_code <= 0x81f7)) ||
				((moji_code == 0x81fc)))
		{
			// ������������Ȃ������`�F�b�N.
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullSymbol;
				break;
			}
		}
		// �S�p����.
		else if (((moji_code >= 0x824F) && (moji_code <= 0x8258)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullNumber;
				break;
			}
		}
		// �S�p�p��.
		else if (((moji_code >= 0x8260) && (moji_code <= 0x8279)) || ((moji_code >= 0x8281)) && (moji_code <= 0x829a))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullEnglish;
				break;
			}
		}
		// �S�p�J�^�J�i.
		else if (((moji_code >= 0x8340) && (moji_code <= 0x8396)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullKatakana;
				break;
			}
		}
		// �S�p�Ђ炪��.
		else if (((moji_code >= 0x829f) && (moji_code <= 0x82f1)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullHiragana;
				break;
			}
		}
		// ����.
		else if (((moji_code >= 0x889f) && (moji_code <= 0x9872)) || ((moji_code >= 0x989f) && (moji_code <= 0xEAA4)))
		{
			// ������������Ȃ������`�F�b�N.
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::Kanji;
				break;
			}
		}


	} while (false);

	return TmpType;
}
// ��1�o�C�g���S�p�����͈̔͂Ȃ�true��Ԃ�.
const bool CCheckSJisMojiCode::CheckMojiIsFull(std::string str)
{
	unsigned char first_byte;
	first_byte = (unsigned char)*str.c_str();
	if (((first_byte >= 0x81) && (first_byte <= 0x9f)) || ((first_byte >= 0xe0) && (first_byte <= 0xfc)))return true;
	return false;
}
