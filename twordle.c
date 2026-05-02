#include <stdio.h>
#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6

void loadWord(char mysteryWord[]);
void toLowerCase(char word[]);
bool isValidGuess(char guess[]);
int stringLength(char str[]);
bool isLetter(char c);
bool compareWords(char guess[], char mysteryWord[]);
void storeGuess(char guesses[MAX_GUESSES][WORD_LENGTH + 1], char guess[], int guessNum);
void displayGuesses(char guesses[MAX_GUESSES][WORD_LENGTH + 1], int numGuesses, char mysteryWord[]);
bool letterInWord(char c, char mysteryWord[]);
void copyString(char dest[], char src[]);

int main() { 
	char mysteryWord[WORD_LENGTH + 1];
    	char guess[WORD_LENGTH + 2];
    	char guesses[MAX_GUESSES][WORD_LENGTH + 1];
    	int guessCount = 0;
    	bool won = false;
	loadWord(mysteryWord);
	
    	printf("Welcome to Twordle!\n");
	while (guessCount < MAX_GUESSES && !won) {
        	displayGuesses(guesses, guessCount, mysteryWord);
        	printf("Enter guess #%d: ", guessCount + 1);
        	scanf("%s", guess);
        	toLowerCase(guess);
		while (!isValidGuess(guess)) {
           		printf("Invalid guess. Enter a 5-letter word: ");
            		scanf("%s", guess);
            		toLowerCase(guess);
        	}
        	storeGuess(guesses, guess, guessCount);
        	if (compareWords(guess, mysteryWord)) {
            		won = true;
        	}
        	guessCount++;
    	}
	displayGuesses(guesses, guessCount, mysteryWord);
	if (won) {
        	printf("Congratulations! You guessed the word!\n");
    	} else {
        	printf("Out of guesses! The word was: %s\n", mysteryWord);
    	}
    return 0;
}

void loadWord(char mysteryWord[]) {
	FILE *file = fopen("mystery.txt", "r");
	if (file == NULL) {
        	printf("Error: Could not open mystery.txt\n");
        	return;
    	}
    	fscanf(file, "%5s", mysteryWord);
    	fclose(file);
   	toLowerCase(mysteryWord);
}

void toLowerCase(char word[]) {
	int i = 0;
	while (word[i] != '\0') {
        	if (word[i] >= 'A' && word[i] <= 'Z') {
            		word[i] = word[i] + ('a' - 'A');
        	}
        	i++;
    	}
}

bool isLetter(char c) {	
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int stringLength(char str[]) {
	int length = 0;
	while (str[length] != '\0') {
        	length++;
	}
    return length;
}

bool isValidGuess(char guess[]) {
	int i;
	if (stringLength(guess) != WORD_LENGTH) {
        	return false;
    	}
    	for (i = 0; i < WORD_LENGTH; i++) {
        	if (!isLetter(guess[i])) {
           		return false;
        	}
    	}
    return true;
}

bool compareWords(char guess[], char mysteryWord[]) {
	int i;
	for (i = 0; i < WORD_LENGTH; i++) {
        	if (guess[i] != mysteryWord[i]) {
            	return false;
        	}
    	}
    return true;
}

void storeGuess(char guesses[MAX_GUESSES][WORD_LENGTH + 1], char guess[], int guessNum) {
	copyString(guesses[guessNum], guess);
}

void copyString(char dest[], char src[]) {
	int i = 0;
	while (src[i] != '\0') {
        	dest[i] = src[i];
        	i++;
    	}
    	dest[i] = '\0';
}

bool letterInWord(char c, char mysteryWord[]) {
	int i;
	for (i = 0; i < WORD_LENGTH; i++) {
       		if (mysteryWord[i] == c) {
            	return true;
        	}
    	}
    return false;
}

void displayGuesses(char guesses[MAX_GUESSES][WORD_LENGTH + 1], int numGuesses, char mysteryWord[]) {
	int i, j;
    	printf("\nPrevious Guesses:\n");
    	for (i = 0; i < numGuesses; i++) {
	for (j = 0; j < WORD_LENGTH; j++) {
        	char displayChar = guesses[i][j];
            	if (guesses[i][j] == mysteryWord[j]) {
                	displayChar = guesses[i][j] - ('a' - 'A');
            	}
            	printf("%c", displayChar);
        }
        printf("\n");
        for (j = 0; j < WORD_LENGTH; j++) {
        	if (guesses[i][j] == mysteryWord[j]) {
                	printf(" ");
            	} else if (letterInWord(guesses[i][j], mysteryWord)) {
                	bool alreadyCorrect = false;
                	int k;
                	for (k = 0; k < WORD_LENGTH; k++) {
				if (guesses[i][j] == mysteryWord[k] && guesses[i][k] == mysteryWord[k]) {
                        		alreadyCorrect = true;
                    		}
                	}
              		if (!alreadyCorrect) {
                    		printf("^");
                	} else {
                    		printf(" ");
                	}
            		} else {
                		printf(" ");
            		}
            		}
        	printf("\n");
    }
    printf("\n");
}










