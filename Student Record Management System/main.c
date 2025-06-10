#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define FILE_PATH "../"
#define FILE_NAME "students.txt"

typedef struct {
    char name[51];
    int is_deleted;
} Student;

void Help();

void Add(Student student);

void List();

void Edit();

bool ClearInputBuffer();

int main(void) {
    printf("Hello, what can i help with today?\n");
    printf("Type -h for commands\n");

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        char command[3] = {0}, input[50] = {0}, line[53];
        fgets(line, sizeof(line), stdin);


        if (strchr(line, '\n') == NULL) {
            if (ClearInputBuffer()) {
                printf("Unknown command\n");
                continue;
            }
        }

        int numScanned = sscanf(line, "%2s %50[^\n]", command, input);

        if (numScanned == 1) {
            switch (command[1]) {
                case 'h': Help();
                    break;
                case 'l': List();
                    break;
                case 'e':
                    Edit();
                    printf("You exited edit mode\n");
                    break;
                default:
                    printf("Unknown command: %s\n", command);
                    break;
            }
        } else if (numScanned == 2) {
            switch (command[1]) {
                case 'a':
                    Student s1;
                    strncpy(s1.name, input, sizeof(s1.name) - 1);
                    s1.name[sizeof(s1.name) - 1] = '\0'; // Ensure null-termination
                    s1.is_deleted = 0;
                    Add(s1);
                    break;
                default:
                    printf("Unknown command: %s\n", command);
                    break;
            }
        } else {
            // Handle invalid input
        }
    }
}

void Help() {
    printf("Available commands:\n");
    printf("-a <person> : to add a student\n");
    printf("-l          : to list all students\n");
    printf("-e          : to enter edit mode\n");
}

void Add(Student student) {
    char fullPATH[PATH_MAX];
    snprintf(fullPATH, sizeof(fullPATH), "%s%s", FILE_PATH, FILE_NAME);
    FILE *fp = fopen(fullPATH, "ab");

    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    fwrite(&student, sizeof(Student), 1, fp);

    fclose(fp);
}

void List() {
    char fullPATH[PATH_MAX];
    snprintf(fullPATH, sizeof(fullPATH), "%s%s", FILE_PATH, FILE_NAME);
    FILE *fp = fopen(fullPATH, "rb");

    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    Student student;
    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        printf("%s\n", student.name);
    }

    fclose(fp);
}

void EditHelp();

Student GetStudentAt(int index);

int IncreaseIndex(int index);

int DecreaseIndex(int index);

int EditDeleteStudent(int index);

void EditUpdateStudent(int index);

void EditClearRemovedStudentsFromFile();

void Edit() {
    printf("you entered edit mode, type -h to see the commands\n");

    int currentIndex = IncreaseIndex(-1);

    while (true) {
        Student student = GetStudentAt(currentIndex);

        char command[3] = {0}, line[4];

        if (strlen(student.name) == 0 || student.is_deleted) {
            printf("No students.\n");
            sscanf("-e", "%2s", command);
        } else {
            printf("Currently editing: \"%s\"\n", student.name);

            fgets(line, sizeof(line), stdin);

            if (strchr(line, '\n') == NULL) {
                if (ClearInputBuffer()) {
                    printf("Unknown command\n");
                    continue;
                }
            }
            sscanf(line, "%2s", command);
        }

        switch (command[1]) {
            case 'h':
                EditHelp();
                break;
            case 'e':
                EditClearRemovedStudentsFromFile();
                return;
            case 'n':
                currentIndex = IncreaseIndex(currentIndex);
                break;
            case 'p':
                currentIndex = DecreaseIndex(currentIndex);
                break;
            case 'd':
                currentIndex = EditDeleteStudent(currentIndex);
                break;
            case 'u':
                EditUpdateStudent(currentIndex);
                break;
            default:
                printf("Unknown command: %s\n", command);
                break;
        }
    }
}

void EditHelp() {
    printf("Available commands:\n");
    printf("-e : to exit\n");
    printf("-n : to go to the next student\n");
    printf("-p : to go to the previous student\n");
    printf("-d : delete the current student\n");
    printf("-u : to update the current student\n");
}

Student GetStudentAt(int index) {
    char fullPATH[PATH_MAX];
    snprintf(fullPATH, sizeof(fullPATH), "%s%s", FILE_PATH, FILE_NAME);

    FILE *fp = fopen(fullPATH, "rb");
    fseek(fp, index * sizeof(Student), SEEK_SET);

    Student student;

    if (fread(&student, sizeof(Student), 1, fp) != 1) {
        // Failed to read, return empty
        fclose(fp);
        memset(&student, 0, sizeof(Student));
    }

    fclose(fp);

    return student;
}


int IncreaseIndex(int index) {
    Student s;
    do {
        index++;
        s = GetStudentAt(index);
        if (strlen(s.name) == 0) {
            index--;
            s = GetStudentAt(index);
            if (s.is_deleted == 1) {
                return DecreaseIndex(index);
            }
            break; // If invalid, maybe end of file
        }
    } while (s.is_deleted == 1);

    return index;
}

int DecreaseIndex(int index) {
    Student s;
    do {
        index--;
        if (index < 0) return IncreaseIndex(index);
        s = GetStudentAt(index);
    } while (s.is_deleted == 1);

    return index;
}

int EditDeleteStudent(int index) {
    Student s1 = GetStudentAt(index);
    s1.is_deleted = 1;

    char fullPATH[PATH_MAX];
    snprintf(fullPATH, sizeof(fullPATH), "%s%s", FILE_PATH, FILE_NAME);
    FILE *fp = fopen(fullPATH, "rb+");

    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    fseek(fp, index * sizeof(Student), SEEK_SET);

    fwrite(&s1, sizeof(Student), 1, fp);

    fclose(fp);

    return DecreaseIndex(index);
}

void EditUpdateStudent(int index) {
    printf("Please enter the new name of the student\n");

    char name[50] = {0}, line[51];
    fgets(line, sizeof(line), stdin);

    if (strchr(line, '\n') == NULL) {
        if (ClearInputBuffer()) {
            printf("To long name\n");
            return;
        }
    }

    sscanf(line, "%50[^\n]", name);

    Student s1;
    strncpy(s1.name, name, sizeof(s1.name) - 1);
    s1.name[sizeof(s1.name) - 1] = '\0'; // Ensure null-termination
    s1.is_deleted = 0;

    char fullPATH[PATH_MAX];
    snprintf(fullPATH, sizeof(fullPATH), "%s%s", FILE_PATH, FILE_NAME);
    FILE *fp = fopen(fullPATH, "rb+");

    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    fseek(fp, index * sizeof(Student), SEEK_SET);

    fwrite(&s1, sizeof(Student), 1, fp);

    fclose(fp);
}

void EditClearRemovedStudentsFromFile() {
    //Create new temp file
    char tempPATH[PATH_MAX];
    snprintf(tempPATH, sizeof(tempPATH), "%s%s%s", FILE_PATH, "temp_", FILE_NAME);
    FILE *fpNew = fopen(tempPATH, "ab");

    if (fpNew == NULL) {
        perror("Failed to open temp file");
        return;
    }

    char oldPATH[PATH_MAX];
    snprintf(oldPATH, sizeof(oldPATH), "%s%s", FILE_PATH, FILE_NAME);
    FILE *fpOld = fopen(oldPATH, "rb+");

    if (fpOld == NULL) {
        perror("Failed to open file");
        return;
    }

    //Run through old file and copy those that are not deleted
    Student student;
    while (fread(&student, sizeof(Student), 1, fpOld) == 1) {
        if (student.is_deleted == 0) {
            fwrite(&student, sizeof(Student), 1, fpNew);
        }
    }

    fclose(fpNew);
    fclose(fpOld);

    //Delete old file
    remove(oldPATH);

    //Rename temp file
    rename(tempPATH, oldPATH);
}

bool ClearInputBuffer() {
    int ch;
    bool somethingLeft = false;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        somethingLeft = true;
    }
    return somethingLeft;
}
