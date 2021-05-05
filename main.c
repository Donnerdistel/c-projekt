#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


struct node {
    char		*w;
    int	 	count;
    char* line;
    struct node	*l, *r;
} *tree;

//char* lineTEMP = NULL;

//Binärbaum aufbauen
void incr(struct node **root, char *w) {

    if (root == NULL || w == NULL || *w == '\0')
        return;
    if (*root == NULL) {
        struct node *n;
        int l = strlen(w);

        n = (struct node *) malloc(sizeof(struct node));
        if (n == NULL)
            return;
        n->w = (char *) malloc(l+1);
        if (n->w == NULL)
            return;
        strcpy(n->w,w);
        n->count = 1;
        n->l = NULL;
        n->r = NULL;
        //n->line = lineTEMP;

        *root = n;
        return;
    }
    int c = strcmp((*root)->w,w);
    if (c == 0) {
        ((*root)->count)++;
        return;
    }
    if (c < 0)
        incr(&((*root)->r),w);
    else
        incr(&((*root)->l),w);
}

//Laufe durch Baum und lese Wörter mit count = 1 aus
void traverse_search(struct node *r) {
    if (r == NULL)		//Baum leer?
        return;
    traverse_search(r->l);		//laufe linken Teilbaum durch
    if((*r).count == 1){
        //printf("%s %d \t\t\t%s\n",r->w, r->count, r->line);
        //printf("%s %d \n",r->w, r->count);
        printf("%s \n",r->w);
    };
    traverse_search(r->r);				//laufe rechten Teilbaum durch
}

void words(char *s, char *end) {
    int state = 0;
    char c;
    char *start = s;

    while (s <= end) {
        c = *s;
        if (!(c == '-') && !(c == '(') && !(c==')') && !(c=='\"') && !isblank(c) && !isdigit(c) && !(c == '<')) {
            if (!state) {
                start = s;
                state = 1;
            }
        } else {
            state = 0;
            if (start) {
                *s = '\0';
                incr(&tree,start);
                *s = c;
            }
            start = NULL;
        }
        s++;
    }
}

//Read authors und editors
char *read_file(char *file_name){
    FILE *fp;
    size_t len = 0;
    char *line = NULL;
    char *word = NULL;

    //öffne Datei im Lesezustand
    fp = fopen(file_name, "r");

    //Test: Datei vorhanden
    if(fp == NULL){
        printf("Fehler bei fopen()...");
        exit(EXIT_FAILURE);
    }

    char *start = NULL;
    char *end = NULL;
    while (getline(&line, &len, fp) != -1) {
        //Suche <author und <editor in der Zeile
        if (strstr(line, "<author") != NULL)
        {
            start = strstr(line, "<author");
            start = strchr(start, '>');               //erstes Vorkommen von >
            end = strstr(line, "</author");        //letztes Vorkommen von </author

            //konvertiere das Intervall in Wörter
            start++;      //Zeichen hinter >
            //lineTEMP = line;
            words(start, end);
        }
        else if (strstr(line, "<editor") != NULL) {
            start = strstr(line, "<editor");
            start = strchr(line, '>');              //erstes Vorkommen von >
            end = strstr(line, "</editor");      //letztes Vorkommen von </editor

            //konvertiere das Intervall in Wörter
            start++;      //Zeichen hinter >
            //lineTEMP = line;
            words(start, end);
        }

        //printf("Start: %s und Ende: %s\n", start, end);
        //lineTEMP = NULL;
        line = NULL;
    }
}

int main(int ac, char **av) {           //Dateiname über Kommandozeile bekommen
    //Test: Ausreichende Argumentanzahl
    if(ac != 2){
        exit(-1);
    }

    tree = NULL;
    char *f = read_file(av[1]);

    traverse_search(tree);

    return 0;
}
