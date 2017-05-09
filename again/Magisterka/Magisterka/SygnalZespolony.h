#pragma once
#include <complex>
#include <vector>

class SygnalZespolony
{
	std::vector<std::complex<int>> mTab;

public:
	SygnalZespolony(size_t rozmiar);
	SygnalZespolony(std::vector<std::complex<int>> tab);
	SygnalZespolony(std::vector<int> rzeczywiste);

	~SygnalZespolony(){}

	std::complex<int> get(int idx) const;
	void set(int idx, int real = 0, int imag = 0)
	{
		mTab[idx] = { real, imag };
	}

	size_t getSize() const { return mTab.size(); }
};


std::vector<std::complex<int>> liczKorelacje(const SygnalZespolony& a, const SygnalZespolony& b);
