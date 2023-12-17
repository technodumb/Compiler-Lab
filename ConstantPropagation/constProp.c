#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>


char keywords[][20] = {"exit", "for", "while", "return", "if", "else", "printf", "main"};
char dtypes[][20] = {"void", "int", "char", "float"};
int symtabType[20], declarationType;
char symtab[20][20];
double symtabval[20];
char word[20], idStack[20][20], opStack[20];
int wordptr=0, idStackTop=0, opStackTop=0, nSymtab=0;
bool isDeclaration = false, isRHS = false;

float getSymbolValue(char *symbol){
    printf("symbol: %s\n", symbol);
    float value;
    if(isdigit(symbol[0]))
        value = atof(symbol);
    else {

        for(int i=0; i<nSymtab; i++){
            if(strcmp(symbol, symtab[i])==0){
                printf("Present in symtab: %s\n", symbol);
                value = symtabval[i];
                break;
            }
        }
    }
    printf("value: %lf\n", value);
    return value;
}

void main() {

    FILE *finp, *fcon, *fout;
    finp = fopen("input.txt", "r");
    fcon = fopen("const.txt", "w+");
    fout = fopen("output.txt", "w");

    if(finp == NULL || fcon == NULL || fout == NULL){
        printf("The files could not be opened.\n");
        exit(0);
    }

    while(true) {
        char c = fgetc(finp);
        if(c == EOF) break;

        // ignoring #includes
        if(c == '#'){
            fprintf(fout, "#");
            while((c = fgetc(finp)) != '\n'){
                fprintf(fout, "%c", c);
            }
            fprintf(fout, "\n");
        }
        // ignoring strings
        else if(c == '"'){
            fprintf(fout, "\"");
            while((c = fgetc(finp)) != '"'){
                fprintf(fout, "%c", c);
            }
            fprintf(fout, "\"");
        }
        else if(c == ' '){
            fprintf(fout, " ");
            continue;
        }
        else if(isalnum(c)){

            if(isdigit(c)){
                word[wordptr++] = c;
                while(isdigit(c=fgetc(finp)) || c=='.'){
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

                for(int i=0; i<8; i++){
                    if(strcmp(word, keywords[i])==0){
                        fprintf(fout, "%s", word);
                        wordptr=0;
                        break;
                    }
                }

                if(!wordptr) continue;

                for(int i=0; i<4; i++){
                    if(strcmp(word, dtypes[i])==0){
                        fprintf(fout, "%s", word);
                        wordptr=0;
                        isDeclaration = true;
                        declarationType = i;
                        break;
                    }
                }

                if(!wordptr) continue;

                if(isDeclaration){
                    strcpy(symtab[nSymtab], word);
                    symtabType[nSymtab] = declarationType;
                    symtabval[nSymtab++] = 0;
                    fprintf(fout, "%s", word);
                }
                strcpy(idStack[idStackTop++], word);
                wordptr=0;
            }
            
            if(opStack[opStackTop-1] == '*' || opStack[opStackTop-1] == '/'){
                float oper2 = getSymbolValue(idStack[--idStackTop]);
                float oper1 = getSymbolValue(idStack[--idStackTop]);
                float res;
                if(opStack[--opStackTop] == '*'){
                    res = oper1 * oper2;
                }
                else {
                    res = oper1 / oper2;
                }
                sprintf(idStack[idStackTop++], "%lf", res);
            }
        }


        else if(c == '+' || c == '-' || c == '*' || c == '/'){
            // if(isRHS){
            opStack[opStackTop++] = c;
        }

        else if(c=='=') {
            if(isDeclaration){
                isDeclaration = false;
            }
            else {
                fprintf(fout, "%s", idStack[0]);
            }
            opStack[opStackTop++] = c;
            fprintf(fout, "=");
        }

        else if(c == ',' || c==';'){            
            if(opStack[0] == '=' && opStackTop > 0){
                float oper1, oper2;
                oper1 = getSymbolValue(idStack[1]); 

                for(int i=1; i<opStackTop; i++){
                    oper2 = getSymbolValue(idStack[i+1]);
                    float res;
                    if(opStack[i] == '+'){
                        res = oper1 + oper2;
                    }
                    else {
                        res = oper1 - oper2;
                    }
                    oper1 = res;
                }

                for(int i=0; i<nSymtab; i++){
                    if(strcmp(idStack[0], symtab[i])==0){
                        symtabval[i] = oper1;
                        if(symtabType[i]==1){
                            fprintf(fout, "%d", (int) oper1);
                        }
                        else if(symtabType[i]==3){
                            fprintf(fout, "%lf", oper1);
                        }
                        break;
                    }
                }

            }
            if(c == ';')
                isDeclaration = false;
            if(c== ',')
                isDeclaration = true;
            opStackTop = 0;
            idStackTop = 0;

            fprintf(fout, "%c", c);
        }
        else{ 
            fprintf(fout, "%c", c);
            printf("%c", c);
        }
    }
}