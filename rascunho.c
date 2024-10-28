#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX 100

//Pega a quantidade de divisores
int divisor(int num, int div)
{
    if (div == num)
    {
        return 1;
    }

    if (num % div == 0)
    {
        return 1 + divisor(num,div+1);
    }

    return divisor(num, div+1);
}


int main()
{   
    char letrasTres[MAX];
    gets(letrasTres);

    //contador para saber o tamanho da palavra digitada pelo usuario
    int cont = 0;
    
    while(letrasTres[cont] != '\0' && letrasTres[cont] != '\n')
    {
        cont++;
    }

    //vetor que ira transformar cada letra em seu respectivo numero tabela ASCII
    int n[cont];
    printf("%d\n", cont);
   
    for (int i = 0; i < cont; i++)
    {
        //convertendo tabela ascii
        n[i] = (int)letrasTres[i];
        printf("Tabela ASC[%d]: %d\n",i,n[i]);
    }

    //p e q devem ser numeros primos (grandes) que ao serem dividos por 6 deixam resto 5;
    //dps devo calcular totiente de Euler em (p-1)(q-1) -- > quantidade de divisores
    //lambda e totiente de Euler devem ser primos entre si.
   
    long long p = 227;
    long long q = 229;
    
    //modular
    long long x = p*q;
    
    //descobrir chave privada
    long long chave = (p-1)*(q-1);
    printf("chave: %lld\n", chave);
    
    //expoente
    printf("Totiente =  \n%d\n", divisor(chave,1));
 
    
    //lambda == e
    int lambida;
    scanf("%d", &lambida);

    //vetor para pegar a menssagem codificada
    int codificada[MAX];
    
    //codificacao
    printf("Mensagem codificada: ");
    for (int i = 0; i < cont; i++)
    {
        long long v = 1;
        for (int j = 0; j < lambida; j++)
        {
            //exponenciacao modular
            v = (v * n[i]) % x;
        }
        codificada[i] = v;
        printf("%d ", codificada[i]);
    }

    //vetor para pegar menssagem decodificada
    int decodificar[MAX];
    
    //achar o inverso modulo (p-1)*(q-1)
    long long w = 0;
    
    for (int i = 1; i < chave; i++)
    {
        if ((i*lambida)%chave == 1)
        {
            //printf("%lld\n",(i*lambida)%chave);
            //printf("%d\n", i);
            w = i;
            break;
        }
    }
    
    printf("\nchave decodificacao: %lld\n", w);
    
    //Pegar numero congruente ao numero codificado elevado ao w mod x;
    printf("Numero decodificado: ");
    for (int i = 0; i < cont; i++)
    {
        long long v = 1;
        for (int j = 0; j < w; j++)
        {
            v = (v * codificada[i]) % x;
        }
        decodificar[i] = v;
        //printf("Original: %d\n", n[i]);
        printf("%d ", decodificar[i]);
    }

    //mensagem decodificada
    printf("\nMensagem decodifcada: ");
    for (int i = 0; i < cont; i++)
    {
        printf("%c", decodificar[i]);
    }
}