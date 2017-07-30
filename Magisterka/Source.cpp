#pragma once

#include <iostream>
#include "FileReader.h"
#include "Struktury.h"
#include <string>
#include "Windows.h"
#include "Odbiornik.h"
#include "SpeedTest.h"
#include "AproksymatorLiniowy.h"

#include "viterbi.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>


std::vector<string> plikiWKatalogu(const string& katalog);
void zrzucCiagDoPliku(const string& sciezka, SygnalBipolarny& sygnal);
void rysujDane(const std::vector<Data>& dane);

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
    KalkulatorDlugosciCiagow k{ Stale::CZESTOTLIWOSC_PROBKOWANIA_DANYCH_MHZ * 1000, Stale::SZYBKOSC_TRANSMISJI_CIAGU_ROZPRASZAJACEGO_MHZ * 1000 };
    const size_t rozmiarTablicy = k.ileProbek(GeneratorCiagow::DLUGOSC_CIAGU_PN * 4); //GeneratorCiagow::DLUGOSC_CIAGU_PN * 2;
    
    BinaryReader czytacz{ pliki[plikidx] };
	const auto dane = czytacz.Czytaj(rozmiarTablicy);
    if (dane.empty())
	{
		cout << "Failed to read file\n";
		system("pause");
		return;
    }
#define rysujKonselacje 1
#define rysujKorektyFaz 0
#define rysujRozwiniecia 0
#define rysujKorelacje 0
#define demoduluj 0

    Odbiornik o{ 25, 24 }; 
    const auto ciagI = GeneratorCiagow::generujCiagI();
    const auto ciagQ = GeneratorCiagow::generujCiagQ();
    const auto ciagWalsha = GeneratorCiagow::generujCiagWalsha(32);

    const auto wynikZespolony = o.liczKorelacje(dane, *ciagI);
    const auto wynikKorelacji = o.liczModuly(wynikZespolony);

#if rysujKorelacje
    RysujWykres("korelacja.txt", wynikKorelacji);
#endif //rysujKorelacje

    const long long domyslnyProgKorelacji = 7000000;
    const auto domyslnaDlugoscCiagowDoZfazowania = 2096;

    const auto progowane = o.znajdzMaksimaKorelacji(wynikKorelacji, domyslnyProgKorelacji);
    cout << "znalazlem " << progowane.size() <<" prazki: \n\n";

    for (size_t i = 0; i < progowane.size(); i++)
    {
        const auto a = progowane[i];
        cout <<"["<<i<<"] "<< "offset " << a.offset << " val: " << a.wartosc << "\n";

        const auto wynikMnozenia = o.mnozenieZespoloneISumowanie(dane, *ciagI, a.offset, domyslnaDlugoscCiagowDoZfazowania);
        const auto katyI = o.liczWartosciKatow(wynikMnozenia);
        const auto rozwinieteFazy = o.rozwinFaze(katyI);

#if rysujRozwiniecia
        RysujWykres({ "katyI.txt", "rozwiniecieFaz.txt" }, katyI, rozwinieteFazy);
#endif //rysujRozwiniecia

        const auto aproksymacjeFazy = o.aproksymujFazyDoKorekty(rozwinieteFazy, domyslnaDlugoscCiagowDoZfazowania);
        const auto korekty = o.wyznaczKorekte(aproksymacjeFazy);

#if 0
        // zastosowanie korekty
        std::vector<Data> noweDane(dane);
        for (size_t j = 0; j < korekty.size();j++)
        {
            std::complex<double> d{ 
                static_cast<double>(noweDane[j + a.offset].I), 
                static_cast<double>(noweDane[j + a.offset].Q) };
           
            const auto w = d*korekty[j];
            const short re = w.real();
            const short im = w.imag();
            noweDane[j + a.offset].I = re;
            noweDane[j + a.offset].Q = im;
        }
#endif


#if rysujKorektyFaz
        // katy po korekcji
        const auto wynikMnozenia2 = o.mnozenieZespoloneISumowanie(noweDane, *ciagI, a.offset, domyslnaDlugoscCiagowDoZfazowania);
        const auto nowekatyI = o.liczWartosciKatow(wynikMnozenia2);
        RysujWykres({"katyI.txt", "noweKaty.txt"}, katyI, nowekatyI);

#endif //rysujKorektyFaz
       
#if rysujKonselacje
        // nie dziala?
        const auto skupionePrzedKorekta = o.skupWidmo(dane, *ciagI, *ciagQ, *ciagWalsha, a.offset);
//        const auto skupionePoKorekcie = o.skupWidmo(noweDane, *ciagI, *ciagQ, *ciagWalsha, a.offset);

        const auto skupionePoKorekcie = o.korygujFaze(skupionePrzedKorekta, korekty);

        //const int krokSumy = 266;
        const int krokSumy = 266;
        std::vector<complex<long long>> przed(skupionePrzedKorekta.size() / krokSumy, 0);
        std::vector<complex<long long>> po(skupionePoKorekcie.size() / krokSumy, 0);

        for (size_t j = 0; j < przed.size()-2; j++)
        {
            complex<long long> sumPrzed{ 0, 0 };
            complex<long long> sumPo{ 0, 0 };

            for (size_t k = j*krokSumy; k < (j+1)*krokSumy; k++)
            {
                sumPrzed += skupionePrzedKorekta[k];
                sumPo += skupionePoKorekcie[k];
            }
            przed[j] = sumPrzed;
            po[j] = sumPo;
        }

        
        RysujKonstelacje("przed.txt", przed);
        RysujKonstelacje("po.txt", po);

#endif //rysujKonstelacje

#if demoduluj
        const auto skupionePoKorekcie = o.skupWidmo(noweDane, *ciagI, *ciagQ, *ciagWalsha, a.offset);
        
        // 26.66667ms to 34133 probek. 
        // po skupieniu mamy przeplywnosc 4.8k. 
        // 26.666667ms w 4.8k to 128. 34133/128 = 266.6640625
        const auto przedzialCalkowania = 266;    
        const auto bity = o.demodulacja(skupionePoKorekcie, przedzialCalkowania);

        cout << "mam " << bity.size() << " bitow:\n";
        Rozplatacz r;
        const int dlugoscRamkiPrzedRozplotem = 128; // 4.8kbps * 26.6...7 ms
        const auto rozplecione = r.rozplot(bity);
        stringstream s;
        for (size_t idRozplecionego = 0; idRozplecionego < rozplecione.size(); idRozplecionego++)
        {
            if (idRozplecionego != 0 && (idRozplecionego % dlugoscRamkiPrzedRozplotem == 0))
                cout << "\n";
            s << rozplecione[idRozplecionego];
            cout << rozplecione[idRozplecionego];
        }

        cout << "\nzdekodowane\n\n";

        //9 491 369
        std::vector<int> polynomials;
        polynomials.push_back(491);
        polynomials.push_back(369);

        ViterbiCodec codec(9, polynomials);

        cout << codec.Decode(s.str());
#endif //demoduluj

        cout << "\n\n";
    }
    

    system("Pause");
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