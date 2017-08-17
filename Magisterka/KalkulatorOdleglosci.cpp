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
        PozycjaGeograficzna out{};
        
        return out;
    }

    int liczOdleglosc(const PozycjaGeograficzna& a, const PozycjaGeograficzna& b)
    {
        return 0;
    }
}