#pragma once
#include <string>
#include <sstream>
#include <assert.h>

namespace RozmiaryRamkiSyncCh
{
    const int ROZMIAR_NAGLOWKA = 8;
    const int MINIMALNY_ROZMIAR_DANYCH = 2;
    const int MAKSYMALNY_ROZMIAR_DANYCH = 2002;
    const int ROZMIAR_CRC = 30;
    const int MINIMALNY_ROZMIAR_RAMKI = 40; // 8 + 2 + 30; // hdr+body+crc
    const int MAKSYMALNY_ROZMIAR_RAMKI = 2040; //8+2002+30
}

enum class StatusRamki : int
{
    UnknownError = 0,
    ZaKrotkaRamka = 1,
    ZaDlugaRamka = 2,
    NiewlasciweCRC = 3,
    ZaMaleMsg = 4,
    ZaDuzeMsg = 5,
    Ok = 6
};


// zalozenie (wyglada dobrze - big endian)
// 8 bitow MSG_LEN | 2-2002 bitow message body | 30 CRC
// tutaj zakladam ze bity sa od MSB-LSB
// 00000101 -> 5
class RamkaSyncCh
{
    const std::string mRamka;
   
public:
    
    static std::string StatusRamkiToString(StatusRamki s)
    {
        switch (s)
        {
        case StatusRamki::UnknownError: return "UnknownError";
        case StatusRamki::ZaKrotkaRamka: return "ZaKrotkaRamka";
        case StatusRamki::ZaDlugaRamka: return "ZaDlugaRamka";
        case StatusRamki::NiewlasciweCRC: return "NiewlasciweCRC";
        case StatusRamki::ZaMaleMsg:  return "ZaMaleMsg";
        case StatusRamki::ZaDuzeMsg:  return "ZaDuzeMsg";
        case StatusRamki::Ok: return "Ok";
        }
    }

    RamkaSyncCh(const std::string& ramka) :
        mRamka(ramka)
    {}

    ~RamkaSyncCh() = default;

    StatusRamki czyOk() const;

    std::string getHeaderRaw() const;
    std::string getBodyRaw() const;
    std::string getCrcRaw() const;

    int getMsgLen() const;
    int getBodySize() const; // ile bitow w headerze zapisanych

    unsigned int getCrc() const;

    bool checkCrc() const;

private:
    unsigned long long binaryToInt(const std::string& data) const;
};

// klasa do sprawdzania poprawnosci poszczegolnych ramek. Odrzuca bity SOM
// i sklada poprawna gotowa ramke do depeszy
class PreprocesorRamek
{
    // pojedyncza ramka ma 32 bity
    const int ROZMIAR_RAMKI = 32;
    std::stringstream mStream;

public:
    PreprocesorRamek() = default;

    ~PreprocesorRamek()
    {
        mStream.flush();
    }

    void dopisz(const std::string& bity);

    // czysci bufor!
    std::string getFrame();

    void odrzucNiepoprawnaCzescRamki();

    size_t getStreamSize()
    {
        if (!mStream.good())
        {
            return 0;
        }

        // ustaw od poczatku do konca
        mStream.seekg(0, std::ios::end);
        const auto size = mStream.tellg();
        // ustaw na poczatek
        mStream.seekg(0, std::ios::beg);

        return size;
        //return (size == -1) ? 0 : size;
    }

    bool sprawdzPoprawnoscCiagu();

    size_t getRozmiarPoOdrzuceniuSOM()
    {
        const auto rozmiar = getStreamSize();
        return rozmiar - rozmiar / ROZMIAR_RAMKI;
    }

private:
    void flush() { mStream.str(std::string()); }

    std::string getSOMs() const;
    int ileZerDoPrzodu(int startIdx, const std::string& data) const;
};

class CrcChecker
{
    // stan rejestru przesuwnego
    std::string mState;
public:
    // ilosc komorek rejestru + 1 (stopien wielomianu + 1)
    // CRC16 -> CrcChecker(16)
    // crc3 -> CrcChecker(3)
    // wielomian wtedy trzeba podac kolejno 17 i 4
    CrcChecker(size_t len) :
        mState(len + 1, '0')
    {}

    bool isCrcOk(const std::string& msg, const std::string& poly)
    {
        assert(poly.size() == mState.size());

        for (auto bit : msg)
        {
            shift(bit);
            if (isMsbOne())
            {
                xor(poly);
            }
        }

        // wszystko powinno byc 0
        // todo: wszystkie czy tylko te gdzie CRC?
        for (auto bit : mState)
        {
            if (bit != '0')
            {
                return false;
            }
        }
        return true;
    }

private:
    char xor(char a, char b) const { return (a == b) ? '0' : '1'; }

    void xor(const std::string& poly)
    {
        for (size_t i = 0; i < mState.size(); i++)
        {
            mState[i] = xor(mState[i], poly[i]);
        }
    }

    void shift(char x)
    {
        for (size_t i = 1; i < mState.size(); i++)
        {
            mState[i - 1] = mState[i];
        }
        mState[mState.size() - 1] = x;
    }

    bool isMsbOne() const { return (mState[0] == '1'); }
};
