#pragma once
#include "Struktury.h"
#include <vector>
#include <assert.h>
#include <iostream>

namespace KalkulatorOdleglosci
{
    // odleglosc w metrach
    int liczOdleglosc(const PozycjaGeograficzna& a, const PozycjaGeograficzna& b);
    
    // lateracja hiperboliczna
    // in: wspolrzedne geograficzne stacji i odpowiadajace im odleglosci w metrach
    PozycjaGeograficzna lateracja(const std::vector<PozycjaGeograficzna>& stacje, const std::vector<int>& odleglosci);

};

