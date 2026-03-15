#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- SONU POSITIONS DATA --- */
const char* SONU_POSITIONS[] = {
    "Chairperson",
    "Vice-Chairperson",
    "Secretary General",
    "Treasurer",
    "Sports and Social Welfare Secretary",
    "Health and Accommodation Secretary"
};

typedef struct {
    int id;
    char name[50];
    char position[50];
    int votes;
} Candidate;

/* --- MODULES (Modular Design) --- */
void register_candidate() {
    FILE *fp = fopen("data/candidates.dat", "ab");
    if (!fp) { printf("Error opening data file!\n"); return; }

    Candidate c;
    int pos_choice;

    printf("\n--- SONU Candidate Registration ---\n");
    printf("Enter Name: ");
    scanf(" %[^\n]s", c.name);
    
    printf("Select Position (1-6):\n");
    for(int i=0; i<6; i++) printf("%d. %s\n", i+1, SONU_POSITIONS[i]);
    scanf("%d", &pos_choice);
    
    if(pos_choice < 1 || pos_choice > 6) { printf("Invalid position.\n"); fclose(fp); return; }

    strcpy(c.position, SONU_POSITIONS[pos_choice-1]);
    c.id = rand() % 1000;
    c.votes = 0;

    fwrite(&c, sizeof(Candidate), 1, fp);
    fclose(fp);
    printf("[✓] Registered! Candidate ID: %d\n", c.id);
}

void tally_results() {
    FILE *fp = fopen("data/candidates.dat", "rb");
    if (!fp) { printf("No election data found.\n"); return; }
    
    Candidate c;
    printf("\n--- OFFICIAL TALLY ---\n");
    while(fread(&c, sizeof(Candidate), 1, fp)) {
        printf("[%s] %s: %d votes\n", c.position, c.name, c.votes);
    }
    fclose(fp);
}

int main() {
    int choice;
    while(1) {
        printf("\n=== SONU VOTING PORTAL 2026 ===\n");
        printf("1. Register Candidate\n2. View Results\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: register_candidate(); break;
            case 2: tally_results(); break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}