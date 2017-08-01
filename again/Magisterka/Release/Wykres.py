#podac sciezke do pliku jako argument. Plik w formacie:
#argument\twartosc funkcji

import sys
import os
import matplotlib.pyplot as plt

if sys.argv.__len__() == 1:
    print("Nie podano sciezki do pliku")
    exit()

sciezkaDoPliku = sys.argv[1]

czyDrugiPlikIstnieje = (sys.argv.__len__() == 3)
sciezkaDoDrugiegoPliku = ""
if(czyDrugiPlikIstnieje):
    sciezkaDoDrugiegoPliku = sys.argv[2]

  
if os.path.isfile(sciezkaDoPliku):
    plik = open(sciezkaDoPliku, 'r')
    if plik:
        print("Wczytano plik, wykreslam...")
        plt.plotfile(sciezkaDoPliku, delimiter='\t', cols=(0, 1),
                     names=('argument', 'wartosc'), marker='o')

        if(czyDrugiPlikIstnieje and os.path.isfile(sciezkaDoDrugiegoPliku)):
           drugiPlik = open(sciezkaDoDrugiegoPliku, 'r')
           if drugiPlik:
               print("wczytalem drugi plik")
               plt.plotfile(sciezkaDoDrugiegoPliku, delimiter='\t', cols=(0,1),
                            newfig=False, marker='o')

        plt.grid()
        plt.show()

else:
    print("Nieprawidlowa sciezka")

