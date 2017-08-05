#include "Odbiornik.h"
#include "SpeedTest.h"

void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<WynikKorelacji>& wynikKorelacji)
{
	std::fstream plik;
	plik.open(sciezka.c_str(), std::ios::out);
	if (plik.is_open())
	{
		for (size_t i = 0; i < wynikKorelacji.size(); i++)
		{
			plik << wynikKorelacji[i].offset << "\t" << wynikKorelacji[i].wartosc << "\n";
		}
	}
	plik.close();
}

void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<double>& wynikKorelacji)
{
    std::fstream plik;
    plik.open(sciezka.c_str(), std::ios::out);
    if (plik.is_open())
    {
        for (size_t i = 0; i < wynikKorelacji.size(); i++)
        {
            plik << i << "\t" << wynikKorelacji[i] << "\n";
        }
    }
    plik.close();
}

void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<complex<long long>>& liczbyZespolone)
{
    std::fstream plik;
    plik.open(sciezka.c_str(), std::ios::out);
    if (plik.is_open())
    {
        for (size_t i = 0; i < liczbyZespolone.size(); i++)
        {
            plik << liczbyZespolone[i].real() << "\t" << liczbyZespolone[i].imag() << "\n";
        }
    }
    plik.close();
}


void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<WynikKorelacji>& wynikKorelacji)
{
    cout << "Przygotowuje wykres\n\n";
	PrzygotujPlikDanychKorelacji(sciezkaDoPliku, wynikKorelacji);

	std::stringstream polecenie;
	polecenie << "python Wykres.py " << sciezkaDoPliku;
	system(polecenie.str().c_str());
}

void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<double>& wynikKorelacji)
{
    cout << "Przygotowuje wykres\n\n";
    PrzygotujPlikDanychKorelacji(sciezkaDoPliku, wynikKorelacji);

    std::stringstream polecenie;
    polecenie << "python Wykres.py " << sciezkaDoPliku;
    system(polecenie.str().c_str());
}

void RysujWykres(const std::vector<std::string>& sciezkaDoPlikow, const std::vector<double>& wykresI, const std::vector<double>& wykresQ)
{
    cout << "Przygotowuje wykres\n\n";
    PrzygotujPlikDanychKorelacji(sciezkaDoPlikow[0], wykresI);
    PrzygotujPlikDanychKorelacji(sciezkaDoPlikow[1], wykresQ);

    std::stringstream polecenie;
    polecenie << "python Wykres.py " << sciezkaDoPlikow[0]<<" " <<sciezkaDoPlikow[1];
    system(polecenie.str().c_str());
}

void RysujKonstelacje(const std::string& sciezkaDoPliku, const std::vector<complex<long long>>& liczbyZespolone)
{
    cout << "Przygotowuje wykres\n\n";
    PrzygotujPlikDanychKorelacji(sciezkaDoPliku, liczbyZespolone);

    std::stringstream polecenie;
    polecenie << "python konstelacje.py " << sciezkaDoPliku;
    system(polecenie.str().c_str());
}

std::vector<complex<long long>> Odbiornik::liczKorelacje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI)
{
    if (dane.size() <  2 * ciagI.getDlugosc())
    {
        cout << "Podano za krotki ciag probek! Dane: " << dane.size() << ", ciag wzorcowy " << ciagI.getDlugosc() << "\n";
        assert(false);
        return std::vector<complex<long long>>();
    }

    cout << "Skupiam widmo...\n";


    KalkulatorDlugosciCiagow kalkulatorDlugosciCiagow{ getCzestotliwoscProbkowania(), getCzestotliwoscSygnalu() }; 
    const auto zakres = kalkulatorDlugosciCiagow.ileProbek(ciagI.getDlugosc()) +1; 
    
    std::vector<complex<long long>> out(zakres, 0);
    
    SpeedTester speedTester;
    speedTester.start();

    // zoffsetowane sa dane! Przeklada sie to na to samo, 
    for (size_t offset = 0; offset < out.size(); offset++)
    {
        std::complex<long long> wynikIteracji = 0;
        size_t indeksDanych = offset;
        
        PiszPostepPetli(offset, out.size());
        size_t indeksCiagu = 0;

        for (size_t i= 0; i< ciagI.getDlugosc(); i++)
        { 
            if (czySkipowac())
            {
                aktualizujIndeksCiagu(indeksCiagu, indeksDanych - offset);
            }

            const auto ciagReal = ciagI.getElement(indeksCiagu);
            const auto daneReal = dane[indeksDanych].I;
            const auto daneImagConj = -1 * dane[indeksDanych].Q;

            std::complex<long long> x{ ciagReal, 0 };
            std::complex<long long> y{ daneReal, daneImagConj };
            
            wynikIteracji += x*y;
               
            indeksDanych++;
            indeksCiagu++;
        }

        out[offset] = wynikIteracji;
    }

    speedTester.stop();
    cout << "\nKorelacja policzona, czas: " << speedTester.getCzas() << " [s]\n";
    return out;
}

std::vector<complex<long long>> Odbiornik::mnozenieZespoloneISumowanie(const std::vector<Data>& dane, const SygnalBipolarny& ciagI, int offset, int dlugoscPodCiagow, bool czyCiagQ)
{
    if (dane.size() < ciagI.getDlugosc())
    {
        cout << "Podano za krotki ciag probek! Dane: " << dane.size() << ", ciag wzorcowy " << ciagI.getDlugosc() << "\n";
        assert(false);

        return std::vector<complex<long long>>{};
    }

    KalkulatorDlugosciCiagow k{ getCzestotliwoscProbkowania(), getCzestotliwoscSygnalu()};
    const auto ileProbekNaCiagWzorcowy = k.ileProbek(ciagI.getDlugosc());

    const auto dlugoscWyjscia =  ileProbekNaCiagWzorcowy / dlugoscPodCiagow;
    std::vector<complex<long long>> out(dlugoscWyjscia);

    size_t indeksCiagu = 0;
    size_t indeksDanych = offset;
    for (size_t outIdx = 0; outIdx < out.size(); outIdx++)
    {
        std::complex<long long> wynikIteracji{};

        for (size_t i = 0; i < dlugoscPodCiagow; i++)
        {
            if (czySkipowac())
            {
                aktualizujIndeksCiagu(indeksCiagu, indeksDanych - offset);
            }
            const auto wartoscCiagu = ciagI.getElement(indeksCiagu);
            const auto daneReal = dane[indeksDanych].I;
            const auto daneImag = dane[indeksDanych].Q;

            std::complex<long long> x{wartoscCiagu, 0};
            
            // ciag Q traktowac jako czysto urojony
            if (czyCiagQ) {
                x = { 0, wartoscCiagu};
            }
          
            const std::complex<long long> y{ daneReal, daneImag };

            wynikIteracji += x*y;
            
            indeksCiagu++;
            indeksDanych++;
        }
        out[outIdx] = wynikIteracji;
    }

    return out;
}

std::vector<double> Odbiornik::liczWartosciKatow(const std::vector<complex<long long>>& liczbyZespolone)
{
    std::vector<double> out(liczbyZespolone.size(), 0);

    for (size_t i = 0; i < out.size(); i++)
    {
        const auto zesp = liczbyZespolone[i];
        out[i] = atan2(zesp.imag(), zesp.real());
    }
    return out;
}

std::vector<WynikKorelacji> Odbiornik::liczModuly(const std::vector<complex<long long>>& daneZespolone)
{
    std::vector<WynikKorelacji> wynik(daneZespolone.size(), 0);
    for (size_t i = 0; i < wynik.size(); i++)
    {
        wynik[i].offset = i;
        const auto x = daneZespolone[i];
        wynik[i].wartosc = sqrt(x.real()*x.real() + x.imag() * x.imag());
    }

    return wynik;
}

std::vector<WynikKorelacji> Odbiornik::znajdzMaksimaKorelacji(const std::vector<WynikKorelacji>& calaKorelacja, long long prog)
{
    std::vector<WynikKorelacji> out{};

    for (size_t i = 0; i < calaKorelacja.size(); i++)
    {
        const auto kor = calaKorelacja[i];
        if (kor.wartosc >= prog)
            out.push_back(kor);
    }

    return out;
}

std::vector<double> Odbiornik::rozwinFaze(const std::vector<double>& fazy, double prog)
{
    std::vector<double>out(fazy);

    for (size_t i = 1; i < out.size(); i++)
    {
        const auto val = fazy[i];
        const auto prevVal= fazy[i - 1];
        const auto roznica = val - prevVal;

        if (roznica > prog/2)
        {
            for (size_t j = i; j < out.size(); j++)
                out[j] = fazy[j] - prog;

            break;
        }
        else if (roznica < (-1 * prog / 2))
        {
            for (size_t j = i; j < out.size(); j++)
                out[j] = fazy[j] + prog;

            break;
        }
    }

    return out;
}

std::vector<double> Odbiornik::aproksymujFazyDoKorekty(const std::vector<double>& rozwinieteFazy, int dlugoscPodciagu)
{
    std::vector<double> out((rozwinieteFazy.size()+1)*dlugoscPodciagu);
    
    for (size_t i = 0; i < rozwinieteFazy.size()-1; i++)
    {
        // wartosc obecna i kolejna
        std::vector<WartosciFunkcji<double>> podciag(2, 0);
        podciag[0].arg = i*dlugoscPodciagu;
        podciag[0].val = rozwinieteFazy[i];

        podciag[1].arg = (i + 1)*dlugoscPodciagu;
        podciag[1].val = rozwinieteFazy[i+1];

        AproksymatorLiniowy<double> aproksymator{ podciag };

        auto stop = dlugoscPodciagu*(i + 1);
        auto start = i*dlugoscPodciagu;

        // ostatni, chce zaproksymowac dalej
        if (i == rozwinieteFazy.size() - 2)
            stop += 2*dlugoscPodciagu;

        for (size_t j = start; j < stop; j++)
            out[j]= aproksymator.getVal(j);
    }
    

    return out;
}

complex<double> Odbiornik::wyznaczKorekte(double faza)
{
    const auto real = cos(faza);
    const auto imag = sin(faza);

    return complex<double>{ real, -1 * imag };
}

std::vector<complex<double>> Odbiornik::wyznaczKorekte(const std::vector<double>& zaproksymowaneFazy)
{
    std::vector<complex<double>> out(zaproksymowaneFazy.size(), 0);

    for (size_t i = 0; i < out.size(); i++)
    {
        const auto fi = zaproksymowaneFazy[i];
        out[i] = wyznaczKorekte(fi);
    }
    return out;
}

std::vector<complex<long long>> Odbiornik::skupWidmo(const std::vector<Data>& dane, const SygnalBipolarny& ciagI, const SygnalBipolarny& ciagQ, const SygnalBipolarny& ciagWalsha, size_t offsetPn)
{
    if (dane.size() < ciagI.getDlugosc() + offsetPn)
    {
        cout << "Podano za krotki ciag probek! Dane: " << dane.size() << ", ciag wzorcowy " << ciagI.getDlugosc() << ", offset: " << offsetPn << "\n";
        assert(false);

        return std::vector<complex<long long>>{};
    }

    KalkulatorDlugosciCiagow k{ getCzestotliwoscProbkowania(), getCzestotliwoscSygnalu() };
    const auto ileProbekNaCiagWzorcowy = k.ileProbek(ciagI.getDlugosc());

    std::vector<complex<long long>> out(ileProbekNaCiagWzorcowy, 0);


    size_t indeksDanych = offsetPn;
    size_t indeksCiagu = 0;

    for (size_t i = 0; i < out.size(); i++)
    {
        if (czySkipowac())
        {
            aktualizujIndeksCiagu(indeksCiagu, indeksDanych - offsetPn);
        }

        const auto ciagReal = ciagI.getElement(indeksCiagu);
        const auto ciagImag = ciagQ.getElement(indeksCiagu);
        const auto ciagRealWalsh = ciagWalsha.getElement(indeksCiagu);

        const auto daneReal = dane[indeksDanych].I;
        const auto daneImag = dane[indeksDanych].Q;

        std::complex<long long> x{ ciagReal, ciagImag };
        std::complex<long long> y{ daneReal, daneImag };
        std::complex<long long>z{ ciagRealWalsh, 0 };

        indeksDanych++;
        indeksCiagu++;

        out[i] = x*y;
        out[i] *= z;
    }

    return out;
}

std::vector<complex<long long>> Odbiornik::korygujFaze(const std::vector<complex<long long>>& skupionyCiag, const std::vector<complex<double>>& korektyFazy)
{
    if (korektyFazy.size() < skupionyCiag.size())
    {
        cout << "Podano za krotki ciag probek! SkupionyCiag: " << skupionyCiag.size() << ", korekty: " << korektyFazy.size() << "\n";
        assert(false);

        return std::vector<complex<long long>>{};
    }
    
    std::vector<complex<long long>> out(skupionyCiag.size(), 0);
    for (size_t i = 0; i < out.size(); i++)
    {
        const complex<double> skupionyDouble = { static_cast<double>(skupionyCiag[i].real()), static_cast<double>(skupionyCiag[i].imag()) };
        const complex<double> wynik = skupionyDouble * korektyFazy[i];

        out[i] = { static_cast<long long>(round(wynik.real())), static_cast<long long>(round(wynik.imag())) };
    }

    return out;
}

std::vector<int> Odbiornik::demodulacja(const std::vector<complex<long long>>& skupioneWidmo, int przedzialCalkowania)
{
    const size_t ile = 2 *(skupioneWidmo.size() / przedzialCalkowania);

    std::vector<int> out(ile,0);
    
    size_t idProbek = 0;
    for (size_t i = 0; i < out.size(); i+=2)
    {
        // calkowanie
        std::complex<long long> zakumulowaneProbki = 0;
        for (size_t j = 0; j < przedzialCalkowania; j++)
        {
            if (idProbek >= skupioneWidmo.size())
                break;

            zakumulowaneProbki += skupioneWidmo[idProbek];
            idProbek++;
        }

        // demodulacja
        const auto re = zakumulowaneProbki.real();
        const auto im = zakumulowaneProbki.imag();

        int bit0 = 0; // bit pierwszy Yi
        int bit1 = 0; // drugi         Yq (3.1.3.1.16 spec)
        
        // pierwsza cwiartka
        if (re >= 0 && im >= 0)
        {
            bit0 = 0;
            bit1 = 0;
        }
        else if (im >= 0 && re < 0) // druga
        {
            bit0 = 1;
            bit1 = 0;
        }
        else if (im < 0 && re < 0) //trzecia
        {
            bit0 = 1;
            bit1 = 1;
        }
        else // czwarta
        {
            bit0 = 0;
            bit1 = 1;
        }
        out[i] = bit0;
        out[i + 1] = bit1;
    }

    return out;
}

std::string Odbiornik::toString(const std::vector<int>& ciag)
{
    std::stringstream s;
    for (auto bit : ciag)
    {
        s << bit;
    }

    return s.str();
}

std::vector<int> Rozplatacz::rozplot(const std::vector<int>& ciag) const
{
    std::vector<int> out(ciag.size(), 0);
    for (size_t i = 0; i < out.size(); i++)
        out[i] = ciag[getId(i)];

    return out;
}

int Rozplatacz::bro(int liczba) const
{
    int out = 0;
    const auto ileBitow = getM();

    for (size_t i = 0; i < ileBitow; i++)
    {
        const auto bitLiczby = (liczba >> i) & 0x1;

        // teraz wrzucam od konca poszczegolne bity:
        const auto pozycjaBitowaOdKonca = ileBitow - i - 1;
        out |= (bitLiczby << pozycjaBitowaOdKonca);
    }

    return out;

}