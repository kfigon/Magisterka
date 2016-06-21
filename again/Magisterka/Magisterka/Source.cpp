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
void zrzucCiagIDoPliku(const string& sciezka)
{
	BinaryWriter writer{ sciezka };
	auto ciagI = GeneratorCiagow::generujCiagI();

	std::vector<UCHAR> buf(ciagI->getDlugosc(), 0);
	for (size_t i = 0; i < ciagI->getDlugosc(); i++)
		buf[i] = static_cast<UCHAR>(ciagI->getElement(i));
	
	writer.pisz(buf);

	delete ciagI;
}

int main()
{
	zrzucCiagIDoPliku("ciagI.txt");

	const string katalog = "D:\\Kamil\\_magisterka\\pomiary";
	auto pliki = plikiWKatalogu(katalog);
	if (pliki.empty())
	{
		cout << "Nie ma plikow .dat w katalogu: " << katalog << "\n";
		return 0;
	}

	BinaryReader czytacz{ pliki[0] };
	//const size_t rozmiarTablicy = czytacz.liczWymaganyRozmiarTablicy();
	const size_t rozmiarTablicy = 1024*4;

	auto endian = BinaryReader::Endian::Big;

	auto dane = czytacz.Czytaj(rozmiarTablicy, endian);
	if (dane.empty())
	{
		cout << "Failed to read file\n";
		system("pause");
		return 0;
	}


	// todo: odbiornik, korelacja
	// todo: zrzucanie ciagow do pliku dla dra

	// jakby lepiej dla BE
	liczKorelacje(dane, 24, endian);


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


std::vector<string> plikiWKatalogu(const string& katalog)
{
	WIN32_FIND_DATA fileData = {};
	HANDLE handleDir = FindFirstFile((katalog + "/*").c_str(), &fileData);

	std::vector<string> out;

	if (handleDir != INVALID_HANDLE_VALUE)
	{
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
	}

	return out;
}
