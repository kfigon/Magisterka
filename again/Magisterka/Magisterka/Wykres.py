#podac sciezke do pliku jako argument. Plik w formacie:
#argument\twartosc funkcji

import sys
import os
import matplotlib.pyplot as plt

if sys.argv.__len__() == 1:
    print("Nie podano sciezki do pliku")
    exit()

sciezkaDoPliku = sys.argv[1]
if os.path.isfile(sciezkaDoPliku):
    plik = open(sciezkaDoPliku, 'r')
    if plik:
        print("Wczytano plik, wykreslam...")
        plt.plotfile(sciezkaDoPliku, delimiter='\t', cols=(0, 1),
                     names=('argument', 'wartosc'), marker='o')
        plt.grid()
        plt.show()
else:
    print("Nieprawidlowa sciezka")

