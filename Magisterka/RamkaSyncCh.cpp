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

    std::string out(getRozmiarPoOdrzuceniuSOM(), '0');
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

std::string PreprocesorRamek::getSOMs() const
{
    const std::string data = mStream.str();
    std::string out;

    for (size_t i = 0; i < data.size(); i += 32)
    {
        // co 32 bity SOM
        out += data[i];
    }

    return out;
}

bool PreprocesorRamek::sprawdzPoprawnoscCiagu()
{
    const auto rozmiar = getStreamSize();
    if (rozmiar < ROZMIAR_RAMKI)
    {
        return false;
    }

    const auto bitySom = getSOMs();
    for (size_t i = 0; i < bitySom.size(); i++)
    {
        if (i % 3 == 0)
        {
            if (bitySom[i] != '1')
            {
                return false;
            }
        }
        else
        {
            if (bitySom[i] != '0')
            {
                return false;
            }
        }
    }
    return true;
}

int PreprocesorRamek::ileZerDoPrzodu(int startIdx, const std::string& data) const
{
    int ileZer = 0;
    for (size_t i = startIdx; i < data.size(); i++)
    {
        if (data[i] == '0')
        {
            ileZer++;
        }
        else
        {
            break;
        }
    }
    return ileZer;
}

void PreprocesorRamek::odrzucNiepoprawnaCzescRamki()
{
    if (sprawdzPoprawnoscCiagu())
    {
        return;
    }

    const auto bitySOM = getSOMs();
    std::string out = mStream.str();
    flush();

#define min(a,b) ((a < b) ? a : b )

    // skoro tu weszlismy to na pewno nie byla na poczatku. Nie chcemy usuwac
    // danych jesli byly zera poprzedzone jedynka
    bool czyBylaJedynka = false;
    for (size_t i = 0; i < bitySOM.size(); i++)
    {
        const auto bit = bitySOM[i];
        const auto ileZer = ileZerDoPrzodu(i, bitySOM);

        if (!czyBylaJedynka)
        {
            if (ileZer != 0)
            {
                const auto start = i*ROZMIAR_RAMKI;
                const auto stop = min((i + ileZer)*ROZMIAR_RAMKI, out.size());
                out.erase(start, stop);
                i += (ileZer - 1);
                czyBylaJedynka = false;
            }
            else
            {
                czyBylaJedynka = true;
            }
        }
    }

    mStream << out;
}

StatusRamki RamkaSyncCh::czyOk() const
{
    if (mRamka.size() < RozmiaryRamkiSyncCh::MINIMALNY_ROZMIAR_RAMKI)
    {
        return StatusRamki::ZaKrotkaRamka;
    }
    else if (mRamka.size() > RozmiaryRamkiSyncCh::MAKSYMALNY_ROZMIAR_RAMKI)
    {
        return StatusRamki::ZaDlugaRamka;
    }
    
    const auto msgLen = getMsgLen();
    if (mRamka.size() < msgLen)
    {
        return StatusRamki::ZaMaleMsg;
    }
    else if (mRamka.size() > msgLen)
    {
        return StatusRamki::ZaDuzeMsg;
    }
    else if (mRamka.size() == msgLen && checkCrc())
    {
        return StatusRamki::Ok;
    }

    return StatusRamki::UnknownError;
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

// todo: wyklad dr marczaka
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