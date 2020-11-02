#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define MILLISECONDS 1000 // microseconds

void *gestiona_mem(int);
void lim_filas( int **, int);
void lim_columnas( int **, int);
int lee_inicio( int **, int);
int lee_fin( int **, int);
void msleep(int);
void imprime_matriz( int **, int, int, int);
void genera_muros ( int **, int, int, int);
void genera_camino ( int **, int);
void resuelve_lab( int **, int, int, int, int, int);
void genera_laberinto(int**,int,int,int);
void encabezado(void);
void bienvenida(void);
void despedida(void);


int main()
{
    int **Laberinto;
    int i, bandera = 0, op;
    int dim, inicio, fin;

    srand(time(NULL));
    
    bienvenida();
    do{
        msleep(2000);
        system("clear");
        encabezado();
        do{
            printf("Selecciona el modo de juego:\n\n");
            printf("1. Laberinto 12 x 12\n2. Laberinto n x n\n\n");
            printf("Opcion: ");
            scanf ("%d", &op);
            if (op != 1 && op != 2)
                printf("Error! Ingrese otra opcion.\n\n"); 
            msleep(2000);
       }while(op != 1 && op != 2);
       
       if (op == 1) {
            dim = 12;
            inicio = 1;
            fin = 12;
            dim += 2;
       }
       else{   
            printf ("\nIngresa la dimension del laberinto: \n");
            printf("Dimension: ");
            scanf ("%d", &dim);
            msleep(200);
            system("clear");
            dim += 2;
            inicio = lee_inicio(Laberinto, dim);
            fin = lee_fin(Laberinto, dim);
       }

       //Reserva memoria
       Laberinto = (int **) gestiona_mem(sizeof(int *) * dim);
       for ( i = 0; i < dim; i++)
            Laberinto[i] = (int *) gestiona_mem(sizeof(int)* dim);
    
       genera_laberinto(Laberinto,dim,inicio,fin);
       genera_muros(Laberinto, dim, inicio, fin);
       imprime_matriz(Laberinto, dim, inicio, 0);
       printf("\nResolviendo laberinto...\n");
       resuelve_lab(Laberinto, dim, inicio, 0, fin, 4);

       //Libera memoria
       for ( i = 0; i < dim; i++)
            free(Laberinto[i]);
       free(Laberinto);
       
       do {      
            printf("\nDesea jugar de nuevo? [1.Si | 2.No]: \n");
            printf("Opcion: ");
            scanf("%d",&bandera);
            if (bandera != 1 && bandera != 2)
                printf("Error! Ingrese otra opcion.\n\n");
       } while(bandera != 1 && bandera != 2);
    }while(bandera !=2);
    despedida();
  
    return 0;
}

void msleep(int time) {
    usleep(time * MILLISECONDS);
}

void *gestiona_mem(int elem)
{
	void *p;
	
	p = malloc(elem);
	if (p == NULL)
	{
		printf("No se pudo reservar memoria \n");
		exit(1);
	}
	return p;
}

void lim_filas( int **Matriz, int tam )
{
     int j;
     
     for (j = 0; j < tam; j++)
        Matriz[0][j] = 2;

        
    for (j = 0; j < tam; j++)
        Matriz[tam -1][j] = 2;
}
     
void lim_columnas( int **Matriz, int tam)
{
     int i;
      
    for (i = 0; i < tam; i++)
        Matriz[i][0] = 2;
        
    for (i = 0; i < tam; i++)
        Matriz[i][tam-1] = 2;
}

int lee_inicio( int **Matriz, int tam)
{
     int i, pI;
     
     do{
        encabezado();
        printf("Introduzca la posicion de entrada.\n\n");
        for (i = 0; i < tam -2; i++)
        {
            printf ("%2d ", i+1);
            printf("%c\n", 178);
        }
        printf("\n");
        printf("Posicion de entrada: ");
        scanf("%d",&pI);
        
        if (pI <= 0 || pI > tam-2)
            printf("Error! Ingrese otra posicion de inicio.\n");
        
        msleep(1000);
        fflush(stdin);
        system("clear");
     }while(pI <= 0 || pI > tam-2);
    
    return pI;
}
     
int lee_fin( int **Matriz, int tam)
{
     int i, pF;
     
     do{
        encabezado();
        printf("Introduzca la posicion de salida.\n\n");
        for (i = 0; i < tam -2; i++)
        {
            printf ("%2d ", i+1);
            printf("%c\n", 178);
        }
        printf("\n");
        printf("Posicion de salida: ");
        scanf("%d",&pF);
        
        if (pF<= 0 || pF > tam-2)
            printf("Error! Ingrese otra posicion de salida.\n");
        
        msleep(1000);
        fflush(stdin);
        system("cls");
     }while(pF <= 0 || pF > tam-2);
    
    return pF;
}

void imprime_matriz( int **Matriz, int tam, int x, int y)
{
     int i, j;
     
     encabezado();
     for ( i = 0; i < tam; i++)
     {
         printf("  ");
         for (j = 0; j < tam; j++)
         {
            if ( Matriz[i][j] == 1 || Matriz[i][j] == 2)
                printf("%c%c", '#','#'); // muro
            else
             	if( i == x && j == y)
                    printf("%c ", '@'); // puntero
                else
                 	if (Matriz[i][j] == 6 || Matriz[i][j] == 3)
    	            	printf ("%c%c", '~','~'); // trayectoria
                    else
                        printf ("  ");
		}
        printf("\n");
    }
}

void genera_muros ( int **Matriz, int tam, int start, int finish)
{
     int i, j;
         
     Matriz[start][1] = 5; 
               
     for(j = 1; j < tam-2; j++)
           for (i = 1; i < tam-1; i++)
           {
                if (Matriz[i][j] != 5) 
                    Matriz[i][j] = rand () % 2;
                else
                    Matriz[i][j] = 0;
           }
           
}

void genera_camino (  int **Matriz, int tam)
{
	int i, j, bandera = 1, bandera2 = 1, var;
	
	for(j = 1; j < tam-3; j++)
	{
		bandera = 1;
		bandera2 = 1;
		for( i = 1; i <= tam-2 && bandera2 == 1; i++)
			if(Matriz[i][j] == 5)
			{
				do{
					var = 1 + rand() % 3;
					switch(var){
						case 1: 
							if( i-1 >= 1)
								bandera = 0;
							break;
						case 3:
							if( i+1 <= tam-3)
								bandera = 0;
							break;
					}
				}while(bandera == 1);
				switch(var){
					case 1:
						Matriz[i-1][j+1] = 5;
						Matriz[i-1][j] = 5;
						break;
					case 2:
						Matriz[i][j+1] = 5;
						break;
					case 3:
						Matriz[i+1][j+1] = 5;
						Matriz[i+1][j] = 5;
						bandera2 = 0;
						break;
				}
			}
	}			
}

void resuelve_lab( int **Matriz, int tam, int i, int j, int finish, int orientacion)
{
     if (i == finish && j == tam-1)
     {
        printf("\r                            ");
        printf("\rBIEN HECHO CAMPEON!\nJUEGO TERMINADO\n");
     }
     else
     {
         switch(orientacion){
            case 1:
                if(Matriz[i][j+1] == 1 || Matriz[i][j+1] == 2)
                    if(Matriz[i-1][j] == 0 || Matriz[i-1][j] == 6)
                    {
                        Matriz[i-1][j] = 6;
                        i--;
                    }
                    else
                    {
                        orientacion = 2;
                    }
                else
                {
                    Matriz[i][j+1] = 6;
                    j++;
                    orientacion = 4;
                }
                break;
            case 2:
                if (Matriz[i-1][j] == 1 || Matriz[i-1][j] == 2)
                    if(Matriz[i][j-1] == 0 || Matriz[i][j-1] == 6)
                    {
                        Matriz[i][j-1] = 6;
                        j--;
                    }
                    else
                    {
                        orientacion = 3;
                    }
                else
                {
                    Matriz[i-1][j] = 6;
                    i--;
                    orientacion = 1;
                }
                break; 
            case 3:
                if (Matriz[i][j-1] == 1 || Matriz[i][j-1] == 2)
                    if(Matriz[i+1][j] == 0 || Matriz[i+1][j] == 6)
                    {
                        Matriz[i+1][j] = 6;
                        i++;
                    }
                    else
                    {
                        orientacion = 4;
                    }
                else
                {
                    Matriz[i][j-1] = 6;
                    j--;
                    orientacion = 2;
                }
                break;    
                
            case 4:
                if (Matriz[i+1][j] == 1 || Matriz[i+1][j] == 2)
                    if(Matriz[i][j+1] == 0 || Matriz[i][j+1] == 6)
                    {
                        Matriz[i][j+1] = 6;
                        j++;
                    }
                    else
                    {
                        orientacion = 1;
                    }
                else
                {
                    Matriz[i+1][j] = 6;
                    i++;
                    orientacion = 3;
                }
                break;
         }

         msleep(200);
         system("clear");
         imprime_matriz(Matriz, tam, i, j);
         printf("\nResolviendo laberinto... ");
         resuelve_lab(Matriz, tam, i, j, finish, orientacion);
     }
}

void genera_laberinto(int**Matriz, int tam, int start, int finish)
{
     int i,j;
     
     //Inicializa la matriz en cero
     for ( i = 0; i < tam; i++)
        for (j = 0; j < tam; j++)
            Matriz[i][j] = 0;
    
    //Crea limites
    lim_filas(Matriz, tam);
    lim_columnas(Matriz, tam);
    
    Matriz[start][1] = 5;
    genera_camino (Matriz, tam);
    
    //Marca posicion inicial y final
    Matriz[start][0] = 3;
    Matriz[finish][tam-1] = 4;
     
}

void encabezado(void)
{ 
    printf("**************************************************************************\n");
	printf("*                                                                        *\n");
	printf("*  *       ******  *****   *****  *****   *****  *    * *******  ******  *\n");
	printf("*  *       *    *  *    *  *      *    *    *    **   *    *     *    *  *\n");
	printf("*  *       ******  *****   *****  *  *      *    * *  *    *     *    *  *\n");
    printf("*  *       *    *  *    *  *      *   *     *    *  * *    *     *    *  *\n");
	printf("*  ******  *    *  *****   *****  *    *  *****  *    *    *     ******  *\n");
    printf("*                                                                        *\n");
	printf("**************************************************************************\n\n");
}

void bienvenida(void)
{

    printf("\n\n\n\n");
	printf("                                                           *\n");
	printf("                                                           * *\n");
    printf("************************************************************   *\n");	                                                                               
    printf("*                                                                *\n");
	printf("* ****  *** **** *   * *       * **** *   * ***  ****  *****       *\n");
	printf("* *   *  *  *    **  *  *     *  *    **  *  *   *   * *   *         *\n"); 
	printf("* ****   *  **** * * *   *   *   **** * * *  *   *   * *   *  	      *\n");
    printf("* *   *  *  *    *  **    * *    *    *  **  *   *   * *   *         *\n");
	printf("* ****  *** **** *   *     *     **** *   * ***  ****  *****       *\n");
    printf("*                                                                *\n");
	printf("************************************************************   *\n");
	printf("                                                           * *\n");
	printf("                                                           *\n");
     
}

void despedida(void)
{
    printf("\n\n\n");
    printf("            *                                           \n");
    printf("          * *                                           \n");
    printf("        *   ********************************************\n");
	printf("      *                                                *\n");
	printf("    *           ******  *****   *****  *****   *****   *\n");
	printf("  *             *    *  *    *    *    *   *   *       *\n");
	printf(" *              ******  *    *    *    *   *   *****   *\n");
    printf("  *             *    *  *    *    *    *   *       *   *\n");
	printf("    *           *    *  *****   *****  *****   *****   *\n");
    printf("      *                                                *\n");
	printf("        *   ********************************************\n");
    printf("          * *                                           \n");
    printf("            *                                           \n");
}
