#include <string>
#include <Windows.h>

class CCheckSJisMojiCode
{
public:
	enum class enMojiType
	{
		None,
		HalfSymbol,		// 半角記号.
		HalfNumber,		// 半角数字.
		HalfEnglish,	// 半角英語.
		HalfKatakana,	// 半角カタカナ.
		FullSymbol,		// 全角記号(ギリシャ文字などは絶対いらないので省く).
		FullNumber,		// 全角数字.
		FullEnglish,	// 全角英語.
		FullKatakana,	// 全角カタカナ.
		FullHiragana,	// 全角ひらがな.
		Kanji,			// 漢字.
		NotSJis			// 当てはまらない.
	};
public:
	CCheckSJisMojiCode()
	{};
	~CCheckSJisMojiCode()
	{};
	// 文字コードチェック.
	const enMojiType CheckMojiCode(std::string str);
	// 全角半角.
	const bool CheckMojiIsFull(std::string str);
};