#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Struktury.h"
#include <assert.h>

using namespace std;

// podstawowa klasa opisu plikow, zawiera plik, jego rozmiar i sciezke
// pelni funkcje RAII dla klasy fstream
class fstreamWrapper : public fstream
{
private:
	size_t mRozmiarPliku = 0;
	const string mSciezkaDoPliku;

public:
	fstreamWrapper(const string& sciezkaDoPliku, bool isRead);
	virtual ~fstreamWrapper();

	// gettery
	string zwrocSciezkeDoPliku() const { return mSciezkaDoPliku; }
	size_t zwrocRozmiarPliku() const { return mRozmiarPliku; }
	bool czyOk() const { return (is_open() && zwrocRozmiarPliku() != 0); }
};

class BinaryReader :public fstreamWrapper
{
public:
	enum Endian{ Big, Little };

	BinaryReader(const string& sciezkaDoPliku);
	~BinaryReader() = default;

	std::vector<Data> Czytaj(size_t dlugoscWektoraWyjsciowego, Endian endian);
	size_t liczWymanaganyRozmiarTablicy() const { return zwrocRozmiarPliku() / sizeof(Data) - 1; }
};

class BinaryWriter : public fstreamWrapper
{
public:
	BinaryWriter(const string& sciezkaDoPliku);
	~BinaryWriter() = default;

	void pisz(const std::vector<UCHAR>& data);
};