#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Constants and File Paths --- */
#define CANDIDATES_FILE "data/candidates.dat"
#define VOTERS_FILE "data/voters.dat"
#define CSV_EXPORT_FILE "data/election_results.csv"

/* --- SONU Official Positions --- */
const char* SONU_POSITIONS[] = {
    "Chairperson",
    "Vice-Chairperson",
    "Secretary General",
    "Treasurer",
    "Sports and Social Welfare Secretary",
    "Health and Accommodation Secretary"
};

/* --- Data Structures (File Design) --- */
typedef struct {
    int id;
    char name[50];
    char position[50];
    int votes;
} Candidate;

typedef struct {
    char student_id[20];
    char password[20];
    int has_voted; // 0 = No, 1 = Yes
} Voter;

/* --- MODULES (Modular Design) --- */

void register_candidate() {
    FILE *fp = fopen(CANDIDATES_FILE, "ab");
    if (!fp) { printf("[!] Error: Ensure 'data' folder exists.\n"); return; }

    Candidate c;
    int pos_choice;
    printf("\n--- SONU Candidate Registration ---\n");
    printf("Enter Name: ");
    scanf(" %[^\n]s", c.name);
    
    printf("Select Position (1-6):\n");
    for(int i=0; i<6; i++) printf("%d. %s\n", i+1, SONU_POSITIONS[i]);
    printf("Choice: "); scanf("%d", &pos_choice);
    
    if(pos_choice < 1 || pos_choice > 6) { printf("Invalid position.\n"); fclose(fp); return; }

    strcpy(c.position, SONU_POSITIONS[pos_choice-1]);
    c.id = rand() % 1000 + 100; 
    c.votes = 0;

    fwrite(&c, sizeof(Candidate), 1, fp);
    fclose(fp);
    printf("[✓] Success! Candidate ID: %d assigned.\n", c.id);
}

void register_voter() {
    FILE *fp = fopen(VOTERS_FILE, "ab");
    if (!fp) { printf("[!] Error accessing voter records.\n"); return; }
    
    Voter v;
    printf("\n--- Voter Registration ---\n");
    printf("Enter Student ID: "); scanf("%s", v.student_id);
    printf("Create Password: "); scanf("%s", v.password);
    v.has_voted = 0;

    fwrite(&v, sizeof(Voter), 1, fp);
    fclose(fp);
    printf("[✓] Voter registered successfully!\n");
}

void cast_vote() {
    char sid[20], pass[20];
    printf("\n--- Voter Login ---\n");
    printf("Student ID: "); scanf("%s", sid);
    printf("Password: "); scanf("%s", pass);

    FILE *vfp = fopen(VOTERS_FILE, "rb+");
    if (!vfp) { printf("[!] No registered voters found.\n"); return; }

    Voter v;
    int voter_found = 0;
    while (fread(&v, sizeof(Voter), 1, vfp)) {
        if (strcmp(v.student_id, sid) == 0 && strcmp(v.password, pass) == 0) {
            if (v.has_voted) {
                printf("[!] Error: You have already voted!\n");
                fclose(vfp); return;
            }
            voter_found = 1; break;
        }
    }

    if (!voter_found) { printf("[!] Invalid Credentials.\n"); fclose(vfp); return; }

    FILE *cfp = fopen(CANDIDATES_FILE, "rb+");
    if (!cfp) { printf("[!] No candidates found.\n"); fclose(vfp); return; }

    Candidate c;
    int choice_id, vote_success = 0;
    printf("\n--- SONU BALLOT ---\n");
    while (fread(&c, sizeof(Candidate), 1, cfp)) {
        printf("ID: %d | [%s] %s\n", c.id, c.position, c.name);
    }
    printf("Enter Candidate ID to vote: "); scanf("%d", &choice_id);

    rewind(cfp);
    while (fread(&c, sizeof(Candidate), 1, cfp)) {
        if (c.id == choice_id) {
            c.votes++;
            fseek(cfp, -sizeof(Candidate), SEEK_CUR);
            fwrite(&c, sizeof(Candidate), 1, cfp);
            vote_success = 1; break;
        }
    }

    if (vote_success) {
        v.has_voted = 1;
        fseek(vfp, -sizeof(Voter), SEEK_CUR);
        fwrite(&v, sizeof(Voter), 1, vfp);
        printf("[✓] Vote cast successfully for ID %d!\n", choice_id);
    } else {
        printf("[!] Invalid Candidate ID.\n");
    }

    fclose(vfp);
    fclose(cfp);
}

void tally_results() {
    FILE *fp = fopen(CANDIDATES_FILE, "rb");
    if (!fp) { printf("[!] No election data found.\n"); return; }
    
    Candidate c;
    printf("\n--- OFFICIAL SONU TALLY ---\n");
    while(fread(&c, sizeof(Candidate), 1, fp)) {
        printf("[%s] %s: %d votes\n", c.position, c.name, c.votes);
    }
    fclose(fp);
}

void export_to_csv() {
    FILE *bfp = fopen(CANDIDATES_FILE, "rb");
    if (!bfp) { printf("[!] No data to export.\n"); return; }

    FILE *cfp = fopen(CSV_EXPORT_FILE, "w");
    if (!cfp) { printf("[!] Error creating CSV.\n"); fclose(bfp); return; }

    Candidate c;
    fprintf(cfp, "Candidate ID,Name,Position,Votes\n");
    while (fread(&c, sizeof(Candidate), 1, bfp)) {
        fprintf(cfp, "%d,%s,%s,%d\n", c.id, c.name, c.position, c.votes);
    }

    fclose(bfp);
    fclose(cfp);
    printf("[✓] Exported to 'data/election_results.csv' for Excel.\n");
}

/* --- MAIN INTERFACE (Top-Down Flow) --- */
int main() {
    int choice;
    while(1) {
        printf("\n====================================");
        printf("\n    SONU VOTING PORTAL 2026       ");
        printf("\n====================================");
        printf("\n1. Candidate Registration");
        printf("\n2. Voter Registration");
        printf("\n3. Cast Vote");
        printf("\n4. View Results");
        printf("\n5. Export to Excel (CSV)");
        printf("\n6. Exit");
        printf("\nChoice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear invalid input
            continue;
        }
        
        switch(choice) {
            case 1: register_candidate(); break;
            case 2: register_voter(); break;
            case 3: cast_vote(); break;
            case 4: tally_results(); break;
            case 5: export_to_csv(); break;
            case 6: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}