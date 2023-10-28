#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct task {
    char title[50];
    int deadline;   // hrs
    int priority;   // 1 to 3 (importance)
    struct task* next;
    struct task* prev;
    bool status;
};


void addTask(struct task** head) {
    struct task* newTask = (struct task*)malloc(sizeof(struct task));
    newTask->next = NULL;
    newTask->status = false;

    printf("\nEnter task name: ");
    getchar(); // Consume newline character
    gets(newTask->title);

    printf("Enter task deadline (in hrs): ");
    scanf("%d", &(newTask->deadline));

    printf("Enter task priority (1 -> High, 2 -> Medium, 3 -> Low): ");
    scanf("%d", &(newTask->priority));

    if (*head == NULL) {
        newTask->prev = NULL;
        *head = newTask;
        return;
    }

    struct task* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newTask;
    newTask->prev = temp;
}

void sortTask(struct task** head) {
    struct task* temp = *head;
    int swapped, i;
    struct task* ptr1;
    struct task* lptr = NULL;

    if (temp == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = temp;

        while (ptr1->next != lptr) {
            // Compare by deadline first
            if (ptr1->deadline > ptr1->next->deadline ||
                (ptr1->deadline == ptr1->next->deadline && ptr1->priority > ptr1->next->priority)) {
                struct task* tempTask = ptr1->next;

                if (tempTask->next != NULL)
                    tempTask->next->prev = ptr1;
                ptr1->next = tempTask->next;
                tempTask->prev = ptr1;
                tempTask->next = ptr1;

                if (ptr1->prev != NULL)
                    ptr1->prev->next = tempTask;
                else
                    *head = tempTask;
                ptr1->prev = tempTask;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void completeTask(struct task** head) {
    char markTask[50];
    printf("Enter task name to mark as complete: ");
    getchar(); // Consume newline character
    gets(markTask);

    struct task* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->title, markTask) == 0) {
            temp->status = true;
            return;
        }
        temp = temp->next;
    }

    printf("Task not found.\n");
}

void removeTask(struct task** head) {
    char removeTask[50];
    printf("Enter task name to be deleted: ");
    getchar(); // Consume newline character
    gets(removeTask);

    struct task* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->title, removeTask) == 0) {
            if (temp->prev != NULL) {
                temp->prev->next = temp->next;
            }
            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            }
            if (temp == *head) {
                *head = temp->next;
            }
            free(temp);
            return;
        }
        temp = temp->next;
    }

    printf("Task not found.\n");
}

void displayTasks(struct task* head) {
    printf("%-30s%-15s%-15s%-10s\n", "Task Name", "Deadline (hrs)", "Priority", "Status");

    while (head != NULL) {
        printf("%-30s%-15d%-15d%-10s\n", head->title, head->deadline, head->priority, head->status ? "Complete" : "Incomplete");
        head = head->next;
    }
}

void saveTasksToFile(struct task* head) {
    char filename[50];
    printf("Enter the filename to save tasks: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    while (head != NULL) {
        fprintf(file, "%s %d %d %d\n", head->title, head->deadline, head->priority, head->status);
        head = head->next;
    }

    fclose(file);
    printf("Tasks have been saved to %s.\n", filename);
}

void loadTasksFromFile(struct task** head) {
    char filename[50];
    printf("Enter the filename to load tasks from: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file for reading.\n");
        return;
    }

    // Read task details from the file and create a linked list
    while (!feof(file)) {
        struct task* newTask = (struct task*)malloc(sizeof(struct task));
        if (fscanf(file, "%s %d %d %d", newTask->title, &(newTask->deadline), &(newTask->priority), &(newTask->status)) != 4) {
            free(newTask);
            break;
        }
        newTask->next = NULL;

        if (*head == NULL) {
            newTask->prev = NULL;
            *head = newTask;
        } else {
            struct task* temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newTask;
            newTask->prev = temp;
        }
    }

    fclose(file);
    printf("Tasks have been loaded from %s.\n", filename);
}

int main() {
    struct user* userList = NULL;
    struct task* taskList = NULL;

    int choice;
    do {
        printf("\nTask Management Menu:\n");
        printf("1. Add Task\n");
        printf("2. Sort Tasks\n");
        printf("3. Complete Task\n");
        printf("4. Remove Task\n");
        printf("5. Display Tasks\n");
        printf("6. Save Tasks to File\n");
        printf("7. Load Tasks from File\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(&taskList);
                break;
            case 2:
                sortTask(&taskList);
                displayTasks(taskList); // Display sorted tasks
                break;
            case 3:
                completeTask(&taskList);
                break;
            case 4:
                removeTask(&taskList);
                break;
            case 5:
                displayTasks(taskList);
                break;
            case 6:
                saveTasksToFile(taskList);
                break;
            case 7:
                loadTasksFromFile(&taskList);
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 8);

    return 0;
}
