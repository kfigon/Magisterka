#podac sciezke do pliku i ilosc linijek jako argument 
#Plik w formacie:
#I:	-5609	Q:	2093
#I:\tliczba\tQ:liczba

import sys
import re

if sys.argv.__len__() == 1:
    print("Nie podano sciezki do pliku")
    exit()

sciezkaDoPliku = sys.argv[1]
ilePrzeczytac = -1

if sys.argv.__len__()==3:
    ilePrzeczytac = int(sys.argv[2])

plikI= open("I.txt", 'w')
plikQ = open("Q.txt", 'w')

plik = open(sciezkaDoPliku, "r")
if plik:
    for indeks, linijka in enumerate(plik):
        matchI = re.search(r"I:\t\-?\d+", linijka)
        matchQ = re.search(r"Q:\t\-?\d+", linijka)

        print(str(indeks)+"\\"+str(ilePrzeczytac))

        if(indeks==ilePrzeczytac):
            break
        
        plikQ.write(str(indeks)+"\t"+str(matchQ.group().split('\t')[1])+"\n")
        plikI.write(str(indeks)+"\t"+str(matchI.group().split('\t')[1])+"\n")

plik.close()
plikI.close()
plikQ.close()
