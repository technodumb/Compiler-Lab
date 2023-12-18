#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main(){
    FILE *finp, *fout;
    finp = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    
    char fir[20], sec[20], res[20], op;
    char opr[10];
    int opMode=0;



    while(!feof(finp)){
        fscanf(finp, "%c%s%s%s\n", &op, fir, sec, res);
        printf("%c %s %s %s\n", op, fir, sec, res);

        switch(op){
            case '+':   strcpy(opr, "ADD");
                        opMode = 0;
                        break;
            case '-':   strcpy(opr, "SUB");
                        opMode = 0;
                        break;
            case '*':   strcpy(opr, "MUL");
                        opMode = 1;
                        break;
            case '/':   strcpy(opr, "DIV");
                        opMode = 1;
                        break;
        }

        if(strlen(fir)==1){
            fprintf(fout, "MOV AX, [%s]\n", fir);
        }
        else {
            if(fir[1] == '1')
                fprintf(fout, "MOV AX, CH\n");
            if(fir[1] == '2')
                fprintf(fout, "MOV BX, CL\n");
            if(fir[1] == '3')
                fprintf(fout, "MOV CX, DH\n");
            if(fir[1] == '4')
                fprintf(fout, "MOV DX, DL\n");
        }

        if(strlen(sec)==1){
            fprintf(fout, "MOV BX, [%s]\n", sec);
        }
        else {
            if(sec[1] == '1')
                fprintf(fout, "MOV BX, CH\n");
            if(sec[1] == '2')
                fprintf(fout, "MOV BX, CL\n");
            if(sec[1] == '3')
                fprintf(fout, "MOV BX, DH\n");
            if(sec[1] == '4')
                fprintf(fout, "MOV BX, DL\n");
        }

        if(opMode==0){
            fprintf(fout, "%s AX BX\n", opr);
        }
        else {
            fprintf(fout, "%s AX\n", opr);
        }

        if(strlen(res)==2){
            if(res[1]=='1')
                fprintf(fout, "MOV CH, AX\n");
            if(res[1]=='2')
                fprintf(fout, "MOV CL, AX\n");
            if(res[1]=='3')
                fprintf(fout, "MOV DH, AX\n");
            if(res[1]=='4')
                fprintf(fout, "MOV DL, AX\n");
        }
        else {
            fprintf(fout, "MOV [%s], AX\n", res);
        }
    }
}