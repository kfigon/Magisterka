#pragma once
#include <vector>

// sygnal cykliczny, getElement() zapetla sie
class SygnalBipolarny
{
private:
	std::vector<int> mCiag;
	long long mIndeksPoczatku = 0;
    size_t mOgraniczonaDlugosc = 0;

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

	// zwraca sygnal cyklicznie
    int getElement(int idx) const
	{    
        auto i = idx + mIndeksPoczatku;
        if (i < mCiag.size())
            return mCiag[i];

        return mCiag[i % mCiag.size()];
	}

    // zwraca "rzeczywiscie". Gdy indeks ujemny lub za duzy - zwraca bit 0
    int getElementEx(int idx) const
    {
        if (idx < 0 || idx >= getDlugosc())
            return map(0);

        return getElement(idx);
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

    void ograniczDlugosc(size_t val) { mOgraniczonaDlugosc = val; }

    size_t getDlugosc() const { return (mOgraniczonaDlugosc == 0) ? mCiag.size() : (mOgraniczonaDlugosc); }
};

