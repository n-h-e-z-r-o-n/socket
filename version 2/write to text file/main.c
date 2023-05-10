#include <stdio.h>
#include <string.h>



int checkDuplicate(char serialNumber[], char regNumber[]) {
    FILE *file;
    char line[100];
    char tempSerialNumber[100], tempRegNumber[100];
    file = fopen("data.txt", "r"); // open file in read mode
    if (file == NULL) {
        return 0; // return 0 if file cannot be opened
    }
    while (fgets(line, 100, file) != NULL) {
        sscanf(line, "%s %s", tempSerialNumber, tempRegNumber); // read serial number and registration number from file
        if (strcmp(tempSerialNumber, serialNumber) == 0 || strcmp(tempRegNumber, regNumber) == 0) {
            fclose(file);

            return 1; // return 1 if either serial number or registration number is a duplicate
        }
    }
    fclose(file);
    return 0; // return 0 if neither serial number nor registration number is a duplicate
}

void write_to_text_file(char serial_n[], char reg_n[], char name[]){
     FILE *file;
     file = fopen("data.txt", "a"); // open file in append mode
     if (file == NULL) {
         printf("Error opening file.\n");
     }
     fprintf(file, "%s %s %s", serial_n, reg_n, name); // write details to file
     fclose(file);
     printf("Details saved to file.\n");
}

int main() {
    char serialNumber[100], regNumber[100], name[100];

    printf("Enter serial number: ");
    fgets(serialNumber, 100, stdin);

    printf("Enter registration number: ");
    fgets(regNumber, 100, stdin);

    printf("Enter name: ");
    fgets(name, 100, stdin);

    // Remove newline character from name
    serialNumber[strcspn(serialNumber, "\n")] = '\0'; // Remove newline character from serialNumber
    regNumber[strcspn(regNumber, "\n")] = '\0'; // Remove newline character from regNumber

    if (checkDuplicate(serialNumber, regNumber)) {
        printf("Serial number or registration number is a duplicate.\n");
    } else {
      write_to_text_file(serialNumber, regNumber, name);
    }

    return 0;
}
