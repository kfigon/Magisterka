#pragma once
#include <string>
#include <sstream>

class RamkaSyncCh
{
    const std::string mRamka;
    const int MINIMALNY_ROZMIAR = 8 + 2 + 30; // hdr+body+crc

public:
    
    enum class Status : int
    {
        UnknownError = 0,
        ZaKrotkaRamka = 1,
        NiewlasciweCRC = 2,
        InvalidBodyLen = 3,
        Ok = 4
    };
    
    static std::string statusToString(Status s)
    {
        switch (s)
        {
        case RamkaSyncCh::Status::UnknownError: return "UnknownError";
        case RamkaSyncCh::Status::ZaKrotkaRamka: return "ZaKrotkaRamka";
        case RamkaSyncCh::Status::NiewlasciweCRC: return "NiewlasciweCRC";
        case RamkaSyncCh::Status::InvalidBodyLen:  return "InvalidBodyLen";
        case RamkaSyncCh::Status::Ok: return "Ok";
        }
        return nullptr;
    }

    RamkaSyncCh(const std::string& ramka) :
        mRamka(ramka)
    {}

    ~RamkaSyncCh();

    Status czyOk() const;

    std::string getHeaderRaw() const;
    std::string getBodyRaw() const;
    std::string getCrcRaw() const;

    int getHeader() const;
    int getBodySize() const; // ile bitow w headerze zapisanych

    unsigned int getCrc() const;

    bool checkCrc() const;
};

// klasa do sprawdzania poprawnosci poszczegolnych ramek. Odrzuca bity SOM
// i sklada poprawna gotowa ramke do depeszy
class ParserRamek
{
    // pojedyncza ramka ma 32 bity
    const int ROZMIAR_RAMKI = 32;

    std::stringstream mStream;

public:
    ParserRamek() = default;

    ~ParserRamek()
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