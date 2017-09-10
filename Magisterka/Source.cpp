#pragma once

#include <iostream>
#include "FileReader.h"
#include "Struktury.h"
#include <string>
#include "Windows.h"
#include "Odbiornik.h"
#include "SpeedTest.h"
#include "AproksymatorLiniowy.h"

#include <cstdlib>
#include <ctime>
#include "fec.h"
#include "viterbi.h"

std::vector<string> plikiWKatalogu(const string& katalog);
void zrzucCiagDoPliku(const string& sciezka, SygnalBipolarny& sygnal);
void rysujDane(const std::vector<Data>& dane);
void asd(int plikidx);

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

    asd(0);
    //asd(1);
    //asd(2);
    //asd(3);


	return 0;
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

void rysujDane(const std::vector<Data>& dane)
{
    std::string sciezka = "zrzutka.txt";
    std::fstream plik;
    plik.open(sciezka.c_str(), std::ios::out);
    if (plik.is_open())
    {
        for (size_t i = 0; i < dane.size(); i++)
        {
            plik << i << "\t" << dane[i].Q << "\n";
        }
    }
    plik.close();

    std::stringstream polecenie;
    polecenie << "python Wykres.py " << sciezka;
    system(polecenie.str().c_str());
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

void asd(int plikidx)
{
    /*const string katalog = "D:\\Kamil\\_magisterka\\pomiary";*/
    const string katalog = "C:\\Users\\kfigon\\Desktop\\uczelnia\\mgr\\_magisterka\\pomiary";
    auto pliki = plikiWKatalogu(katalog);
    if (pliki.empty())
    {
        cout << "Nie ma plikow .dat w katalogu: " << katalog << "\n";
        return;
    }

    //const size_t rozmiarTablicy = czytacz.liczWymaganyRozmiarTablicyNaCalyPlik();
    KalkulatorDlugosciCiagow k{ Stale::CZESTOTLIWOSC_PROBKOWANIA_HZ, Stale::CZESTOTLIWOSC_SYGNALU_HZ };
    const size_t rozmiarTablicy = k.ileProbek(GeneratorCiagow::DLUGOSC_CIAGU_PN * 11); //GeneratorCiagow::DLUGOSC_CIAGU_PN * 2;

    BinaryReader czytacz{ pliki[plikidx] };
    const auto dane = czytacz.Czytaj(rozmiarTablicy);
    if (dane.empty())
    {
        cout << "Failed to read file\n";
        system("pause");
        return;
    }

    // flagi konfiguracyjne
#define liczKorelacje 0
#define rysujKonselacje 0
#define rysujKorektyFaz 0
#define rysujRozwiniecia 0
#define rysujKorelacje 0
#define demoduluj 1

    Odbiornik o{ Stale::CZESTOTLIWOSC_PROBKOWANIA_HZ, Stale::CZESTOTLIWOSC_SYGNALU_HZ };
    const auto ciagI = GeneratorCiagow::generujCiagI();
    const auto ciagQ = GeneratorCiagow::generujCiagQ();
    // todo: 32 do synchro
    const auto ciagWalsha = GeneratorCiagow::generujCiagWalsha(32); // ciag Walsha32 do skupienia kanalu synchronizacyjnego

#if liczKorelacje
    //const auto wynikZespolony = o.liczKorelacje(dane, *ciagI);
    //const auto wynikKorelacji = o.liczModuly(wynikZespolony);
#endif

#if rysujKorelacje
    RysujWykres("korelacja.txt", wynikKorelacji);
#endif //rysujKorelacje

    const long long domyslnyProgKorelacji = 9000000;
    // w celu wyznaczenia korekty biore tylo probkowe
    // odcinki ciagu danych i je sumuje. Faza tak czy siak powinna dac 0, wiec bedize to
    // uzyte do korekty
    const auto domyslnaDlugoscCiagowDoZfazowania = 2096;

#if liczKorelacje == 0
    std::vector<WynikKorelacji> pikiKorelacji{ { 1266, 19575512 }, { 10777, 9987865 }, { 20940, 27329830 },
    { 20941, 19925612 }, { 21207, 18924286 }, { 30537, 25115077 }, { 30538, 34816201 } };
#endif

    cout << "znalazlem " << pikiKorelacji.size() << " prazki: \n\n";

    KalkulatorDlugosciCiagow asd{ Stale::CZESTOTLIWOSC_PROBKOWANIA_HZ, Stale::CZESTOTLIWOSC_SYGNALU_HZ };

    const auto ileProbekNaPn = asd.ileProbek(GeneratorCiagow::DLUGOSC_CIAGU_PN);
    
    for (size_t i = 0; i < pikiKorelacji.size(); i++)
    {
        stringstream output;
        auto prazekKorelacji = pikiKorelacji[i];
        prazekKorelacji.offset += (i*ileProbekNaPn);
        cout << "\n\n[" << i << "] " << "offset " << prazekKorelacji.offset << " val: " << prazekKorelacji.wartosc << "\n";

        // todo: wyniesc to wszystko ponizej do osobnej funkcji i agregowac bity, ktore wypluwa demodulator
        // potem to wszystko (jak sie uzbiera ich troche) wrzucic do obrobki kanalu synchronizacyjnego

        for (size_t ileProb = 0; ileProb < 8; ileProb++)
        {

            const auto wynikMnozenia = o.mnozenieZespoloneISumowanie(dane, *ciagI, prazekKorelacji.offset, domyslnaDlugoscCiagowDoZfazowania);
            const auto katyI = o.liczWartosciKatow(wynikMnozenia);
            const auto rozwinieteFazy = o.rozwinFaze(katyI);

#if rysujRozwiniecia
            RysujWykres({ "katyI.txt", "rozwiniecieFaz.txt" }, katyI, rozwinieteFazy);
#endif //rysujRozwiniecia

            const auto aproksymacjeFazy = o.aproksymujFazyDoKorekty(rozwinieteFazy, domyslnaDlugoscCiagowDoZfazowania);
            const auto korekty = o.wyznaczKorekte(aproksymacjeFazy);

#if rysujKorektyFaz
            {
                // zastosowanie korekty
                std::vector<Data> noweDane(dane);
                for (size_t j = 0; j < korekty.size(); j++)
                {
                    std::complex<double> d{
                    static_cast<double>(noweDane[j + prazekKorelacji.offset].I),
                    static_cast<double>(noweDane[j + prazekKorelacji.offset].Q) };

                    const auto w = d*korekty[j];
                    const short re = w.real();
                    const short im = w.imag();
                    noweDane[j + prazekKorelacji.offset].I = re;
                    noweDane[j + prazekKorelacji.offset].Q = im;
                }

                // katy po korekcji
                const auto wynikMnozenia2 = o.mnozenieZespoloneISumowanie(noweDane, *ciagI, prazekKorelacji.offset, domyslnaDlugoscCiagowDoZfazowania);
                const auto nowekatyI = o.liczWartosciKatow(wynikMnozenia2);
                RysujWykres({ "katyI.txt", "noweKaty.txt" }, katyI, nowekatyI);
            }
#endif //rysujKorektyFaz

            const auto skupionePrzedKorekta = o.skupWidmo(dane, *ciagI, *ciagQ, *ciagWalsha, prazekKorelacji.offset);
            const auto skupionePoKorekcie = o.korygujFaze(skupionePrzedKorekta, korekty);

#if rysujKonselacje

            // todo algorytm: 
            // - jak juz mam korelacje to wszystko robic kawalkami o dlugosci 26.66667ms
            // - korekta na caly ciag wzorcowy
            // - skupianie o czasie trwania wzorca
            // - kolejne probki przesuniete o 26.66667ms
            // - piki korelacji bardzo ladnie widac na wielokrotnosciach 34133 probek:
            //      30538, 64671, 98804 (wszystko oddalone o 34133)
            // moze byc koniecznosc zachowania tego samego powtarzania probek wzorca w kolejnyc wielokrotnosciach


            const int krokCalkowania = 266;
            const auto przed = o.calkowanie(skupionePrzedKorekta, krokCalkowania);
            const auto po = o.calkowanie(skupionePoKorekcie, krokCalkowania);

            // todo: te konstelacje nie wygladaja za dobrze
            RysujKonstelacje("przed.txt", przed);
            RysujKonstelacje("po.txt", po);

#endif //rysujKonstelacje

#if demoduluj
            // 26.66667ms to 34133 probek. 
            // po skupieniu mamy przeplecione bity o przeplywnosci 4.8k. 
            // 26.666667ms w 4.8k to 128 bity. symbol to 2 bity (QPSK) (64 symbole). BPSK 1:1
            // 34133/128 = 266.6640625

            const auto przedzialCalkowania = 266;
            const auto symbole = o.calkowanie(skupionePoKorekcie, przedzialCalkowania);

            Rozplatacz<std::complex<long long>> r;
            const auto rozplecione = r.rozplot(symbole);
            const auto zdemodulowane = o.demodulacjaBsk(rozplecione);
            const auto bityPowtorzone = o.toString(zdemodulowane);

            for (auto b : bityPowtorzone)
                cout << b;

            cout << "\nusuniete powtorzenia\n";
            const auto symboleOdrzucone = o.odrzucPowtorzenia(rozplecione);
            const auto bityOdrzucone = o.toString(o.demodulacjaBsk(symboleOdrzucone));

            for (auto b : bityOdrzucone)
                cout << b;

            cout << "\nzdekodowane\n";

            output << bityOdrzucone;
            
#endif //demoduluj

            cout << "\n\n";
        }

        cout << "\njedizemy z dekodowaniem:\n";

        //9 491 369
        std::vector<int> polynomials;
        polynomials.push_back(491);
        polynomials.push_back(369);

        // todo: dostaje 24 bity zamiast 32 na wyjsciu dekodera :(
        ViterbiCodec codec(9, polynomials);

        cout << codec.Decode(output.str());
    }
    // zatrzymanie programu, zeby przyjrzec sie danym
#if demoduluj
    system("Pause");
#endif

    cout << "\n";
}
