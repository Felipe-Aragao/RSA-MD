#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX 100

long long euclidesEstendido(long long a, long long b);

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
    
    // Euclides estendido para achar s de e(mod totiente)
    long long inverso = euclidesEstendido(e, totiente);
    //Garantir que o s é positivo, portanto o inverso
    while (inverso < 0)
    {
        inverso += totiente;
    }
    
    printf("\nchave decodificacao: %lld\n", inverso);
    
    //Pegar numero congruente ao numero codificado elevado ao inverso mod n;
    printf("Numero decodificado: ");
    for (int i = 0; i < tamanho; i++)
    {
        long long v = 1;
        for (int j = 0; j < inverso; j++)
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

long long euclidesEstendido(long long a, long long b)
{
    long long s, t, quociente;
    long long s0 = 1;
    long long s1 = 0;
    long long t0 = 0;
    long long t1 = 1;

    for (int i = 1; b != 0; i++)
    {
        if (i >= 2)
        {
            s = s0 - s1 * quociente;
            t = t0 - t1 * quociente;

            s0 = s1;
            s1 = s;
            t0 = t1;
            t1 = t;
        }
        quociente = a / b;

        long long temp = b;
        b = a % b;
        a = temp;
        
    }
    if (a < 0)
        return -s;
    return s;
}