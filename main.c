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


//FIXME: Está dando erro para números pequenos, provavelmente o problema e na parte da descriptografia
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
        char menssagem[MAX];
        printf("Menssagem: ");
        //TODO Mudar para fgets
        getchar();
        gets(menssagem);
        int tamanho = strlen(menssagem);

        printf("e: ");
        scanf("%llu", &e);
        printf("n: ");
        scanf("%llu", &n);

        FILE *f;
        f = fopen("encriptado.txt", "w");

        for (int i = 0; i < tamanho; i++)
        {
            fprintf(f, "%llu ", criptografar((int) menssagem[i], e, n));
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

/*     //Entrada da mesnsagem pelo usúario
    char menssagem[MAX];
    printf("Menssagem: ");
    getchar();
    gets(menssagem);

    int tamanho = strlen(menssagem);
    printf("Tamanho: %d\n", tamanho);

    //p e q devem ser numeros primos
    //TODO: Confirmar que p e q são primos
    p = 999863;
    q = 999671;
    
    //modular
    n = p*q;
    
    //Achar o totiente (phi)
    ull totiente = (p-1)*(q-1);
    printf("Totiente: %lld\n", totiente);
    
    //e e totiente devem ser primos entre si.
    //TODO: Confirmar que e e totiente são primos entre si
    printf("e: ");
    scanf("%llu", &e);

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
    } */
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