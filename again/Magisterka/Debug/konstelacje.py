#podac sciezke do pliku jako argument. Plik w formacie:
#argument\twartosc funkcji

import sys
import os
import matplotlib.pyplot as plt
import csv

if sys.argv.__len__() == 1:
    print("Nie podano sciezki do pliku")
    exit()

sciezkaDoPliku = sys.argv[1]
 
if os.path.isfile(sciezkaDoPliku):
    plik = open(sciezkaDoPliku, 'r')
    if plik:
        print("Wczytano plik, wykreslam...")
        dane = list(csv.reader(plik ,delimiter='\t'))

        real = [[i][0][0] for i in dane]
        imag = [[i][0][1] for i in dane]
        plt.scatter(real,imag)
        plt.grid()
        plt.show()

else:
    print("Nieprawidlowa sciezka")

