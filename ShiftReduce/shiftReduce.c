#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct production{
    char rhs[10];
    char lhs[10];
};

void main() {
    char input[10], inputCopy[10], *lhs, *rhs, stack[10];
    int nProds = 0, stackLen;
    struct production productions[10];
    FILE *fp;

    fp = fopen("input.txt", "r");

    if(fp==NULL) {
        printf("Cannot read the productions.");
        return;
    }

    printf("Enter the input string: ");
    scanf("%s", input);

    strcpy(inputCopy, input);

    char prodStr[20];
    while(!feof(fp)){
        fscanf(fp, "%s\n", prodStr);

        lhs = strtok(prodStr, "=");
        rhs = strtok(NULL, "=");
        strcpy(productions[nProds].lhs, lhs);
        strcpy(productions[nProds++].rhs, rhs);
    }

    printf("Stack\tInput\tAction\n");
    printf("$\t%s$\n", input);
    stackLen = 0;
    for(int i=0; i<strlen(input); i++){
        stack[stackLen++] = input[i];
        stack[stackLen] = 0;
        inputCopy[i] = ' ';

        printf("$%s\t%s$\tSHIFT\n", stack, inputCopy);

        for(int j=0; j<nProds; j++){
            char *temp = strstr(stack, productions[j].rhs);
            // printf("%s, %s\n", stack, productions[j].rhs);
            if(temp==NULL) continue;
            stackLen -= strlen(temp);
            stack[stackLen] = 0;
            strcat(stack, productions[j].lhs);
            stackLen = strlen(stack);
            printf("$%s\t%s$\tREDUCE: %s->%s\n", stack, inputCopy, productions[j].lhs, productions[j].rhs);
            j = -1;
        }
    }

    if(strcmp(stack, productions[0].lhs)==0){
        printf("%s is accepted.", input);
    }
    else {
        printf("%s is not accepted.", input);
    }
}