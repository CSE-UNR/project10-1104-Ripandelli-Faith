#include <stdio.h>
#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6
#define MAX_WORDS 100
#define MAX_PLAYERS 5
#define NAME_LENGTH 20

void loadRandomWord(char mysteryWord[]);
int pseudoRandom(int max);
void toLowerCase(char word[]);
bool isValidGuess(char guess[]);
int stringLength(char str[]);
bool isLetter(char c);
bool compareWords(char guess[], char mysteryWord[]);
void storeGuess(char guesses[MAX_GUESSES][WORD_LENGTH + 1], char guess[], int guessNum);
void displayGuesses(char guesses[MAX_GUESSES][WORD_LENGTH + 1], int numGuesses, char mysteryWord[]);
void copyString(char dest[], char src[]);
void updateScoreboard(char playerName[], int score);
void displayScoreboard();
void sortScores(char names[MAX_PLAYERS][NAME_LENGTH], int scores[MAX_PLAYERS]);

int main() {
    char mysteryWord[WORD_LENGTH + 1];
    char guess[WORD_LENGTH + 2];
    char guesses[MAX_GUESSES][WORD_LENGTH + 1];
    char playerName[NAME_LENGTH];

    int guessCount = 0;
    bool won = false;

    printf("Enter your name: ");
    scanf("%s", playerName);

    loadRandomWord(mysteryWord);

    printf("\nWelcome to Twordle Extra Credit Edition!\n");

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
        printf("Congratulations, %s! You guessed the word in %d guesses!\n", playerName, guessCount);
        updateScoreboard(playerName, guessCount);
    } else {
        printf("Out of guesses! The word was: %s\n", mysteryWord);
    }

    displayScoreboard();

    return 0;
}

void loadRandomWord(char mysteryWord[]) {
    FILE *file = fopen("mystery.txt", "r");
    char words[MAX_WORDS][WORD_LENGTH + 1];
    int count = 0;
    int chosen;

    if (file == NULL) {
        printf("Error opening mystery.txt\n");
        return;
    }

    while (fscanf(file, "%s", words[count]) != EOF && count < MAX_WORDS) {
        toLowerCase(words[count]);
        count++;
    }

    fclose(file);

    chosen = pseudoRandom(count);
    copyString(mysteryWord, words[chosen]);
}

int pseudoRandom(int max) {
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) % 2147483647;
    return seed % max;
}

void toLowerCase(char word[]) {
    int i = 0;

    while (word[i] != '\0') {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] += ('a' - 'A');
        }
        i++;
    }
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

int stringLength(char str[]) {
    int count = 0;

    while (str[count] != '\0') {
        count++;
    }

    return count;
}

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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

void displayGuesses(char guesses[MAX_GUESSES][WORD_LENGTH + 1], int numGuesses, char mysteryWord[]) {
    int i, j, k;

    printf("\nPrevious Guesses:\n");

    for (i = 0; i < numGuesses; i++) {

        bool matched[WORD_LENGTH] = {false, false, false, false, false};
       
        for (j = 0; j < WORD_LENGTH; j++) {
            char displayChar = guesses[i][j];

            if (guesses[i][j] == mysteryWord[j]) {
                displayChar -= ('a' - 'A');
                matched[j] = true;
            }

            printf("%c", displayChar);
        }

        printf("\n");

        for (j = 0; j < WORD_LENGTH; j++) {

            if (guesses[i][j] == mysteryWord[j]) {
                printf(" ");
            } else {
                bool found = false;

                for (k = 0; k < WORD_LENGTH; k++) {
                    if (!matched[k] && guesses[i][j] == mysteryWord[k]) {
                        found = true;
                        matched[k] = true;
                        break;
                    }
                }

                if (found) {
                    printf("^");
                } else {
                    printf(" ");
                }
            }
        }

        printf("\n");
    }

    printf("\n");
}

void updateScoreboard(char playerName[], int score) {
    FILE *file = fopen("scores.txt", "r");
    char names[MAX_PLAYERS + 1][NAME_LENGTH];
    int scores[MAX_PLAYERS + 1];
    int count = 0, i;

    if (file != NULL) {
        while (fscanf(file, "%s %d", names[count], &scores[count]) != EOF && count < MAX_PLAYERS) {
            count++;
        }
        fclose(file);
    }

    copyString(names[count], playerName);
    scores[count] = score;
    count++;

    sortScores(names, scores);

    file = fopen("scores.txt", "w");

    for (i = 0; i < count && i < MAX_PLAYERS; i++) {
        fprintf(file, "%s %d\n", names[i], scores[i]);
    }

    fclose(file);
}


void sortScores(char names[MAX_PLAYERS][NAME_LENGTH], int scores[MAX_PLAYERS]) {
    int i, j, tempScore;
    char tempName[NAME_LENGTH];

    for (i = 0; i < MAX_PLAYERS - 1; i++) {
        for (j = i + 1; j < MAX_PLAYERS; j++) {

            if (scores[j] < scores[i]) {

                tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;

                copyString(tempName, names[i]);
                copyString(names[i], names[j]);
                copyString(names[j], tempName);
            }
        }
    }
}
void displayScoreboard() {
    FILE *file = fopen("scores.txt", "r");
    char name[NAME_LENGTH];
    int score;

    printf("\n--- TOP 5 SCOREBOARD ---\n");

    if (file == NULL) {
        printf("No scores yet.\n");
        return;
    }

    while (fscanf(file, "%s %d", name, &score) != EOF) {
        printf("%s - %d guesses\n", name, score);
    }

    fclose(file);
}

