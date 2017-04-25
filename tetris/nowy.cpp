#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <malloc.h>
#include <time.h>
#include <cstdlib>
#include "Header.h"

extern "C" {
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
}

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void Wczytaj_grafiki(SDL_Surface **docelowy, const char *nazwa_pliku, okno_t *Okno)
{
	*docelowy = SDL_LoadBMP(nazwa_pliku);
	if (*docelowy == NULL) {
		printf("SDL_LoadBMP(%s)error: %s\n",nazwa_pliku, SDL_GetError());
		SDL_FreeSurface((*Okno).screen);
		SDL_DestroyTexture((*Okno).scrtex);
		SDL_DestroyWindow((*Okno).window);
		SDL_DestroyRenderer((*Okno).renderer);
		SDL_Quit();
		return;
	}
}

void Ustaw_okno(okno_t *Okno, int *rc, tryb_okna tryb)
{
	if(tryb == FULLSCREEN)
		*rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,&(*Okno).window, &(*Okno).renderer);
	else *rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,&(*Okno).window, &(*Okno).renderer);
	
	if (*rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize((*Okno).renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor((*Okno).renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle((*Okno).window, "Szablon do zadania drugiego 2014");

	(*Okno).screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	(*Okno).scrtex = SDL_CreateTexture((*Okno).renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	// wy≥πczenie widocznoúci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);
}

void Ustaw_kolory(kolory_t *Kolory, okno_t Okno)
{
	(*Kolory).czarny = SDL_MapRGB(Okno.screen->format, 0x00, 0x00, 0x00);
	(*Kolory).zielony = SDL_MapRGB(Okno.screen->format, 0x00, 0xFF, 0x00);
	(*Kolory).czerwony = SDL_MapRGB(Okno.screen->format, 0xFF, 0x00, 0x00);
	(*Kolory).niebieski = SDL_MapRGB(Okno.screen->format, 0x11, 0x11, 0xCC);
}

void Ustaw_grafiki(bloki_t *Bloki, okno_t *Okno)
{
	Wczytaj_grafiki(&(*Bloki).block_blue, "./blue.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_cyan, "./cyan.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_green, "./green.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_pink, "./pink.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_purple, "./purple.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_red, "./red.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_yellow, "./yellow.bmp", &(*Okno));
	Wczytaj_grafiki(&(*Bloki).block_wall, "./wall.bmp", &(*Okno));
}


void Obsluga_zdarzen(SDL_Event event, klocek_t *aktualny, klocek_t next, typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry, 
	kolory_t Kolory, bloki_t Bloki, okno_t *Okno, SDL_Surface *charset, czas_t *czas_lokalny)
{
	switch (event.type) {
	case SDL_KEYDOWN: {
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE: {
			wartosci_gry->quit = true;
			break;
		}
		case SDLK_LEFT: {
			if(!Wystepowanie_kolizji_lewo(*aktualny, mapa))
				for (int i = 0; i < 4; i++) aktualny->polozenie[i].x -= SZEROKOSC_BLOKU;
			break;
		}
		case SDLK_RIGHT: {
			if (!Wystepowanie_kolizji_prawo(*aktualny, mapa))
				for (int i = 0; i < 4; i++) aktualny->polozenie[i].x += SZEROKOSC_BLOKU;
			break;
		}
		case SDLK_UP: case SDLK_SPACE:{
			Rotacja(&(*aktualny), mapa);
			break;
		}
		case SDLK_DOWN: {
			wartosci_gry->natychmiastowe_opadniecie = true;
			Natychmiastowe_opadniecie(mapa, &(*Okno), Kolory, Bloki, *aktualny, next, &(*wartosci_gry), charset, &(*czas_lokalny));
			break;
		}
		case SDLK_n: {
			Resetuj_gre(mapa, &(*wartosci_gry));
			break;
		}
		case SDLK_s: {
			if (wartosci_gry->etap_gry < MAKSYMALNA_ILOSC_PRZYSPIESZEN)
			{
				wartosci_gry->etap_gry++;
				wartosci_gry->speed++;
				wartosci_gry->czas_od_przyspieszenia = 0.0;
			}
			break;
		}
		case SDLK_p: {
			Zatrzymaj_gre(&(*Okno), charset, &(*wartosci_gry));
			break;
		}
		}
		break;
	}
	case SDL_QUIT:
		wartosci_gry->quit = true;
		break;
	};
}

void Czynnosci_poczatkowe(okno_t *Okno, kolory_t *Kolory, bloki_t *Bloki)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return;
	}
	int rc;
	Ustaw_okno(Okno, &rc, WINDOW);
	Ustaw_kolory(Kolory, *Okno);
	Ustaw_grafiki(Bloki, Okno);
	SDL_DestroyWindow((*Okno).window);
}

typ_bloku Losuj_typ_bloku()
{
	switch (rand() % 7)
	{
	case 0: {
		return I;
		break;
	}
	case 1: {
		return T;
		break;
	}
	case 2: {
		return O;
		break;
	}
	case 3: {
		return L;
		break;
	}
	case 4: {
		return J;
		break;
	}
	case 5: {
		return S;
		break;
	}
	case 6: {
		return Z;
		break;
	}
	}
	return I;
}

void Aktualizuj_plansze(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki,klocek_t nowy, klocek_t next,SDL_Surface *charset, gra_t wartosci)
{
	char text[128];
	SDL_FillRect((*Okno).screen, NULL, Kolory.czarny);
	Rysuj_blok(nowy, Bloki, *Okno);
	Rysuj_blok(next, Bloki, *Okno);
	Wyswietl_elementy_statyczne(mapa, Bloki, *Okno);
	Wyswietl_najlepsze_wyniki(wartosci, charset, Okno);
	//wyswietlenie punktow
	sprintf(text, "SCORE: %u ", wartosci.score);
	DrawString(Okno->screen,POZYCJA_PUNKTOW_X, POZYCJA_PUNKTOW_Y, text, charset);
	//wyswietlenie zegara:
	sprintf(text, "TIME: %.1lf ", wartosci.worldTime);
	DrawString(Okno->screen, POZYCJA_ZEGARA_X, POZYCJA_ZEGARA_Y, text, charset);
	//etap
	sprintf(text, "ETAP: %d ", wartosci.etap_gry);
	DrawString(Okno->screen, POZYCJA_ETAPU_X, POZYCJA_ETAPU_Y, text, charset);
	if (wartosci.aktualna_pozycja_high_score > 0)
		Wyswietl_aktualny_highscore(wartosci, charset, Okno);
	SDL_UpdateTexture((*Okno).scrtex, NULL, (*Okno).screen->pixels, (*Okno).screen->pitch);
	SDL_RenderCopy((*Okno).renderer, (*Okno).scrtex, NULL, NULL);
	SDL_RenderPresent((*Okno).renderer);
}

void Rysuj_blok(klocek_t aktualny, bloki_t Bloki, okno_t Okno)
{
	SDL_Surface *blok_do_wyswietlenia;
	switch (aktualny.typ)
	{
	case I: {
		blok_do_wyswietlenia = Bloki.block_red;
		break;
	}
	case T: {
		blok_do_wyswietlenia = Bloki.block_purple;
		break;
	}
	case O: {
		blok_do_wyswietlenia = Bloki.block_cyan;
		break;
	}
	case L: {
		blok_do_wyswietlenia = Bloki.block_yellow;
		break;
	}
	case J: {
		blok_do_wyswietlenia = Bloki.block_pink;
		break;
	}
	case S: {
		blok_do_wyswietlenia = Bloki.block_blue;
		break;
	}
	case Z: {
		blok_do_wyswietlenia = Bloki.block_green;
		break;
	}
	case SCIANA: {
		blok_do_wyswietlenia = Bloki.block_wall;
		break;
	}
	case PUSTE: {
		break;
	}
	}
	for (int i = 0; i < 4; i++)
		DrawSurface(Okno.screen, blok_do_wyswietlenia, aktualny.polozenie[i].x, aktualny.polozenie[i].y);
}

void Rysuj_pojedynczy_blok(typ_bloku typ, wspolrzedne_t aktualny, bloki_t Bloki, okno_t Okno)
{
	switch (typ)
	{
	case I: {
	DrawSurface(Okno.screen, Bloki.block_red, aktualny.x, aktualny.y);
	break;
	}
	case T: {
	DrawSurface(Okno.screen, Bloki.block_purple, aktualny.x, aktualny.y);
	break;
	}
	case O: {
	DrawSurface(Okno.screen, Bloki.block_cyan, aktualny.x, aktualny.y);
	break;
	}
	case L: {
	DrawSurface(Okno.screen, Bloki.block_yellow, aktualny.x, aktualny.y);
	break;
	}
	case J: {
	DrawSurface(Okno.screen, Bloki.block_pink, aktualny.x, aktualny.y);
	break;
	}
	case S: {
	DrawSurface(Okno.screen, Bloki.block_blue, aktualny.x, aktualny.y);
	break;
	}
	case Z: {
	DrawSurface(Okno.screen, Bloki.block_green, aktualny.x, aktualny.y);
	break;
	}
	case SCIANA: {
	DrawSurface(Okno.screen, Bloki.block_wall, aktualny.x, aktualny.y);
	break;
	}
	case PUSTE: {
	break;
	}
	}
}

//Funkcja zawiera wszystkie czynnosci w obrebie opadania pojedynczego klocka - od jego pojawienia sie na gorze do zatrzymania
void Opadanie_klocka(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki,klocek_t nowy,klocek_t next, gra_t *wartosci_gry, SDL_Surface *charset)
{
	czas_t czas_lokalny = {0, 0, 0.0, 0.0};
	SDL_Event event;
	czas_lokalny.t1 = SDL_GetTicks();
	while (!wartosci_gry->quit && !wartosci_gry->nowa_gra && !wartosci_gry->natychmiastowe_opadniecie)
	{
		Aktualizuj_czas(&czas_lokalny, &(*wartosci_gry));
		Sprawdz_zmiane_etapu(&(*wartosci_gry));
		if (czas_lokalny.czas_od_zmiany_pozycji >= 1/wartosci_gry->speed)
		{
			for (int i = 0; i < 4; i++) nowy.polozenie[i].y += SZEROKOSC_BLOKU;
			czas_lokalny.czas_od_zmiany_pozycji = 0.0;
		}
		//sprawdzenie kolizji
		Aktualizuj_plansze(mapa, Okno, Kolory, Bloki, nowy, next, charset, *wartosci_gry);
		if (Wystepowanie_kolizji_dol(nowy, mapa))
		{
			Przypisz_do_planszy(nowy, mapa);
			if (Sprawdzenie_konca_gry(mapa)) wartosci_gry->quit = true;
			Usun_linie(mapa, &(*wartosci_gry));
			return;
		}
		while (SDL_PollEvent(&event)) {
			Obsluga_zdarzen(event, &nowy, next, mapa, &(*wartosci_gry), Kolory, Bloki, &(*Okno), charset, &czas_lokalny);
		}
	}
}

void Wyswietl_elementy_statyczne(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], bloki_t Bloki, okno_t Okno)
{
	wspolrzedne_t miejsce_na_ekranie = {POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y};
	for (int i = 0; i < WYSOKOSC_PLANSZY + 1; i++)
	{
		for (int j = 0; j < SZEROKOSC_PLANSZY + 2; j++)
		{
			Rysuj_pojedynczy_blok(mapa[i][j], miejsce_na_ekranie, Bloki, Okno);
			miejsce_na_ekranie.x += SZEROKOSC_BLOKU;
		}
		miejsce_na_ekranie.x = POZYCJA_PLANSZY_X;
		miejsce_na_ekranie.y += SZEROKOSC_BLOKU;
	}
}

//ustawia wartosci poczatkowe mapy - zaznacza obrys planszy
void Ustaw_mape(typ_bloku docelowa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	for (int i = 0; i < WYSOKOSC_PLANSZY; i++)
	{
		docelowa[i][0] = docelowa[i][SZEROKOSC_PLANSZY + 1] = SCIANA;
	}
	for (int i = 0; i < SZEROKOSC_PLANSZY + 2; i++) docelowa[WYSOKOSC_PLANSZY][i] = SCIANA;
}

void Rozpocznij_runde(okno_t *Okno, bloki_t Bloki, kolory_t Kolory, SDL_Surface *charset)
{
	typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2]; //przechowuje stan planszy - elementy to pola wielkosci jednego bloku na planszy
	klocek_t nowy, next;
	int quit = 0;
	srand(time(NULL));
	next.typ = Losuj_typ_bloku();
	for (int i = 0; i < WYSOKOSC_PLANSZY; i++)
	{
		for (int j = 1; j < SZEROKOSC_PLANSZY + 2; j++)
		{
			mapa[i][j] = PUSTE;
		}
	}
	gra_t wartosci_gry = { false, false, false, PREDKOSC_POCZATKOWA};
	Pobierz_liste_wynikow(&wartosci_gry);
	Ustaw_aktualna_pozycje_highscore(&wartosci_gry);
	Ustaw_mape(mapa);
	while(!wartosci_gry.quit)
	{
		if (wartosci_gry.nowa_gra) wartosci_gry.nowa_gra = false;
		if (wartosci_gry.natychmiastowe_opadniecie) wartosci_gry.natychmiastowe_opadniecie = false;
		nowy.typ = next.typ;
		Ustaw_nowy(&nowy, POZYCJA_NOWEGO_X, POZYCJA_NOWEGO_Y);
		Ustaw_rozmiar_klocka(&nowy);
		next.typ = Losuj_typ_bloku();
		Ustaw_nowy(&next, POZYCJA_X_NASTEPNEGO, POZYCJA_Y_NASTEPNEGO);
		Ustaw_rozmiar_klocka(&next);
		Opadanie_klocka(mapa, Okno, Kolory, Bloki, nowy, next, &wartosci_gry, charset);
	}
	Zakoncz_gre(Okno, charset, Kolory, wartosci_gry);
}

void Ustaw_nowy(klocek_t *docelowy, int x_startowe, int y_startowe)
{
	switch (docelowy->typ) //0 - glowny blok
	{
	case I: {
		for (int i = 0; i < 4; i++)
		{
			docelowy->polozenie[i].x = x_startowe;
			docelowy->polozenie[i].y = y_startowe + i * SZEROKOSC_BLOKU;
		}
		break;
	}
	case J: {
		for (int i = 0; i < 3; i++)
		{
			docelowy->polozenie[i].x = x_startowe;
			docelowy->polozenie[i].y = y_startowe + (i * SZEROKOSC_BLOKU);
		}
		docelowy->polozenie[3].x = x_startowe - SZEROKOSC_BLOKU;
		docelowy->polozenie[3].y = y_startowe + (2 * SZEROKOSC_BLOKU);
		break;
	}
	case L: {
		for (int i = 0; i < 3; i++)
		{
			docelowy->polozenie[i].x = x_startowe;
			docelowy->polozenie[i].y = y_startowe + (i * SZEROKOSC_BLOKU);
		}
		docelowy->polozenie[3].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].y = y_startowe + (2 * SZEROKOSC_BLOKU);
		break;
	}
	case O: {
		docelowy->polozenie[0].x = x_startowe;
		docelowy->polozenie[0].y = y_startowe;
		docelowy->polozenie[1].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[1].y = y_startowe;
		docelowy->polozenie[2].x = x_startowe;
		docelowy->polozenie[2].y = y_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].y = y_startowe + SZEROKOSC_BLOKU;
		break;
	}
	case S: {
		docelowy->polozenie[0].x = x_startowe;
		docelowy->polozenie[0].y = y_startowe;
		docelowy->polozenie[1].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[1].y = y_startowe;
		docelowy->polozenie[2].x = x_startowe - SZEROKOSC_BLOKU;
		docelowy->polozenie[2].y = y_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].x = x_startowe;
		docelowy->polozenie[3].y = y_startowe + SZEROKOSC_BLOKU;
		break;
	}
	case T: {
		for (int i = 0; i < 3; i++)
		{
			docelowy->polozenie[i].x = x_startowe + (i * SZEROKOSC_BLOKU);
			docelowy->polozenie[i].y = y_startowe;
		}
		docelowy->polozenie[3].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].y = y_startowe + SZEROKOSC_BLOKU;
		break;
	}
	case Z: {
		docelowy->polozenie[0].x = x_startowe;
		docelowy->polozenie[0].y = y_startowe;
		docelowy->polozenie[1].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[1].y = y_startowe;
		docelowy->polozenie[2].x = x_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[2].y = y_startowe + SZEROKOSC_BLOKU;
		docelowy->polozenie[3].x = x_startowe + (2 * SZEROKOSC_BLOKU);
		docelowy->polozenie[3].y = y_startowe + SZEROKOSC_BLOKU;
		break;
	}
	}
}

bool Wystepowanie_kolizji_dol(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2] )
{
	int indeks_x, indeks_y;
	for (int i = 0; i < 4; i++)
	{
		indeks_x = (klocek.polozenie[i].x - POZYCJA_PLANSZY_X) / SZEROKOSC_BLOKU;
		indeks_y = (klocek.polozenie[i].y - POZYCJA_PLANSZY_Y) / SZEROKOSC_BLOKU;

		if (indeks_y < WYSOKOSC_PLANSZY)
			if (mapa_planszy[indeks_y + 1][indeks_x] != PUSTE)
			{
				return true;
				break;
			}
	}
	return false;
}

bool Wystepowanie_kolizji_lewo(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	int indeks_x, indeks_y;
	for (int i = 0; i < 4; i++)
	{
		indeks_x = (klocek.polozenie[i].x - POZYCJA_PLANSZY_X) / SZEROKOSC_BLOKU;
		indeks_y = (klocek.polozenie[i].y - POZYCJA_PLANSZY_Y) / SZEROKOSC_BLOKU;
		if (indeks_x > 0)
			if (mapa_planszy[indeks_y][indeks_x - 1] != PUSTE) return true;
	}
	return false;
}

bool Wystepowanie_kolizji_prawo(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	int indeks_x, indeks_y;
	for (int i = 0; i < 4; i++)
	{
		indeks_x = (klocek.polozenie[i].x - POZYCJA_PLANSZY_X) / SZEROKOSC_BLOKU;
		indeks_y = (klocek.polozenie[i].y - POZYCJA_PLANSZY_Y) / SZEROKOSC_BLOKU;
		if (indeks_x > 0)
			if (mapa_planszy[indeks_y][indeks_x + 1] != PUSTE) return true;
	}
	return false;
}

void Przypisz_do_planszy(klocek_t klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	int indeks_x, indeks_y;
	for (int i = 0; i < 4; i++)
	{
		indeks_x = (klocek.polozenie[i].x - POZYCJA_PLANSZY_X) / SZEROKOSC_BLOKU;
		indeks_y = (klocek.polozenie[i].y - POZYCJA_PLANSZY_Y) / SZEROKOSC_BLOKU;
		mapa_planszy[indeks_y][indeks_x] = klocek.typ;
	}
}

bool Sprawdzenie_konca_gry(typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	for (int i = 1; i < SZEROKOSC_PLANSZY;i++)
	{
		if (mapa_planszy[0][i] != PUSTE) return true;
	}
	return false;
}

//ustawia dlugosc i szerokosc klocka - do funkcji obracajπcej
void Ustaw_rozmiar_klocka(klocek_t *klocek)
{
	int max_X, max_Y, min_X, min_Y;
	max_X = min_X = klocek->polozenie[0].x;
	max_Y = min_Y = klocek->polozenie[0].y;
	//maksimum
	for (int i = 0; i < 4; i++)
	{
		if(klocek->polozenie[i].x > max_X) max_X = klocek->polozenie[i].x;
		if (klocek->polozenie[i].y > max_Y) max_Y = klocek->polozenie[i].y;
	}
	//minimum
	for (int i = 0; i < 4; i++)
	{
		if (klocek->polozenie[i].x < min_X) min_X = klocek->polozenie[i].x;
		if (klocek->polozenie[i].y < min_Y) min_Y = klocek->polozenie[i].y;
	}
	klocek->szerokosc = ((max_X - min_X) / SZEROKOSC_BLOKU) + 1;
	klocek->dlugosc = ((max_Y - min_Y) / SZEROKOSC_BLOKU) + 1;
}

void Rotacja(klocek_t *klocek, typ_bloku mapa_planszy[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	wspolrzedne_t nowe_wspolrzedne[4];
	//znajdujemy skrajne bloki:
	int min_X, min_Y;
	min_X = klocek->polozenie[0].x;
	min_Y = klocek->polozenie[0].y;
	for (int i = 0; i < 4; i++)
	{
		if (klocek->polozenie[i].x < min_X) min_X = klocek->polozenie[i].x;
		if (klocek->polozenie[i].y < min_Y) min_Y = klocek->polozenie[i].y;
	}
	int nr_od_lewej, nr_od_gory, nr_od_lewej_nowego, nr_od_gory_nowego; //pozycja bloku w calym klocku <1;4>
	for (int i = 0; i < 4; i++)
	{
		//obliczamy nowe wspolrzedne kazdego bloku na podstawie jego polozenia w obrebie calego klocka
		nr_od_lewej = ((klocek->polozenie[i].x - min_X) / SZEROKOSC_BLOKU) + 1;
		nr_od_gory = ((klocek->polozenie[i].y - min_Y) / SZEROKOSC_BLOKU) + 1;
		nr_od_lewej_nowego = nr_od_gory;
		nr_od_gory_nowego = klocek->szerokosc - nr_od_lewej + 1;
		nowe_wspolrzedne[i].x = min_X + (nr_od_lewej_nowego - 1) * SZEROKOSC_BLOKU;
		nowe_wspolrzedne[i].y = min_Y + (nr_od_gory_nowego - 1) * SZEROKOSC_BLOKU;
	}
	klocek_t kopia;
	for (int i = 0; i < 4; i++) kopia.polozenie[i] = nowe_wspolrzedne[i];
	kopia.typ = klocek->typ;
	Ustaw_rozmiar_klocka(&kopia);
	Wyrownaj_polozenie_klocka(&kopia);
	
	if (!Mozliwosc_rotacji(kopia.polozenie, mapa_planszy)) return;
	for (int i = 0; i < 4; i++) klocek->polozenie[i] = kopia.polozenie[i];
}

//spawdza czy klocek po rotacji nie koliduje z innymi blokami
bool Mozliwosc_rotacji(wspolrzedne_t nowe_wspolrzedne[], typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2])
{
	int indeks_x, indeks_y;
	for (int i = 0; i < 4; i++)
	{
		indeks_x = (nowe_wspolrzedne[i].x - POZYCJA_PLANSZY_X) / SZEROKOSC_BLOKU;
		indeks_y = (nowe_wspolrzedne[i].y - POZYCJA_PLANSZY_Y) / SZEROKOSC_BLOKU;
		if (indeks_x < 1 || indeks_x > SZEROKOSC_PLANSZY) return false;
		if (indeks_y > WYSOKOSC_PLANSZY - 1) return false;
		if (mapa[indeks_y][indeks_x] != PUSTE) return false;
	}
	return true;
}

//Rotacja w przypadku niektorych blokow powoduje przesuniecie calosci w dol badz gore
//Funkcja wyrownuje te przesuniecia
void Wyrownaj_polozenie_klocka(klocek_t *klocek)
{
	if (klocek->typ == I && klocek->polozenie[0].x == klocek->polozenie[1].x) //klocek I w wersji poziomej
	{
		for (int i = 0; i < 4; i++) klocek->polozenie[i].y += 3 * SZEROKOSC_BLOKU;
	}
	if ((klocek->typ == J || klocek->typ == L) && klocek->dlugosc > klocek->szerokosc)
		for (int i = 0; i < 4; i++) klocek->polozenie[i].y += SZEROKOSC_BLOKU;
	if (klocek->typ == T && klocek->dlugosc > klocek->szerokosc)
		for (int i = 0; i < 4; i++) klocek->polozenie[i].y -= SZEROKOSC_BLOKU;
}

void Usun_linie(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry)
{
	bool linia_pelna = true;
	wartosci_gry->ilosc_usunietych_wierszy = 0;
	for (int i = 0; i < WYSOKOSC_PLANSZY; i++)
	{
		for (int j = 1; j < SZEROKOSC_PLANSZY + 1; j++)
		{
			if (mapa[i][j] == PUSTE) linia_pelna = false;
		}
		//usuniecie linii
		if (linia_pelna == true) {
			wartosci_gry->ilosc_usunietych_wierszy++;
			for (int k = 1; k < SZEROKOSC_PLANSZY + 1; k++)
			{
				mapa[i][k] = PUSTE;
			}
			Przesun_plansze(mapa, i);
		}
		linia_pelna = true;
	}
	Dodaj_punkty(&(*wartosci_gry));
	if(wartosci_gry->ilosc_usunietych_wierszy != 0)
		wartosci_gry->ilosc_poprzednio_usunietych_wierszy = wartosci_gry->ilosc_usunietych_wierszy;
}

//przesuwa statyczne bloki na planszy w dol(po usunieciu linii)
void Przesun_plansze(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], int indeks_startowy)
{
	for (int i = indeks_startowy; i > 0; i--)
	{
		for (int j = 1; j < SZEROKOSC_PLANSZY + 1; j++)
		{
			mapa[i][j] = mapa[i - 1][j];
		}
	}
}

void Resetuj_gre(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t *wartosci_gry)
{
	for (int i = 0; i < WYSOKOSC_PLANSZY; i++)
	{
		for (int j = 1; j < SZEROKOSC_PLANSZY + 1; j++)
		{
			mapa[i][j] = PUSTE;
		}
	}
	wartosci_gry->nowa_gra = true;
	wartosci_gry->worldTime = 0;
	wartosci_gry->score = 0;
	wartosci_gry->etap_gry = 0;
	wartosci_gry->aktualna_pozycja_high_score = 0;
	wartosci_gry->speed = PREDKOSC_POCZATKOWA;
}

void Aktualizuj_czas(czas_t *czas_lokalny, gra_t *wartosci_gry)
{
	czas_lokalny->t2 = SDL_GetTicks();
	czas_lokalny->czas_od_aktualizacji = (czas_lokalny->t2 - czas_lokalny->t1) * 0.001; //czas od ostatniego narysowania ekranu w sekundach
	czas_lokalny->t1 = czas_lokalny->t2;
	wartosci_gry->worldTime += czas_lokalny->czas_od_aktualizacji;
	wartosci_gry->czas_od_przyspieszenia += czas_lokalny->czas_od_aktualizacji;
	czas_lokalny->czas_od_zmiany_pozycji += czas_lokalny->czas_od_aktualizacji;
}

void Sprawdz_zmiane_etapu(gra_t *wartosci_gry)
{
	if (wartosci_gry->etap_gry <= MAKSYMALNA_ILOSC_PRZYSPIESZEN)
	{
		if (wartosci_gry->czas_od_przyspieszenia > CZESTOTLIWOSC_PRZYSPIESZENIA)
		{
			wartosci_gry->etap_gry++;
			wartosci_gry->speed += 0.7;
			wartosci_gry->czas_od_przyspieszenia = 0.0;
		}
	}
}

void Natychmiastowe_opadniecie(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], okno_t *Okno, kolory_t Kolory, bloki_t Bloki, 
								klocek_t nowy, klocek_t next, gra_t *wartosci_gry, SDL_Surface *charset, czas_t *czas_lokalny)
{
	bool kolizja = false;
	while (!kolizja)
	{
		Aktualizuj_czas(&(*czas_lokalny), &(*wartosci_gry));
		if (czas_lokalny->czas_od_zmiany_pozycji >= 1/2)
		{
			for (int i = 0; i < 4; i++) nowy.polozenie[i].y += SZEROKOSC_BLOKU;
			czas_lokalny->czas_od_zmiany_pozycji = 0.0;
		}
		Aktualizuj_plansze(mapa, Okno, Kolory, Bloki, nowy, next, charset, *wartosci_gry);
		if (Wystepowanie_kolizji_dol(nowy, mapa))
		{
			Przypisz_do_planszy(nowy, mapa);
			if (Sprawdzenie_konca_gry(mapa)) wartosci_gry->quit = true;
			kolizja = true;
		}
	}
	Usun_linie(mapa, &(*wartosci_gry));
}

void Dodaj_punkty(gra_t *wartosci_gry)
{
	switch (wartosci_gry->ilosc_usunietych_wierszy)
	{
	case 4: {
		if (wartosci_gry->ilosc_poprzednio_usunietych_wierszy == 4)
			wartosci_gry->score += 1200 * (wartosci_gry->etap_gry + 1);
		else wartosci_gry->score += 800 * (wartosci_gry->etap_gry + 1);
		break;
	}
	case 3: {
		wartosci_gry->score += 400 * (wartosci_gry->etap_gry + 1);
		break;
	}
	case 2: {
		wartosci_gry->score += 200 * (wartosci_gry->etap_gry + 1);
		break;
	}
	case 1: {
		wartosci_gry->score += 100 * (wartosci_gry->etap_gry + 1);
		break;
	}
	default: return;
	}
	Ustaw_aktualna_pozycje_highscore(&(*wartosci_gry));
}

void Pobierz_liste_wynikow(gra_t *wartosci_gry)
{
	FILE *plik;
	plik = fopen("high_score.txt", "r");
	char znak;
	int nr_litery = 0, nr_wiersza = 0;
	do
	{
		znak = fgetc(plik);
		if (znak == EOF) break;
		if (znak == '\n')
		{
			nr_wiersza++;
			continue;
		}
		if (znak == ' ')
		{
			nr_litery = 0;
			fscanf(plik, "%d", &wartosci_gry->high_score_punkty[nr_wiersza]);
		}
		else {
			wartosci_gry->high_score_imiona[nr_wiersza][nr_litery] = znak;
			nr_litery++;
		}
	}while (znak != EOF);
	fclose(plik);
}

void Wyswietl_najlepsze_wyniki(gra_t wartosci_gry, SDL_Surface *charset, okno_t *Okno)
{
	char text[128];
	sprintf(text, "               HIGH SCORE");
	DrawString(Okno->screen, POZYCJA_HIGH_SCORE_X, POZYCJA_HIGH_SCORE_Y - 20, text, charset);
	for (int i = 0; i < ILOSC_NAJLEPSZYCH_WYNIKOW; i++)
	{
		sprintf(text,"%d. %s  %d", i + 1, wartosci_gry.high_score_imiona[i], wartosci_gry.high_score_punkty[i]);
		DrawString(Okno->screen, POZYCJA_HIGH_SCORE_X, POZYCJA_HIGH_SCORE_Y + ((i + 1) * 10), text, charset);
	}
}

void Ustaw_aktualna_pozycje_highscore(gra_t *wartosci_gry)
{
	for (int i = ILOSC_NAJLEPSZYCH_WYNIKOW - 1; i >= 0; i--)
	{
		if (wartosci_gry->score < wartosci_gry->high_score_punkty[i]) break;
		else wartosci_gry->aktualna_pozycja_high_score = i + 1;
	}
}

void Wyswietl_aktualny_highscore(gra_t wartosci_gry, SDL_Surface *charset, okno_t *Okno)
{
	char text[128];
	sprintf(text, "GRATULACJE! OSIAGNALES LISTE NAJLEPSZYCH WYNIKOW!");
	DrawString(Okno->screen, POZYCJA_AKTUALNY_HIGHSCORE_X, POZYCJA_AKTUALNY_HIGHSCORE_Y, text, charset);
	sprintf(text, "AKTUALNA POZYCJA: %d", wartosci_gry.aktualna_pozycja_high_score);
	DrawString(Okno->screen, POZYCJA_AKTUALNY_HIGHSCORE_X, POZYCJA_AKTUALNY_HIGHSCORE_Y + 25, text, charset);
}

char *Pobierz_imie(okno_t *Okno, SDL_Surface *charset, kolory_t Kolory, gra_t wartosci_gry)
{
	SDL_Event event;
	char imie[30], text[128], aktualne_miejsce[128];
	imie[0] = '\0';
	char aktualny = NULL;
	int nr_litery = 0;
	sprintf(text, "Podaj imie: ");
	sprintf(aktualne_miejsce, "Gratulacje, osiagnales %d miejsce na liscie najlepszych wynikow!", wartosci_gry.aktualna_pozycja_high_score);
	SDL_FillRect((*Okno).screen, NULL, Kolory.czarny);
	DrawString(Okno->screen, POZYCJA_IMIENIA_X - 100, POZYCJA_IMIENIA_Y, text, charset);
	DrawString(Okno->screen, POZYCJA_IMIENIA_X, POZYCJA_IMIENIA_Y - 30, aktualne_miejsce, charset);
	while (aktualny != 13 && nr_litery < 29)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				aktualny = event.key.keysym.sym;
				if ((aktualny >= 48 && aktualny <= 57) || (aktualny >= 65 && aktualny <= 90) || (aktualny >= 97 && aktualny <= 122) || aktualny == 45)
				{
					imie[nr_litery] = aktualny;
					if (aktualny == 45) imie[nr_litery] = '_';
					imie[nr_litery + 1] = '\0';
					SDL_FillRect((*Okno).screen, NULL, Kolory.czarny);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X - 100, POZYCJA_IMIENIA_Y, text, charset);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X, POZYCJA_IMIENIA_Y - 30, aktualne_miejsce, charset);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X, POZYCJA_IMIENIA_Y, imie, charset);
					nr_litery++; 
				}
				if (aktualny == 8 && nr_litery > 0) //backspace
				{
					imie[nr_litery] = NULL;
					imie[nr_litery - 1] = '\0';
					SDL_FillRect((*Okno).screen, NULL, Kolory.czarny);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X - 100, POZYCJA_IMIENIA_Y, text, charset);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X, POZYCJA_IMIENIA_Y - 30, aktualne_miejsce, charset);
					DrawString(Okno->screen, POZYCJA_IMIENIA_X, POZYCJA_IMIENIA_Y, imie, charset);
					nr_litery--;
				}
			}
		}
		SDL_UpdateTexture((*Okno).scrtex, NULL, (*Okno).screen->pixels, (*Okno).screen->pitch);
		SDL_RenderCopy((*Okno).renderer, (*Okno).scrtex, NULL, NULL);
		SDL_RenderPresent((*Okno).renderer);
	}
	return imie;
}

void Zakoncz_gre(okno_t *Okno, SDL_Surface *charset, kolory_t Kolory, gra_t wartosci_gry)
{
	if (wartosci_gry.aktualna_pozycja_high_score > 0)
	{
		char *imie;
		imie = Pobierz_imie(Okno, charset, Kolory, wartosci_gry);
		//przesuwamy wyniki w dol
		int j = 0;
		for (int i = ILOSC_NAJLEPSZYCH_WYNIKOW - 1; i >= wartosci_gry.aktualna_pozycja_high_score; i--)
		{
			wartosci_gry.high_score_punkty[i] = wartosci_gry.high_score_punkty[i - 1];
			for (int k = 0; k < 30; k++)
				wartosci_gry.high_score_imiona[i][k] = NULL;
			j = 0;
			while (wartosci_gry.high_score_imiona[i - 1][j] != '\0')
			{
				wartosci_gry.high_score_imiona[i][j] = wartosci_gry.high_score_imiona[i - 1][j];
				j++;
			}
			wartosci_gry.high_score_imiona[i][j] = '\0';
		}
		//przypisujemy imie i punkty do listy wynikow
		for (int k = 0; k < 30; k++)
			wartosci_gry.high_score_imiona[wartosci_gry.aktualna_pozycja_high_score - 1][k] = NULL;
		int i = 0;
		while (imie[i] != '\0')
		{
			wartosci_gry.high_score_imiona[wartosci_gry.aktualna_pozycja_high_score - 1][i] = imie[i];
			i++;
		}
		wartosci_gry.high_score_punkty[wartosci_gry.aktualna_pozycja_high_score - 1] = wartosci_gry.score;
	}
	Aktualizuj_liste_wynikow(wartosci_gry);
}

void Aktualizuj_liste_wynikow(gra_t wartosci_gry)
{
	FILE *plik;
	plik = fopen("high_score.txt", "w");
	char *aktualne_imie;
	for (int i = 0; i < ILOSC_NAJLEPSZYCH_WYNIKOW; i++)
	{
		if (plik != NULL)
		{
			aktualne_imie = wartosci_gry.high_score_imiona[i];
			fputs(aktualne_imie, plik);
			fputc(' ', plik);
			fprintf(plik, "%d", wartosci_gry.high_score_punkty[i]);
			if (i == ILOSC_NAJLEPSZYCH_WYNIKOW - 1)
				break;
			fputc('\n', plik);
		}
	}
	fclose(plik);
}

void Zatrzymaj_gre(okno_t *Okno, SDL_Surface *charset, gra_t *wartosci_gry)
{
	bool pauza = true;
	char text[128];
	sprintf(text, "PAUZA.Nacisnij p aby wrocic do gry.");
	SDL_Event event;
	while (pauza)
	{
		DrawString(Okno->screen, 200, 200, text, charset);
		SDL_UpdateTexture((*Okno).scrtex, NULL, (*Okno).screen->pixels, (*Okno).screen->pitch);
		SDL_RenderCopy((*Okno).renderer, (*Okno).scrtex, NULL, NULL);
		SDL_RenderPresent((*Okno).renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_p) pauza = false;
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					pauza = false;
					wartosci_gry->quit = true;
				}
			}
		}
	}
}

void Ekran_startowy(okno_t *Okno, bloki_t Bloki, kolory_t Kolory, SDL_Surface *charset)
{
	char text[128];
	SDL_Event event;
	sprintf(text, "WITAJ W GRZE TETRIS. NACISNIJ n ABY ROZPOCZAC NOWA GRE.");
	bool koniec_gry = false;
	while (!koniec_gry)
	{
		SDL_FillRect((*Okno).screen, NULL, Kolory.czarny);
		DrawString(Okno->screen, 300, 300, text, charset);
		SDL_UpdateTexture((*Okno).scrtex, NULL, (*Okno).screen->pixels, (*Okno).screen->pitch);
		SDL_RenderCopy((*Okno).renderer, (*Okno).scrtex, NULL, NULL);
		SDL_RenderPresent((*Okno).renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_n) Rozpocznij_runde(&(*Okno), Bloki, Kolory, charset);
				if (event.key.keysym.sym == SDLK_ESCAPE) koniec_gry = true;
			}
		}
	}
}

void Wylacz_gre(SDL_Surface *charset, okno_t *Okno)
{
	SDL_FreeSurface(charset);
	SDL_FreeSurface(Okno->screen);
	SDL_DestroyTexture(Okno->scrtex);
	SDL_DestroyRenderer(Okno->renderer);
	SDL_DestroyWindow(Okno->window);
	SDL_Quit();
}

void Zapisz_gre(typ_bloku mapa[WYSOKOSC_PLANSZY + 1][SZEROKOSC_PLANSZY + 2], gra_t wartosci_gry, klocek_t aktualny, klocek_t next)
{

}