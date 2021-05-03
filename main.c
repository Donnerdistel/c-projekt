#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <xmllite.h>


struct node {
    char		*w;
    int	 	count;
    struct node	*l, *r;
} *tree;

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


void traverse_search(struct node *r) {
    //TODO: Laufe durch Baum und lese Wörter mit count = 1 aus
    if (r == NULL)		//Baum leer?
        return;
    traverse_search(r->l);		//laufe linken Teilbaum durch
    if((*r).count == 1){
        printf("%s %d\n",r->w, r->count);
    };
    traverse_search(r->r);				//laufe rechten Teilbaum durch
}

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod(s, &p);
    return *p == '\0';
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

    char delimiter[] = " <>-(){}'\\,\"$;[]";
    bool begin_tag = false;
    while (getline(&line, &len, fp) != -1) {
        begin_tag = false;
        word = strtok(line, delimiter);
        while( word != NULL ) {
            if(strcmp(word, "author") == 0 || strcmp(word, "editor") == 0){
                begin_tag = true;
                //get next word
                word = strtok(NULL, delimiter);
            }
            else if(strcmp(word, "/author") == 0 || strcmp(word, "/editor") == 0){
                begin_tag = false;
            }

            if(begin_tag){
                if (!isNumeric(word)){
                    //printf( " %s\n", word );
                    incr(&tree,word);
                }
            }

            //get next word
            word = strtok(NULL, delimiter);
        }
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
