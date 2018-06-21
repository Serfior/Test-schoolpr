#ifndef EGZAMIN_H_
#define EGZAMIN_H_

void menu();
int egzamin();
int pobranie_bazy();
void aktualizacja_bazy();
void menu_b();
////
void wyswietl();
void zmiana_nazwy();
void dodaj_pytanie();
void usun_pytanie();
void czysta_baza();
void kopia();
void wybor_bazy();
void usun_baze();
////
void alokujd(char ***,int);
void zwolnij(char **, int);
int ileLini(const char*);
int czy_istnieje();
int czy_pusty(FILE *);
////



#endif //EGZAMIN_H_
