#include<stdio.h>
#include<string.h>

int nStates, nTerminals, finalStates[20], nFinal=0;
char table[20][20][20], dfaTable[20][20][20];
char epsClosure[20];

void removeDuplicates(char *str){
    // sort
    for(int i=0; i<strlen(str); i++){
        for(int j=i+1; j<strlen(str); j++){
            if(str[i] > str[j]){
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }

    // remove duplicates
    int curind=0;
    for(int i=1; i<strlen(str); i++){
        if(str[i] != str[curind]){
            str[++curind] = str[i];
        }
    }
    str[++curind] = 0;
}

void getEpsilonClosureRecur(int state){
    if(table[state][nTerminals][0] != '-'){
        strcat(epsClosure, table[state][nTerminals]);
        printf("epsClosure: %s, state:%d\n", epsClosure, state);
        for(int j=0; j<strlen(table[state][nTerminals]); j++){
            getEpsilonClosureRecur(table[state][nTerminals][j] - 'A');
        }
    }
}

void getEpsilonClosure(int state){
    epsClosure[0] = state+'A';
    epsClosure[1] = 0;
    getEpsilonClosureRecur(state);
    removeDuplicates(epsClosure);
}


void main(){
    // terminals: 0, 1, 2...
    // states: a, b, c...
    printf("Enter the number of states: ");
    scanf("%d", &nStates);
    printf("Enter the number of terminals: ");
    scanf("%d", &nTerminals);

    printf("\nEnter the Table:\n\n");
	for(int i=0;i<nTerminals;i++)
		printf("\t%d", i);
	printf("\tepsln\tIs Final? (y/n)\n");
	for(int i=0;i<nStates;i++) {
		printf("%c\t", ('A'+i));
		for(int j=0;j<nTerminals+2;j++) {
			scanf("%s",table[i][j]);
		}

        if(table[i][nTerminals+1][0]=='y')
            finalStates[nFinal++]=i;
	}

    // convert to nfa without epsilon
    // steps to do that:
    // 1. find epsilon closure of each state
    // 2. for each state, for each element in its epsilon closure, for each terminal, find the union of the epsilon closure of the next state
    // 3. Check if it is a final state

    // 1. find epsilon closure of each state
    for(int i=0; i<nStates; i++){
        getEpsilonClosure(i);
        strcpy(table[i][nTerminals+2], epsClosure);
    }

    // 2. for each state, for each element in its epsilon closure, for each terminal, find the union of the epsilon closure of the next state
    for(int i=0; i<nStates; i++){
        for(int j=0; j<nTerminals; j++){
            char temp[20] = "";
            for(int k=0; k<strlen(table[i][nTerminals+2]); k++){
                int nextState = table[i][nTerminals+2][k] - 'A';
                
                if(table[nextState][j][0] != '-'){
                    // getEpsilonClosure(table[nextState][j][0] - 'A');
                    strcat(temp, table[table[nextState][j][0] - 'A'][nTerminals+2]);
                }
                printf("temp: %s\n", temp);
                // remove duplicates
            }
            removeDuplicates(temp);
            if(strlen(temp) == 0){
                strcpy(table[i][j], "-");
            } else {
                strcpy(table[i][j], temp);
            }
        }
    }

    // // 3. Check if it is a final state
    for(int i=0; i<nFinal; i++){
        for(int j=0; j<nStates; j++){
            for(int k=0; k<strlen(table[j][nTerminals+2]); k++){
                if(table[j][nTerminals+2][k] - 'A' == finalStates[i]){
                    table[j][nTerminals+1][0] = 'y';
                    break;
                }
            }
        }
    }

    int nDfaStates = 0;
    strcpy(dfaTable[nDfaStates++][0], table[0][nTerminals+2]);


    for(int i=0; i<nDfaStates; i++){
        for(int j=0; j<nTerminals; j++){
            char temp[20] = "";
            for(int k=0; k<strlen(dfaTable[i][0]); k++){
                int curState = dfaTable[i][0][k] - 'A';
                if(table[curState][j][0]!='-'){
                    strcat(temp, table[curState][j]);
                }
            }
            removeDuplicates(temp);

            strcpy(dfaTable[i][j+1], "-");

            int flag = 1;
            for(int k=0; k<nDfaStates; k++){
                if(strcmp(dfaTable[k][0], temp) == 0){
                    // strcpy(dfaTable[i][j+1], k+'A');
                    dfaTable[i][j+1][0] = k+'A';
                    flag = 0;
                    break;
                }
            }
            if(flag){
                dfaTable[i][j+1][0] = nDfaStates+'A';
                strcpy(dfaTable[nDfaStates++][0], temp);
            }
        }

        // check if final
        for(int j=0; j<nFinal; j++){
            dfaTable[i][nTerminals+1][0] = 'n';
            for(int k=0; k<strlen(dfaTable[i][0]); k++){
                if(dfaTable[i][0][k] - 'A' == finalStates[j]){
                    dfaTable[i][nTerminals+1][0] = 'y';
                    break;
                }
            }
        }
    }

    // print the dfa Table

    printf("\n\nDFA Table:\n\n");
    for(int i=0;i<nTerminals;i++)
        printf("\t%d", i);

    printf("\tIs Final? (y/n)\n");
    for(int i=0;i<nDfaStates;i++) {
        printf("%c\t", i+'A');
        for(int j=1;j<nTerminals+2;j++) {
            printf("%s\t", dfaTable[i][j]);
        }
        printf("\n");
    }
}