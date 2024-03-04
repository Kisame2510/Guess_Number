#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "player_data.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int total_guesses;
} Player;

char magic_number[5]; // one extra for null terminator
int total_guesses = 0;
//int base_ratio = 0;
Player players[MAX_PLAYERS];
int num_players = 0;

void generate_magic_number() {
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
        magic_number[i] = '0' + rand() % 10;
    }
    magic_number[4] = '\0';

    printf("%s \n", magic_number);
}

void add_player(const char *name) {
    if (num_players < MAX_PLAYERS) {
        strcpy(players[num_players].name, name);
        players[num_players].total_guesses = total_guesses;

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
        fprintf(file, "%s %d\n", players[i].name, players[i].total_guesses);
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
        fscanf(file, "%s %d", players[i].name, &players[i].total_guesses);
    }

    fclose(file);
}

void print_stats() {
    float lucky_ratio = (float)total_guesses/9999;
    printf("Your lucky ratio: %f with %d total guesses\n", lucky_ratio, total_guesses);
    printf("Lucky ratio history (Top 5):\n");
    for (int i = 0; i < num_players; i++) {
        float ratio = (float)players[i].total_guesses / 9999;
        printf("%d. %s: %f with %d guess.\n", i + 1, players[i].name, ratio, players[i].total_guesses);
    }
}

void check_guess(const char *guess) {
    total_guesses++;
    for (int i = 0; i < 4; i++) {
        if (guess[i] == magic_number[i]) {
            printf("%c", guess[i]);
        } else {
            printf("-");
        }
    }
    printf("\n");
}

int main() {
    printf("\tGUESS A MAGIC NUMBER GAME\n");
    generate_magic_number();

    load_player_data();

    char name[MAX_NAME_LENGTH];
    char play_again;

    do {
        printf("Enter your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // remove newline character from name


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

        add_player(name);

        print_stats();

        total_guesses = 0;
        printf("Do you want to play again? (Y/N): ");
        scanf(" %c", &play_again); // Note the space before %c to consume the newline character
        getchar(); // Consume the newline character left in the input buffer

    } while (play_again == 'Y' || play_again == 'y');

    save_player_data();

    return 0;
}
