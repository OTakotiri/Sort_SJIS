#include <string>
#include <Windows.h>

class CCheckSJisMojiCode
{
public:
	enum class enMojiType
	{
		None,
		HalfSymbol,		// ���p�L��.
		HalfNumber,		// ���p����.
		HalfEnglish,	// ���p�p��.
		HalfKatakana,	// ���p�J�^�J�i.
		FullSymbol,		// �S�p�L��(�M���V�������Ȃǂ͐�΂���Ȃ��̂ŏȂ�).
		FullNumber,		// �S�p����.
		FullEnglish,	// �S�p�p��.
		FullKatakana,	// �S�p�J�^�J�i.
		FullHiragana,	// �S�p�Ђ炪��.
		Kanji,			// ����.
		NotSJis			// ���Ă͂܂�Ȃ�.
	};
public:
	CCheckSJisMojiCode()
	{};
	~CCheckSJisMojiCode()
	{};
	// �����R�[�h�`�F�b�N.
	const enMojiType CheckMojiCode(std::string str);
	// �S�p���p.
	const bool CheckMojiIsFull(std::string str);
};