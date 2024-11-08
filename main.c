#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX 9999

typedef unsigned long long ull;

void chave();
ull criptografar(ull letra, ull e, ull n);
ull descriptografar(ull codigo, ull d, ull n);
ull mod_multiplicao(ull c, ull d, ull n);
long long euclidesEstendido(long long a, long long b);

int main()
{   
    int modo;
    printf("1 - Gerar chave pública\n2 - Encriptar\n3 - Desencriptar\n");
    scanf("%d", &modo);

    if (modo > 3 || modo < 1)
    {
        printf("Escolha uma opção válida");
        return 1;
    }

    ull p, q, e, n;
    
    // Gerar chave pública
    if (modo == 1)
    {
        chave();
    }
    
    // Encriptar
    if (modo == 2)
    {
        //Entrada da mesnsagem pelo usúario
        char mensagem[MAX];
        printf("Mensagem: ");
        //TODO Mudar para fgets
        getchar();
        gets(mensagem);
        int tamanho = strlen(mensagem);

        printf("e: ");
        scanf("%llu", &e);
        //TODO: Confirmar que e é válido
        printf("n: ");
        scanf("%llu", &n);

        FILE *f;
        f = fopen("encriptado.txt", "w");

        for (int i = 0; i < tamanho; i++)
        {
            fprintf(f, "%llu ", criptografar((int) mensagem[i], e, n));
        }

        fclose(f);
    }

    //Desencriptar
    if (modo == 3)
    {
        printf("P: ");
        scanf("%llu", &p);
        printf("Q: ");
        scanf("%llu", &q);
        printf("e: ");
        scanf("%llu", &e);
        //TODO: Confirmar que e é válido
        //TODO: Confirmar que P e Q são números primos

        FILE *cript;
        FILE *descript;
        cript = fopen("encriptado.txt", "r");
        descript = fopen("desencriptado.txt", "w");

        //Achar o totiente (phi)
        ull totiente = (p-1)*(q-1);

        // Euclides estendido para achar s de e(mod totiente)
        long long inverso = euclidesEstendido(e, totiente);
        //Garantir que o s é positivo, portanto o inverso
        while (inverso < 0)
        {
            inverso += totiente;
        } 
        n = p * q;

        ull codificada;
        while (fscanf(cript, "%llu", &codificada) != EOF)
        {
            printf("%llu ", codificada);
            fprintf(descript, "%c", descriptografar(codificada, inverso, n));
        }
        fclose(cript);
        fclose(descript);
    }
}

void chave()
{
    ull p, q, e, n;

    FILE *f;
    f = fopen("publica.txt", "w");

    printf("P: ");
    scanf("%llu", &p);
    printf("Q: ");
    scanf("%llu", &q);
    printf("e: ");
    scanf("%llu", &e);
    //TODO: Confirmar que e é válido
    //TODO: Confirmar que P e Q são números primos
    n = p*q;

    fprintf(f, "%llu %llu", e, n);
    fclose(f);
}

ull criptografar(ull letra, ull e, ull n)
{
    ull resultado = 1;
    for (int j = 0; j < e; j++)
    {
        //Exponenciacao modular
        resultado = (resultado * letra) % n;
    }

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