#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "player_data.txt"

// Define a structure to represent a player
typedef struct {
    char name[MAX_NAME_LENGTH];
    int total_guesses;
} Player;

char magic_number[5]; // Store the magic number as a string
int total_guesses = 0; // Track the total number of guesses
Player players[MAX_PLAYERS]; // Array to store player data
int num_players = 0; // Track the number of players

// Function to generate a random 4-digit magic number
void generate_magic_number() {
    srand(time(NULL)); // Seed the random number generator with the current time
    for (int i = 0; i < 4; i++) {
        magic_number[i] = '0' + rand() % 10; // Generate a random digit between 0 and 9
    }
    magic_number[4] = '\0'; // Null-terminate the string
}

// Function to add a player with their name and total guesses to the players array
void add_player(const char *name) {
    if (num_players < MAX_PLAYERS) {
        strcpy(players[num_players].name, name);
        players[num_players].total_guesses = total_guesses;

        // Sort the players array based on total guesses
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

// Function to save player data to a file
void save_player_data() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Sort players by total guesses
    for (int i = 0; i < num_players - 1; i++) {
        for (int j = 0; j < num_players - i - 1; j++) {
            if (players[j].total_guesses > players[j + 1].total_guesses) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    fprintf(file, "%d\n", num_players);
    for (int i = 0; i < num_players; i++) {
        fprintf(file, "%s %d\n", players[i].name, players[i].total_guesses);
    }

    fclose(file);
}

// Function to load player data from a file
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

// Function to print game statistics
void print_stats() {
    float lucky_ratio = 1/(float)total_guesses; // Calculate the lucky ratio
    printf("Your lucky ratio: %f %% with %d total guesses\n", lucky_ratio*100, total_guesses);
    printf("Lucky ratio history (Top 5):\n");
    for (int i = 0; i < 5; i++) {
        float ratio = 1/(float)players[i].total_guesses; // Calculate player's lucky ratio
        printf("%d. %s: %0.2f %% with %d guesses.\n", i + 1, players[i].name, ratio*100, players[i].total_guesses);
    }
}

// Function to check a guess against the magic number
void check_guess(const char *guess) {
    total_guesses++; // Increment total guesses
    for (int i = 0; i < 4; i++) {
        if (guess[i] == magic_number[i]) {
            printf("%c", guess[i]); // Print the digit if it matches
        } else {
            printf("-"); // Print a dash if it doesn't match
        }
    }
    printf("\n");
}

// Main function
int main() {
    printf("----------------------------------------\n");
    printf("|\tGUESS A MAGIC NUMBER GAME\t|\n");
    printf("----------------------------------------\n");

    printf("I have a 4-digit magic number, can you guess it with the least number of guesses possible?\n");
    printf("Let's see your lucky ratio !!\n");

    generate_magic_number(); // Generate the magic number

    load_player_data(); // Load existing player data from file

    char name[MAX_NAME_LENGTH];
    char play_again;

    do {
        printf("Enter your name: ");
        fgets(name, sizeof(name), stdin); // Get player's name
        name[strcspn(name, "\n")] = '\0'; // Remove newline character from name

        char guess[5];
        printf("Enter your guess (4 digits): ");
        scanf("%s", guess); // Get player's guess

        while (strcmp(guess, "exit") != 0) {
            check_guess(guess); // Check the guess against the magic number
            if (strcmp(guess, magic_number) == 0) {
                printf("Congratulations! You've guessed the magic number!\n");
                add_player(name); // Add player to the list of players
                goto guessed;
            }
            printf("Enter your guess (4 digits): ");
            scanf("%s", guess); // Get next guess
        }
        printf("Don't give up. Let's try again\n");

        guessed:
        print_stats(); // Print game statistics

        total_guesses = 0; // Reset total guesses for next player
        printf("Do you want to play again? (Y/N): ");
        scanf(" %c", &play_again); // Get player's choice to play again
        getchar(); // Consume the newline character left in the input buffer

    } while (play_again == 'Y' || play_again == 'y');

    save_player_data(); // Save player data to file

    return 0;
}