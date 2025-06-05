#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "patient.txt"
#define DOCTORFILE "doctor.txt"

typedef struct {
    int id;
    char name[50];
    int age;
    char symptoms[5][50];
    int symptomCount;
    char appointmentDate[20];
} Patient;

char *getDoctorBySymptom(char *symptom) {
    static char doctorInfo[256];
    FILE *dfp = fopen(DOCTORFILE, "r");
    char line[512];
    char fileSymptom[50];
    char doctorID[10], doctorName[50], department[50], specialization[50], contact[50], patientIDs[256];

    if (!dfp) {
        strcpy(doctorInfo, "General Physician (No doctor file)");
        return doctorInfo;
    }

    while (fgets(line, sizeof(line), dfp)) {
        sscanf(line, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[\n]",
               doctorID, doctorName, department, specialization, contact, fileSymptom, patientIDs);

        if (strcmp(symptom, fileSymptom) == 0) {
            fclose(dfp);
            snprintf(doctorInfo, sizeof(doctorInfo),
                     "%s - %s, %s - Contact: %s", doctorName, department, specialization, contact);
            return doctorInfo;
        }
    }

    fclose(dfp);
    strcpy(doctorInfo, "General Physician");
    return doctorInfo;
}

void assignDoctor(char symptom[]) {
    char *doctor = getDoctorBySymptom(symptom);
    printf(" -> Assigned to: %s\n", doctor);
}

void writePatient(FILE *fp, Patient p) {
    fprintf(fp, "ID: %d\n", p.id);
    fprintf(fp, "Name: %s\n", p.name);
    fprintf(fp, "Age: %d\n", p.age);
    fprintf(fp, "Symptoms:");
    for (int i = 0; i < p.symptomCount; i++) {
        fprintf(fp, "%s%s", (i > 0 ? "," : ""), p.symptoms[i]);
    }
    fprintf(fp, "\nAppointment: %s", p.appointmentDate); 
    fprintf(fp, "\n");
}

Patient readPatient(FILE *fp) {
    Patient p;
    char line[256], symptoms[256];

    
    do {
        if (!fgets(line, sizeof(line), fp)) return p;
    } while (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0);

    sscanf(line, "ID: %d", &p.id);
    fgets(line, sizeof(line), fp); sscanf(line, "Name: %[^\n]", p.name);
    fgets(line, sizeof(line), fp); sscanf(line, "Age: %d", &p.age);
    fgets(line, sizeof(line), fp); sscanf(line, "Symptoms:%[^\n]", symptoms);
    fgets(line, sizeof(line), fp); sscanf(line, "Appointment: %[^\n]", p.appointmentDate);

    p.symptomCount = 0;
    char *token = strtok(symptoms, ",");
    while (token && p.symptomCount < 5) {
        strcpy(p.symptoms[p.symptomCount++], token);
        token = strtok(NULL, ",");
    }

    return p;
}

void addPatient() {
    Patient p;
    FILE *fp = fopen(FILENAME, "r");
    char line[256];

    printf("Enter Patient ID: ");
    scanf("%d", &p.id);

    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            int id;
            if (sscanf(line, "ID: %d", &id) == 1 && id == p.id) {
                printf("Patient ID already exists.\n");
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", p.name);
    printf("Enter Age: ");
    scanf("%d", &p.age);

    p.symptomCount = 0;
    int count;
    printf("How many symptoms? ");
    scanf("%d", &count);
    for (int i = 0; i < count && i < 5; i++) {
        printf("Enter symptom %d: ", i + 1);
        scanf(" %[^\n]", p.symptoms[i]);
        p.symptomCount++;
    }

    printf("Enter appointment date (YYYY-MM-DD): ");
    scanf(" %[^\n]", p.appointmentDate);

    fp = fopen(FILENAME, "a+");
    if (!fp) {
        printf("Error opening file for writing.\n");
        return;
    }

    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if (size > 0) {
        fseek(fp, -1, SEEK_END);
        char lastChar;
        if (fread(&lastChar, 1, 1, fp) == 1 && lastChar != '\n') {
            fprintf(fp, "\n");
        }
    }

    writePatient(fp, p);
    fclose(fp);

    printf("Patient added.\n");
}


void viewHistory() {
    int id;
    printf("Enter Patient ID (or 0 to search by name): ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int found = 0;
    char searchName[50];
    if (id == 0) {
        printf("Enter patient name to search: ");
        scanf(" %[^\n]", searchName);
    }

    while (!feof(fp)) {
        Patient p = readPatient(fp);
        if (feof(fp)) break;

        if ((id != 0 && p.id == id) || (id == 0 && strcmp(p.name, searchName) == 0)) {
            found = 1;

            printf("\n--- Patient Record ---\n");
            printf("ID: %d\nName: %s\nAge: %d\nSymptoms:\n", p.id, p.name, p.age);
            for (int i = 0; i < p.symptomCount; i++) {
                printf(" - %s", p.symptoms[i]);
                assignDoctor(p.symptoms[i]);
                printf("\n");
            }
            printf("Appointment Date: %s\n", p.appointmentDate);
            break;
        }
    }

    if (!found) printf("Patient not found.\n");
    fclose(fp);
}

void updatePatient() {
    int id;
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error opening temp file.\n");
        fclose(fp);
        return;
    }
    int found = 0;

    while (!feof(fp)) {
        Patient p = readPatient(fp);
        if (feof(fp)) break;

        if (p.id == id) {
            found = 1;
            printf("Updating patient %s (ID: %d)\n", p.name, p.id);
            printf("Enter new age: ");
            scanf("%d", &p.age);
            printf("How many new symptoms? ");
            scanf("%d", &p.symptomCount);
            for (int i = 0; i < p.symptomCount && i < 5; i++) {
                printf("Enter symptom %d: ", i + 1);
                scanf(" %[^\n]", p.symptoms[i]);
            }
            printf("Enter new appointment date (YYYY-MM-DD): ");
            scanf(" %[^\n]", p.appointmentDate);
        }

        writePatient(temp, p);
    }

    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Patient updated.\n");
    else
        printf("Patient not found.\n");
}

void deletePatient() {
    int id;
    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error opening temp file.\n");
        fclose(fp);
        return;
    }
    int found = 0;

    while (!feof(fp)) {
        Patient p = readPatient(fp);
        if (feof(fp)) break;

        if (p.id == id) {
            found = 1;
            continue;
        }

        writePatient(temp, p);
    }

    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Patient deleted.\n");
    else
        printf("Patient not found.\n");
}

int main() {
    int choice;
    do {
        printf("\n--- Hospital Record Management ---\n");
        printf("1. Add Patient\n2. View History\n3. Update Patient\n4. Delete Patient\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewHistory(); break;
            case 3: updatePatient(); break;
            case 4: deletePatient(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}