#include "biblioteca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AbreLog(){

    FILE *pfile;
    FILE *pfile2;
    FILE *pfile3;

    pfile = fopen("Log_eq3.txt", "w");   
    pfile2 = fopen("Log_eq2.txt", "w");
    pfile3 = fopen("Log_Alg.txt", "w");

    if (pfile == NULL)
    {
        perror("Erro");
        clearerr(pfile);
    }

    if (pfile2 == NULL)
    {
        perror("Erro");
        clearerr(pfile);
    }
    
    fprintf(pfile, "--------------------Histórico---------------------");
    fprintf(pfile2, "--------------------Histórico---------------------");
    fprintf(pfile3, "--------------------Histórico---------------------");

    fclose(pfile);
    fclose(pfile2);
    fclose(pfile3);    

}

//concatena int de 3 digitos;
int concatena(int c, int d, int u){

    int r;
    r = c * 100; //centena;
    r = r + (d*10);//dezena;
    r = r + u;//unidade;
    return r;
}

// Transforma de string para int "char x[], é um ponteiro";
int stoi(char *x)
{
        
    //inicializa variaveis;
    int teste[strlen(x)], menos, z = 0, sinal = 1;

    //verifica se o numero é negativo;
    if(x[0] == '-')
    {
        z = 1;//se for negativo z recebe 1;
    }

    //for pulando a primeira posição do vetor passando char x para int teste;
    for (int i = sinal; i < strlen(x); i++)
    {
        teste[i] = x[i]-48;//faz a conversão de cada posição do vetor para int(faz se -48 por causa da tabela ascii);
        //printf("\n\t\tteste[%d] = %d\n", i, teste[i]);
    }

    if(strlen(x) == 2)//se x tiver 2, ou seja ser apenas 1 numero;
    {
        menos = concatena(0/*centena*/, 0/*dezena*/, teste[1]/*unidade*/);
    }
    
    else if (strlen(x) == 3)//se x tiver 3, ou seja ser 2 numero;
    {
        menos = concatena(0, teste[1], teste[2]);
    }
    
    else if (strlen(x) == 4)//se x tiver 4, ou seja ser 3 numero;
    {       
        menos = concatena(teste[1], teste[2], teste[3]);
    }
    
    if(z == 1)// verifica se z é igual a 1; sendo multiplica por -1, para inverter o sinal;
    {
        //printf("aqui o menos = %d, teste[0] = %d", menos, teste[0]);
        menos = menos * -1;
        return menos;//retorna o numero negativo;
    }return menos;//retorna o numero positivo;
    
}