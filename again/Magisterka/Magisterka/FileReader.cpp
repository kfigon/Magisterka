#include "FileReader.h"


fstreamWrapper::fstreamWrapper(const string& sciezkaDoPliku, bool isRead) :
mSciezkaDoPliku(sciezkaDoPliku)
{
	auto flagaInOut = (isRead) ? ios::in : ios::out;
	open(sciezkaDoPliku, ios::binary | flagaInOut | ios::ate);

	if (is_open())
	{
		seekg(0, end);
		mRozmiarPliku = static_cast<size_t>(tellg());
		seekg(0, beg);
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

BinaryReader::BinaryReader(const string& sciezkaDoPliku) :
fstreamWrapper(sciezkaDoPliku, true)
{}

std::vector<Data> BinaryReader::Czytaj(size_t dlugoscWektoraWyjsciowego, Endian endian)
{
	if (!is_open())
	{
		cout << "Nie udalo sie przeczytac pliku " << zwrocSciezkeDoPliku() << "\n";
		assert(false);
		return std::vector<Data>();
	}

	std::vector<Data> bufor(dlugoscWektoraWyjsciowego, 0);

	read(reinterpret_cast<char*>(bufor.data()), bufor.size()*sizeof(Data));
	cout << "Przeczytano plik" << zwrocSciezkeDoPliku() << ". ";

	cout << "Dane zwrocone w formacie " <<
		((endian == Endian::Big) ? "bin endian" : "little endian") << "\n";

	// szybkosc: w releasie nie ma znaczenia endianess, jest tak samo szybko przy tej implementacji
	// w debugu tez nie ma tragedii
	if (endian == Endian::Big)
	{
		for (size_t i = 0; i < bufor.size(); i++)
		{
			bufor[i].raw = static_cast<int>(_byteswap_ulong(bufor[i].raw));
		}
	}
	return bufor;
}

BinaryWriter::BinaryWriter(const string& sciezkaDoPliku) :
fstreamWrapper(sciezkaDoPliku, false)
{}

void BinaryWriter::pisz(const std::vector<UCHAR>& data)
{
	if (is_open())
	{
		// fstream::write() nie ma przeciazonego UCHARa, wiec zapisywal mi nulle i znak ascii 1
		for (size_t i = 0; i < data.size(); i++)
			operator<<(data[i]);
	}
	else
	{
		cout << "Zapis sie nie udal. Plik " << zwrocSciezkeDoPliku() << " nie jest otwarty!\n";
		assert(false);
	}
}