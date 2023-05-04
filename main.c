#include <stdio.h>
//rand() from stdlib for shuffling the deck, and time used for the seed
#include <stdlib.h>
#include <time.h>

void printMap(char map[7][52][2]){
    //Printing the 7 rows
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    for(int j = 0; j < 52; j++){
        //seen is used for breaking the forloop in case no item is found
        int seen = 0;
        //this nested for-loop checks the rows first and then iterates by column.
        for(int i = 0; i < 7; i++){
            //0 is empty, h is hidden.
            if(map[i][j][0]!='0'){
                seen = 1;
                if(map[i][j][0]=='h'){
                    printf("[]\t");
                } else
                    //.2 is used because when I copy the string it apparently likes to copy the ENTIRE array after index
                    printf("%.2s\t",map[i][j]);
            } else printf("  \t");
        }
        if(seen == 0) break;
        //prints a new line.
        printf("\n");
        }

}
//due to the fact that we can't take a pointers value we have to hardcode the value 💀💀💀
void shuffle(char bandit[][2],int size){
    printf("\n");
    //sets the seed of the random generator to the current time so we get a fresh deck everytime we shuffle
    srand(time(NULL));
    //shuffling the deck by iterating through the array and swapping the element with a randomly selected one
    for(int i = 0; i < size; i++){
        int random = rand()%53;
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

    //TEMP prints the shuffled deck
    printf("\n");
    for(int i = 0; i < 52; i++)
        printf("%.2s ", bandit[i]);

}
void initializeMap(char map[7][52][2], char deck[52][2]){
    //checks where we are in the current deck, very important value since we don't use linked list.
    int place = 1;
    //in this nested for-loop we go column deep before changing row.
    for(int i = 0; i < 7; i++) {
        for (int j = 0; j < 52; j++) {
            //so we can have an increasing amount of hidden values
            if(j<i)
                map[i][j][0]='h';
                //starts placing 5 values given by the deck, in columns 1-7 but not 0 as it only needs 1 value
            else if(j<i+5 && i>0){
                map[i][j][0]=deck[place][0];
                map[i][j][1]=deck[place][1];
                //increment place so we can keep track where we are in the deck
                place++;
            } else
                //if a value hasn't been given then set it to empty.
                map[i][j][0] = '0';
        }
    }
    //sets the top left card to the first card in the deck.
    map[0][0][0] = deck[0][0];
    map[0][0][1] = deck[0][1];

}

int main() {
    //the deck :)
    char deck[52][2]={"CA","DA","HA","SA","C2","D2","H2","S2",
                      "C3","D3","H3","S3","C4","D4","H4","S4",
                      "C5","D5","H5","S5","C6","D6","H6","S6",
                      "C7","D7","H7","S7","C8","D8","H8","S8",
                      "C9","D9","H9","S9","CT","DT","HT","ST",
                      "CJ","DJ","HJ","SJ","CQ","DQ","HQ","SQ",
                      "CK","DK","HK","SK"};

    //TEMP prints the deck :)
    for(int i = 0; i < 52; i++)
        printf("%.2s ", deck[i]);
    printf("\n");
    //shuffles the deck, the reason we use size is because we might wanna shuffle later? 💀idk
    shuffle(deck,sizeof(deck)/2);
    printf("\n");

    //initialize 7 rows with 52 columns of 2 values
    //the maps values can either be: '0'(empty) 'h'(hidden) or the value given by the deck
    char map[7][52][2];
    initializeMap(map,deck);

    //prints the map
    printMap(map);
    //TEMP - random test 💰💰
    printf (" \n The random number is: %d", rand()%53);


    return 0;
}

