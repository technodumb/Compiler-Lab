#include<stdio.h>
#include<string.h>
#include<stdbool.h>

int nStates, nTerminals, equiTable[30][30];

char table[30][30], minTable[30][30];

bool isFinal[30];

void main(){
    printf("Enter the number of states: ");
    scanf("%d", &nStates);
    printf("Enter the number of terminals: ");
    scanf("%d", &nTerminals);

    printf("\nEnter the DFA Transition Table:\n\n");
    for(int i=0;i<nTerminals;i++)
        printf("\t%d", i);
    printf("Is Final? (y/n)\n");
    for(int i=0;i<nStates;i++) {
        printf("%c\t", i+'A');
        isFinal[i] = false;

        for(int j=0;j<nTerminals+1;j++) {
            char temp[2];
            scanf("%s", temp);
            table[i][j] = temp[0];
            if(j==nTerminals) {
                if(temp[0]=='y') {
                    // finalStates[nFinal++] = i;
                    isFinal[i] = true;
                }
            }
        }
    }
    for(int i=0;i<nStates;i++) {
        for(int j=0;j<nStates;j++) {
            if(i==j) {
                equiTable[i][j] = 0;
            }
            else if(isFinal[i] != isFinal[j]) {
                equiTable[i][j] = 1;
            }
            else {
                equiTable[i][j] = 2;
            }
        }
    }

    int nCrossed=0, nCrossedPrev=0;

    do{
        nCrossedPrev = nCrossed;
        for(int i=0; i<nStates; i++){
            for(int j=0; j<nStates; j++){
                if(equiTable[i][j] != 2)
                    continue;
                for(int k=0; k<nTerminals; k++){
                    int state1 = table[i][k] - 'A';
                    int state2 = table[j][k] - 'A';

                    if(equiTable[state1][state2] == 1){
                        equiTable[i][j] = 1;
                        equiTable[j][i] = 1;
                        nCrossed++;
                        break;
                    }
                }

            }
        }
    }
    while(nCrossedPrev != nCrossed);

    printf("\n\nEquivalent Table:\n\n");
    for(int i=0; i<nStates; i++){
        for(int j=0; j<nStates; j++){
            printf("%d ", equiTable[i][j]);
        }
        printf("\n");
    }
    
    // find the teams
    int teams[30];
    int nTeams = 0;
    for(int i=0; i<nStates; i++)
        teams[i] = -1;

    for(int i=0; i<nStates; i++){
        if(teams[i] != -1)
            continue;
        teams[i] = nTeams++;
        for(int j=0; j<nStates; j++){
            if(equiTable[i][j] == 2){
                teams[j] = teams[i];
                equiTable[i][j] = 0;
                equiTable[j][i] = 0;
            }
        }
    }

    for(int i=0; i<nStates; i++){
        for(int j=0; j<nTerminals; j++){
            // for(int k=0; k<nStates; k++){
            //     if(table[i][j] - 'A' == k){
            //         // table[k][j] = teams[i] + 'A';
            //         minTable[teams[i]][j] = teams[k] + 'A';
            //     }
            // }
            minTable[teams[i]][j] = teams[table[i][j] - 'A'] + 'A';
        }
    }

    // print the teams
    printf("\n\nTeams:\n\n");
    for(int i=0; i<nStates; i++){
        printf("%c: %d\n", i+'A', teams[i]);
    }

    // print the minimized DFA
    printf("\n\nMinimized DFA:\n\n");
    for(int i=0;i<nTerminals;i++)
        printf("\t%d", i);
    printf("\tIs Final? (y/n)\n");
    for(int i=0;i<nTeams;i++) {
        printf("%c\t", i+'A');
        for(int j=0;j<nTerminals;j++) {
            printf("%c\t", minTable[i][j]);
            // printf("%c\t", teams[table[i][j]-'A']+'A');
        }
        printf("%c\t", isFinal[i]?'y':'n');
        printf("\n");
    }
}