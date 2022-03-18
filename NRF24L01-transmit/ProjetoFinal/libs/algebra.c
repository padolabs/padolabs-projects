#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "biblioteca.h"

#define RED       "\033[31m"
#define YELLOW    "\033[33m"
#define BLUE      "\033[34m"
#define RESET     "\033[34m"


char remove_espaco(char expressao[], int tamanho_expressao);

void algebra()
{
    FILE *pfile;

    pfile = fopen("Log_Alg.txt", "a");
    if (pfile == NULL)
    {
        perror("Erro");
        clearerr(pfile);
    }

    char expressao[999] = {0}, letras_maiusculas[999] = {0};
    int contador=0, flag=0, tamanho_expressao=0;; 
    
    //#####_PEDE_E_VERIFICA_SE_EXPRESSAO_EH_CORRETA_#####
    do
    {
        flag=0;//flag recebe 0 pois assim que o loop for concluido esse valor mudarah ou nao

        //#####_ARMAZENA_EXPRESSAO_#####
        printf("\n============================================================\n");
        printf("|"YELLOW"-----------------Digite a espressão:----------------------"RESET"|\n");
        printf("============================================================\n");
    
        fgets(expressao, sizeof(expressao)-2, stdin);

        tamanho_expressao = strlen(expressao);
        
        //#####_TRANFORMA_MAISCULAS_#####
        for(int i = 0; i < tamanho_expressao-1; i++)
        {
            expressao[i]=toupper(expressao[i]);//tranforma a expressao toda em maiusculas, comando de <ctype.h>
        }
        //printf("%s", expressao);

        //#####_VERIFICA_CARACTERER_APOS_"!"_#####
        if(expressao[0] == '!')
            if(expressao[1] < 'A' || expressao[1] > 'Z')
                if(expressao[1]!='!')
                    if(expressao[1]!='(')
                        {
                            if(flag == 0)
                            {
                                printf("\nExpressao invalida");
                                printf("\nInsira uma letra ou sinal \"!\" seguido de \"(\" depos de \"!\"");
                            }
                            flag=1;
                        }

        //#####_VERIFICA_PRIMEIRO_CARACTERER_#####
        if(expressao[0]!='!')
            if(expressao[0] < 'A' || expressao[0] > 'Z')
                if(expressao[0] != '(')
                {
                    if(flag == 0)
                    {
                        printf("\nExpressao invalida");
                        printf("\nO pirmeiro caractere deve ser uma letra ou sinais \"!\" e \"(\"");
                    }
                    flag=1;
                }

        for (int i = 0; i < tamanho_expressao-1; i++)//roda ate i ser menor que "\n" da "expressao"
        {
            //#####_VERIFICA_CARACTERER_ANTERIOR_"("_#####
            if(i!=0)
                if(expressao[i] == '(')
                    if(expressao[i-1] != '!')
                        if(expressao[i-1] != '*')
                            if(expressao[i-1] != '+')
                                if(expressao[i-1] != '(')
                            {
                                if(flag == 0)
                                {
                                    printf("\nExpressao invalida");
                                    printf("\nInsira um sinal \"!\", \"*\", \"+\" antes de \"(\"");
                                }
                                flag=1;
                            }

            //#####_VERIFICA_CARACTERER_POSTERIOR_")"_#####
            if(i!=0)
                if(expressao[i] == ')')
                    if(expressao[i+1] != 'NULL')
                        if(expressao[i+1] != '\n')
                            if(expressao[i+1] != '*')
                                if(expressao[i+1] != '+')
                                    if(expressao[i+1] != ')')
                                        {
                                            if(flag == 0)
                                            {
                                                printf("\nExpressao invalida");
                                                printf("\nInsira um sinal \"!\", \"*\", \"+\" depois de \")\"");
                                            }
                                            flag=1;
                                        }


            //#####_VERIFICA_LETRAS_DUPLICADAS_SEQUENCIA_#####        
            if(expressao[i]>='A' && expressao[i] <='Z')
                if(expressao[i+1]>='A' && expressao[i+1] <='Z')
                    {
                        if(flag == 0)
                        {
                            printf("\nExpressao invalida");
                            printf("\nInsira um sinal antes de uma letra");
                        }
                        flag=1;
                    }
                    
            //#####_VERIFICA_CARACTERER_ANTERIOR_"+"_"*"_#####        
            if(expressao[i] == '+' || expressao[i] == '*')
                if(expressao[i-1] < 'A' || expressao[i-1] > 'Z')
                        if(expressao[i+1] != '(')
                            if(expressao[i-1] != ')')
                            {
                                if(flag == 0)
                                {
                                    printf("\nExpressao invalida");
                                    printf("\nInsira uma letra, ou sinais \"(\" e \")\" antes de \"*\" ou \"+\"");
                                }
                                flag=1;
                            }

            //#####_VERIFICA_CARACTERER_POSTERIOR_"!"_#####
            if(expressao[i]=='!')
                if(expressao[i+1] < 'A' || expressao[i+1] > 'Z')
                    if(expressao[i+1]!='!')
                        if(expressao[i+1]!='(')
                        {
                            if(flag == 0)
                            {
                                printf("\nExpressao invalida");
                                printf("\nInsira uma letra ou o sinal \"!\" seguido de \"(\" depois de \"!\"");
                            }
                            flag=1;
                        }

            //#####_VERIFICA_CARACTERER_ANTERIOR_"!"_#####
            if(i!=0)
                if(expressao[i]=='!')
                    if(expressao[i-1] != '*')
                        if(expressao[i-1] != '+')
                            if(expressao[i-1] != '!')
                                if(expressao[i-1] != '(')
                                 {
                                    if(flag == 0)
                                    {
                                        printf("\nExpressao invalida");
                                        printf("\nInsira uma letra ou os sinais \"!\",\"*\",\"+\",\"(\" antes de \"!\"");
                                    }
                                    flag=1;
                                }

            //#####_VERIFICA_EXIXTENCIA_CARACTERES_NAO_PERMITIDOS_#####
            if(expressao[i] < 'A' || expressao[i] > 'Z')
                if(expressao[i] != '!')
                    if(expressao[i] != '*')
                        if(expressao[i] != '+')
                            if(expressao[i] != '(')
                                if(expressao[i] != ')')
                                {
                                    if(flag == 0)
                                    {
                                        printf("\nExpressao invalida");
                                        printf("\nO programa aceita somente letras e os caracteres \"!\",\"*\",\"+\",\"(\"");
                                    }
                                    flag=1;
                                }
        }

        //#####_VERIFICACAO_PARENTESES_#####
        int flag_2=0, inicio_parenteses=0, final_parenteses=0;
        for (int i = 0; i < tamanho_expressao-1; i++)//roda ate i ser menor que "\n" da "expressao"
        {
            //#####_VERIFICACAO_PARENTESES_FECHADOS_SEM_ABRIR_#####
            if(expressao[i] == ')')
            {
                inicio_parenteses=i;
                for (int j = inicio_parenteses; j >= 0; j--)
                {
                    if(expressao[j] == '(')
                    {
                        flag_2=1;
                        break;
                    }
                }
                if(flag_2 == 0)
                {
                    if(flag == 0)
                    {
                        printf("\nExpressao invalida");
                        printf("\nInsira corretamente os parenteses, sendo o primeiro \"(\" para abrir e o segundo \")\" para fechar a conta dos termos");
                    }
                    flag=1;
                }
            }

            //#####_VERIFICACAO_PARENTESES_ABERTOS_SEM_FECHAR_#####
            flag_2=0;
            contador=tamanho_expressao-2;//contador recebe a quantidade de posicoes de "expressao" exceto "\n" 
            if(expressao[i] == '(')
            {
                for (int j = contador; j >=0; j--)
                {
                    if(expressao[j]==')')
                    {
                        flag_2=1;
                        contador--;
                        final_parenteses=j;
                        break;
                    }
                }
                if(flag_2 == 0)
                {
                    if(flag == 0)
                    {
                        printf("\nExpressao invalida");
                        printf("\nInsira corretamente os parenteses, sendo o primeiro \"(\" para abrir e o segundo \")\" para fechar a conta dos termos");
                    }
                    flag=1;
                }
            }
        }
       
    } while (flag == 1);
    
    //#####_SEPARA_LETRAS_#####
    contador=0;
    //separa somente as letras da expressao e armazena em "letras"
    for(int i = 0; i < tamanho_expressao; i++)
    {
        if(expressao[i]>='A' && expressao[i]<='Z') //aceita somente letras para armazenar no vetor
        {
            letras_maiusculas[contador]=expressao[i];
            contador++;
        }
    } 
    
    fprintf(pfile, "\nLetras expressão: %s \n", letras_maiusculas);

    //#####_REMOVE_ELEMENTOS_DUPLICADOS_#####
    int tamanho_letras_maiusculas = strlen(letras_maiusculas), comparador = 0;
    char letras_sem_repetir[tamanho_letras_maiusculas];
    contador = 0;
    for(int posicao = 0; posicao <= tamanho_letras_maiusculas; posicao++)
    {
        //roda somente no segundo loop do primeiro for
        for(comparador = 0; comparador < contador; comparador++)//se comparador=0 e contador=1 entao incrementa comparador, porem nao roda o for
        {                                                       
            //compara todas as "letras_sem_repetir" com a proxima letra que ainda nao foi copiada
            if(letras_sem_repetir[comparador] == letras_maiusculas[posicao])
                break;
        }
            if(comparador == contador)
            {
                letras_sem_repetir[contador] = letras_maiusculas[posicao];
                contador++;
            }
    }
    fprintf(pfile, "Letras sem repetir: %s\n", letras_sem_repetir); 

    //#####_ARMAZENA_VALOR_LETRAS_#####
    //aceita somente 0 e 1 para o valor das letras
    int tamanho_valor_letras=strlen(letras_sem_repetir), tamanho_temp=0;
    char valor_letras[tamanho_valor_letras];
    flag=0;
    for (int i = 0; i < tamanho_valor_letras; i++)
    {
        do
        {
            if(flag==1)
               printf("Erro, insira 0 ou 1 para %c\n", letras_sem_repetir[i]);
            
            flag=1;
            printf("Insira o valor de %c: ", letras_sem_repetir[i]);
            setbuf(stdin, 0);
            scanf(" %s", &valor_letras[i]);
            printf("\n");
            tamanho_temp=strlen(valor_letras);
            if(valor_letras[i]=='1' && tamanho_temp==(1+i) || valor_letras[i]=='0' && tamanho_temp==(1+i))
                flag=0;

        }while(flag==1);
        flag=0;
    }
    for (int i = 0; i < tamanho_valor_letras; i++)//printa o valor das letras na tela
    {
       fprintf(pfile, "%c = %c\n",letras_sem_repetir[i], valor_letras[i]);
    }

    //#####_SUBSTITUI_LETRAS_POR_VALORES_#####
    for (int i = 0; i < tamanho_expressao; i++)
    {
        for (int j = 0; j < tamanho_expressao; j++)
        {
            if(expressao[i] == letras_sem_repetir[j])
            {
                if(valor_letras[j] == '1')
                {
                    expressao[i] = '1';
                    break;
                }
                else
                {
                    expressao[i] = '0';
                    break;
                }
            }
        }
    }
    fprintf(pfile,"expressão com valores: %s",expressao);
    //#####_RESOLUCAO_PARENTESES_INTERNOS_#####
    //resolve primeiro o parentese mais interno da expressao, depois os posteriores a este ate nao haver mais nenhum
    int comeco=0, final=0;
    for (int i = tamanho_expressao; i >= 0; i--)
    {
        //#####_PROCURA_PRIMEIRO_PARENTESE_#####
        if(expressao[i]=='(')
        {
            expressao[i]=' ';
            comeco = i;

            for (int j = comeco; j <= tamanho_expressao; j++)
            {
                if(expressao[j]==')')
                {
                    final=j;
                    expressao[j]=' ';
                    break;
                }   
            }
            fprintf(pfile, "\n%s",expressao);
            remove_espaco(&expressao, tamanho_expressao);

            //#####_RESOLVE_PORTA_NOT_DENTRO_PARENTESE_#####
            contador=0;
            for (int j = final; j >= comeco ; j--)
            {
                if(expressao[j] == '!')
                { 
                    if(expressao[j+1] == '1')
                        expressao[j] = '0';
                    else
                        expressao[j] = '1';

                    expressao[j+1] = ' ';
                    if(contador == 0)
                    {
                        final-=2;
                        contador++;
                    }
                    else
                        final-=1; 
                    remove_espaco(&expressao, tamanho_expressao);
                    fprintf(pfile, "%s",expressao);
                    j=final;
                } 
            }
            //#####_RESOLVE_PORTA_AND_DENTRO_PARENTESE_#####
            for (int j = comeco+1; j < final; j++)
            {
                if(expressao[j] == '*')
                {
                    expressao[j-1] = expressao[j+1]&expressao[j-1];
                    expressao[j] = ' ';
                    expressao[j+1] = ' ';
                    final-=2;
                    remove_espaco(&expressao, tamanho_expressao);
                    fprintf(pfile, "\n%s",expressao);
                    j=comeco;
                }
            }
            
            //#####_RESOLVE_PORTA_OR_DENTRO_PARENTESE_#####
            for (int j = comeco+1; j < final; j++)
            {
                if(expressao[j] == '+')
                {
                    expressao[j-1] = expressao[j+1]|expressao[j-1];
                    expressao[j] = ' ';
                    expressao[j+1] = ' ';
                    final-=2;
                    remove_espaco(&expressao, tamanho_expressao);
                    fprintf(pfile, "\n%s\n",expressao);
                    j=comeco;
                }
            }
            i=tamanho_expressao;
        }
    }

    //#####_RESOLVE_PORTA_NOT_FORA_PARENTESES_#####
    for (int i = tamanho_expressao; i >= 0; i--)
    {
        if(expressao[i] == '!')
        {
            if(expressao[i+1] == '1')
                expressao[i] = '0';
            else
                expressao[i] = '1';

            expressao[i+1] = ' ';
            remove_espaco(&expressao, tamanho_expressao);
            fprintf(pfile, "\n%s",expressao);
            i=tamanho_expressao;
        }
    }

    //#####_RESOLVE_PORTA_AND_FORA_PARENTESES_#####
    for (int i = 0; i < tamanho_expressao; i++)
    {
        if(expressao[i] == '*')
        {
            expressao[i-1] = expressao[i+1]&expressao[i-1];
            expressao[i] = ' ';
            expressao[i+1] = ' ';
            remove_espaco(&expressao, tamanho_expressao);
            fprintf(pfile, "\n%s",expressao);
            i=0;
        }
    }

    //#####_RESOLVE_PORTA_OR_FORA_PARENTESES_#####
    for (int i = 0; i < tamanho_expressao; i++)
    {
        if(expressao[i] == '+')
        {
            expressao[i-1] = expressao[i+1]|expressao[i-1];
            expressao[i] = ' ';
            expressao[i+1] = ' ';
            remove_espaco(&expressao, tamanho_expressao);
            fprintf(pfile, "\n%s",expressao);
            i=0;
        }
    }
    remove_espaco(&expressao, tamanho_expressao); 
    printf("\nResultado = ");
    if(expressao[0] == '1')
    printf("%c - (Verdadeiro)/(Ligado)",expressao[0]);
    else
    printf("%c (Falso)/(Desligado)",expressao[0]);
    printf("\n\n\n\n");

    fprintf(pfile, "\nResultado = ");
    if(expressao[0] == '1')
    fprintf(pfile, "%c - (Verdadeiro)/(Ligado)\n",expressao[0]);
    else
    fprintf(pfile, "%c (Falso)/(Desligado)\n",expressao[0]);
    fprintf(pfile, "--------------------------------------------------");
    fclose(pfile);

}

//#####_FUNCAO_REMOVE_ESPACO_EXPRESSAO_#####
char remove_espaco(char expressao[], int tamanho_expressao)
{
    for(int i = 0; i < tamanho_expressao; i++)
        {  
            if(expressao[i] == ' ')
            {  
                for(int j = i; j < tamanho_expressao; j++)  
                {  
                    expressao[j] = expressao[j+1];
                    i=0;  
                }  
            tamanho_expressao--;
            }  
        }
    return expressao;
}

//TODO = fazer os printf dos erros do usuario para que ele saiba corrigir, FINALIZADO
//TODO = fazer printar na tela todo o passo a passo do programa, FINALIZADO
//TODO = excluir comentarios inuteis e deixar somente aqueles que realmente interessam, FINALIZADO