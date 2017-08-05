#include "RamkaSyncCh.h"


void PreprocesorRamek::dopisz(const std::string& bity)
{
    for (char b : bity)
    {
        mStream << b;
    }
}

std::string PreprocesorRamek::getFrame()
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

bool PreprocesorRamek::sprawdzPoprawnoscCiagu()
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
    if (mRamka.size() < RozmiaryRamkiSyncCh::MINIMALNY_ROZMIAR_RAMKI)
    {
        return Status::ZaKrotkaRamka;
    }
    else if (mRamka.size() > RozmiaryRamkiSyncCh::MAKSYMALNY_ROZMIAR_RAMKI)
    {
        return Status::ZaDlugaRamka;
    }
    
    const auto msgLen = getMsgLen();
    if (mRamka.size() < msgLen)
    {
        return Status::ZaMaleMsg;
    }
    else if (mRamka.size() > msgLen)
    {
        return Status::ZaDuzeMsg;
    }
    else if (mRamka.size() == msgLen && checkCrc())
    {
        return Status::Ok;
    }

    return Status::UnknownError;
}

std::string RamkaSyncCh::getHeaderRaw() const
{
    return mRamka.substr(0, RozmiaryRamkiSyncCh::ROZMIAR_NAGLOWKA);
}

std::string RamkaSyncCh::getBodyRaw() const
{
    const int rozmiarDanych = getBodySize();
    return mRamka.substr(RozmiaryRamkiSyncCh::ROZMIAR_NAGLOWKA, rozmiarDanych);
}

std::string RamkaSyncCh::getCrcRaw() const
{
    const int rozmiarDanych = getBodySize();
    const auto start = RozmiaryRamkiSyncCh::ROZMIAR_NAGLOWKA + rozmiarDanych;
    const auto stop = start + RozmiaryRamkiSyncCh::ROZMIAR_CRC;
    
    return mRamka.substr(start, stop);
}

int RamkaSyncCh::getMsgLen() const
{
    const auto header = getHeaderRaw();
    return binaryToInt(header) * 8;
}

int RamkaSyncCh::getBodySize() const
{
    return getMsgLen() - RozmiaryRamkiSyncCh::ROZMIAR_CRC - RozmiaryRamkiSyncCh::ROZMIAR_NAGLOWKA;
}

unsigned int RamkaSyncCh::getCrc() const
{
    const auto crcRaw = getCrcRaw();
    return binaryToInt(crcRaw);
}

bool RamkaSyncCh::checkCrc() const
{
    const auto odczytaneCrc = getCrc();
    const auto rozmiarDanychINaglowka = RozmiaryRamkiSyncCh::ROZMIAR_NAGLOWKA + getBodySize();
    return false;
}

unsigned long long RamkaSyncCh::binaryToInt(const std::string& data) const
{
    // zalozenie: big endian (po ludzku)

    unsigned long long out = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[data.size()-1-i] == '1')
        {
            out += pow(2, i);
        }
    }
    return out;
}