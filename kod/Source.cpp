#pragma once

#include <iostream>
#include <string>
#include <time.h>

#include "Odbiornik.h"
using namespace std;


int main(int argc, char *argv[])
{
    // todo: argumenty dla programu: pierwszy to sciezka do stacji, druga do katalogu z probkami
    const string sciezka = "D:\\Kamil\\_magisterka\\pomiary\\Probki-2014-04-12-11-41-40.dat";
    const string sciezkaDoStacji = "D:\\Kamil\\_magisterka\\pomiary\\Stacje.txt";
    //const string sciezka = "C:\\Users\\kfigon\\Desktop\\Mgr\\Probki-2014-04-12-11-41-40.dat";
    //const string sciezkaDoStacji = "C:\\Users\\kfigon\\Desktop\\Mgr\\Stacje.txt";


    Odbiornik o(sciezkaDoStacji, sciezka);
	o.SkupWidmo();

	


	//system("pause");
    return 0;
}
