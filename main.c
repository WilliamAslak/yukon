#include <stdio.h>
//rand() from stdlib for shuffling the deck, and time used for the seed
#include <stdlib.h>
#include <time.h>
//to be able to use strlen.
#include <string.h>
//to find the current path of the directory
#include <unistd.h>

char *errorMessage = (NULL);

void printMap(char map[7][52][3],char score[4][2],char* lastCommand,char* msg, int showHidden){
    //Printing the 7 rows
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    for(int j = 0; j < 52; j++){
        //seen is used for breaking the forloop in case no item is found
        int seen = 0;
        //this nested for-loop checks the rows first and then iterates by column.
        for(int i = 0; i < 7; i++){
            //0 is empty, h is hidden.
            if(map[i][j][2]!='0'){
                seen = 1;
                if(map[i][j][2]=='h' && showHidden == 0){
                    printf("[]\t");
                } else
                    //.2 is used because when I copy the string it apparently likes to copy the ENTIRE array after index
                    printf("%.2s\t",map[i][j]);
            } else printf("  \t");
        }
        if(j%2==0 && j<=6){
            printf("\t%.2s  F%d",score[j/2],j/2+1);
        }
        //prints a new line.
        printf("\n");
        if(seen == 0 && j>6) break;
        }
    printf("LAST command: %s\n",lastCommand);
    printf("Message: %s\n",msg);
    printf("INPUT >");
}
//due to the fact that we can't take a pointers size we have to hardcode the value 💀💀💀
void shuffle(char bandit[][2],int size){
    //sets the seed of the random generator to the current time so we get a fresh deck everytime we shuffle
    srand(time(NULL));
    //shuffling the deck by iterating through the array and swapping the element with a randomly selected one
    for(int i = 0; i < size; i++){
        int random = rand()%52;
        char temp[2];
        //creates a temporary value
        temp[0] = bandit[random][0];
        temp[1] = bandit[random][1];
        //sets the value of what we took the temporary value of to another value
        bandit[random][0] = bandit[i][0];
        bandit[random][1] = bandit[i][1];
        //sets the other value to the temporary value effectively swapping the two values.
        bandit[i][0] = temp[0];
        bandit[i][1] = temp[1];

    }



}
void initializeMap(char map[7][52][3], char deck[52][2]){
    //checks where we are in the current deck, very important value since we don't use linked list.
    int place = 1;
    //in this nested for-loop we go column deep before changing row.
    for(int i = 0; i < 7; i++) {
        for (int j = 0; j < 52; j++) {
            //so we can have an increasing amount of hidden values
            if(j<i) {
                map[i][j][0]=deck[place][0];
                map[i][j][1]=deck[place][1];
                place++;
                map[i][j][2] = 'h';
            }
                //starts placing 5 values given by the deck, in columns 1-7 but not 0 as it only needs 1 value
            else if(j<i+5 && i>0){
                map[i][j][0]=deck[place][0];
                map[i][j][1]=deck[place][1];
                map[i][j][2]='s';
                //increment place so we can keep track where we are in the deck
                place++;
            } else {
                //if a value hasn't been given then set it to empty.
                map[i][j][2] = '0';
            }
        }
    }
    //sets the top left card to the first card in the deck.
    map[0][0][0] = deck[0][0];
    map[0][0][1] = deck[0][1];
    map[0][0][2] = 's';
    errorMessage = "OK";

}
void loadDeck(char deck[52][2], char* fileName){
    if(strlen(fileName) == 0) {
        errorMessage = "OK";
    } else {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            for(int i = sizeof(cwd)-1; i > 0; i--){
                if(cwd[i] == '\\') {
                    break;
                } else cwd[i] = '\0';
            }
            strcat(cwd,"load\\");
            strcat(cwd,fileName);
        } else {
            perror("getcwd() error");
        }

        // Open the file in read only mode
        FILE* file = fopen(cwd, "r");
        if (file != NULL) {
            // Read the deck data from the file
            for (int i = 0; i < 52; i++) {
                fscanf(file, "%s", deck[i]);
            }
            fclose(file);
            errorMessage = "OK";
        } else {
            errorMessage = "file not found";
        }
    }
}

//play phase stuff
//works only for, for example C#->C# type of commands
void moveCC(char map[7][52][3], const char* from, const char* to) {
    //column numbers set to variables, put a -1 cus of arrays
    int fromColumn = from[1]-1 - '0';
    int toColumn = to[1]-1 - '0';

    //finding top cards of the columns
    int topIndexFrom = -1;
    int topIndexTo = -1;
    for (int i = 51; i >= 0; i--) {
        if (map[fromColumn][i][2] != '0') {
            topIndexFrom = i;
            break;
        }
    }
    for (int i = 51; i >= 0; i--) {
        if (map[toColumn][i][2] != '0') {
            topIndexTo = i;
            break;
        }
    }
    //checking if legal operation
    char fromSuit = map[fromColumn][topIndexFrom][0];
    char fromValue = map[fromColumn][topIndexFrom][1];
    char toSuit = map[toColumn][topIndexTo][0];
    char toValue = map[toColumn][topIndexTo][1];
    printf("fromValue:%c\n",fromValue);

    if (fromSuit == toSuit) {
        errorMessage = "Same suit error";
        return;
    }
    int fromNumValue, toNumValue;
    if (fromValue == 'A') {
        fromNumValue = 1;
    } else if (fromValue == 'T') {
        fromNumValue = 10;
    } else if (fromValue == 'J') {
        fromNumValue = 11;
    } else if (fromValue == 'Q') {
        fromNumValue = 12;
    } else if (fromValue == 'K') {
        fromNumValue = 13;
    } else {
        fromNumValue = fromValue - '0';
    }

    if (toValue == 'A') {
        toNumValue = 1;
    } else if (toValue == 'T') {
        toNumValue = 10;
    } else if (toValue == 'J') {
        toNumValue = 11;
    } else if (toValue == 'Q') {
        toNumValue = 12;
    } else if (toValue == 'K') {
        toNumValue = 13;
    } else {
        toNumValue = toValue - '0';
    }
    printf("%d\n",fromNumValue);
    if (fromNumValue+1!=toNumValue) {
        errorMessage = "Illegal numerical operation";
        return;
    }
    //checking if the columns are empty
    if (topIndexFrom == -1) {
        errorMessage = "The source column is empty";
        return;
    }
    if (topIndexTo == -1) {
        errorMessage = "The destination column is empty";
        return;
    }
    //moves the top card from the "from" column to the "to" column
    map[toColumn][topIndexTo+1][0] = map[fromColumn][topIndexFrom][0];
    map[toColumn][topIndexTo+1][1] = map[fromColumn][topIndexFrom][1];
    map[toColumn][topIndexTo+1][2] = 's';
    map[fromColumn][topIndexFrom][2] = '0';
    if(topIndexFrom>0)
        if(map[fromColumn][topIndexFrom-1][2]=='h')
            map[fromColumn][topIndexFrom-1][2] = 's';
    errorMessage = "OK";
}

void moveCF(char map[7][52][3], char score[4][2], const char* from, const char* to) {
    //column numbers set to variables, put a -1 cus of arrays
    int fromColumn = from[1]-1 - '0';
    int toFoundation = to[1]-1 - '0';

    //finding top cards
    int topIndexFrom = -1;
    for (int i = 51; i >= 0; i--) {
        if (map[fromColumn][i][2] != '0') {
            topIndexFrom = i;
            break;
        }
    }
    //checking if legal operation
    char fromSuit = map[fromColumn][topIndexFrom][0];
    char fromValue = map[fromColumn][topIndexFrom][1];
    char toSuit = score[toFoundation][0];
    char toValue = score[toFoundation][1];

    if (score[toFoundation][0] == '[' && score[toFoundation][1] == ']') {
        //if destination foundation is empty, check if the card is an ace
        if (map[fromColumn][topIndexFrom][1] == 'A') {
            //move the card to the foundation
            score[toFoundation][0] = map[fromColumn][topIndexFrom][0];
            score[toFoundation][1] = map[fromColumn][topIndexFrom][1];
            map[fromColumn][topIndexFrom][2] = '0';
            errorMessage = "OK";
            return;
        }
        else {
            errorMessage = "Illegal foundation operation";
            return;
        }
    }
    int fromNumValue, toNumValue;
    if (fromValue == 'A') {
        fromNumValue = 1;
    } else if (fromValue == 'T') {
        fromNumValue = 10;
    } else if (fromValue == 'J') {
        fromNumValue = 11;
    } else if (fromValue == 'Q') {
        fromNumValue = 12;
    } else if (fromValue == 'K') {
        fromNumValue = 13;
    } else {
        fromNumValue = fromValue - '0';
    }

    if (toValue == 'A') {
        toNumValue = 1;
    } else if (toValue == 'T') {
        toNumValue = 10;
    } else if (toValue == 'J') {
        toNumValue = 11;
    } else if (toValue == 'Q') {
        toNumValue = 12;
    } else if (toValue == 'K') {
        toNumValue = 13;
    } else {
        toNumValue = toValue - '0';
    }
    if (fromNumValue-1!=toNumValue) {
        errorMessage = "Illegal numerical operation";
        return;
    }
    //checking if the columns are empty
    if (topIndexFrom == -1) {
        errorMessage = "The source column is empty";
        return;
    }
    if (fromSuit != toSuit) {
        errorMessage = "Not same suit error";
        return;
    }
    //moves the top card from the "from" column to the "to" column
    score[toFoundation][0] = map[fromColumn][topIndexFrom][0];
    score[toFoundation][1] = map[fromColumn][topIndexFrom][1];
    map[fromColumn][topIndexFrom][0] = '0';
    map[fromColumn][topIndexFrom][1] = '0';
    map[fromColumn][topIndexFrom][2] = '0';
    if(topIndexFrom>0)
        if(map[fromColumn][topIndexFrom-1][2]=='h')
            map[fromColumn][topIndexFrom-1][2] = 's';

    errorMessage = "OK";
}
void multiMoveCC(char map[7][52][3], char* from, char* value, char* to) {
    int fromColumn = from[1]-1 - '0';
    int toColumn = to[1]-1 - '0';

    int indexFrom = -1;
    int topIndexTo = -1;

    for (int i = 51; i >= 0; i--) {
        if (map[fromColumn][i][0] == value[0] && map[fromColumn][i][1] == value[1]) {
            indexFrom = i;
            break;
        }
    }
    for (int i = 51; i >= 0; i--) {
        if (map[toColumn][i][2] != '0') {
            topIndexTo = i;
            break;
        }
    }
    //checking if legal operation
    char fromSuit = map[fromColumn][indexFrom][0];
    char fromValue = map[fromColumn][indexFrom][1];
    char toSuit = map[toColumn][topIndexTo][0];
    char toValue = map[toColumn][topIndexTo][1];

    if (fromSuit == toSuit) {
        errorMessage = "Same suit error";
        return;
    }
    int fromNumValue, toNumValue;
    if (fromValue == 'A') {
        fromNumValue = 1;
    } else if (fromValue == 'T') {
        fromNumValue = 10;
    } else if (fromValue == 'J') {
        fromNumValue = 11;
    } else if (fromValue == 'Q') {
        fromNumValue = 12;
    } else if (fromValue == 'K') {
        fromNumValue = 13;
    } else {
        fromNumValue = fromValue - '0';
    }

    if (toValue == 'A') {
        toNumValue = 1;
    } else if (toValue == 'T') {
        toNumValue = 10;
    } else if (toValue == 'J') {
        toNumValue = 11;
    } else if (toValue == 'Q') {
        toNumValue = 12;
    } else if (toValue == 'K') {
        toNumValue = 13;
    } else {
        toNumValue = toValue - '0';
    }
    //multi mover 3000
    for (int i = indexFrom; i < 52; i++) {
        //copies card and visibility to the "toColumn"
        int nextEmpty = topIndexTo + 1;
        map[toColumn][nextEmpty][0] = map[fromColumn][i][0];
        map[toColumn][nextEmpty][1] = map[fromColumn][i][1];
        map[toColumn][nextEmpty][2] = map[fromColumn][i][2];

        //mark old slots as empty
        map[fromColumn][i][2] = '0';

        //update new top card
        topIndexTo = nextEmpty;
    }
    errorMessage = "OK";
}
int didIWin(char score[4][2]){
    int youWon = 1;
    for(int i = 0; i < 4; i++)
        if(score[i][1]!='K')
            youWon = 0;
    return youWon;
}


int main() {
    //the deck :)
    char deck[52][2] = {"CA", "DA", "HA", "SA", "C2", "D2", "H2", "S2",
                        "C3", "D3", "H3", "S3", "C4", "D4", "H4", "S4",
                        "C5", "D5", "H5", "S5", "C6", "D6", "H6", "S6",
                        "C7", "D7", "H7", "S7", "C8", "D8", "H8", "S8",
                        "C9", "D9", "H9", "S9", "CT", "DT", "HT", "ST",
                        "CJ", "DJ", "HJ", "SJ", "CQ", "DQ", "HQ", "SQ",
                        "CK", "DK", "HK", "SK"};
    char score[4][2]={"[]","[]","[]","[]"};
    /*
    //TEMP prints the deck :)
    for(int i = 0; i < 52; i++)
        printf("%.2s ", deck[i]);
    printf("\n");
    */

    //shuffles the deck, the reason we use size is because we might wanna shuffle later? 💀idk
    /*shuffle(deck,sizeof(deck)/2);
    for(int i = 0; i < 52; i++)
        printf("%.2s ", deck[i]);
    printf("\n");
    */
    //initialize 7 rows with 52 columns of 2 values
    //the maps values can either be: '0'(empty) 'h'(hidden) or the value given by the deck
    char map[7][52][3];
    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 52; j++)
            map[i][j][2] = '0';
    //initialize the playable map with the deck, and the hidden characters.
    //initializeMap(map,deck);
    //prints the map
    printMap(map,score,"nocommand",errorMessage,0);
    //de koder der virker indtil videre er
    //QQ som quitter spillet
    //LD og LD<filnavn> som loader enten decket eller fil
    //SR som shuffler decket
    //SW som toggler om der skal vises de skjulte kort
    //P som sætter spillet i playmode
    int game = 1,showHidden = 0;
    while(game > 0) {

        char *str = malloc(sizeof(char)*100);
        scanf("%s",str);
        printf("%s\n",str);
        //Because you're apparently not allowed to use switch case with strings we have to use an if-else ladder.
        if(strcmp(str,"QQ") == 0) {
            game = 0;
        } else if(game == 2){
            if(strcmp(str,"LD")==0||strcmp(str,"SR")==0||strcmp(str,"SW")==0||strcmp(str,"P")==0)
                errorMessage = "Command not available in PLAY phase";
            else if(strcmp(str,"Q")==0){
                game = 1;
                errorMessage = "Exit PLAY phase";
            }
                //example C6->C4
            else if (str[0] == 'C' && str[2] == '-' && str[4] == 'C' && strlen(str) == 6) {
                char from[3];
                char to[3];

                //ignoring the symbols like -> and just putting the locations into variables
                strncpy(from, &str[0], 2);
                strncpy(to, &str[4], 2);
                //function does the work
                moveCC(map, from, to);
            }
                //example C7->F2
            else if (str[0] == 'C' && str[2] == '-' && str[4] == 'F' && strlen(str) == 6) {
                char from[3];
                char to[3];

                strncpy(from, &str[0], 2);
                strncpy(to, &str[4], 2);

                moveCF(map, score, from, to);
            }
                //example C6:4H->C4
            else if (str[0] == 'C' && str[6] == '>' && str[7] == 'C' && strlen(str) == 9) {
                char from[3];
                char value[3];
                char to[3];

                strncpy(from, &str[0], 2);
                strncpy(value, &str[3], 2);
                strncpy(to, &str[7], 2);

                multiMoveCC(map, from,value, to);
            }
                //example C7:AS->F2
            else if (str[0] == 'C' && str[6] == '>' && str[7] == 'F' && strlen(str) == 9) {

            }
                //example F4->C6
            else if (str[0] == 'F' && str[3] == '>' && str[4] == 'C' && strlen(str) == 6) {

            }
            else {
                errorMessage = "Unrecognized input";
            }

            system("cls");
            printMap(map,score,str,errorMessage,showHidden);
            if(didIWin(score) == 1) {
                printf("\n-----------------------\nCONGRATULATIONS!!!! YOU WON THE GAME!\n-----------------------");
            game = 0;
            }
        } else {
            if(strcmp(str,"SW") == 0) {
                if (showHidden == 1)
                    showHidden = 0;
                else showHidden = 1;
            } else if(strcmp(str,"P") == 0){
                game = 2;
                showHidden = 0;
                initializeMap(map,deck);
                errorMessage = "Started play phase";

            } else if(str[0] == 'L' && str[1] == 'D'){
                if(str[2]=='<') {
                    char *subString = malloc(sizeof(char)*100);
                    //sizeof giver byte værdi med pointers :(
                    for(int i = 3; i < 100; i++){
                        if(str[i] == '>') {
                            subString[i-3] = '\0';
                            break;
                        }else
                            subString[i-3] = str[i];
                    }

                    loadDeck(deck,subString);
                    initializeMap(map,deck);
                    } else {
                    loadDeck(deck, "defaultDeck.txt");
                    initializeMap(map, deck);
                    }
            } else if(strcmp(str,"SR") == 0){
                shuffle(deck,sizeof(deck)/2);
                initializeMap(map,deck);
                errorMessage = "Deck shuffled";
            } else errorMessage = "Unrecognized command";
            system("cls");
            printMap(map,score,str,errorMessage,showHidden);


        }
    }
    printf("\nGoodbye!");
    return 0;
}

