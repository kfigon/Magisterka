#include "FileReader.h"


fstreamWrapper::fstreamWrapper(const string& sciezkaDoPliku) :
mSciezkaDoPliku(sciezkaDoPliku)
{
	open(sciezkaDoPliku, ios::binary | ios::in | ios::ate);
	if (is_open())
	{
		mRozmiarPliku = static_cast<size_t>(tellg());
		seekg(0);
	}
	else
	{
		cout << "Nie udalo sie przeczytac pliku " << sciezkaDoPliku << "\n";
		assert(false);
	}
}

fstreamWrapper::~fstreamWrapper()
{
	close();
}

CzytaczoPisaczBinarnychPlikow::CzytaczoPisaczBinarnychPlikow(const string& sciezkaDoPliku) :
fstreamWrapper(sciezkaDoPliku)
{}

std::vector<Data> CzytaczoPisaczBinarnychPlikow::Czytaj(size_t dlugoscWektoraWyjsciowego, Endian endian)
{
	std::vector<Data> bufor(dlugoscWektoraWyjsciowego);
	if (is_open())
	{
		read(reinterpret_cast<char*>(bufor.data()), bufor.size()*sizeof(Data));
		cout << "Przeczytano plik" << zwrocSciezkeDoPliku() << ". ";

		cout << "Dane zwrocone w formacie " <<
			((endian == Endian::Big) ? "bin endian" : "little endian") << "\n";

		// w releasie nie ma znaczenia endianess, jest tak samo szybko przy tej implementacji
		// w debugu tez nie ma tragedii
		if (endian == Endian::Big)
		{
			for (size_t i = 0; i < bufor.size(); i++)
			{
				bufor[i].I = static_cast<short>(_byteswap_ushort(bufor[i].I));
				bufor[i].Q = static_cast<short>(_byteswap_ushort(bufor[i].Q));
			}
		}
	}
	else
	{
		cout << "Nie udalo sie przeczytac pliku " << zwrocSciezkeDoPliku() << "\n";
		assert(false);
	}

	return bufor;
}
void CzytaczoPisaczBinarnychPlikow::ZapiszPlik(const std::vector<Data>& buf, const string& sciezkaDoPlikuWyjsciowego)
{
	fstream zapisDanych;
	zapisDanych.open(sciezkaDoPlikuWyjsciowego, ios::out);

	if (zapisDanych.is_open())
	{
		auto* dane = buf.data();

		cout << "Zapisuje dane do " << sciezkaDoPlikuWyjsciowego << "\n";

		for (size_t i = 0; i < buf.size(); i++)
		{
			zapisDanych << "I:\t" << dane[i].I << "\tQ:\t" << dane[i].Q << "\n";
			PiszPostepPetli(i, buf.size());
		}
		zapisDanych.close();
		cout << "\n";
	}
}

void CzytaczoPisaczBinarnychPlikow::ZapiszPlik(const std::vector<char>& buf, const string& sciezkaDoPlikuWyjsciowego)
{
	fstream zapisDanych;
	zapisDanych.open(sciezkaDoPlikuWyjsciowego, ios::out);

	if (zapisDanych.is_open())
	{
		auto* dane = buf.data();

		cout << "Zapisuje dane do " << sciezkaDoPlikuWyjsciowego << "\n";
		for (size_t i = 0; i < buf.size(); i++)
		{
			zapisDanych << i << "\t" << static_cast<int>(dane[i]) << "\n";
			PiszPostepPetli(i, buf.size());
		}
		zapisDanych.close();
		cout << "\n";
	}
}