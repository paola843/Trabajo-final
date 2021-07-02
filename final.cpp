#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
#include <mmsystem.h>
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}

void dibujarLimite()
{
	for (int i = 2; i < 78;i++)
	{
		gotoxy(i, 3); printf("_", 205);
		gotoxy(i, 33); printf("_", 205);
	}
	for (int i = 4; i < 33; i++)
	{
		gotoxy(2, i); printf("|", 186);
		gotoxy(77, i); printf("|", 186);
	}
}

class NAVE
{
	int x, y;
	int corazones;
	int vidas;
public:
	NAVE();
	NAVE(int _x, int _y, int _corazones, int _vidas);
	void dibujar();
	void borrar();
	void mover();
	void dibujarCorazones();
	void perderVida();
	void disminuirCorazones(){ corazones--; }
	int X(){ return x; }
	int Y(){ return y; }
	int vidasDelJugador(){ return vidas; }

};
NAVE::NAVE(int _x, int _y, int _corazones,int _vidas)
{
	x = _x;
	y = _y;
	corazones = _corazones;
	vidas = _vidas;
}
void NAVE::dibujar()
{
	gotoxy(x, y); printf("  +", 30);
	gotoxy(x, y + 1); printf(" +++", 40, 207, 41);
    gotoxy(x, y + 2); printf("+++++", 30, 190, 190, 30);
}
void NAVE::borrar()
{
	gotoxy(x, y);     printf("         ");
	gotoxy(x, y + 1); printf("         ");
	gotoxy(x, y + 2); printf("         ");
}
void NAVE::mover()
{
	if (_kbhit())
	{
		char tecla = _getch();
		borrar();
		if (tecla == IZQUIERDA && x > 3)
		{
			x--;
		}
		else if (tecla == DERECHA && x + 6 < 77)
		{
			x++;
		}
		else if (tecla == ARRIBA && y > 4 )
		{
			y--;
		}
		else if (tecla == ABAJO && y + 3 < 33  )
		{
			y++;
		}
		dibujar();
		dibujarCorazones();
	}
}
void NAVE::dibujarCorazones()
{
	gotoxy(50, 2);
	printf("Vida: %d",vidas);
	gotoxy(64, 2);
	printf("Salud:");
	gotoxy(70, 2);
	printf("      ");
	for (int i = 0; i < corazones; i++)
	{
		gotoxy(70 + i, 2);
		printf("%c", 3);
	}
}
void NAVE::perderVida()
{
	if (corazones == 0)
	{
		borrar();
		gotoxy(x, y);     printf("   **   ");
		gotoxy(x, y + 1); printf("  ****  ");
		gotoxy(x, y + 2); printf("   **   ");
		Sleep(200);
		borrar();
		vidas--;
		corazones = 3;
		dibujarCorazones();
		dibujar();
		
	}
}
class Asteroide
{
	int x, y;
public:
	Asteroide(int _x, int _y);
	void dibujar();
	void mover();
	void colisionConLaNave(NAVE &nave);
	int X(){ return x; }
	int Y(){ return y; }
};
Asteroide::Asteroide(int _x, int _y)
{
	x = _x;
	y = _y;
}
void Asteroide::dibujar()
{
	gotoxy(x, y); printf("*", 184);
}
void Asteroide::mover()
{
	gotoxy(x, y); printf(" ");
	y++;
	if (y > 32)
	{
		x = (rand() % 71) + 4;
		y = 4;
	}
	dibujar();
}
void Asteroide::colisionConLaNave(NAVE &nave)
{
	if (x >= nave.X() && x < nave.X() + 6 && y >= nave.Y() && y <= nave.Y() + 2)
	{
		nave.disminuirCorazones();
		nave.borrar();
		nave.dibujar();
		nave.dibujarCorazones();
		x = (rand() % 71) + 4;
		y = 4;
	}
}
class Bala
{
	int x, y;
public:
	Bala(int _x, int _y);
	void mover();
	bool fuera();
	int X(){ return x; }
	int Y(){ return y; }
};
Bala::Bala(int _x, int _y)
{
	x = _x;
	y = _y;
}
void Bala::mover()
{
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("*");
	
}
bool Bala::fuera()
{
	if (y == 4) return true;
	return false;
	
}

int main()
{
	dibujarLimite();
	NAVE n(37,30,3,3);
	n.dibujar();
	n.dibujarCorazones();
	list<Asteroide*> A;
	list<Asteroide*>::iterator itA;
	int puntos = 0;
	for (int  i = 0; i < 5; i++)
	{
		A.push_back(new Asteroide(rand() % 75 + 3, rand() % 5 + 4));
	}
	list<Bala*> B;
	list<Bala*>::iterator iteracion;
	bool gameOver = false;
	
	while (!gameOver)
	{	
		gotoxy(4, 2); printf("Puntos: %d", puntos);
		if(_kbhit())
		{
			char tecla = _getch();
			if (tecla == 'x' || tecla == 'X')
			{
				B.push_back(new Bala(n.X() + 2, n.Y() - 1));
			}
		}

		for (iteracion = B.begin() ; iteracion != B.end() ;)
		{
			(*iteracion)->mover();
			if ((*iteracion)->fuera())
			{
				
				gotoxy((*iteracion)->X(), (*iteracion)->Y()); printf(" ");
				delete(*iteracion);
				iteracion = B.erase(iteracion);
			}
			else
			{
				iteracion++;
			}
		}
		for (itA = A.begin(); itA != A.end(); itA++)
		{
			(*itA)->mover();
			(*itA)->colisionConLaNave(n);
		}
		
		for (itA = A.begin(); itA != A.end();itA++)
		{
			for (iteracion = B.begin(); iteracion != B.end();)
			{
				if ((*itA)->X() == (*iteracion)->X() && ((*itA)->Y() + 1 == (*iteracion)->Y() || (*itA)->Y() == (*iteracion)->Y()))
				{
					gotoxy((*iteracion)->X(), (*iteracion)->Y()); printf(" ");
					delete(*iteracion);
					iteracion = B.erase(iteracion);				
					A.push_back(new Asteroide(rand() % 75 + 3, 4));
					gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
					delete(*itA);
					itA = A.erase(itA);
					puntos += 5;
				}
				else
				{
					iteracion++;
				}
			}
		}
		if (n.vidasDelJugador() <= 0)
		{
			gameOver = true;
			gotoxy(25, 18);
			printf("Game Over");
		}
		n.perderVida();
		n.mover();
		Sleep(30);
	}
	_getch();
	
	return 0;
}