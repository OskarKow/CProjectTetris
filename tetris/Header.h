#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
}

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	1000

//pozycje na ekranie - w pikselach
#define POZYCJA_PLANSZY_X 25
#define POZYCJA_PLANSZY_Y 75
#define POZYCJA_NOWEGO_X 325
#define POZYCJA_X_NASTEPNEGO 800
#define POZYCJA_Y_NASTEPNEGO 125
#define POZYCJA_NOWEGO_Y 75
#define POZYCJA_PUNKTOW_X 800
#define POZYCJA_PUNKTOW_Y 400
#define POZYCJA_ZEGARA_X 800
#define POZYCJA_ZEGARA_Y 450
#define POZYCJA_ETAPU_X 800
#define POZYCJA_ETAPU_Y 500
#define POZYCJA_HIGH_SCORE_X 650
#define POZYCJA_HIGH_SCORE_Y 550
#define POZYCJA_AKTUALNY_HIGHSCORE_X 710
#define POZYCJA_AKTUALNY_HIGHSCORE_Y 330
#define POZYCJA_IMIENIA_X 300
#define POZYCJA_IMIENIA_Y 300

#define CZESTOTLIWOSC_PRZYSPIESZENIA 20.0 //w sekundach
#define MAKSYMALNA_ILOSC_PRZYSPIESZEN 10
#define PREDKOSC_POCZATKOWA 1 //dlugosci bloku na sekunde
#define ILOSC_NAJLEPSZYCH_WYNIKOW 10

#define SZEROKOSC_BLOKU 50 // w pikselach
#define SZEROKOSC_PLANSZY 10 //w blokach
#define WYSOKOSC_PLANSZY 18 //w blokach

enum tryb_okna
{
	WINDOW,
	FULLSCREEN
};

enum typ_bloku
{
	//kszta³ty bloków
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	SCIANA,
	PUSTE
};

struct okno_t
{
	SDL_Surface *screen;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

struct kolory_t
{
	int czerwony;
	int niebieski;
	int czarny;
	int zielony;
};

struct bloki_t
{
	SDL_Surface *block_blue;
	SDL_Surface *block_cyan;
	SDL_Surface *block_green;
	SDL_Surface *block_pink;
	SDL_Surface *block_purple;
	SDL_Surface *block_red;
	SDL_Surface *block_yellow;
	SDL_Surface *block_wall;
};

struct wspolrzedne_t //piksele
{
	int x;
	int y;
};

struct klocek_t
{
	wspolrzedne_t polozenie[4];
	typ_bloku typ;
	int szerokosc; //w blokach
	int dlugosc; //w blokach
};

struct gra_t
{
	bool quit;
	bool nowa_gra;
	bool natychmiastowe_opadniecie; //przechowuje informacje czy aktualny klocek byl poddany natychmiastowemu opadnieciu
	double speed;
	double worldTime;
	double czas_od_przyspieszenia;
	unsigned int score;
	int etap_gry;
	int ilosc_usunietych_wierszy;
	int ilosc_poprzednio_usunietych_wierszy;
	char high_score_imiona[ILOSC_NAJLEPSZYCH_WYNIKOW][30];
	int high_score_punkty[ILOSC_NAJLEPSZYCH_WYNIKOW];
	int aktualna_pozycja_high_score;
};
struct czas_t
{
	int t1;
	int t2;
	double czas_od_aktualizacji;
	double czas_od_zmiany_pozycji;
};

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);

// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);

// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

//nazwa_pliku podawana ze scie¿k¹ (./)
void Wczytaj_grafiki(SDL_Surface **docelowy, const char *nazwa_pliku, okno_t *Okno);
void Ustaw_okno(okno_t *Okno, int *rc, tryb_okna tryb);
void Ustaw_kolory(kolory_t *Kolory, okno_t Okno);
void Ustaw_grafiki(bloki_t *bloki, okno_t *Okno);
void Obsluga_zdarzen(SDL_Event event, klocek_t *aktualny,klocek_t next, typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry,
					kolory_t Kolory, bloki_t Bloki, okno_t *Okno, SDL_Surface *charset, czas_t *czas_lokalny);
void Graj(okno_t Okno, bloki_t Bloki, kolory_t Kolory, int t1, double worldTime, double distance, double speed, SDL_Surface *charset);
void Czynnosci_poczatkowe(okno_t *Okno, kolory_t *Kolory, bloki_t *Bloki);
void Aktualizuj_plansze(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki,klocek_t aktualny, klocek_t next, SDL_Surface *charset, gra_t wartosci);

//Funkcja zawiera wszystkie czynnosci w obrebie opadania pojedynczego klocka - od jego pojawienia sie na gorze do zatrzymania
void Opadanie_klocka(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki, klocek_t nowy, klocek_t next,gra_t *wartosci_gry, SDL_Surface *charset);

//Funkcja sprawdza koniec gry, dodaje punkty wywoluje Opadanie_klocka z losowym klockiem
void Nowy_klocek(); //poki co to nie

//Funkcja ustawia wartosci poczatkowe dla pojedynczej gry i wywo³uje funkcje Opadanie_klocka
void Rozpocznij_runde(okno_t *Okno, bloki_t Bloki, kolory_t Kolory, SDL_Surface *charset);

//Rysowanie blokow:

void Rysuj_blok(klocek_t aktualny, bloki_t Bloki, okno_t Okno);
void Rysuj_pojedynczy_blok(typ_bloku typ,wspolrzedne_t polozenie, bloki_t Bloki, okno_t Okno);
void Wyswietl_elementy_statyczne(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], bloki_t Bloki, okno_t Okno);

void Ustaw_nowy(klocek_t *docelowy, int x_startowe, int y_startowe);
bool Wystepowanie_kolizji_dol(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
void Przypisz_do_planszy(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
bool Wystepowanie_kolizji_lewo(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
bool Wystepowanie_kolizji_prawo(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);

bool Sprawdzenie_konca_gry(typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
void Ustaw_rozmiar_klocka(klocek_t *klocek);
void Rotacja(klocek_t *klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
bool Mozliwosc_rotacji(wspolrzedne_t nowe_wspolrzedne[], typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]);
void Wyrownaj_polozenie_klocka(klocek_t *aktualny);
void Usun_linie(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry);
void Przesun_plansze(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], int indeks_startowy);
void Resetuj_gre(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry);
void Aktualizuj_czas(czas_t *czas_lokalny, gra_t *wartosci_gry);
void Sprawdz_zmiane_etapu(gra_t *wartosci_gry);

void Natychmiastowe_opadniecie(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki,
	klocek_t nowy, klocek_t next, gra_t *wartosci_gry, SDL_Surface *charset, czas_t *czas_lokalny);
void Dodaj_punkty(gra_t *wartosci_gry);
void Wyswietl_najlepsze_wyniki(gra_t wartosci_gry, SDL_Surface *charset, okno_t *Okno);
void Pobierz_liste_wynikow(gra_t *wartosci_gry);
void Ustaw_aktualna_pozycje_highscore(gra_t *wartosci_gry);
void Wyswietl_aktualny_highscore(gra_t wartosci_gry, SDL_Surface *charset, okno_t *Okno);
char *Pobierz_imie(okno_t *Okno, SDL_Surface *charset, kolory_t Kolory, gra_t wartosci_gry);

void Zakoncz_gre(okno_t *Okno, SDL_Surface *charset, kolory_t Kolory, gra_t wartosci_gry);
void Aktualizuj_liste_wynikow(gra_t wartosci_gry);
void Zatrzymaj_gre(okno_t *Okno, SDL_Surface *charset, gra_t *wartosci_gry);
void Ekran_startowy(okno_t *Okno, bloki_t Bloki, kolory_t Kolory, SDL_Surface *charset);
void Wylacz_gre(SDL_Surface *charset, okno_t *Okno);
void Zapisz_gre(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t wartosci_gry, klocek_t aktualny, klocek_t next);
