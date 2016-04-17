#pragma once
#include <vector>

// sygnal cykliczny, getElement() zapetla sie
class SygnalBipolarny
{
private:
	std::vector<int> mCiag;
	int mIndeksPoczatku = 0;

public:
	SygnalBipolarny(const std::vector<int>& ciagBinarny);
	SygnalBipolarny(const std::vector<int>& ciagBinarny, int indeksPierwszejJedynkiPoZerach);
	SygnalBipolarny(const SygnalBipolarny& sygnalDoSkopiowania);

	~SygnalBipolarny() = default;

	// mapuje binarna wartosc na bipolarna
	static int map(int bit)
	{
		if (bit == 0)
			return 1;
		else
			return -1;
	}

	// mapuje bipolarna wartosc na bit
	static int unmap(int bipolar)
	{
		if (bipolar == 1)
			return 0;
		else
			return 1;
	}

	int getElement(int idx) const
	{
		return mCiag[(mIndeksPoczatku + idx) % getDlugosc()];
	}

	int operator[](int idx) const
	{
		return getElement(idx);
	}

	void przesunWLewo()
	{
		mIndeksPoczatku++;
	}

	void przesunWPrawo()
	{
		mIndeksPoczatku--;
		if (mIndeksPoczatku < 0)
			mIndeksPoczatku = getDlugosc() - 1;
	}

	size_t getDlugosc() const { return mCiag.size(); }
};

