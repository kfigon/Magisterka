#pragma once
#include <string>
#include <sstream>

namespace RozmiaryRamkiSyncCh
{
    const int ROZMIAR_NAGLOWKA = 8;
    const int MINIMALNY_ROZMIAR_DANYCH = 2;
    const int MAKSYMALNY_ROZMIAR_DANYCH = 2002;
    const int ROZMIAR_CRC = 30;
    const int MINIMALNY_ROZMIAR_RAMKI = 40; // 8 + 2 + 30; // hdr+body+crc
    const int MAKSYMALNY_ROZMIAR_RAMKI = 2040; //8+2002+30
}

// todo: zalozenie (moze bledne - big endian)
class RamkaSyncCh
{
    const std::string mRamka;
   
public:
    enum class Status : int
    {
        UnknownError = 0,
        ZaKrotkaRamka = 1,
        ZaDlugaRamka =2,
        NiewlasciweCRC = 3,
        ZaMaleMsg = 4,
        ZaDuzeMsg = 5,
        Ok = 6
    };
    
    static std::string statusToString(Status s)
    {
        switch (s)
        {
        case RamkaSyncCh::Status::UnknownError: return "UnknownError";
        case RamkaSyncCh::Status::ZaKrotkaRamka: return "ZaKrotkaRamka";
        case RamkaSyncCh::Status::ZaDlugaRamka: return "ZaDlugaRamka";
        case RamkaSyncCh::Status::NiewlasciweCRC: return "NiewlasciweCRC";
        case RamkaSyncCh::Status::ZaMaleMsg:  return "ZaMaleMsg";
        case RamkaSyncCh::Status::ZaDuzeMsg:  return "ZaDuzeMsg";
        case RamkaSyncCh::Status::Ok: return "Ok";
        }
    }

    RamkaSyncCh(const std::string& ramka) :
        mRamka(ramka)
    {}

    ~RamkaSyncCh() = default;

    Status czyOk() const;

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

    size_t getRozmiarPoOdrzuceniu()
    {
        const auto rozmiar = getStreamSize();
        return rozmiar - rozmiar / ROZMIAR_RAMKI;
    }

private:
    void flush()
    {
        mStream.str(std::string());
    }
};