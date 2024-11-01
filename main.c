#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX 9999

typedef unsigned long long ull;

ull criptografar(ull letra, ull e, ull n);
ull descriptografar(ull codigo, ull d, ull n);
ull mod_multiplicao(ull c, ull d, ull n);
long long euclidesEstendido(long long a, long long b);

int main()
{   
    //Entrada da mesnsagem pelo usúario
    char menssagem[MAX];
    printf("Menssagem: ");
    gets(menssagem);

    int tamanho = strlen(menssagem);
    printf("Tamanho: %d\n", tamanho);

    //p e q devem ser numeros primos
    //TODO: Confirmar que p e q são primos
    ull p = 999863;
    ull q = 999671;

/*     printf("P: ");
    printf("Q: ");
    scanf("%d%d", &p, q); */
    
    //modular
    ull n = p*q;
    
    //Achar o totiente (phi)
    ull totiente = (p-1)*(q-1);
    printf("Totiente: %lld\n", totiente);
    
    //e e totiente devem ser primos entre si.
    //TODO: Confirmar que e e totiente são primos entre si
    int e;
    printf("e: ");
    scanf("%d", &e);

    //Criptografar
    ull codificada[MAX];
    printf("Mensagem codificada: ");
    for (int i = 0; i < tamanho; i++)
    {
        codificada[i] = criptografar((int) menssagem[i], e, n);
    }

    // Euclides estendido para achar s de e(mod totiente)
    long long inverso = euclidesEstendido(e, totiente);
    //Garantir que o s é positivo, portanto o inverso
    while (inverso < 0)
    {
        inverso += totiente;
    }
    
    printf("\nd: %llu\n", inverso);
    
    //Descriptografar
    ull decodificar[MAX];
    printf("Numero decodificado: ");
    for (int i = 0; i < tamanho; i++)
    {
        decodificar[i] = descriptografar(codificada[i], inverso, n);
        printf("%llu ", decodificar[i]);
    }

    //Imprimir a menssagem descriptografada
    printf("\nMensagem decodifcada: ");
    for (int i = 0; i < tamanho; i++)
    {
        printf("%c", decodificar[i]);
    }
}

ull criptografar(ull letra, ull e, ull n)
{
    ull resultado = 1;
    for (int j = 0; j < e; j++)
    {
        //Exponenciacao modular
        resultado = (resultado * letra) % n;
    }
    printf("%llu ", resultado);

    return resultado;
}

ull descriptografar(ull codigo, ull d, ull n)
{
    ull resultado = 1;
    codigo = codigo % n;
    while (d > 0)
    {
        if(d % 2 == 1)
        {
            resultado = mod_multiplicao(resultado, codigo, n);
        }
        d = d / 2;
        codigo = mod_multiplicao(codigo, codigo, n);
    }

    return resultado;
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

ull mod_multiplicao(ull c, ull d, ull n)
{
    ull resultado = 0;

    c = c % n;
    while (d > 0)
    {
        if(d % 2 == 1)
        {
            resultado = (resultado + c) % n;
        }

        c = (c * 2) % n;
        d = d / 2;
    }
    return resultado;
}