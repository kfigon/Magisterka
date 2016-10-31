#pragma once

#include <iostream>
#include "FileReader.h"
#include "Struktury.h"
#include <string>
#include "Windows.h"
#include "Odbiornik.h"


// w releasie nie ma znaczenia endianess, jest tak samo szybko przy tej implementacji
// w debugu tez nie ma tragedii
void testPredkosciEndianessu(BinaryReader::Endian endian);
std::vector<string> plikiWKatalogu(const string& katalog);
void zrzucCiagDoPliku(const string& sciezka, SygnalBipolarny& sygnal);

void asd(BinaryReader::Endian endian, int skip, int plikidx)
{
	const string katalog = "D:\\Kamil\\_magisterka\\pomiary";
	auto pliki = plikiWKatalogu(katalog);
	if (pliki.empty())
	{
		cout << "Nie ma plikow .dat w katalogu: " << katalog << "\n";
		//return 0;
	}

	BinaryReader czytacz{ pliki[plikidx] };
	//const size_t rozmiarTablicy = czytacz.liczWymaganyRozmiarTablicy();
	const size_t rozmiarTablicy = 1024 * 4;

	//const auto endian = BinaryReader::Endian::Big;
	//const auto skip = 24;	// 24

	auto dane = czytacz.Czytaj(rozmiarTablicy, endian);
	if (dane.empty())
	{
		cout << "Failed to read file\n";
		system("pause");
		//return 0;
	}


	// todo: odbiornik, korelacja

	// jakby lepiej dla BE
	liczKorelacje(dane, skip, endian);

	cout << "\n";
}

int main()
{
	// zrzuty dla dra Sadowskiego
#if 0
	auto ciagI = GeneratorCiagow::generujCiagI();
	zrzucCiagDoPliku("ciagI.txt", *ciagI.get());

	auto ciagQ = GeneratorCiagow::generujCiagQ();
	zrzucCiagDoPliku("ciagQ.txt", *ciagQ.get());

	return 1;
#endif

	//asd(BinaryReader::Endian::Little, 23, 1);
	//asd(BinaryReader::Endian::Little, 24, 1);
	//asd(BinaryReader::Endian::Little, 25, 1);
	//asd(BinaryReader::Endian::Little, 26, 1);
	//asd(BinaryReader::Endian::Big, 23,	  1);
	//asd(BinaryReader::Endian::Big, 24,    1);
	//asd(BinaryReader::Endian::Big, 25,    1);
	//asd(BinaryReader::Endian::Big, 26,    1);

	asd(BinaryReader::Endian::Little, 25, 0);
	asd(BinaryReader::Endian::Little, 25, 1);
	asd(BinaryReader::Endian::Little, 25, 2);
	asd(BinaryReader::Endian::Little, 25, 3);


	return 0;
}

void testPredkosciEndianessu(BinaryReader::Endian endian)
{
	const std::string sciezka = "D:\\Kamil\\_magisterka\\pomiary\\Probki-2014-04-12-11-41-40.dat";

	LARGE_INTEGER start = {};
	LARGE_INTEGER stop = {};
	QueryPerformanceCounter(&start);

	BinaryReader cz{ sciezka };
	auto dane = cz.Czytaj(cz.liczWymanaganyRozmiarTablicy(), endian);
	dane;

	QueryPerformanceCounter(&stop);

	cout << ((endian == BinaryReader::Endian::Little) ? "LE: " : "BE: ")
		<< stop.QuadPart - start.QuadPart << "\n";
}

void zrzucCiagDoPliku(const string& sciezka, SygnalBipolarny& sygnal)
{
	BinaryWriter writer{ sciezka };
	std::vector<UCHAR> buf(sygnal.getDlugosc(), 0);
	for (size_t i = 0; i < sygnal.getDlugosc(); i++)
	{
		auto el = SygnalBipolarny::unmap(sygnal[i]);
		buf[i] = static_cast<UCHAR>(el);
	}
	writer.pisz(buf);
}

std::vector<string> plikiWKatalogu(const string& katalog)
{
	WIN32_FIND_DATA fileData = {};
	HANDLE handleDir = FindFirstFile((katalog + "/*").c_str(), &fileData);

	std::vector<string> out;

	if (handleDir == INVALID_HANDLE_VALUE)
	{
		cout << "Nie udalo sie utworzyc katalogu " << katalog << "\n";
		return out;
	}

	do
	{
		const string fileFullName = katalog + "\\" + fileData.cFileName;
		const bool isDirectory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (fileData.cFileName[0] == '.')
			continue;

		if (isDirectory)
			continue;

		out.push_back(fileFullName);
	} while (FindNextFile(handleDir, &fileData));

	FindClose(handleDir);

	return out;
}
