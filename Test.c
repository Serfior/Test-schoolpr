#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "egzamin.h"

static FILE *f = NULL;
static char baza[255];


void menu()
{
  int wybor;
  if(pobranie_bazy()!=1)
  {
      printf("Baza nie zostala pobrana.");
      exit(2);
  }
  printf("\t\tEGZAMIN\n\n");
  printf("1. Przeprowadz egzamin.\n");
  printf("2. Tworzenie bazy pytan.\n");
  printf("3. Wyjscie.\n");
  scanf("%d",&wybor);
  while(wybor!=1 && wybor !=2 && wybor !=3)
        scanf("%d",&wybor);
  fflush(stdin);
  switch(wybor)
  {
        case 1:
            if(egzamin()==1)
                {
                    printf("Egzamin przebiegl pomyslnie\n");
                    system("pause");
                    exit(1);
                }
            break;
        case 2:
            menu_b();
            break;
        case 3:
            exit(1);
  }
}

void alokujd(char ***ch,int l)
{
    (*ch) = (char**)malloc(sizeof(char*)*(l));
    int i;
    for(i=0;i<l;i++)
    {
        (*ch)[i] =(char*)malloc(255*sizeof(char));
    }
}

void zwolnij(char **ch,int l)
{
   int i;
   for(i = 0; i<l; i++)
   {
    free(ch[i]);
   }
   free(ch);
}

int ileLini(const char *plik)
{
 FILE *plk;
 char g;
 int linie = 0;
 if((plk = fopen(plik,"r"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
while((g=fgetc(plk))!=EOF)
    {
        if(g == '\n')
            linie ++;
    }
fclose(plk);
return linie;
}

int czy_pusty(FILE *f)
{
    int rozmiar = 0;
    if (NULL != f)
    {
        fseek (f, 0, SEEK_END);
        rozmiar = ftell(f);
        fseek (f, 0, SEEK_SET);
        if (rozmiar == 0)
            return 1;
}
    return 0;
}

int czy_istnieje(const char *plik)
{
    f = fopen(plik, "r");
    if ( f )
    {
        fclose(f);
        f=NULL;
        return 1;
    }
    fclose(f);
    f = NULL;
    return 0;
}


pobranie_bazy()
{
    char *bufor;
    if(czy_istnieje("Base.txt")==0)
    {
        if((f=fopen("Base.txt","w"))==NULL)
        {
            printf("Blad otwarcia pliku glownego baza nie zostala pobrana.\n");
            system("pause");
            exit(1);
        }
        printf("Podaj nazwe bazy pytan do utworzenia:");
        gets(baza);
        if(strstr(baza,".txt") == 0)
        {
            strcat(baza,".txt");
        }
        fprintf(f,"%s",baza);
        fclose(f);
        f = NULL;
    }
    else
    {
        if((f=fopen("Base.txt","r+"))==NULL)
        {
            printf("Blad otwarcia pliku glownego baza nie zostala pobrana.\n");
            system("pause");
            exit(1);
        }
        if(czy_pusty(f)==1)
        {
            printf("W pliku glownym nie ma bazy pytan.\n");
            printf("Podaj nowa nazwe bazy pytan do utworzenia:\n");
            gets(baza);
            if(strstr(baza,".txt") == 0)
            {
                strcat(baza,".txt");
            }
            fprintf(f,"%s",baza);
            fclose(f);
            f=NULL;
        }
        else
        {
            bufor =(char*)malloc(sizeof(char)*255);
            fgets(bufor,255,f);
            strcpy(baza,bufor);
            fclose(f);
            free(bufor);
            f=NULL;
        }
    }
    if(czy_istnieje(baza)==0)
    {
        if((f=fopen(baza,"w"))==NULL)
            {
            printf("Blad utworzenia pliku bazy\n");
            system("pause");
            exit(1);
            }
        fclose(f);
        f = NULL;
    }
    return 1;
}

void aktualizacja_bazy(const char * nazwa)
{
    if((f=fopen("Base.txt","w"))==NULL)
    {
        printf("Blad otwarcia pliku glownego baza nie zostala zaktualizowana.\n");
        system("pause");
        exit(1);
    }
    fputs(nazwa,f);
    fclose(f);
    f=NULL;
}


void menu_b()
{
    int wybor;
    printf("\nCo chcesz zrobic?\n");
    printf("1.Nazwa bazy pytan.\n");
    printf("2.Zmiana nazwy bazy pytan. \n");
    printf("3.Dodaj pytanie.\n");
    printf("4.Wyswietl baze pytan.\n");
    printf("5.Usun pytanie.\n");
    printf("6.Wyczysc baze pytan.\n");
    printf("7.Zrob kopie bazy pytan.\n");
    printf("8.Zmien baze pytan.\n");
    printf("9.Usun baze pytan.\n");
    printf("10.Powrot do menu\n");
    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2 && wybor !=3 && wybor !=4 && wybor !=5 && wybor !=6 && wybor !=7 && wybor !=8 && wybor !=9 && wybor !=10)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
    {
        case 1:
            printf("\n%s\n",baza);
            menu_b();
            break;
        case 2:
            zmiana_nazwy();
            break;
        case 3:
            dodaj_pytanie();
            break;
        case 4:
            wyswietl();
            menu_b();
            break;
        case 5:
            usun_pytanie();
            break;
        case 6:
            czysta_baza();
            menu_b();
            break;
        case 7:
            kopia();
            menu_b();
            break;
        case 8:
            wybor_bazy();
            break;
        case 9:
            usun_baze();
            break;
        case 10:
            menu();
            break;
    }
}


int egzamin()
{
    int i,j,n,wybor,pkt = 0;
    int l = ileLini(baza);
    if(l<6)
    {
        printf("W bazie nie ma pytan!.\n");
        menu();
        exit(1);
    }
    char **ch,odp;
    alokujd(&ch,l);

    if((f = fopen(baza,"r"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
    for(i = 0; fgets(ch[i],255,f);i++);
    fclose(f);
    f = NULL;
    n = i;
    i = 0;
    while(i<n)
    {
        printf("%s",ch[i]);
        i++;
        for(j = 0; j<4; j++,i++)
        printf("%s",ch[i]);
        scanf("%c",&odp);
        fflush(stdin);
        if(odp == ch[i][0] || (odp-32) == ch[i][0])
        {
            printf("Poprawna odpowiedz!\n");
            pkt ++;
        }
        else
        {
            printf("Bledna odpowiedz!\n");
        }
        i++;
    }
    zwolnij(ch,l);
    if(pkt == 1)
        printf("\nKoniec!\nZdobyles: %d punkt.\n\n",pkt);
    else if( pkt == 2 || pkt == 3 || pkt == 4)
        printf("\nKoniec!\nZdobyles: %d punkty.\n\n",pkt);
    else
        printf("\n\nZdobyles: %d punktow.\n\n",pkt);
    printf("Kontynuowac?\n");
    printf("1.Tak.\n");
    printf("2.Nie.\n");
    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
        {
        case 1:
            menu();
            break;
        case 2:
            return 1;
            break;
        }

    return 1;
}

void dodaj_pytanie()
{
    int i = 0;
    int wybor;
    char **t;
    alokujd(&t,6);

    printf("\nPodaj pytanie\n");
    gets(t[0]);
    printf("Podaj odpowiedz A:\n");
    t[1][0] = 'A';
    t[1][1] = ' ';
    gets(t[1]+2);
    printf("Podaj odpowiedz B:\n");
    t[2][0] = 'B';
    t[2][1] = ' ';
     gets(t[2]+2);
    printf("Podaj odpowiedz C:\n");
    t[3][0] = 'C';
    t[3][1] = ' ';
    gets(t[3]+2);
    printf("Podaj odpowiedz D:\n");
    t[4][0] = 'D';
    t[4][1] = ' ';
    gets(t[4]+2);
    printf("Podaj poprawna odpowiedz(A,B,C,D):\n");
    gets(t[5]);
    while(t[5][0] != 'A' && t[5][0] != 'B' && t[5][0] != 'C' && t[5][0] !='D')
        gets(t[5]);
    if((f = fopen(baza,"a"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
    fseek(f,0,SEEK_END);
    for(i = 0; i<6 ; i++)
    {
        fputs(t[i],f);
        fputs("\n",f);
    }
    fclose(f);
    f=NULL;
    zwolnij(t,6);

    printf("\nCo chcesz zrobic?\n");
    printf("1.Dodaj kolejne pytanie.\n");
    printf("2.Edycja bazy pytan\n");
    printf("3.Koniec\n");

    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2 && wybor !=3)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
    {
    case 1:
        dodaj_pytanie();
        break;
    case 2:
        menu_b();
        break;
    case 3:
        exit(1);
    }
}

void usun_pytanie()
{
int wybor,i,j=0;
int lpytan = ileLini(baza)/6;
char **ch,*g;
g = (char*)malloc(sizeof(char)*255);
if(lpytan == 0)
{
    printf("W bazie nie ma pytan. ");
    menu_b();
    exit(1);
}

printf("\nW bazie jest %d pytan, ktore chcesz usunac? 0 anuluj.",lpytan);
scanf("%d",&wybor);
while(wybor > lpytan || wybor <0)
{
    printf("Pytanie o numerze %d nie istnieje, prosze wybrac inne.\n", wybor);
    scanf("%d",&wybor);
    fflush(stdin);
}
fflush(stdin);
if(wybor == 0)
{
    menu_b();
    exit(1);
}
if(lpytan == 1)
    {
        czysta_baza();
        menu_b();
        exit(1);
    }
alokujd(&ch,(lpytan*6));

if((f = fopen(baza,"r"))==NULL)
{
    printf("Blad otwarcia pliku!");
    exit(1);
}
for( i = 0; i < lpytan*6;i++)
{
    fgets(g,255,f);
    if(i<(wybor-1)*6 || i>=wybor*6)
    {
    strcpy(ch[j],g);
    j++;
    }
}
fclose(f);
if((f = fopen(baza,"w"))==NULL)
{
    printf("Blad otwarcia pliku!");
    exit(1);
}
for(i=0;i<j;i++)
    fputs(ch[i],f);
fclose(f);
f = NULL;
zwolnij(ch,(lpytan-1)*6);
free(g);
printf("\nUsuwanie pytanie zakonczone pomyslnie.\n\n");
printf("Co chcesz zrobic dalej?\n");
printf("1.Usun pytanie.\n");
printf("2.Edycja bazy pytan\n");
printf("3.Koniec\n");

scanf("%d",&wybor);
while(wybor!=1 && wybor !=2 && wybor !=3)
    scanf("%d",&wybor);
fflush(stdin);
switch(wybor)
{
    case 1:
        usun_pytanie();
        break;
    case 2:
        menu_b();
        break;
    case 3:
        exit(1);
    }
}

void czysta_baza()
{
    if((f = fopen(baza,"w"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
    fclose(f);
    f = NULL;
    printf("Baza pytan pomyslne wyczyszona!\n\n");
}

void wyswietl()
{
    int i;
    int l = ileLini(baza);
    char **ch;
    alokujd(&ch,l);

    if((f = fopen(baza,"r"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
    for(i = 0; fgets(ch[i],255,f);i++);
    fclose(f);
    f=NULL;

    printf("\nW bazie sa %d nasteupujace pytania:\n",l/6);
    for(i = 0; i < l; i ++)
    {
        printf("%s",ch[i]);
    }
    zwolnij(ch,l);
}

void kopia()
{
    int i;
    int l = ileLini(baza);
    char **ch,*path;
    alokujd(&ch,l);
    path=(char*)malloc(sizeof(char)*255);

    if((f = fopen(baza,"r"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }

    for(i = 0; fgets(ch[i],255,f);i++);
    fclose(f);

    printf("\nPodaj nazwe kopi bazy: ");
    scanf("%s",path);
    strcat(path,".txt");
    printf("%s",path);
    if((f = fopen(path,"w"))==NULL)
    {
        printf("Blad otwarcia pliku!");
        exit(1);
    }
    for(i=0;i<l;i++)
        fputs(ch[i],f);
    fclose(f);
    f = NULL;
    zwolnij(ch,l);
    free(path);

    printf("\nOperacja udana!\n");
}

void wybor_bazy()
{
    int wybor;
    char *ch;

    printf("\nChcesz zmienic baze pytan?\n");
    printf("1.Tak\n");
    printf("2.Nie\n");

    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
        {
        case 1:
            ch =(char*)malloc(sizeof(char)*255);
            printf("\nPodaj nazwe bazy.\n");
            scanf("%s",ch);
            if(strstr(ch,".txt")==0)
            {
                strcat(ch,".txt");
            }
            if(czy_istnieje(ch)== 0)
            {
                printf("W folderze glownym nie ma bazy pytan o podanej nazwie.\nZmiana bazy pytan nie udana!\n");
                menu_b();
                exit(1);
            }
            if((f = fopen(ch,"r"))==NULL)
            {
                printf("Blad otwarcia bazy!");
                system("pause");
                exit(1);
            }
            fclose(f);
            f = NULL;
            strcpy(baza,ch);
            aktualizacja_bazy(ch);
            pobranie_bazy();
            printf("Zamiana udana!\n");
            free(ch);
            menu_b();
            break;
        case 2:
            menu_b();
            exit(1);
            break;
        }
}

void zmiana_nazwy()
{
    int wybor;
    char *ch;
    ch =(char*)malloc(sizeof(char)*255);

    printf("\nChcesz zmienic nazwe bazy?\n");
    printf("1.Tak\n");
    printf("2.Nie\n");

    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
        {
        case 1:
            ch =(char*)malloc(sizeof(char)*255);
            printf("Podaj nowa nazwe.\n");
            scanf("%s",ch);
            if(strstr(ch,".txt")==0)
            {
                strcat(ch,".txt");
            }
            rename(baza,ch);

            aktualizacja_bazy(ch);
            pobranie_bazy();
            printf("Zamiana udana!\n");
            free(ch);
            menu_b();
            break;
        case 2:
            menu_b();
            exit(1);
            break;
        }
}

void usun_baze()
{
    int wybor;
    char *ch;
    ch =(char*)malloc(sizeof(char)*255);

    printf("Co chcesz zrobic?\n");
    printf("1.Usun aktualna baze pytan.\n");
    printf("2.Usun wybrana baze pytan.\n");
    printf("3.Anuluj.\n");
    scanf("%d",&wybor);
    while(wybor!=1 && wybor !=2 && wybor !=3)
        scanf("%d",&wybor);
    fflush(stdin);
    switch(wybor)
    {
        case 1:
            remove(baza);
            if((f=fopen("Base.txt","w"))==NULL)
               {
                   printf("Blad otwarica glownego pliku!");
                   exit(1);
               }
            fclose(f);
            f = NULL;
            if(pobranie_bazy()!=1)
            {
                  printf("Baza nie zostala pobrana.");
                  exit(2);
            }
            menu_b();
            break;
        case 2:
            printf("Podaj nazwe bazy pytan do usuniecia: ");
            scanf("%s",ch);
            if(strstr(ch,".txt")==0)
            {
                strcat(ch,".txt");
            }
            if(strcmp(ch,baza)==0)
            {
                printf("\nWybrana baza pytan to akutalna baza pytan!\n");
                printf("Jesli chcesz usunac aktualna baze pytan wybierz pierwsza opcje.\n");
                menu_b();
                break;
            }
            printf("Czy na pewno chcesz usunac baze pytan %s ?\n",ch);
            printf("1. Tak.\n");
            printf("2. Nie.\n");
            scanf("%d",&wybor);
            while(wybor!=1 && wybor !=2)
                scanf("%d",&wybor);
            fflush(stdin);
            switch(wybor)
            {
                case 1:
                    if(czy_istnieje(ch)==0)
                    {
                        printf("Baza o podanej nazwie nie istnieje!\nOperacja nie udana.\n");
                        break;
                    }
                    remove(ch);
                    break;
                case 2:
                    break;
            }
            menu_b();
            break;
        case 3:
            menu_b();
            break;
    }
}
