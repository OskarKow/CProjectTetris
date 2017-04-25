#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "Header.h"

extern "C" {
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
}

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
	int rc;
	SDL_Surface *charset;
	okno_t Okno = { NULL, NULL, NULL, NULL };
	kolory_t Kolory;
	bloki_t Bloki;
	char text[128];

	Czynnosci_poczatkowe(&Okno, &Kolory, &Bloki);
	Wczytaj_grafiki(&charset, "./cs8x8.bmp", &Okno);
	SDL_SetColorKey(charset, true, 0x000000);
	Ustaw_okno(&Okno, &rc, WINDOW);

	Ekran_startowy(&Okno, Bloki, Kolory, charset);

	SDL_FreeSurface(charset);
	SDL_FreeSurface(Okno.screen);
	SDL_DestroyTexture(Okno.scrtex);
	SDL_DestroyRenderer(Okno.renderer);
	SDL_DestroyWindow(Okno.window);
	SDL_Quit();
	return 0;
}


