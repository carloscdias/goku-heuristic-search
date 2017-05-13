#include "stdafx.h"
#include <windows.h> 

#define TAMANHO_MAPA 42 //Tamanho do mapa

struct Posicao2D{ //Estrutura para armazenar a posicao 2D do agente e destino
	int x;
	int y;
};

Posicao2D Agente;
Posicao2D Destino;

char Mapa[TAMANHO_MAPA][TAMANHO_MAPA];


/*Funcao para ler o mapa de um arquivo*/
void LoadMapa(char *filename)
{
    int c;
    int count_line = 0;
    int count_col = 0;
    FILE *file;
    file = fopen(filename, "r");
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            if (count_col == TAMANHO_MAPA)
            {
                count_line++;
                count_col = 0;
            }
            else
            {
                Mapa[count_col][count_line] = (char)c;
                count_col++;
            }
        }
        fclose(file);
    }
}


/*Funcao para mover o agente para cima uma vez*/
void MoverParaCima()
{
	if (Agente.y > 0)
		Agente.y--;
}


/*Funcao para mover o agente para baixo uma vez*/
void MoverParaBaixo()
{
	if (Agente.y < TAMANHO_MAPA)
		Agente.y++;
}


/*Funcao para mover o agente para direita uma vez*/
void MoverParaDireita()
{
	if (Agente.x < TAMANHO_MAPA)
		Agente.x++;
}


/*Funcao para mover o agente para esquerda uma vez*/
void MoverParaEsquerda()
{
	if (Agente.x > 0)
		Agente.x--;
}


/*Funcao usada apenas para alterar a cor de fundo do console*/
void SetColorBackground(short color){
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color){
		case 0:
			SetConsoleTextAttribute(hstdout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			break;		
        case 1:
            SetConsoleTextAttribute(hstdout, 47);
            break;       
        case 2:
            SetConsoleTextAttribute(hstdout, 111);
            break;        
        case 3:
            SetConsoleTextAttribute(hstdout, 159);
            break;        
        default:
            return;
    }
}


/*Funcao para desenhar o mapa*/
void DesenhaMap()
{
	for (int x = 0; x < TAMANHO_MAPA; x++)
	{
		for (int y = 0; y < TAMANHO_MAPA; y++)
		{	
			if (Mapa[y][x] == 'G') //Escolhe a cor do fundo com base no tipo do terreno		
				SetColorBackground(1);				
			else if (Mapa[y][x] == 'M')		
				SetColorBackground(2);		
			else if (Mapa[y][x] == 'A')	
				SetColorBackground(3);		
			else 
				SetColorBackground(0);
			
			if ((x == Agente.x)&&(y == Agente.y))
				printf("%c", 1);
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
	SetColorBackground(0);
}


/*Funcao para atualizar a posicao do agente*/
void AtualizaPosicaoAgente()
{
	////////AS PROXIMAS LINHAS SAO APENAS PARA ILUSTRAR A MOVIMENTACAO///////
	////////DO AGENTE, ELAS DEVEM SER REMOVIDAS.                      ///////	
	if (Agente.x < Destino.x)
		MoverParaDireita();
	if (Agente.x > Destino.x)
		MoverParaEsquerda();

	if (Agente.y < Destino.y)
		MoverParaBaixo();
	if (Agente.y > Destino.y)
		MoverParaCima();
	/////////////////////////////////////////////////////////////////////
}


/*Funcao principal*/
int main(int argc, _TCHAR* argv[])
{	
	LoadMapa("Mapa01.txt");
	
	Agente.x = 19; //Define a posicao inicial do agente
	Agente.y = 19;

	Destino.x = Agente.x; 
	Destino.y = Agente.y;	

	SetColorBackground(0);

	while (true)
	{
		DesenhaMap(); //Desenha o mapa
		
		if ((Agente.x == Destino.x)&&(Agente.y == Destino.y)) //Se o agente esta no seu destino solicita um novo destino
		{
			printf("Digite a coordenada de destino X: ");
			scanf("%d", &Destino.x);
			printf("Digite a coordenada de destino Y: ");
			scanf("%d", &Destino.y);
		}
			
		Sleep(500); //Um pequeno intervalo para podermos visualizar a movimentacao do agente

		AtualizaPosicaoAgente();

		system("cls"); //Limpa a tela
	}
	return 0;
}

