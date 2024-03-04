#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "player_data.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int correct_guesses;
    int total_guesses;
} Player;

char magic_number[5]; // one extra for null terminator
int total_guesses = 0;
int correct_guesses = 0;
Player players[MAX_PLAYERS];
int num_players = 0;

void generate_magic_number() {
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
        magic_number[i] = '0' + rand() % 10;
    }
    magic_number[4] = '\0';
}

void add_player(const char *name) {
    if (num_players < MAX_PLAYERS) {
        strcpy(players[num_players].name, name);
        players[num_players].correct_guesses = 0;
        players[num_players].total_guesses = 0;

        // Find the correct position to insert the new player based on total_guesses
        int i = num_players - 1;
        while (i >= 0 && players[i].total_guesses > players[i+1].total_guesses) {
            Player temp = players[i];
            players[i] = players[i+1];
            players[i+1] = temp;
            i--;
        }

        num_players++;
    }
}

void save_player_data() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d\n", num_players);
    for (int i = 0; i < num_players; i++) {
        fprintf(file, "%s %d %d\n", players[i].name, players[i].correct_guesses, players[i].total_guesses);
    }

    fclose(file);
}

void load_player_data() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing player data found.\n");
        return;
    }

    fscanf(file, "%d", &num_players);
    for (int i = 0; i < num_players; i++) {
        fscanf(file, "%s %d %d", players[i].name, &players[i].correct_guesses, &players[i].total_guesses);
    }

    fclose(file);
}

void print_stats() {
    float lucky_ratio = (float)correct_guesses / total_guesses;
    printf("Your lucky ratio: %.2f (%d/%d)\n", lucky_ratio, correct_guesses, total_guesses);
    printf("Lucky ratio history (Top 5):\n");
    for (int i = 0; i < num_players; i++) {
        float ratio = (float)players[i].correct_guesses / players[i].total_guesses;
        printf("%d. %s: %.2f (%d/%d)\n", i + 1, players[i].name, ratio, players[i].correct_guesses, players[i].total_guesses);
    }
}

void check_guess(const char *guess) {
    total_guesses++;
    for (int i = 0; i < 4; i++) {
        if (guess[i] == magic_number[i]) {
            printf("%c", guess[i]);
            correct_guesses++;
            players[num_players - 1].correct_guesses++;
        } else {
            printf("-");
        }
    }
    printf("\n");
    players[num_players - 1].total_guesses++;
}

int main() {
    printf("\tGUESS A MAGIC NUMBER GAME\n");
    load_player_data();

    char name[MAX_NAME_LENGTH];
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // remove newline character from name


    add_player(name);

    generate_magic_number();

    char guess[5];
    printf("Enter your guess (4 digits): ");
    scanf("%s", guess);

    while (strcmp(guess, "exit") != 0) {
        check_guess(guess);
        if (strcmp(guess, magic_number) == 0) {
            printf("Congratulations! You've guessed the magic number!\n");
            break;
        }
        printf("Enter your guess (4 digits): ");
        scanf("%s", guess);
    }

    print_stats();

    save_player_data();

    return 0;
}
