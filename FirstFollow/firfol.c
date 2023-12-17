#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

int nProd, nTempFirst, nFirst, nFollow, nDone, nPrinted, nTempFollow;
char productions[10][10], done[10];

char first[10][100], tempFirst[100];
char follow[10][100], tempFollow[100];
bool leftRec = false, eps=false;

void findFirst(char c, int curProd, int nextChar);
void findFollow(char c);
void getFirst(char c, int curProd, int nextChar);

void main(){
    printf("Enter the no of productions: ");
    scanf("%d", &nProd);

    printf("Enter the productions:\n");
    for(int i=0; i<nProd; i++){
        scanf("%s", productions[i]);
    }

    for(int i=0; i<nProd; i++){
        for(int j=0; j<100; j++){
            first[i][j]=0;
        }
    }

    nDone = 0;

    for(int i=0; i<nProd; i++) {
        char nonTerm = productions[i][0];
        bool isDone=false;

        for(int j=0; j<nDone; j++){
            if(done[j] == nonTerm){
                isDone = true;
                break;
            }
        }

        if(isDone) continue;

        nPrinted = 0;
        nTempFirst = 0;
        leftRec = false;
        eps = false;
        findFirst(nonTerm, 0, 0);
        printf("\n  First(%c) = { ", nonTerm);
        first[nDone][nPrinted++] = nonTerm;

        for(int j=0; j<nTempFirst; j++){
            // if(tempFirst[j])
            bool isPrinted = false;
            for(int k = 0; k < nPrinted; k++){
                if(tempFirst[j] == first[nDone][k]) {
                    isPrinted = true;
                    break;
                }
            }

            if(isPrinted){
                continue;
            }

            printf("%c, ", tempFirst[j]);
            first[nDone][nPrinted++] = tempFirst[j];
        }
        printf("}\n");
        done[nDone++] = nonTerm;
    }
    nFirst = nDone;

    printf("\n");
    printf("_________________________________________________");
    printf("\n\n");

    nDone = 0;
    for(int i=0; i<nProd; i++){
        for(int j=0; j<100; j++){
            follow[i][j] = 0;
        }
    }

    for(int i=0; i<nProd; i++){
        char nonTerm = productions[i][0];
        bool isDone = false;

        for(int j=0; j<nDone; j++){
            if(done[j] == nonTerm){
                isDone = true;
                break;
            }
        }
        if(isDone) continue;
        
        findFollow(nonTerm);
        printf("    Follow(%c) = {", nonTerm);
        
        int nPrinted = 0;
        follow[nDone][nPrinted++] = nonTerm;

        for(int j=0; j<nTempFollow; j++){
            bool isPrinted = false;

            for(int k=1; k<nPrinted; k++){
                if(follow[nDone][k] == tempFollow[j]){
                    isPrinted = true;
                    break;
                }
            }

            if(isPrinted) continue;

            printf("%c, ", tempFollow[j]);
            follow[nDone][nPrinted++] = tempFollow[j];
        }
        done[nDone++] = nonTerm;
        printf(" }\n");
    }
}


void findFirst(char c, int curProd, int nextChar){
    if(!isupper(c)){
        tempFirst[nTempFirst++] = c;
        return;
    }

    for(int i=0; i<nProd; i++){
        if(productions[i][0] == c){
            if(productions[i][0] == productions[i][2]){
                leftRec = true;
                continue;
            }
            if(productions[i][2] == '#'){
                if(productions[curProd][nextChar] == '\0' || (curProd==0 && nextChar==0)){
                    tempFirst[nTempFirst++] = '#';
                    eps = true;
                    printf("eps");
                }
                else {
                    findFirst(productions[curProd][nextChar], curProd, nextChar+1);
                }
            }
            else {
                findFirst(productions[i][2], i, 3);
            }
        }
    }

    if(eps == true && leftRec == true){
        // findFirst(productions[curProd][nextChar], curProd, nextChar+1);
        for(int i=0; i<nProd; i++){
            if(productions[i][0] == c && productions[i][0]==productions[i][2]){
                findFirst(productions[i][3], i, 4);
            }
        }
        
    }
}

void findFollow(char nonTerm){
    if(productions[0][0] == nonTerm){
        tempFollow[nTempFollow++] = '$'; 
    }

    for(int i=0; i<nProd; i++){
        for(int j=2; j<strlen(productions[i]); j++){
            if(productions[i][j] == nonTerm){
                if(productions[i][j+1] != '\0'){
                    // getFirst(productions[i][j+1], i, j+2);
                    getFirst(productions[i][j+1], i, j+2);
                }

                else if(nonTerm != productions[i][0]) {
                    findFollow(productions[i][0]);
                }
            }
        }
    }
}


void getFirst(char term, int curProd, int nextChar){
    if(!isupper(term)){
        tempFollow[nTempFollow++] = term;
        return;
    }

    for(int i=0; i < nFirst; i++){
            if(first[i][0] == term){
            for(int j=1; j<strlen(first[i]); j++){
                if(first[i][j] == '#'){
                    if(first[curProd][nextChar] == '\0') {
                        findFollow(first[curProd][0]);
                    }
                    else {
                        getFirst(first[curProd][nextChar], curProd, nextChar+1);
                    }
                }
                else {
                    tempFollow[nTempFollow++] = first[i][j];
                }
            }
        }
    }
}
