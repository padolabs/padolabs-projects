#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void inv();
int determinante();
void result();
void input(char y[], int qual);
int stoi(char *x);
int validaEquacao2(char x[]);
void logeq2();

static int pos[2], posn = 0, I[2][2], U[2], h = 0;
static long double x[2][2], a[1][2];

int equacao2(){

    system("clear");
    char string[100];

    //inserindo as equacoes;
    printf("Como usar: sinal(+-)número(0-999).i1sinal(+-)número(0-999).i2=sinal(+-)número(0-999)\n");
    printf("Exemplo: +23.i1-06.i2=+00\n\n");

    for (int i = 0; i < 2; i++)
    {
        int valida = 0;
        while (valida == 0)
        {
            printf("Informe a equacao - %d: ", i+1);
            fgets(string, sizeof(string)-2, stdin);
            
            valida = 1; //validaEquacao2(string);
        }
        input(string, i);
    
    }
 

    //Calculo de determinante;
    determinante();

    //Calcula Matriz inversa;
    inv();

    printf("\nResultado:\n");
    result();   
    logeq2();
}

void logeq2(){

    FILE *pfile;

    pfile = fopen("Log_eq2.txt", "a");
    if (pfile == NULL)
    {
        perror("Erro");
        clearerr(pfile);
    }

    //matriz Correntes;
    fprintf(pfile, "\n Matriz Corrente\n");
    for (int x = 0; x < 2; x++)
    {
        for (int j = 0; j < 2; j++)
        {
            fprintf(pfile, "\t\t\t\t%d", I[x][j]);
        }
        fprintf(pfile, "\n");
    }
    fprintf(pfile, "\n");

    //matriz Tensoes;
    fprintf(pfile, "\n Matriz Tensão\n");
    for (int x = 0; x < 2; x++)
    {
        fprintf(pfile, "\t\t\t\t%d", U[x]);
        fprintf(pfile, "\n");
    } 

    //determinante;
    fprintf(pfile, "\n Determinante: %d\n", determinante());

    //Inversa matriz Correntes;
    fprintf(pfile, "\n Matriz Inversa\n");
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            fprintf(pfile, "\t\t\t\t%Lf", x[i][j]);
        }
        fprintf(pfile, "\n");
    }
    
    fprintf(pfile, "\n");

    //resultado;
    for (int i = 0; i < 2; i++)
    {
        fprintf(pfile," Corrente %d = %Lf\n", i+1, a[0][i]);
    }

    fprintf(pfile, "-------------------------------------");
    fprintf(pfile, "\n");
    fclose(pfile);   

}

int validaEquacao2(char x[]){

    int tamanho = 0, ponto = 0, sinal = 0;
    tamanho = strlen(x);
    
    for (int i = 0; i < tamanho; i++)
    {
        //conta a quantidade de pontos;
        if (x[i] == '.' && x[i+1] != '.' && x[i-1] != '.')
        {
            ponto++;
        }     

        //conta a quantidade dos sinais;
        if (x[i] == '+' || x[i] == '-')
        {
            sinal++;
        }
       

        //procura por espaços;
        if (x[i] == ' ')
        {
            printf("Equação invalida, não coloque espaço!\n");
            return 0;
        }
        
           
    }

    //verifica se a quantidade de ponto achado é igual a 3;
    if (ponto != 2)
    {
        printf("Equação invalida!\n");
        return 0;
    }

    //verifica se a quantidade de sinal achado é igual a 4;
    if (sinal != 2)
    {
         printf("Equação invalida, não esqueça dos sinais(+-) antes de cada numero!\n");
        return 0;
    }    
    
    //verifica se o codigo é muito pequeno ou muito grande;
    if (tamanho <= 14 || tamanho >=  21)
    {
        printf("Equação invalida, divergencia no tamanho!\n");
        return 0;
    }
    return 1;

}


//calcula a determinante;
int determinante()
{  
   return (I[1][1]*I[0][0])-(I[1][0]*I[0][1]);
}

//inverte a matriz;
void inv()
{
    float temp[2][2], det = determinante();
    //etapa 1
    for (int k = 0; k < 2; k++)
    {
        for (int j = 0; j < 2; j++)
        {
            temp[k][j] = I[k][j]/det;
        }
    }
    //etapa 2:
    x[0][0] = temp[1][1];
    x[1][1] = temp[0][0];
    x[0][1] = (temp[0][1]*-1);
    x[1][0] = (temp[1][0]*-1);
}

void result()
{
    a[0][0] = (x[0][0] * U[0]) + (x[0][1]*U[1]);
    a[0][1] = (x[1][0] * U[0]) + (x[1][1]*U[1]);  

    for (int i = 0; i < 2; i++)
    {
        printf("\tCorrente %d = %Lf\n", i+1, a[0][i]);
    }

    printf("\n");
}

void input(char y[], int qual)
{
    //contador que define I:1,2,3 na matriz;
    int contador = 0, semsinal = 0;

    //roda por toda a string
    for (int i = 0; i < strlen(y); i++)
    {
        
        //procura por .
        if(y[i] == '.')
        {
            
            pos[0] = i;//declara onde achou;
            contador++;//contador que define I:1,2,3 na matriz;

            //conta quantas cassas até o sinal;
            for (int j = 0; j < 5; j++)
            {
                if (i-j == 0)
                {
                    pos[1] = j;
                    semsinal = 1;
                    break;

                }else if (y[i-j] == '+' || y[i-j] == '-')
                {   
                    //define quantidade de casas;
                    pos[1] = j;
                }
            }

            posn = pos[0] - pos[1];//define a posição incial do que achou;

            //printf("\npos[0] = %d;\n pos[1] = %d\n posf = %d\n x = %d\n", pos[0], pos[1], posn, strlen(y)-posn);

            //inicializa variavel temporaria que armazena char que será transformado em int;
            char temp[pos[1]+1];

            if (semsinal == 1)
            {
                temp[0] = '+';
                int z = 0;

                //define com as posições que achou;
                for (int k = 1; k <= pos[1]; k++)
                {
                    temp[k] = y[k-1];
                    //printf("\n\ttemp[%d] = %c", k-posn, y[k]);
                }

            }else 
            {
                //define com as posições que achou;
                for (int k = posn; k <= posn + pos[1]; k++)
                {
                temp[k-posn] = y[k];
                //printf("\n\ttemp[%d] = %c", k-posn, y[k]);
                 }

            }
            
            
            
            temp[pos[1]] = '\0';
                 
            //transforma de sting para int;
            I[qual][contador-1] = stoi(temp);

            //printf("\nqual = %d, contador = %d", qual, contador);
            //printf("\n I[%d][%d] = %d\n", qual, contador-1, stoi(temp));
        }

        //procura por '=';
        if (y[i] == '=')
        { 
            //h recebe a posição onde achou o '=' + 1;
            h = i+1; 
            
            //se não começou com + ou -;
            if (y[h] != '+' && y[h] != '-'){
                
                //define o inicio em '=';
                h = i;

            }

            //marcador guardara o tamanho do char que chegou
            int marcador = (strlen(y)-h)-1;

            //inicializa variavel que armazena char que será transformado em int;
            char x[marcador];
            
            //printa quantas posiçoes foram criadas no vetor x;
            //printf("\nX tem[%d]\n", marcador);

            //define x;
            for (int j = 0; j < marcador; j++)
            {
                //printf("\n temp[%d] = %c\n", j, y[i+j]);
                x[j] = y[h+j];

            }

            //redefine o final da string;
            x[marcador] = '\0';

            //chama stoi(x), para transformar em int[];
            U[qual] = stoi(x);
        }
        
    }    

}
