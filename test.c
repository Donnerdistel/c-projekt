#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod(s, &p);
    return *p == '\0';
}

int main() {
    char str[80] = "<author>Jim-Will Browning</author>";
    char str2[80] = "<editor>Jim-Will F. Browning 0001</editor>";
    char *word = NULL;
    char delimiter[] = " <>-";
    int wlength = 0;

    word = strtok(str2, delimiter);
    bool begin_tag = false;
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
                printf( " %s\n", word );
            }
        }

        //get next word
        word = strtok(NULL, delimiter);
    }

    return 0;
}
