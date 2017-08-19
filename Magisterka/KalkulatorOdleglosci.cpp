#include "KalkulatorOdleglosci.h"


namespace KalkulatorOdleglosci
{
    PozycjaGeograficzna lateracja(const std::vector<PozycjaGeograficzna>& stacje, const std::vector<int>& odleglosci)
    {
        if (stacje.size() != odleglosci.size())
        {
            std::cout << "Bledne dane! ilosc stacji: " << stacje.size() << " podana ilosc odleglosci: "
                << odleglosci.size() << "\n";
            
            assert(stacje.size() == odleglosci.size());
            return PozycjaGeograficzna{};
        }
        else if (stacje.size() != 3 && odleglosci.size() != 3)
        {
            // w tej chwili nie wspieramy innego niz trilateracja
            return PozycjaGeograficzna{};
        }

        PozycjaGeograficzna out{};
        
        return out;
    }

    int liczOdleglosc(const PozycjaGeograficzna& a, const PozycjaGeograficzna& b)
    {
        const double pi = 3.14159;

        const auto x = b.szerokosc.getPostacDziesietna() - a.szerokosc.getPostacDziesietna();
        const auto y = b.dlugosc.getPostacDziesietna() - a.dlugosc.getPostacDziesietna();

        const auto kosinus = cos(a.szerokosc.getPostacDziesietna()*pi / 180);
        const auto mnoznik = 40075.704 / 360;

        const auto kwadrat = x*x;
        const auto drugiKwadrat = (kosinus*y)*(kosinus*y);
        
        return sqrt(kwadrat + drugiKwadrat) * mnoznik * 1000;
    }
}