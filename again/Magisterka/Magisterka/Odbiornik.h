#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Struktury.h"
#include "GeneratorCiagow.h"
#include "FileReader.h"


void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<WynikKorelacji>& wynikKorelacji);
void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<WynikKorelacji>& wynikKorelacji);
long long liczIteracje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI);
std::vector<WynikKorelacji> liczKorelacje(std::vector<Data>& dane, int coKtoraPominac);
