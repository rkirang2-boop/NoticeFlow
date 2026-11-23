
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH "../data/notices.txt"

struct Notice {
    int id;
    char title[100];
    char description[300];
    char category[30];
    char priority[20];
    struct Notice *next;
};

struct Notice *head = NULL;

// Add notice to linked list
void addNotice(int id, char title[],
               char description[],
               char category[],
               char priority[]) {

    struct Notice *newNotice = malloc(sizeof(struct Notice));

    if (newNotice == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newNotice->id = id;

    snprintf(newNotice->title, sizeof(newNotice->title), "%s", title);
    snprintf(newNotice->description, sizeof(newNotice->description), "%s", description);
    snprintf(newNotice->category, sizeof(newNotice->category), "%s", category);
    snprintf(newNotice->priority, sizeof(newNotice->priority), "%s", priority);

    newNotice->next = NULL;

    if (head == NULL) {
        head = newNotice;
    } else {
        struct Notice *temp = head;

        while (temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = newNotice;
    }
}

// Save notices to file
void saveNotices() {

    FILE *file = fopen(FILE_PATH, "w");

    if (file == NULL) {
        printf("Unable to open notice file for saving.\n");
        return;
    }

    struct Notice *temp = head;

    while (temp != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s\n",
                temp->id,
                temp->title,
                temp->description,
                temp->category,
                temp->priority);

        temp = temp->next;
    }

    fclose(file);

    printf("Notices saved successfully!\n");
}

// Load notices from file
void loadNotices() {

    FILE *file = fopen(FILE_PATH, "r");

    if (file == NULL) {
        printf("No previous notice file found.\n");
        return;
    }

    char line[600];

    while (fgets(line, sizeof(line), file) != NULL) {

        int id;
        char title[100];
        char description[300];
        char category[30];
        char priority[20];

        if (sscanf(line, "%d|%99[^|]|%299[^|]|%29[^|]|%19[^\n]",
                   &id, title, description, category, priority) == 5) {

            addNotice(id, title, description, category, priority);
        }
    }

    fclose(file);

    printf("Saved notices loaded successfully!\n");
}

// Display all notices
void displayNotices() {

    struct Notice *temp = head;

    printf("\n===== NOTICEFLOW NOTICES =====\n");

    if (temp == NULL) {
        printf("No notices available.\n");
        return;
    }

    while (temp != NULL) {

        printf("\nID: %d\n", temp->id);
        printf("Title: %s\n", temp->title);
        printf("Description: %s\n", temp->description);
        printf("Category: %s\n", temp->category);
        printf("Priority: %s\n", temp->priority);

        temp = temp->next;
    }
}

// Search notice
void searchNotice(char searchTitle[]) {

    struct Notice *temp = head;
    int found = 0;

    while (temp != NULL) {

        if (strstr(temp->title, searchTitle) != NULL) {
            printf("\nFound: %s\n", temp->title);
            printf("Category: %s\n", temp->category);
            printf("Priority: %s\n", temp->priority);
            found = 1;
        }

        temp = temp->next;
    }

    if (!found) {
        printf("Notice not found!\n");
    }
}

// Display urgent notices first
void displayPriorityNotices() {

    struct Notice *temp;

    printf("\n===== PRIORITY NOTICES =====\n");

    for (int priority = 0; priority < 2; priority++) {

        temp = head;

        while (temp != NULL) {

            if ((priority == 0 &&
                 strcmp(temp->priority, "Urgent") == 0) ||
                (priority == 1 &&
                 strcmp(temp->priority, "Normal") == 0)) {

                printf("%s - %s\n",
                       temp->title, temp->priority);
            }

            temp = temp->next;
        }
    }
}

// Free linked list memory
void freeNotices() {

    struct Notice *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {

    printf("\n===== WELCOME TO NOTICEFLOW =====\n");

    // Load previous notices
    loadNotices();

    // Add sample notices only when file is empty
    if (head == NULL) {

        addNotice(1, "College Fest",
                  "Annual college fest registration is open.",
                  "Events", "Urgent");

        addNotice(2, "Exam Update",
                  "Internal examination timetable announced.",
                  "Education", "Normal");

        addNotice(3, "Water Maintenance",
                  "Water supply will be interrupted tomorrow.",
                  "Maintenance", "Urgent");
    }

    displayNotices();

    searchNotice("College");

    displayPriorityNotices();

    // Save notices permanently
    saveNotices();

    freeNotices();

    printf("\nNoticeFlow completed successfully!\n");

    return 0;
}
