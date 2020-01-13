# Generator muzyki
Program generujący losowe melodie lub wielogłosy na podstawie podanej przez użytkownika progresji akordów lub tonacji. 


## Komunikacja na poziomie sprzętowym
Moduł realizujący komunikajcę między programem a systemem operacyjnym. Między innymi odpowiedzialny za tworzenie nowych wątków, operowanie Samplami, odpowiednie alokowanie i zwalnianie pamięci.

## Generowanie fal sinusoidalnych i melodii
Moduł odpowiedzialny za tworzenie fal odpowiadających danym dzwiękom oraz ich modyfikacje.  

Generowanie melodii przy podanej progresji akordów:
Odczytywanie informacji o zawartych dźwiękach w danym akordzie, generowanie fal o odpowiednich parametrach oraz odgrywanie melodii z elementem losowym jakim jest czas trwania dźwięku. 
 
 Generowanie melodii dla danej tonacji:
 Odczytanie informacji o akordach w danej tonacji i wygenerowanie odpowiednich fal. Odgrywanie ich z losową sekwencją akordów (pasującą do danej tonacji) i czasem trwania poszczególnych akordów.

## Interakcja użytkownika z programem
Użytkownik będzie miał możliwość wprowadzenia tonacji lub progresji akordów, czego konsekwencją będzie wygenerowana melodia na podstawie wprowadzonych wytycznych. Program zrealizowany zostanie bez interfejsu graficznego. 

## Biblioteki i inne zasoby
Biblioteki:
- libsoundio,
- thread.h,
- atomic.h.

Struktury danych:
- buffor cykliczny.

Inne: 
- możliwe bazy danych przechowujące informacje o akordach, tonacjach i parametrach potrzebnych do wygenerowania fal odpowiadających danym dźwiękom.

//Opis projektu może ulec nieznacznym modyfikacjom.

