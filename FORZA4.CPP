#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <iostream.h>
#include <alloc.h>
#include <dos.h>
#include <bmp.h>
#include <string.h>
#include <ctype.h>
#include <mem.h>
#include <vga.h>
#include <svgacc.h>
#include <svga_drv.h>

#define DIMCHAR 25

#define ARROW_RIGHT 77
#define ARROW_LEFT 75
#define ARROW_UP 77
#define ARROW_DOWN 80
#define ESCAPE 27

int huge G320(void) {
  return (0); //320x200 256
}

//r  c
int griglia[6][7]; // tavolo di gioco
int posizioni[7]; // posizioni riempite
int controlli(char * , char * );
int or(void);
int ver(void);
int obli(void);
int obli1(void);
void disegno(char * , char * );
void azzera(void);
void visualizza(void);
void titoli(char * , char * );
void copia(char * , char * );
void statusBar(void);
void lampeggio(char * );
void vittorie(double, double);
int input_nome(char * , char * , int);
char player_1[15];
char player_2[15];
double vittorie_1 = 0, vittorie_2 = 0;
void titoli_coda(void);
main() {
  int x = 25, y = 415;
  int numero_dischi = 0;
  int gdriver = DETECT, gmode, errorcode;
  int i = 0, j;
  int dischetto = 0;
  int tasto = 80;
  char tocca1[50] = {
    "Tocca a "
  };
  char tocca2[50] = {
    "Tocca a "
  };

  unsigned int c;
  void * disco, * player1, * player2;
  titoli(player_1, player_2);
  initgraph( & gdriver, & gmode, "c:\\borlandc\\bgi\\");

  disegno(player_1, player_2);
  strcat(tocca1, player_1);
  strcat(tocca2, player_2);

  //per effettuare il conteggio dei punti del player_1
  vittorie(vittorie_1, vittorie_2);

  player1 = (void * ) farmalloc(imagesize(getmaxx() - 150, 50, getmaxx() - 10, 90));
  if (player1 == NULL) exit(0);
  getimage(getmaxx() - 150, 50, getmaxx() - 10, 90, player1);
  player2 = (void * ) farmalloc(imagesize(getmaxx() - 150, 150, getmaxx() - 10, 190));
  if (player2 == NULL) exit(0);
  getimage(getmaxx() - 150, 150, getmaxx() - 10, 190, player2);

  azzera();
  do {
    if (tasto == 80) {
      i = 0;
      if (dischetto % 2 == 0) {
        // visualizzo il dischetto ROSSO
        setfillstyle(SOLID_FILL, RED);
        setcolor(LIGHTBLUE);
        circle((34 + (68 * 0)), (29 + (58 * 0)), 26);
        floodfill((34 + (68 * 0)), (29 + (58 * 0)), LIGHTBLUE);
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        setcolor(WHITE);
        rectangle(getmaxx() - 151, 49, getmaxx() - 9, 91);
        rectangle(getmaxx() - 152, 48, getmaxx() - 10, 92);
        putimage(getmaxx() - 150, 50, player1, COPY_PUT);
        putimage(getmaxx() - 150, 150, player2, NOT_PUT);
        //
        settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
        setcolor(BLUE);
        outtextxy(x, y, tocca2);
        setcolor(MAGENTA);
        outtextxy(x, y, tocca1);
        //
      } else {
        // visualizzo il dischetto GIALLO
        setfillstyle(SOLID_FILL, YELLOW);
        setcolor(LIGHTBLUE);
        circle((34 + (68 * 0)), (29 + (58 * 0)), 26);
        floodfill((34 + (68 * 0)), (29 + (58 * 0)), LIGHTBLUE);
        setcolor(WHITE);
        rectangle(getmaxx() - 151, 149, getmaxx() - 10, 191);
        rectangle(getmaxx() - 152, 148, getmaxx() - 9, 192);
        putimage(getmaxx() - 150, 50, player1, NOT_PUT);
        putimage(getmaxx() - 150, 150, player2, COPY_PUT);
        //
        settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
        setcolor(BLUE);
        outtextxy(x, y, tocca1);
        setcolor(MAGENTA);
        outtextxy(x, y, tocca2);
        //
      }
      disco = (void * ) farmalloc(imagesize(0, 0, 68, 58));
      getimage(0, 0, 68, 58, disco);
    }
    inizio:
      tasto = getch();
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    circle((34 + (68 * i)), (29 + (58 * 0)), 26);
    floodfill((34 + (68 * i)), (29 + (58 * 0)), BLACK);
    switch (tasto) {
    case ARROW_RIGHT: // FRECCIA A DESTRA
      if (i == 6) i = 0;
      else i++;
      putimage(68 * i, 0, disco, COPY_PUT);
      break;
    case ARROW_LEFT: // FRECCIA A SINISTRA
      if (i == 0) i = 6;
      else i--;
      putimage(68 * i, 0, disco, COPY_PUT);
      break;
    case ARROW_DOWN: // FRECCIA GIU'
      if (numero_dischi >= 42) {
        setcolor(BLUE);
        outtextxy(x, y, tocca2);
        setcolor(BLUE);
        outtextxy(x, y, tocca2);
        lampeggio("Pareggio");
        azzera();
        disegno(player_1, player_2);
        vittorie(vittorie_1, vittorie_2);
        free(disco);
        i = j = 0;
        numero_dischi = 0;
        break;
      }

      j = posizioni[i];
      j++;
      //			posizioni[i]=posizioni[i]-1;
      if (posizioni[i] >= 0) {
        numero_dischi++;
        if (dischetto % 2 == 0) {
          // visualizzo il dischetto ROSSO  Player_1
          setfillstyle(SOLID_FILL, RED);
          setcolor(LIGHTBLUE);
          circle((34 + (68 * i)), (29 + (58 * j)), 26);
          floodfill((34 + (68 * i)), (29 + (58 * j)), LIGHTBLUE);
          griglia[posizioni[i]][i] = 1;
          setcolor(BLUE);
          outtextxy(x, y, tocca1);
        } else {
          // visualizzo il dischetto GIALLO Player_2
          setfillstyle(SOLID_FILL, YELLOW);
          setcolor(LIGHTBLUE);
          circle((34 + (68 * i)), (29 + (58 * j)), 26);
          floodfill((34 + (68 * i)), (29 + (58 * j)), LIGHTBLUE);
          griglia[posizioni[i]][i] = 2;
          setcolor(BLUE);
          outtextxy(x, y, tocca2);
        }
      }
      if ((or() == 1) || (or() == 0) ||
        (ver() == 1) || (ver() == 0) || (obli() == 0) || (obli() == 1) || (obli1() == 0) || (obli1() == 1)) {
        if (dischetto % 2 == 0) vittorie_1++;
        else vittorie_2++;
        azzera();
        numero_dischi = 0;
        disegno(player_1, player_2);
        vittorie(vittorie_1, vittorie_2);
        free(disco);
        i = j = 0;
      } else {
        if (posizioni[i] >= 0) {
          dischetto++;
          free(disco);
        }
        posizioni[i] = posizioni[i] - 1;
      }
      break;
    case ESCAPE:
      closegraph();
      titoli_coda();
      break;
    default:
      putimage(68 * i, 0, disco, COPY_PUT);
      break;
    }
  } while (tasto != ESCAPE);

  free(disco);
  free(player1);
  free(player2);

  return (1);
}
/****************************************************************/
int ver(void) {
  int i, j, k;
  int cont = 0;
  char vittoria[25] = {
    "HAI VINTO "
  };
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  for (i = 0; i < 7; i++) { //5
    for (j = 0; j < 3; j++) {
      cont = 0;
      for (k = 0; k <= 3; k++)
        if (griglia[j + k][i] == 1) cont++;
      if (cont == 4) {
        //		vittorie_1++;
        strcat(vittoria, player_1);
        lampeggio(vittoria);
        return (1);
      }

      cont = 0;
      for (k = 0; k <= 3; k++)
        if (griglia[j + k][i] == 2) cont++;
      if (cont == 4) {
        strcat(vittoria, player_2);
        lampeggio(vittoria);
        //	vittorie_2++;
        return (0);
      }
    }
  }
}
int or(void) {
  int i, j;
  int k, cont = 0;
  char vittoria[25] = {
    "HAI VINTO "
  };
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 4; j++) {
      cont = 0;
      for (k = 0; k <= 3; k++)
        if (griglia[i][j + k] == 1) cont++;
      if (cont == 4) {
        strcat(vittoria, player_1);
        lampeggio(vittoria);
        //		vittorie_1++;
        return (1);
      }
      cont = 0;
      for (k = 0; k <= 3; k++)
        if (griglia[i][j + k] == 2) cont++;
      if (cont == 4) {
        strcat(vittoria, player_2);
        lampeggio(vittoria);
        //		vittorie_2++;
        return (0);
      }
    }
  }
}
int obli(void) {
  int j, i, f = 3, p;
  char vittoria[25] = {
    "HAI VINTO "
  };
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  for (j = 5; j > 2; j--) {
    p = j;
    for (i = 0; i < f; i++) {
      if (griglia[p][i] == 1)
        if (griglia[p - 1][i + 1] == 1)
          if (griglia[p - 2][i + 2] == 1)
            if (griglia[p - 3][i + 3] == 1) {
              strcat(vittoria, player_1);
              lampeggio(vittoria);
              //				vittorie_1++;
              return (1);
            }
      if (griglia[p][i] == 2)
        if (griglia[p - 1][i + 1] == 2)
          if (griglia[p - 2][i + 2] == 2)
            if (griglia[p - 3][i + 3] == 2) {
              strcat(vittoria, player_2);
              lampeggio(vittoria);
              //				vittorie_2++;
              return (0);
            }
      p--;
    }
    f--;
  }
  f = 3;
  for (j = 0; j < 3; j++) {
    p = j;
    for (i = 6; i > f; i--) {
      if (griglia[p][i] == 1)
        if (griglia[p + 1][i - 1] == 1)
          if (griglia[p + 2][i - 2] == 1)
            if (griglia[p + 3][i - 3] == 1) {
              strcat(vittoria, player_1);
              lampeggio(vittoria);
              //			vittorie_1++;
              return (1);
            }
      if (griglia[p][i] == 2)
        if (griglia[p + 1][i - 1] == 2)
          if (griglia[p + 2][i - 2] == 2)
            if (griglia[p + 3][i - 3] == 2) {
              strcat(vittoria, player_2);
              lampeggio(vittoria);
              //	vittorie_2++;
              return (0);
            }
      p++;
    }
    f--;
  }
}
int obli1(void) {
  int j, i, f = 3, p;
  char vittoria[25] = {
    "HAI VINTO "
  };
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  for (j = 5; j > 2; j--) {
    p = j;
    for (i = 6; i > f; i--) {
      if (griglia[p][i] == 1)
        if (griglia[p - 1][i - 1] == 1)
          if (griglia[p - 2][i - 2] == 1)
            if (griglia[p - 3][i - 3] == 1) {
              strcat(vittoria, player_1);
              lampeggio(vittoria);
              //	vittorie_1++;
              return (1);
            }
      if (griglia[p][i] == 2)
        if (griglia[p - 1][i - 1] == 2)
          if (griglia[p - 2][i - 2] == 2)
            if (griglia[p - 3][i - 3] == 2) {
              strcat(vittoria, player_2);
              lampeggio(vittoria);
              //	vittorie_2++;
              return (0);
            }
      p--;
    }
    f--;
  }
  f = 3;
  for (j = 0; j < 3; j++) {
    p = j;
    for (i = 0; i < f; i++) {
      if (griglia[p][i] == 1)
        if (griglia[p + 1][i + 1] == 1)
          if (griglia[p + 2][i + 2] == 1)
            if (griglia[p + 3][i + 3] == 1) {
              strcat(vittoria, player_1);
              lampeggio(vittoria);
              //	vittorie_1++;
              return (1);
            }
      if (griglia[p][i] == 2)
        if (griglia[p + 1][i + 1] == 2)
          if (griglia[p + 2][i + 2] == 2)
            if (griglia[p + 3][i + 3] == 2) { //20
              strcat(vittoria, player_2);
              lampeggio(vittoria);
              //	vittorie_2++;
              return (0);

            }
      p++;
    }
    f--;
  }
}
void azzera(void) {
  int i = 0, j = 0;
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 7; j++) {
      posizioni[j] = 5;
      griglia[i][j] = 0;
    }
  }
}

void visualizza(void) {
  int i = 0, j = 0;
  for (i = 0; i < 6; i++) {
    gotoxy(19, 10 + i);
    for (j = 0; j < 7; j++) {
      gotoxy(19 + j, 10 + i);
      printf("%d", griglia[i][j]);
    }
  }
}

void disegno(char * nome1, char * nome2) {
  int i, j;
  setfillstyle(SOLID_FILL, BLACK);
  bar(0, 0, getmaxx() - 161, 58);
  setfillstyle(SOLID_FILL, BLUE);
  bar(0, 58, getmaxx() - 161, 408);
  setcolor(DARKGRAY);
  for (i = 0; i < 7; i++) {
    line(68 * i, 58, 68 * i, 408);
  }
  for (i = 1; i <= 6; i++) {
    line(0, i * 58, getmaxx() - 161, i * 58);
  }
  for (i = 1; i <= 6; i++) {
    for (j = 0; j < 7; j++) {
      setfillstyle(SOLID_FILL, LIGHTGRAY);
      setcolor(LIGHTBLUE);
      circle((34 + (68 * j)), (29 + (58 * i)), 27);
      floodfill((34 + (68 * j)), (29 + (58 * i)), LIGHTBLUE);
    }
  }

  setfillstyle(SOLID_FILL, LIGHTBLUE);
  bar(getmaxx() - 160, 0, getmaxx(), getmaxy());
  // sfondo Giocatore
  setfillstyle(SOLID_FILL, RED);
  bar(getmaxx() - 155, 44, getmaxx() - 5, 145);
  setfillstyle(SOLID_FILL, YELLOW);
  bar(getmaxx() - 155, 146, getmaxx() - 5, 245);
  //
  setfillstyle(SOLID_FILL, CYAN);
  bar(getmaxx() - 150, 295, getmaxx() - 15, 470);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  setcolor(MAGENTA);
  outtextxy(getmaxx() - 140, 300, "PRESS");
  outtextxy(getmaxx() - 120, 340, "ESC");
  outtextxy(getmaxx() - 110, 380, "TO");
  outtextxy(getmaxx() - 125, 420, "EXIT");
  setcolor(BLUE);
  setfillstyle(SOLID_FILL, LIGHTGRAY);

  bar(getmaxx() - 150, 50, getmaxx() - 10, 90);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  outtextxy(getmaxx() - 150, 55, nome1);
  setcolor(BLUE);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  bar(getmaxx() - 150, 150, getmaxx() - 10, 190);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  outtextxy(getmaxx() - 150, 155, nome2);
  setcolor(WHITE);
  rectangle(getmaxx() - 160, 0, getmaxx(), getmaxy());
  rectangle(getmaxx() - 151, 49, getmaxx() - 9, 91);
  rectangle(getmaxx() - 152, 48, getmaxx() - 10, 92);
  rectangle(getmaxx() - 151, 149, getmaxx() - 10, 191);
  rectangle(getmaxx() - 152, 148, getmaxx() - 9, 192);
  statusBar();
}

void screen_write(char output[], int xs, int ys, int tempo) {
  int i, k, step;
  char vector[] = { 
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '!', '.', '?', ':', '-', '[', ']', ' ', ',', '\'', '/'
  };
  i = 0;
  step = 0;
  while (output[i] != '\0') {
    for (k = 0; k < 47; k++) {
      if (output[i] == vector[k]) {
        Put_sprite((k % 10) * DIMCHAR, (k / 10) * DIMCHAR, (k % 10) * DIMCHAR + (DIMCHAR - 1),
          (k / 10) * DIMCHAR + (DIMCHAR - 1), xs + step, ys, -1, 0, SPECIALCOPY);
        delay(tempo);
        if (output[i] == 'i') step += DIMCHAR - 18;
        else step += DIMCHAR - 8;
        break;
      }
    }
    i++;
  }
}

void titoli(char * nome1, char * nome2) {
  int i = 0, j;
  int t;
  int ni;
  char car[27][1] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
	'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
  };
  char app[15];
  char loadingFiles[][30] = {
    "\\resources\\loading-01.bmp",
    "\\resources\\loading-02.bmp",
    "\\resources\\loading-03.bmp",
    "\\resources\\loading-04.bmp",
    "\\resources\\loading-05.bmp",
    "\\resources\\loading-06.bmp",
    "\\resources\\loading-07.bmp",
    "\\resources\\loading-08.bmp",
    "\\resources\\loading-09.bmp"
  };
  ingraf();
  while (!kbhit()) {
    loadbmp(loadingFiles[i]);
    if (i == 8) i = 0;
    else i++;
    setvgapalette();
    visbmp();
    delay(200);
    elimina_bmp();
  }
  clear();
  Close();
  ingraf();

  input_nome("p1", nome1, 1);
  input_nome("p1", nome1, 0);
  input_nome("p2", nome2, 0);

  clear();
  loadbmp("font.bmp");
  setvgapalette();
  screen_write(" press any key", 20, 10, 0);
  screen_write("  to start..", 50, 60, 0);
  screen_write("  forza 4", 70, 100, 0);
  getch();
  elimina_bmp();
  Close();
}

void statusBar(void) {
  setfillstyle(SOLID_FILL, BLUE);
  bar(0, 410, getmaxx() - 161, 480);
  setcolor(WHITE);
  line(22, 468, getmaxx() - 183, 468);
  line(19, 418, getmaxx() - 181, 418);
  line(getmaxx() - 183, 421, getmaxx() - 183, 468);
  line(18, 418, 18, 470);
  setcolor(YELLOW);
  rectangle(0, 410, getmaxx() - 161, 480);
  rectangle(1, 411, getmaxx() - 162, 479);
  setcolor(DARKGRAY);
  rectangle(20, 420, getmaxx() - 181, 470);
  rectangle(21, 421, getmaxx() - 182, 469);
  rectangle(19, 419, getmaxx() - 180, 471);
}

void copia(char * sorg, char * dest) {
  int i = 0;
  while ( * (sorg + i + 2) != '\0') {
    *(dest + i) = * (sorg + i + 2);
    i++;
  }
  *(dest + i) = '\0';
}
void lampeggio(char * scritta) {
  int i = 0;
  int x = 25, y = 415;
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
  while (!kbhit()) {
    if (i >= 16) i = 0;
    delay(60);
    setcolor(i);
    outtextxy(x, y, scritta);
    i++;
  }
  setcolor(BLUE);
  outtextxy(x, y, scritta);

}
void vittorie(double vittorie_1, double vittorie_2) {
  char buf[20];

  setcolor(RED);
  setfillstyle(SOLID_FILL, BLUE);
  bar(getmaxx() - 150, 100, getmaxx() - 10, 140);
  outtextxy(getmaxx() - 150, 105, "Vittorie");
  gcvt(vittorie_1, 5, buf); // conversione a stringa player_1
  outtextxy(getmaxx() - 30, 105, buf);

  setcolor(YELLOW);
  bar(getmaxx() - 150, 200, getmaxx() - 10, 240);
  outtextxy(getmaxx() - 150, 205, "Vittorie");
  gcvt(vittorie_2, 5, buf); // conversione a stringa player_2
  outtextxy(getmaxx() - 30, 205, buf);

}
int input_nome(char * p, char * app, int flag) {
  int t;
  register int i = 0;
  char nome[11];
  fflush(stdin);
  loadbmp("\\resources\\inputUsername.bmp");
  setvgapalette();
  visbmp();
  loadbmp("font.bmp");
  screen_write(p, 20, 10, 0);
  if (flag != 1) t = getch();
  else {
    screen_write("premi invio", 70, 20, 0);
    screen_write("per", 120, 65, 0);
    screen_write("continuare", 70, 100, 0);
    t = getch();
  }
  while ((t != 13) && (i != 9)) {
    if (t != 8) {
      nome[i] = t;
      nome[i + 1] = '\0';
      i++;
    } else {
      i--;
      nome[i] = ' ';
      nome[i + 1] = '\0';
    }
    screen_write(nome, 90, 99, 0);
    t = getch();
  }
  screen_write(nome, 90, 99, 0);
  strcpy(app, nome);
  app[0] = toupper(app[0]); // nomi con lettere maiuscole
  delay(500);
  elimina_bmp();
}
void titoli_coda(void) {
  PALETTE p[256];
  FILE * bmp;
  void * pu;
  void * ps;
  int cont, x, y, driver, mode = SVGA1024x768;
  int offsetx; // spostamento delle X
  int offsety; // spostamento delle Y
  unsigned char buf;
  if ((bmp = fopen("\\resources\\credits.bmp", "rb")) == NULL) {
    printf("%s non trovato !\n");
  }
  fread( & fheader, sizeof(struct header), 1, bmp);
  fread( & finfo, sizeof(struct info), 1, bmp);

  fseek(bmp, 54, 0);
  for (cont = 0; cont < 255; cont++) {
    p[cont].blue = p[cont + 1].blue = 0;
    p[cont].green = p[cont + 1].green = 0;
    p[cont].red = p[cont + 1].red = 0;
  }
  driver = installuserdriver("SVGA256", NULL);
  initgraph( & driver, & mode, "");
  SetPalette(p);
  for (cont = 0; cont < 256; cont++) {
    p[cont].blue = fgetc(bmp) >> 2;
    p[cont].green = fgetc(bmp) >> 2;
    p[cont].red = fgetc(bmp) >> 2;
    fgetc(bmp);
  }
  offsetx = (getmaxx() - finfo.width) / 2;
  offsety = (getmaxy() - finfo.height) / 2;

  pu = (void * ) farmalloc(imagesize(offsetx + 640, finfo.height + offsety + 8,
    offsetx + 726, offsety - 8));
  if (pu == NULL) exit(0);

  ps = (void * ) farmalloc(imagesize(offsetx + 640, finfo.height + offsety + 8,
    offsetx + 726, offsety - 8));
  if (ps == NULL) exit(0);
  getimage(offsetx + 640, finfo.height + offsety + 8, offsetx + 724, offsety - 8, ps);

  for (y = finfo.height; y > 0; y--) {
    for (x = 85; x < finfo.width + 85; x++) {
      buf = fgetc(bmp);
      putpixel(x + offsetx, y + offsety, buf);
    }
  }
  getimage(offsetx + 640, finfo.height + offsety + 8, offsetx + 724, offsety - 8, pu);
  putimage(offsetx + 640, offsety - 8, ps, COPY_PUT);
  putimage(offsetx, offsety - 8, pu, COPY_PUT);
  free(pu);
  free(ps);

  Fade_In2(p);
  getch();
  fclose(bmp);
  closegraph();
}