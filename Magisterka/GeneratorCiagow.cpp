#include "GeneratorCiagow.h"

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagI()
{
	const std::vector<int> odczepy{ 15, 13, 9, 8, 7, 5, 0 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagQ()
{
	const std::vector<int> odczepy{ 15, 12, 11, 10, 6, 5, 4, 3, 0 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagPn(RejestrPrzesuwny& rejestr)
{
	std::vector<int> ciagBinarny(GeneratorCiagow::DLUGOSC_CIAGU_PN, 0);
	int iloscZerPodRzad = 0;
	int indeksPierwszejJedynkiPoZerach = 0;

    for (size_t i = 0; i < ciagBinarny.size(); i++)
    {
        const auto wygenerowanyBit = rejestr.getVal();
        ciagBinarny[i] = wygenerowanyBit;
        if (wygenerowanyBit == 0)
        {
            iloscZerPodRzad++;
            if (iloscZerPodRzad == 14)
            {
                DbgPrint("14 zer pod rzad pod indeksem %d\n", i);
                i++;
                // wstawiamy dodatkowe zero po 14 zerach. Jest 15 zer na samym koncu
                ciagBinarny[i] = 0;
                // kolejny element bedzie jedynka, wiec zaznaczamy jego pozycje, 
                // zeby potem stworzyc poprawny ciag PN
                indeksPierwszejJedynkiPoZerach = i + 1;
            }
        }
        else
        {
            iloscZerPodRzad = 0;
        }
    }
	return std::make_unique<SygnalBipolarny>(ciagBinarny, indeksPierwszejJedynkiPoZerach);
}

std::vector<int> GeneratorCiagow::generujBinarnyCiagWalsha(int nr)
{
    std::vector<int> ciag(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, 0);

    const auto liczbaBinarna = zamienNaBinarne(nr);
    int idBinarnej = liczbaBinarna.size() - 1;
    for (size_t idCiagu = 0; idCiagu < ciag.size(); )
    {
        if (idCiagu == 0)
        {
            ciag[idCiagu] = 0;
            idCiagu++;
        }
        else
        {
            // idziemy wstecz - to jest MSB...LSB
            const auto bit = (idBinarnej >= 0) ? liczbaBinarna[idBinarnej] : 0;
            idBinarnej--;
            if (bit == 0)
            {
                // bit 0 - dopsiz na koncu powtorzny ciag
                dopiszCiag(ciag, idCiagu, false);
            }
            else
            {
                // bit 1 - dopisz na koncu odwrocony ciag
                dopiszCiag(ciag, idCiagu, true);
            }
        }
    }

    return ciag;
}

void GeneratorCiagow::dopiszCiag(std::vector<int>& ciag, size_t& indeksCiagu, bool czyOdwracacBity)
{
    const auto dlugosc = (indeksCiagu ) * 2;
    
    int idPomocnicze = 0;
    for (size_t i = indeksCiagu; i < dlugosc; i++)
    {
        const auto bit = (czyOdwracacBity) ? !ciag[idPomocnicze] : ciag[idPomocnicze];
        
        ciag[i] = bit;
        idPomocnicze++;
        indeksCiagu++;
    }
}

std::vector<int> GeneratorCiagow::zamienNaBinarne(int num)
{
    if (num == 0)
    {
        return std::vector<int>{0};
    }

    std::vector<int> bin(liczIleBitowTrzeba(num), 0);

    int liczba = num;
    int idx = bin.size() - 1;

    while (liczba != 0 && idx >=0)
    {
        const auto resztaZDzielenia = liczba % 2;
        liczba = liczba / 2;
        
        bin[idx] = resztaZDzielenia;
        idx--;
    }
    
    return bin;
}

int GeneratorCiagow::liczIleBitowTrzeba(int num)
{
    int bity = 0;
    for (size_t i = 0; num > 0; num >>= 1)
    {
        bity++;
    }
    return bity;
}

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagWalsha(int nr)
{
    const auto ciagBinarny = generujBinarnyCiagWalsha(nr);
    return std::make_unique<SygnalBipolarny>(ciagBinarny);
}