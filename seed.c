#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char position[50];
    int votes;
} Candidate;

int main() {
    FILE *fp = fopen("data/candidates.dat", "wb"); // 'wb' overwrites fresh
    if (!fp) return 1;

    Candidate data[] = {
        {101, "Alice Otieno", "Chairperson", 45},
        {102, "Bob Kamau", "Chairperson", 38},
        {201, "Charlie Maina", "Secretary General", 60},
        {301, "David Mwangi", "Treasurer", 25}
    };

    fwrite(data, sizeof(Candidate), 4, fp);
    fclose(fp);
    printf("Sample data created in data/candidates.dat\n");
    return 0;
}