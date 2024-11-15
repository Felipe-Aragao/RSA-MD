#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define TAM 9999

GtkBuilder *builder;
GtkWidget *window, *mensagem_aviso;
GtkStack *stack;
GtkTextView *text_mensagem, *text_show_encriptado, *text_encriptado, *text_show_desencriptado;
GtkEntry *entry_chave_p, *entry_chave_q, *entry_chave_e, *entry_encriptar_e, *entry_encriptar_n,
         *entry_desencriptar_p, *entry_desencriptar_q, *entry_desencriptar_e;


typedef unsigned long long ull;

ull criptografar(ull letra, ull e, ull n);
long long euclidesEstendido(long long a, long long b);
ull mod_multiplicao(ull c, ull d, ull n);
ull descriptografar(ull codigo, ull d, ull n);
int primo(ull num);
ull mdc (ull a, ull b);
int validar_e(ull e, ull totiente);
int validar_pq(ull p, ull q);


void on_Main_destroy(GtkWidget *widget, gpointer data)
{
        gtk_main_quit();
}

void mensagem_avisar(char text[], char secondaty_text[], char icon_name[])
{

    g_object_set(mensagem_aviso, "text", text, NULL);
    g_object_set(mensagem_aviso, "secondary_text", secondaty_text, NULL);

    gtk_window_set_icon_from_file(GTK_WINDOW(mensagem_aviso), icon_name, NULL);

    GtkWidget *primario = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(mensagem_aviso));
    GtkStyleContext *context1 = gtk_widget_get_style_context(primario);
    gtk_style_context_add_class(context1, "dialog_primary");

    GtkWidget *secundario = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(mensagem_aviso));
    GtkStyleContext *context2 = gtk_widget_get_style_context(secundario);
    gtk_style_context_add_class(context2, "dialog_secundary");

    gtk_widget_show_all(mensagem_aviso);
    gtk_dialog_run(GTK_DIALOG(mensagem_aviso));
    gtk_widget_hide(mensagem_aviso);
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

        int pq_valido = validar_pq(p, q);
        if (!pq_valido)
        {
            return;
        }
        
        //Pegar e;
        const char *e_text = gtk_entry_get_text(entry_chave_e);

        ull e = strtoull(e_text, NULL, 0);

        ull totiente = (p-1)*(q-1);

        int e_valido = validar_e(e, totiente);
        if (!e_valido)
        {
            return;
        }
        
        //Achar n
        ull n = p * q;

        //Overflow
        if (p > (ULLONG_MAX / q))
        {
            mensagem_avisar("ERRO", "\"n\" overflow", "images/erro.png");
            return;
        }

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

        mensagem_avisar("Sucesso", "Chave pública salva em publica.txt", "images/chave.png");

}

void on_button_encriptar_clicked(GtkWidget *widget, gpointer data)
{
        //Pegar e
        const char *e_text = gtk_entry_get_text(entry_encriptar_e);

        ull e = strtoull(e_text, NULL, 0);
        
        if (e == 0)
        {
            mensagem_avisar("ERRO", "\"e\" inválido", "images/erro.png");
            return;
        }

        //Pegar n
        const char *n_text = gtk_entry_get_text(entry_encriptar_n);

        ull n = strtoull(n_text, NULL, 0);
        
        if (n == 0)
        {
            mensagem_avisar("ERRO", "\"n\" inválido", "images/erro.png");
            return;
        }
        
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

        int pq_valido = validar_pq(p, q);
        if (!pq_valido)
        {
            return;
        }
        
        //Pegar e;
        const char *e_text = gtk_entry_get_text(entry_desencriptar_e);

        ull e = strtoull(e_text, NULL, 0);

        //Achar totiente
        ull totiente = (p-1)*(q-1);

        int e_valido = validar_e(e, totiente);
        if (!e_valido)
        {
            return;
        }
        
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
        mensagem_aviso = GTK_WIDGET(gtk_builder_get_object(builder, "mensagem_aviso"));
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
        
        gtk_window_set_icon_from_file(GTK_WINDOW(window), "images/principal.png", NULL);
        
        gtk_widget_show_all(window);
        gtk_main();

        return 0;
}

ull criptografar(ull letra, ull e, ull n)
{
    ull resultado = 1;
    letra = letra % n;
    while (e > 0)
    {
        if(e % 2 == 1)
        {
            resultado = mod_multiplicao(resultado, letra, n);
        }
        e = e / 2;
        letra = mod_multiplicao(letra, letra, n);
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

int primo(ull num)
{
    if (num == 2 || num == 3)
    {
        return 1;
    }
    else if (num <= 1 || num % 2 == 0)
    {
        return 0;
    }
    
    ull raiz = sqrt((double) num);
    for (ull i = 3; i <= raiz; i += 2)
    {
        if (num % i == 0)
            return 0;
    }

    return 1;
}

ull mdc (ull a, ull b)
{
    if (a % b == 0)
    {
        if (b < 0)
            return -b;
        return b;
    }
    return mdc(b, a % b);
}

int validar_e(ull e, ull totiente)
{
    if (e == 0)
    {
        mensagem_avisar("ERRO", "\"e\" inválido", "images/erro.png");
        return 0;
    }
    //e não pode ser maior que o totiente
    if (e >  totiente)
    {
        mensagem_avisar("ERRO", "\"e\" não pode ser maior que o totiente", "images/erro.png");
        return 0;
    }
    //Garatir que e e o totiente são coprimos
    if (mdc(e, totiente) != 1)
    {
        mensagem_avisar("ERRO", "\"e\" e o \"totiente\" não são coprimos", "images/erro.png");
        return 0;
    }

    return 1;
}

int validar_pq(ull p, ull q)
{
    int p_primo = primo(p);
    int q_primo = primo(q);


    //Garantir que p e q são primos
    if (p_primo == 0)
    {
        mensagem_avisar("ERRO", "\"p\" não é primo", "images/erro.png");
        return 0;
    }
    if (q_primo == 0)
    {
        mensagem_avisar("ERRO", "\"q\" não é primo", "images/erro.png");
        return 0;
    }

    return 1;
}