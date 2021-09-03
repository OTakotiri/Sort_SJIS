#include "CheckSJISCode.h"

// 文字コードチェック.
const CCheckSJisMojiCode::enMojiType CCheckSJisMojiCode::CheckMojiCode(std::string str)
{
	unsigned int moji_code;					// 文字コード(16bitコード).
	unsigned char first_byte, second_byte;	// 第1byte、第2byte(8bitコード).
	enMojiType TmpType = enMojiType::NotSJis;			// 文字種.
	first_byte = (unsigned char)*str.c_str();			// 第1バイトに変換.
	second_byte = (unsigned char)*(str.c_str() + 1);	// 第2バイトに変換.
	moji_code = first_byte * 256 + second_byte;			// 文字コードに変換.
	// 漢字「亜」だと.
	// 0x889fの位置に「亜」はあって、第１バイトが 0x88 (10進は136)、第2バイトが0x9f(10進は159)となる.
	do
	{
		// 半角記号.
		if (((first_byte >= 0x21) && (first_byte <= 0x2f)) || ((first_byte >= 0x3a)) && (first_byte <= 0x40) ||
			((first_byte >= 0x5b) && (first_byte <= 0x60)) || ((first_byte >= 0x7b)) && (first_byte <= 0x7e) ||
			((first_byte >= 0xa1) && (first_byte <= 0xa5)) || (first_byte == 0xb0 ) || ((first_byte >= 0xde)) && (first_byte <= 0xdf))
		{
			TmpType = enMojiType::HalfSymbol;
			break;
		}
		// 半角数字.
		else if (((first_byte >= 0x30) && (first_byte <= 0x39)))
		{
			TmpType = enMojiType::HalfNumber;
			break;
		}
		// 半角英語.
		else if (((first_byte >= 0x41) && (first_byte <= 0x5a)) || ((first_byte >= 0x61)) && (first_byte <= 0x7a))
		{
			TmpType = enMojiType::HalfEnglish;
			break;
		}
		// 半角カタカナ.
		else if (((first_byte >= 0xa6) && (first_byte <= 0xdd)))
		{
			TmpType = enMojiType::HalfKatakana;
			break;
		}
		// 全角記号.
		else if (((moji_code >= 0x8141) && (moji_code <= 0x8156)) || ((moji_code >= 0x8159) && (moji_code <= 0x81ac)) ||
				((moji_code >= 0x81b8) && (moji_code <= 0x81bf)) || ((moji_code >= 0x81c8) && (moji_code <= 0x81ce)) ||
				((moji_code >= 0x81da) && (moji_code <= 0x81e8)) || ((moji_code >= 0x81f0) && (moji_code <= 0x81f7)) ||
				((moji_code == 0x81fc)))
		{
			// 無効文字じゃないかをチェック.
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullSymbol;
				break;
			}
		}
		// 全角数字.
		else if (((moji_code >= 0x824F) && (moji_code <= 0x8258)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullNumber;
				break;
			}
		}
		// 全角英語.
		else if (((moji_code >= 0x8260) && (moji_code <= 0x8279)) || ((moji_code >= 0x8281)) && (moji_code <= 0x829a))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullEnglish;
				break;
			}
		}
		// 全角カタカナ.
		else if (((moji_code >= 0x8340) && (moji_code <= 0x8396)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullKatakana;
				break;
			}
		}
		// 全角ひらがな.
		else if (((moji_code >= 0x829f) && (moji_code <= 0x82f1)))
		{
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::FullHiragana;
				break;
			}
		}
		// 漢字.
		else if (((moji_code >= 0x889f) && (moji_code <= 0x9872)) || ((moji_code >= 0x989f) && (moji_code <= 0xEAA4)))
		{
			// 無効文字じゃないかをチェック.
			if (((second_byte >= 0x40) && (second_byte <= 0xFC)) && ((first_byte < 0xA0) || (first_byte > 0xDF)) && (second_byte != 0x7F)) {
				TmpType = enMojiType::Kanji;
				break;
			}
		}


	} while (false);

	return TmpType;
}
// 第1バイトが全角文字の範囲ならtrueを返す.
const bool CCheckSJisMojiCode::CheckMojiIsFull(std::string str)
{
	unsigned char first_byte;
	first_byte = (unsigned char)*str.c_str();
	if (((first_byte >= 0x81) && (first_byte <= 0x9f)) || ((first_byte >= 0xe0) && (first_byte <= 0xfc)))return true;
	return false;
}
