#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX 100


int main()
{   
    char menssagem[MAX];
    printf("Menssagem: ");
    gets(menssagem);

    int tamanho = strlen(menssagem);

    printf("Tamanho %d\n", tamanho);

    //p e q devem ser numeros primos (grandes) que ao serem dividos por 6 deixam resto 5;
    //lambda e totiente de Euler devem ser primos entre si.
   
    long long p = 227;
    long long q = 229;

/*     printf("P: ");
    printf("Q: ");
    scanf("%d%d", &p, q); */
    
    //modular
    long long n = p*q;
    
    //descobrir totiente privada
    long long totiente = (p-1)*(q-1);
    printf("totiente: %lld\n", totiente);
    
    //lambda == e
    int e;
    scanf("%d", &e);

    //vetor para pegar a menssagem codificada
    int codificada[MAX];
    
    //codificacao
    printf("Mensagem codificada: ");
    for (int i = 0; i < tamanho; i++)
    {
        long long resultado = 1;
        for (int j = 0; j < e; j++)
        {
            //exponenciacao modular
            resultado = (resultado * (int) menssagem[i]) % n;
        }
        codificada[i] = resultado;
        printf("%d ", codificada[i]);
    }

    //vetor para pegar menssagem decodificada
    int decodificar[MAX];
    
    //achar o inverso modulo (p-1)*(q-1)
    long long w = 0;
    
    for (int i = 1; i < totiente; i++)
    {
        if ((i*e)%totiente == 1)
        {
            //printf("%lld\n",(i*e)%totiente);
            //printf("%d\n", i);
            w = i;
            break;
        }
    }
    
    printf("\nchave decodificacao: %lld\n", w);
    
    //Pegar numero congruente ao numero codificado elevado ao w mod n;
    printf("Numero decodificado: ");
    for (int i = 0; i < tamanho; i++)
    {
        long long v = 1;
        for (int j = 0; j < w; j++)
        {
            v = (v * codificada[i]) % n;
        }
        decodificar[i] = v;
        //printf("Original: %d\n", n[i]);
        printf("%d ", decodificar[i]);
    }

    //mensagem decodificada
    printf("\nMensagem decodifcada: ");
    for (int i = 0; i < tamanho; i++)
    {
        printf("%c", decodificar[i]);
    }
}