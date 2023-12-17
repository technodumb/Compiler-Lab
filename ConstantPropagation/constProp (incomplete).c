#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

void main() {
    char keywords[][20] = {"void", "int", "char", "exit", "for", "while", "return", "if", "else", "printf", "main"};
    char dtypes[][20] = {"int", "char", "float"};

    FILE *finp, *fcon, *fout;
    finp = fopen("input.txt", "r");
    fcon = fopen("const.txt", "w+");
    fout = fopen("output.txt", "w");

    if(finp == NULL || fcon == NULL || fout == NULL){
        printf("The files could not be opened.\n");
        exit(0);
    }

    char word[20], idStack[20][20], opStack[20];
    int wordptr=0, idStackTop=0, opStackTop=0;

    char c = fgetc(finp);

    while(!feof(finp)) {
        if(c == '#'){
            fprintf(fout, "#");
            while((c = fgetc(finp)) != '\n'){
                fprintf(fout, "%c", c);
            }
            fprintf(fout, "\n");
        }
        if(c == '"'){
            fprintf(fout, "\"");
            while((c = fgetc(finp)) != '"'){
                fprintf(fout, "%c", c);
            }
            fprintf(fout, "\"");
        }
        else if(isdigit(c)){
            word[wordptr++] = c;
            while(isdigit(c=fgetc(finp))){
                word[wordptr++] = c;
            }
            if(isalpha(c)){
                printf("Invalid lexeme.\n");
                exit(0);
            }
            fseek(finp, -1, SEEK_CUR);
            word[wordptr++] = 0;
            strcpy(idStack[idStackTop++], word);
            wordptr=0;
        }
        else if(isalpha(c)){
            word[wordptr++] = c;
            while(isalnum(c=fgetc(finp))){
                word[wordptr++] = c;
            }
            fseek(finp, -1, SEEK_CUR);
            word[wordptr++] = 0;
            strcpy(idStack[idStackTop++], word);
            wordptr=0;
        }
        else{ 
            fprintf(fout, "%c", c);
            printf("%c", c);
        }
        c = fgetc(finp);   
    }
}