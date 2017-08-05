#include "RamkaSyncCh.h"


void ParserRamek::dopisz(const std::string& bity)
{
    for (char b : bity)
    {
        mStream << b;
    }
}

std::string ParserRamek::getFrame()
{
    const auto rozmiar = getStreamSize();

    if (rozmiar < ROZMIAR_RAMKI)
    {
        const auto out = mStream.str();
        flush();
        return out;
    }

    std::string out(getRozmiarPoOdrzuceniu(), '0');
    char znak = 0;
    int i = 0;  // indeks do liczenia jak daleko jestesmy w sturmieniu
    int indeksOut = 0;

    // jesli znak ok, 
    // jesli nie wyjechalismy poza strumien
    // jesli nie wyjechalismy poza wyjsciowa tablice
    while (mStream.get(znak) && i < rozmiar && indeksOut < out.size())
    {
        if (i % ROZMIAR_RAMKI == 0)
        {
            i++;
            continue;
        }
        
        out[indeksOut] = znak;
        i++;
        indeksOut++;
    }

    flush();
    return out;
}

bool ParserRamek::sprawdzPoprawnoscCiagu()
{
    const auto rozmiar = getStreamSize();
    if (rozmiar < ROZMIAR_RAMKI)
    {
        return false;
    }

    const std::string out = mStream.str();

    for (size_t i = 0; i < out.size(); i+=32)
    {
        const int numerRamkiWSekwencji = static_cast<int>(i / ROZMIAR_RAMKI);
        
        // start of message bit
        const auto bitSOM = out[i];
        // co trzecia ramka ma miec jeden
        if (numerRamkiWSekwencji % 3 == 0) // ramki 0,3,6,9
        {
            if (bitSOM == '0')
            {
                return false;
            }
        }
        else // ramki 1,2, 4,5, 7,8...
        {
            if (bitSOM == '1')
            {
                return false;
            }
        }
    }

    return true;
}


RamkaSyncCh::Status RamkaSyncCh::czyOk() const
{
    return RamkaSyncCh::Status::UnknownError;
}

std::string RamkaSyncCh::getHeaderRaw() const
{
    return "";
}

std::string RamkaSyncCh::getBodyRaw() const
{
    return "";
}

std::string RamkaSyncCh::getCrcRaw() const
{
    return "";
}

int RamkaSyncCh::getHeader() const
{
    return 0;
}

int RamkaSyncCh::getBodySize() const
{
    return 0;
}

unsigned int RamkaSyncCh::getCrc() const
{
    return 0;
}

bool RamkaSyncCh::checkCrc() const
{
    return false;
}
