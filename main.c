#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define TAM 9999

GtkBuilder *builder;
GtkWidget *window;
GtkStack *stack;
GtkTextView *text_mensagem, *text_show_encriptado, *text_encriptado, *text_show_desencriptado;
GtkEntry *entry_chave_p, *entry_chave_q, *entry_chave_e, *entry_encriptar_e, *entry_encriptar_n,
         *entry_desencriptar_p, *entry_desencriptar_q, *entry_desencriptar_e;


typedef unsigned long long ull;

ull criptografar(ull letra, ull e, ull n);
long long euclidesEstendido(long long a, long long b);
ull mod_multiplicao(ull c, ull d, ull n);
ull descriptografar(ull codigo, ull d, ull n);


void on_Main_destroy(GtkWidget *widget, gpointer data)
{
        gtk_main_quit();
}

void on_button_chave_clicked(GtkWidget *widget, gpointer data)
{
        gtk_stack_set_visible_child_name(stack, "chave");
}

void on_button_home_encriptar_clicked(GtkWidget *widget, gpointer data)
{
        gtk_stack_set_visible_child_name(stack, "encriptar");
}

void on_button_home_desencriptar_clicked(GtkWidget *widget, gpointer data)
{
        gtk_stack_set_visible_child_name(stack, "desencriptar");

        FILE *f;
        f = fopen("encriptado.txt", "r");

        char codificada[TAM * 15];
        int i = -1;
        do
        {
                i++;
                codificada[i] = fgetc(f);

        } while (codificada[i] != EOF);

        codificada[i] = '\0';

        fclose(f);

        GtkTextBuffer *buffer_output2 = gtk_text_view_get_buffer(text_encriptado);
        gtk_text_buffer_set_text(buffer_output2, codificada, -1);
}

void on_button_gerar_chave_clicked(GtkWidget *widget, gpointer data)
{
        //Pegar p
        const char *p_text = gtk_entry_get_text(entry_chave_p);

        ull p = strtoull(p_text, NULL, 0);
        
        //Pegar q
        const char *q_text = gtk_entry_get_text(entry_chave_q);

        ull q = strtoull(q_text, NULL, 0);
        
        //Pegar e;
        const char *e_text = gtk_entry_get_text(entry_chave_e);

        ull e = strtoull(e_text, NULL, 0);
        
        //Achar n
        ull n = p * q;

        //Abrir arquivo
        FILE *f;
        f = fopen("publica.txt", "w");
        fprintf(f, "%llu %llu", e, n);
        fclose(f);

        f = fopen("publica.txt", "r");
        char n_text[TAM];
        int i = -1;
        int is_n = 0;
        do
        {
                i++;
                n_text[i] = fgetc(f);
                
                if (n_text[i] == ' ')
                {
                        is_n = 1;
                }
                
                if (!is_n)
                {
                        i--;
                }

        } while (n_text[i] != EOF);
        fclose(f);
        n_text[i] = '\0';

        gtk_entry_set_text(entry_encriptar_e, e_text);
        gtk_entry_set_text(entry_encriptar_n, n_text);

        gtk_entry_set_text(entry_desencriptar_p, p_text);
        gtk_entry_set_text(entry_desencriptar_q, q_text);
        gtk_entry_set_text(entry_desencriptar_e, e_text);

}

void on_button_encriptar_clicked(GtkWidget *widget, gpointer data)
{
        //Pegar e
        const char *e_text = gtk_entry_get_text(entry_encriptar_e);

        ull e = strtoull(e_text, NULL, 0);
        

        //Pegar n
        const char *n_text = gtk_entry_get_text(entry_encriptar_n);

        ull n = strtoull(n_text, NULL, 0);
        
        
        //Entrada do texto
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_mensagem);

        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        //Encriptar
        char mensagem[TAM];
        strcpy(mensagem, text);

        int tamanho = strlen(mensagem);

        FILE *f;
        f = fopen("encriptado.txt", "w");

        for (int i = 0; i < tamanho; i++)
        {
            ull letra_cript = criptografar((int) mensagem[i], e, n);
            fprintf(f, "%llu ", letra_cript);
        }
        fclose(f);
        
        //Saída de texto
        f = fopen("encriptado.txt", "r");

        char codificada[TAM * 15];
        int i = -1;
        do
        {
                i++;
                codificada[i] = fgetc(f);

        } while (codificada[i] != EOF);

        codificada[i] = '\0';

        fclose(f);

        GtkTextBuffer *buffer_output = gtk_text_view_get_buffer(text_show_encriptado);
        GtkTextBuffer *buffer_output2 = gtk_text_view_get_buffer(text_encriptado);
        gtk_text_buffer_set_text(buffer_output, codificada, -1);
        gtk_text_buffer_set_text(buffer_output2, codificada, -1);
}

void on_button_desencriptar_clicked(GtkWidget *widget, gpointer data)
{
        //Pegar p
        const char *p_text = gtk_entry_get_text(entry_desencriptar_p);

        ull p = strtoull(p_text, NULL, 0);
        
        //Pegar q
        const char *q_text = gtk_entry_get_text(entry_desencriptar_q);

        ull q = strtoull(q_text, NULL, 0);
        
        //Pegar e;
        const char *e_text = gtk_entry_get_text(entry_desencriptar_e);

        ull e = strtoull(e_text, NULL, 0);
        
        //Entrada do texto
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_encriptado);

        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        //Desencriptar
        FILE *fencriptar;
        FILE *fdesencriptar;
        fdesencriptar = fopen("desencriptado.txt", "w");
        fencriptar = fopen("encriptado.txt", "w");

        fprintf(fencriptar, "%s", text);
        fclose(fencriptar);
        
        fencriptar = fopen("encriptado.txt", "r");
        //Totiente
        ull totiente = (p-1)*(q-1);
        // Euclides estendido para achar s de e(mod totiente)
        long long inverso = euclidesEstendido(e, totiente);
        //Garantir que o s é positivo, portanto o inverso
        while (inverso < 0)
        {
            inverso += totiente;
        }
        ull n = p * q;

        ull codificada;
        while (fscanf(fencriptar, "%llu", &codificada) != EOF)
        {
            fprintf(fdesencriptar, "%c", descriptografar(codificada, inverso, n));
        }

        fclose(fencriptar);
        fclose(fdesencriptar);

        //Saída de texto

        fdesencriptar = fopen("desencriptado.txt", "r");

        char descriptografado[TAM];
        int i = -1;
        
        do
        {
                i++;
                descriptografado[i] = fgetc(fdesencriptar);

        } while (descriptografado[i] != EOF);

        descriptografado[i] = '\0';

        fclose(fdesencriptar);

        GtkTextBuffer *buffer_output = gtk_text_view_get_buffer(text_show_desencriptado);
        gtk_text_buffer_set_text(buffer_output, descriptografado, -1);
}

void on_button_voltar_clicked(GtkWidget *widget, gpointer data)
{
        gtk_stack_set_visible_child_name(stack, "home");
}

int main (int argc, char *argv[])
{
        gtk_init(&argc, &argv);

        //CSS
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(provider, "style.css", NULL);

        GtkStyleContext *context = gtk_style_context_new();
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                  GTK_STYLE_PROVIDER(provider),
                                                  GTK_STYLE_PROVIDER_PRIORITY_USER);


        builder = gtk_builder_new_from_file("janela.glade");

        gtk_builder_add_callback_symbols(
        builder,
        "on_Main_destroy",                      G_CALLBACK(on_Main_destroy),
        "on_button_chave_clicked",              G_CALLBACK(on_button_chave_clicked),
        "on_button_home_encriptar_clicked",     G_CALLBACK(on_button_home_encriptar_clicked),
        "on_button_home_desencriptar_clicked",  G_CALLBACK(on_button_home_desencriptar_clicked),
        "on_button_gerar_chave_clicked",        G_CALLBACK(on_button_gerar_chave_clicked),
        "on_button_encriptar_clicked",          G_CALLBACK(on_button_encriptar_clicked),
        "on_button_desencriptar_clicked",       G_CALLBACK(on_button_desencriptar_clicked),
        "on_button_voltar_clicked",             G_CALLBACK(on_button_voltar_clicked),
        NULL
        );
        gtk_builder_connect_signals(builder, NULL);

        window = GTK_WIDGET(gtk_builder_get_object(builder, "Main"));
        stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
        
        text_mensagem = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_mensagem"));
        text_show_encriptado = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_show_encriptado"));
        text_encriptado = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_encriptado"));
        text_show_desencriptado = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_show_desencriptado"));


        entry_chave_p = GTK_ENTRY(gtk_builder_get_object(builder, "entry_chave_p"));
        entry_chave_q = GTK_ENTRY(gtk_builder_get_object(builder, "entry_chave_q"));
        entry_chave_e = GTK_ENTRY(gtk_builder_get_object(builder, "entry_chave_e"));
        entry_encriptar_e = GTK_ENTRY(gtk_builder_get_object(builder, "entry_encriptar_e"));
        entry_encriptar_n = GTK_ENTRY(gtk_builder_get_object(builder, "entry_encriptar_n"));
        entry_desencriptar_p = GTK_ENTRY(gtk_builder_get_object(builder, "entry_desencriptar_p"));
        entry_desencriptar_q = GTK_ENTRY(gtk_builder_get_object(builder, "entry_desencriptar_q"));
        entry_desencriptar_e = GTK_ENTRY(gtk_builder_get_object(builder, "entry_desencriptar_e"));
        gtk_widget_show_all(window);
        gtk_main();

        return 0;
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