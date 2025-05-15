#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char username[30];
    char password[20];
} User;

typedef struct Student
{
    char id[20];
    char name[30];
    char gender[10];
    float scores[5];
    float total;
    float average;
    int rank;
    struct Student *next;
} Student;

// 函数声明
// 1. 用户登录相关
int login(char currentUser[]);
int verifyUser(const char *username, const char *password);
void saveUsers(User *users, int count);
int loadUsers(User **users);
void exportGradesToCSV(Student *head);
void registerUser();
void deleteUser();
void changePassword();
void viewCurrentUser(char currentUser[]);

// 2. 学生信息管理
Student *createStudentList();
Student *addStudent(Student *head);
Student *deleteStudentById(Student *head, const char *id);
Student *deleteStudentByName(Student *head, const char *name);
Student *findStudentById(Student *head, const char *id);
Student *findStudentByName(Student *head, const char *name);
Student *modifyStudentById(Student *head, const char *id);
Student *modifyStudentByName(Student *head, const char *name);
void displayAllStudents(Student *head);
void freeStudentList(Student *head);

// 3. 成绩统计与分析
void calculateTotalAndAverage(Student *student);
void calculateAllTotalAndAverage(Student *head);
void rankStudentsByTotal(Student *head);
Student *sortStudentsById(Student *head);
Student *sortStudentsByTotal(Student *head);
void displayStudentsWithScoreAbove(Student *head, float score, int course_index);
void displayStudentsWithScoreBelow(Student *head, float score, int course_index);
void calculateCourseAverages(Student *head, float averages[5]);
void displayStudentsBelowAverage(Student *head, float averages[5]);
void displayFailingStudents(Student *head);

// 4. 文件操作
int saveStudentsToFile(Student *head, const char *filename);
Student *loadStudentsFromFile(const char *filename);

// 5. 菜单和主程序
void displayMenu();
void processChoice(int choice, Student **head, char currentUser[]);

int main()
{
    int choice;
    Student *head = NULL;
    char currentUser[30] = "";
    int loggedIn = 0;
    int exitFlag = 0;

    loggedIn = login(currentUser);
    if (!loggedIn)
    {
        printf("Login failed, program exiting!\n");
        return 1;
    }

    head = loadStudentsFromFile("students.txt");
    if (head == NULL)
    {
        printf("No student data found or data is empty, new student list will be created.\n");
        head = createStudentList();
    }

    // 主菜单循环，till选择退出
    do
    {
        displayMenu();
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        if (choice == 0)
        {
            char confirm[10];
            printf("Are you sure you want to exit? (yes/no): ");
            scanf("%s", confirm);
            if (strcmp(confirm, "yes") == 0)
            {
                exitFlag = 1;
            }
            else
            {
                continue;
            }
        }
        else
        {
            processChoice(choice, &head, currentUser);
        }
    } while (!exitFlag);

    if (saveStudentsToFile(head, "students.txt"))
    {
        printf("Data saved successfully!\n");
    }
    else
    {
        printf("Data saving failed!\n");
    }

    freeStudentList(head);

    printf("Thank you for using this system, goodbye!\n");
    return 0;
}

int login(char currentUser[])
{
    char username[30];
    char password[20];
    int attempts = 0;

    while (attempts < 3)
    {
        printf("Please enter username: ");
        scanf("%s", username);
        printf("Please enter password: ");
        scanf("%s", password);

        if (verifyUser(username, password))
        {
            strcpy(currentUser, username);
            printf("Login successful! Welcome to the student grade management system.\n");
            return 1;
        }
        else
        {
            attempts++;
            printf("Username or password incorrect, you have %d more attempts.\n", 3 - attempts);
        }
    }

    printf("Attempts exceeded, login failed!\n");
    return 0;
}

int verifyUser(const char *username, const char *password)
{
    User *users = NULL;
    int count = loadUsers(&users);

    if (count <= 0)
    {
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            User defaultUser;
            strcpy(defaultUser.username, "admin");
            strcpy(defaultUser.password, "admin");
            saveUsers(&defaultUser, 1);
            return 1;
        }
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0)
        {
            free(users);
            return 1;
        }
    }

    free(users);
    return 0;
}

void displayMenu()
{
    printf("\n=== Student Grade Management System ===\n");
    printf("1. Add student information\n");
    printf("2. Find student by ID\n");
    printf("3. Find student by name\n");
    printf("4. Modify student information\n");
    printf("5. Delete student information\n");
    printf("6. Display all student information\n");
    printf("7. Sort by ID\n");
    printf("8. Sort by total score\n");
    printf("9. Display course statistics\n");
    printf("10. Display students above specified score\n");
    printf("11. Display failing students\n");
    printf("12. Display students below average\n");
    printf("13. Save data to file\n");
    printf("14. Register new user\n");
    printf("15. Delete user\n");
    printf("16. Change password\n");
    printf("17. View current user\n");
    printf("18. Export grades to CSV\n");
    printf("0. Exit system\n");
}

void processChoice(int choice, Student **head, char currentUser[])
{
    char id[20];
    char name[30];
    float averages[5];

    switch (choice)
    {
    case 0:
        printf("Saving data and exiting...\n");
        break;
    case 1:
        *head = addStudent(*head);
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
        break;
    case 2:
        while (1)
        {
            printf("Please enter the ID of the student to find (or -1 to return to main menu): ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("Returning to main menu.\n");
                break;
            }
            findStudentById(*head, id);
        }
        break;
    case 3:
        while (1)
        {
            printf("Please enter the name of the student to find (or -1 to return to main menu): ");
            scanf("%s", name);
            if (strcmp(name, "-1") == 0)
            {
                printf("Returning to main menu.\n");
                break;
            }
            findStudentByName(*head, name);
        }
        break;
    case 4:
        while (1)
        {
            printf("Please enter the ID of the student to modify (or -1 to return to main menu): ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("Returning to main menu.\n");
                break;
            }
            *head = modifyStudentById(*head, id);
            calculateAllTotalAndAverage(*head);
            rankStudentsByTotal(*head);
        }
        break;
    case 5:
        while (1)
        {
            printf("Please enter the ID of the student to delete (or -1 to return to main menu): ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("Returning to main menu.\n");
                break;
            }
            *head = deleteStudentById(*head, id);
            calculateAllTotalAndAverage(*head);
            rankStudentsByTotal(*head);
        }
        break;
    case 6:
        displayAllStudents(*head);
        break;
    case 7:
        *head = sortStudentsById(*head);
        displayAllStudents(*head);
        break;
    case 8:
        *head = sortStudentsByTotal(*head);
        displayAllStudents(*head);
        break;
    case 9:
        calculateCourseAverages(*head, averages);
        printf("\nCourse averages:\n");
        for (int i = 0; i < 5; i++)
        {
            printf("Course %d: %.2f\n", i + 1, averages[i]);
        }
        break;
    case 10:
    {
        int course_index;
        printf("Please enter course number (1-5, or -1 to return to main menu): ");
        scanf("%d", &course_index);
        if (course_index == -1)
        {
            printf("Returning to main menu.\n");
            break;
        }
        if (course_index < 1 || course_index > 5)
        {
            printf("Invalid course number!\n");
            break;
        }
        displayStudentsWithScoreAbove(*head, 0, course_index - 1); // 0无实际意义
    }
    break;
    case 11:
        displayFailingStudents(*head);
        break;
    case 12:
        calculateCourseAverages(*head, averages);
        displayStudentsBelowAverage(*head, averages);
        break;
    case 13:
        if (saveStudentsToFile(*head, "students.txt"))
        {
            printf("Data saved successfully!\n");
        }
        else
        {
            printf("Data saving failed!\n");
        }
        break;
    case 14:
        registerUser();
        break;
    case 15:
        deleteUser();
        break;
    case 16:
        changePassword();
        break;
    case 17:
        viewCurrentUser(currentUser);
        break;
    case 18:
        exportGradesToCSV(*head);
        break;
    default:
        printf("Invalid choice, please enter again!\n");
    }
}

Student *createStudentList()
{
    return NULL;
}

// 二次确认
int confirmReturn()
{
    char confirm[10];
    printf("Are you sure you want to cancel and return to main menu? (yes/no): ");
    scanf("%s", confirm);
    return strcmp(confirm, "yes") == 0;
}

Student *addStudent(Student *head)
{
    while (1)
    {
        Student *newStudent = (Student *)malloc(sizeof(Student));
        if (newStudent == NULL)
        {
            printf("Memory allocation failed!\n");
            return head;
        }

        printf("Please enter ID (or -1 to return to main menu): ");
        scanf("%s", newStudent->id);
        if (strcmp(newStudent->id, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(newStudent);
            return head;
        }

        if (findStudentById(head, newStudent->id) != NULL)
        {
            printf("This ID already exists, cannot add!\n");
            free(newStudent);
            continue;
        }

        printf("Please enter name (or -1 to return to main menu): ");
        scanf("%s", newStudent->name);
        if (strcmp(newStudent->name, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(newStudent);
            return head;
        }

        while (1)
        {
            printf("Please enter gender (male/female, or -1 to return to main menu): ");
            scanf("%s", newStudent->gender);
            if (strcmp(newStudent->gender, "-1") == 0)
            {
                printf("Returning to main menu.\n");
                free(newStudent);
                return head;
            }
            if (strcmp(newStudent->gender, "male") == 0 || strcmp(newStudent->gender, "female") == 0)
            {
                break;
            }
            else
            {
                printf("Invalid gender! Please enter 'male' or 'female'.\n");
            }
        }

        printf("Please enter 5 course scores (0~100, or -1 to return to main menu):\n");
        for (int i = 0; i < 5; i++)
        {
            float score;
            while (1)
            {
                printf("Course %d: ", i + 1);
                scanf("%f", &score);
                if (score == -1)
                {
                    printf("Returning to main menu.\n");
                    free(newStudent);
                    return head;
                }
                if (score >= 0 && score <= 100)
                {
                    newStudent->scores[i] = score;
                    break;
                }
                else
                {
                    printf("Invalid score! Please enter a value between 0 and 100.\n");
                }
            }
        }

        calculateTotalAndAverage(newStudent);

        newStudent->next = head;
        head = newStudent;

        printf("Student information added successfully!\n");
        return head;
    }
}

void calculateTotalAndAverage(Student *student)
{
    student->total = 0;
    for (int i = 0; i < 5; i++)
    {
        student->total += student->scores[i];
    }
    student->average = student->total / 5;
}

void calculateAllTotalAndAverage(Student *head)
{
    Student *current = head;
    while (current != NULL)
    {
        calculateTotalAndAverage(current);
        current = current->next;
    }
}

void rankStudentsByTotal(Student *head)
{

    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    if (count == 0)
    {
        return;
    }

    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (students[j]->total < students[j + 1]->total)
            {
                Student *temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        students[i]->rank = i + 1;
    }

    free(students);
}

Student *findStudentById(Student *head, const char *id)
{
    if (strcmp(id, "-1") == 0)
    {
        printf("Operation cancelled.\n");
        return NULL;
    }

    Student *current = head;

    while (current != NULL)
    {
        if (strcmp(current->id, id) == 0)
        {
            printf("\nStudent information found:\n");
            printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                   "ID", "Name", "Gender", "Course1", "Course2", "Course3", "Course4", "Course5", "Total", "Average", "Rank");
            printf("--------------------------------------------------------------------------------------------------------------\n");
            printf("%-10s %-15s %-5s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-5d\n",
                   current->id, current->name, current->gender,
                   current->scores[0], current->scores[1], current->scores[2], current->scores[3], current->scores[4],
                   current->total, current->average, current->rank);
            printf("--------------------------------------------------------------------------------------------------------------\n");
            return current;
        }
        current = current->next;
    }

    printf("Student with ID %s not found!\n", id);
    return NULL;
}

Student *findStudentByName(Student *head, const char *name)
{
    if (strcmp(name, "-1") == 0)
    {
        printf("Operation cancelled.\n");
        return NULL;
    }

    Student *current = head;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            if (!found)
            {
                printf("\nStudent information found:\n");
                printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                       "ID", "Name", "Gender", "Course1", "Course2", "Course3", "Course4", "Course5", "Total", "Average", "Rank");
                printf("--------------------------------------------------------------------------------------------------------------\n");
                found = 1;
            }
            printf("%-10s %-15s %-5s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-5d\n",
                   current->id, current->name, current->gender,
                   current->scores[0], current->scores[1], current->scores[2], current->scores[3], current->scores[4],
                   current->total, current->average, current->rank);
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Student with name %s not found!\n", name);
        return NULL;
    }

    printf("--------------------------------------------------------------------------------------------------------------\n");
    return head;
}

int saveStudentsToFile(Student *head, const char *filename)
{
    (void)filename;
    FILE *file = fopen("students.txt", "w");
    if (file == NULL)
    {
        printf("Cannot open file %s for writing!\n", "students.txt");
        return 0;
    }
    Student *current = head;
    while (current != NULL)
    {
        fprintf(file, "%s %s %s ", current->id, current->name, current->gender);
        for (int i = 0; i < 5; i++)
        {
            fprintf(file, "%.2f ", current->scores[i]);
        }
        fprintf(file, "%.2f %.2f %d\n", current->total, current->average, current->rank);
        current = current->next;
    }
    fclose(file);
    return 1;
}

Student *loadStudentsFromFile(const char *filename)
{
    (void)filename;
    FILE *file = fopen("students.txt", "r");
    if (file == NULL)
    {
        printf("Cannot open file %s for reading!\n", "students.txt");
        return NULL;
    }
    Student *head = NULL;
    Student *tail = NULL;
    while (1)
    {
        Student temp;
        int ret = fscanf(file, "%s %s %s ", temp.id, temp.name, temp.gender);
        if (ret != 3)
            break;
        for (int i = 0; i < 5; i++)
        {
            if (fscanf(file, "%f ", &temp.scores[i]) != 1)
                break;
        }
        if (fscanf(file, "%f %f %d\n", &temp.total, &temp.average, &temp.rank) != 3)
            break;
        Student *newStudent = (Student *)malloc(sizeof(Student));
        *newStudent = temp;
        newStudent->next = NULL;
        if (head == NULL)
        {
            head = newStudent;
            tail = newStudent;
        }
        else
        {
            tail->next = newStudent;
            tail = newStudent;
        }
    }
    fclose(file);
    return head;
}

void freeStudentList(Student *head)
{
    Student *current = head;
    Student *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void saveUsers(User *users, int count)
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Cannot open user file for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

int loadUsers(User **users)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    int capacity = 10;
    int count = 0;
    *users = (User *)malloc(sizeof(User) * capacity);
    if (*users == NULL)
    {
        fclose(file);
        return 0;
    }
    while (!feof(file))
    {
        if (count >= capacity)
        {
            capacity *= 2;
            *users = (User *)realloc(*users, sizeof(User) * capacity);
        }
        User temp;
        if (fscanf(file, "%s %s", temp.username, temp.password) == 2)
        {
            (*users)[count++] = temp;
        }
    }
    fclose(file);
    return count;
}

Student *deleteStudentById(Student *head, const char *id)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return NULL;
    }

    printf("Are you sure you want to delete student with ID %s? (y/n, or -1 to return to previous menu): ", id);
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("Returning to previous menu.\n");
        return head;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("Deletion cancelled.\n");
        return head;
    }

    if (strcmp(head->id, id) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("Student with ID %s deleted!\n", id);
        return head;
    }

    Student *current = head;
    while (current->next != NULL && strcmp(current->next->id, id) != 0)
    {
        current = current->next;
    }

    if (current->next != NULL)
    {
        Student *temp = current->next;
        current->next = temp->next;
        free(temp);
        printf("Student with ID %s deleted!\n", id);
    }
    else
    {
        printf("Student with ID %s not found!\n", id);
    }

    return head;
}

Student *deleteStudentByName(Student *head, const char *name)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return NULL;
    }

    printf("Are you sure you want to delete student(s) with name %s? (y/n, or -1 to return to previous menu): ", name);
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("Returning to previous menu.\n");
        return head;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("Deletion cancelled.\n");
        return head;
    }

    while (head != NULL && strcmp(head->name, name) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("Student with name %s deleted!\n", name);
    }

    if (head == NULL)
    {
        return NULL;
    }

    Student *current = head;
    int deleted = 0;

    while (current->next != NULL)
    {
        if (strcmp(current->next->name, name) == 0)
        {
            Student *temp = current->next;
            current->next = temp->next;
            free(temp);
            deleted++;
        }
        else
        {
            current = current->next;
        }
    }

    if (deleted > 0)
    {
        printf("Deleted %d students with name %s!\n", deleted, name);
    }
    else
    {
        printf("Student with name %s not found!\n", name);
    }

    return head;
}

Student *modifyStudentById(Student *head, const char *id)
{
    if (strcmp(id, "-1") == 0)
    {
        printf("Operation cancelled.\n");
        return head;
    }

    Student *student = findStudentById(head, id);
    if (student == NULL)
    {
        return head;
    }

    while (1)
    {
        printf("\nPlease choose the information to modify:\n");
        printf("1. Name\n");
        printf("2. Gender\n");
        printf("3. Scores\n");
        printf("-1. Return to previous menu\n");

        int choice;
        printf("Please enter choice: ");
        scanf("%d", &choice);
        if (choice == -1)
        {
            printf("Returning to previous menu.\n");
            return head;
        }

        switch (choice)
        {
        case 1:
            printf("Please enter new name (or -1 to cancel): ");
            scanf("%s", student->name);
            if (strcmp(student->name, "-1") == 0)
            {
                printf("Operation cancelled.\n");
                continue;
            }
            printf("Name modified successfully!\n");
            break;
        case 2:
            while (1)
            {
                printf("Please enter new gender (male/female, or -1 to cancel): ");
                scanf("%s", student->gender);
                if (strcmp(student->gender, "-1") == 0)
                {
                    printf("Operation cancelled.\n");
                    break;
                }
                if (strcmp(student->gender, "male") == 0 || strcmp(student->gender, "female") == 0)
                {
                    printf("Gender modified successfully!\n");
                    break;
                }
                else
                {
                    printf("Invalid gender! Please enter 'male' or 'female'.\n");
                }
            }
            break;
        case 3:
            printf("\nPlease select which course to modify:\n");
            printf("1. Course 1\n");
            printf("2. Course 2\n");
            printf("3. Course 3\n");
            printf("4. Course 4\n");
            printf("5. Course 5\n");
            printf("-1. Return to previous menu\n");

            int courseChoice;
            printf("Please enter course number to modify (1-5, or -1 to cancel): ");
            scanf("%d", &courseChoice);

            if (courseChoice == -1)
            {
                printf("Returning to previous menu.\n");
                continue;
            }
            else if (courseChoice >= 1 && courseChoice <= 5)
            {
                float score;
                while (1)
                {
                    printf("Please enter new score for Course %d (0~100, or -1 to cancel): ", courseChoice);
                    scanf("%f", &score);
                    if (score == -1)
                    {
                        printf("Operation cancelled.\n");
                        break;
                    }
                    if (score >= 0 && score <= 100)
                    {
                        student->scores[courseChoice - 1] = score;
                        calculateTotalAndAverage(student);
                        printf("Course %d score modified successfully!\n", courseChoice);
                        break;
                    }
                    else
                    {
                        printf("Invalid score! Please enter a value between 0 and 100.\n");
                    }
                }
            }
            else
            {
                printf("Invalid course selection!\n");
            }
            break;
        default:
            printf("Invalid choice!\n");
        }
    }

    return head;
}

Student *modifyStudentByName(Student *head, const char *name)
{
    int count = 0;
    Student *current = head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("Student with name %s not found!\n", name);
        return head;
    }

    if (count > 1)
    {
        printf("Found %d students with name %s, please use ID to modify!\n", count, name);
        return head;
    }

    current = head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            printf("\nPlease choose the information to modify:\n");
            printf("1. ID\n");
            printf("2. Gender\n");
            printf("3. Scores\n");
            printf("-1. Cancel modification\n");

            int choice;
            printf("Please enter choice: ");
            scanf("%d", &choice);
            if (choice == -1)
            {
                printf("Operation cancelled.\n");
                return head;
            }

            switch (choice)
            {
            case 1:
                printf("Please enter new ID (or -1 to cancel): ");
                scanf("%s", current->id);
                if (strcmp(current->id, "-1") == 0)
                {
                    printf("Operation cancelled.\n");
                    return head;
                }
                printf("ID modified successfully!\n");
                break;
            case 2:
                while (1)
                {
                    printf("Please enter new gender (male/female, or -1 to cancel): ");
                    scanf("%s", current->gender);
                    if (strcmp(current->gender, "-1") == 0)
                    {
                        printf("Operation cancelled.\n");
                        return head;
                    }
                    if (strcmp(current->gender, "male") == 0 || strcmp(current->gender, "female") == 0)
                    {
                        printf("Gender modified successfully!\n");
                        break;
                    }
                    else
                    {
                        printf("Invalid gender! Please enter 'male' or 'female'.\n");
                    }
                }
                break;
            case 3:
                printf("Please enter new 5 course scores (0~100, or -1 to cancel):\n");
                for (int i = 0; i < 5; i++)
                {
                    float score;
                    while (1)
                    {
                        printf("Course %d: ", i + 1);
                        scanf("%f", &score);
                        if (score == -1)
                        {
                            printf("Operation cancelled.\n");
                            return head;
                        }
                        if (score >= 0 && score <= 100)
                        {
                            current->scores[i] = score;
                            break;
                        }
                        else
                        {
                            printf("Invalid score! Please enter a value between 0 and 100.\n");
                        }
                    }
                }
                calculateTotalAndAverage(current);
                printf("Scores modified successfully!\n");
                break;
            default:
                printf("Invalid choice!\n");
            }
            break;
        }
        current = current->next;
    }

    return head;
}

void displayAllStudents(Student *head)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nAll student information:\n");
    printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
           "ID", "Name", "Gender", "Course1", "Course2", "Course3", "Course4", "Course5", "Total", "Average", "Rank");
    printf("--------------------------------------------------------------------------------------------------------------\n");

    Student *current = head;
    while (current != NULL)
    {
        printf("%-10s %-15s %-5s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-5d\n",
               current->id, current->name, current->gender,
               current->scores[0], current->scores[1], current->scores[2], current->scores[3], current->scores[4],
               current->total, current->average, current->rank);
        current = current->next;
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
}

Student *sortStudentsById(Student *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(students[j]->id, students[j + 1]->id) > 0)
            {
                Student *temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        students[i]->next = students[i + 1];
    }
    students[count - 1]->next = NULL;

    head = students[0];

    free(students);

    return head;
}

Student *sortStudentsByTotal(Student *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (students[j]->total < students[j + 1]->total)
            {
                Student *temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        students[i]->rank = i + 1;
    }

    for (int i = 0; i < count - 1; i++)
    {
        students[i]->next = students[i + 1];
    }
    students[count - 1]->next = NULL;

    head = students[0];

    free(students);

    return head;
}

void calculateCourseAverages(Student *head, float averages[5])
{
    if (head == NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            averages[i] = 0.0;
        }
        return;
    }

    float sums[5] = {0};
    int count = 0;

    Student *current = head;
    while (current != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            sums[i] += current->scores[i];
        }
        count++;
        current = current->next;
    }

    for (int i = 0; i < 5; i++)
    {
        averages[i] = (count > 0) ? sums[i] / count : 0;
    }
}

void displayStudentsWithScoreAbove(Student *head, float score, int course_index)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nPlease enter the minimum score (0-100, or -1 to cancel): ");
    float minScore;
    scanf("%f", &minScore);
    if (minScore == -1)
    {
        printf("Operation cancelled.\n");
        return;
    }
    if (minScore < 0 || minScore > 100)
    {
        printf("Invalid score! Please enter a value between 0 and 100.\n");
        return;
    }

    printf("\nStudents with course %d score above %.2f:\n", course_index + 1, minScore);
    printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
    printf("------------------------------------------\n");

    Student *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (current->scores[course_index] >= minScore)
        {
            printf("%-10s %-15s %-5s %-8.2f\n",
                   current->id, current->name, current->gender, current->scores[course_index]);
            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("No student's course %d score above %.2f!\n", course_index + 1, minScore);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("Total %d students' course %d score above %.2f.\n", count, course_index + 1, minScore);
    }
}

void displayStudentsWithScoreBelow(Student *head, float score, int course_index)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nPlease enter the maximum score (0-100, or -1 to cancel): ");
    float maxScore;
    scanf("%f", &maxScore);
    if (maxScore == -1)
    {
        printf("Operation cancelled.\n");
        return;
    }
    if (maxScore < 0 || maxScore > 100)
    {
        printf("Invalid score! Please enter a value between 0 and 100.\n");
        return;
    }

    printf("\nStudents with course %d score below %.2f:\n", course_index + 1, maxScore);
    printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
    printf("------------------------------------------\n");

    Student *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (current->scores[course_index] < maxScore)
        {
            printf("%-10s %-15s %-5s %-8.2f\n",
                   current->id, current->name, current->gender, current->scores[course_index]);
            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("No student's course %d score below %.2f!\n", course_index + 1, maxScore);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("Total %d students' course %d score below %.2f.\n", count, course_index + 1, maxScore);
    }
}

void displayStudentsBelowAverage(Student *head, float averages[5])
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nStudents below average:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("\nCourse %d (Average: %.2f):\n", i + 1, averages[i]);
        printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
        printf("------------------------------------------\n");

        Student *current = head;
        int count = 0;

        while (current != NULL)
        {
            if (current->scores[i] < averages[i])
            {
                printf("%-10s %-15s %-5s %-8.2f\n",
                       current->id, current->name, current->gender, current->scores[i]);
                count++;
            }
            current = current->next;
        }

        if (count == 0)
        {
            printf("No student's score below average!\n");
        }
        else
        {
            printf("------------------------------------------\n");
            printf("Total %d students' course %d score below average.\n", count, i + 1);
        }
    }
}

void displayFailingStudents(Student *head)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nFailing students (below 60 points):\n");
    printf("%-10s %-15s %-5s %-10s %-10s %-10s %-10s %-10s\n",
           "ID", "Name", "Gender", "Course1", "Course2", "Course3", "Course4", "Course5");
    printf("--------------------------------------------------------------------------------\n");

    Student *current = head;
    int totalFailingStudents = 0;

    while (current != NULL)
    {
        int hasFailingScore = 0;

        for (int i = 0; i < 5; i++)
        {
            if (current->scores[i] < 60.0)
            {
                hasFailingScore = 1;
                break;
            }
        }

        if (hasFailingScore)
        {
            totalFailingStudents++;
            printf("%-10s %-15s %-5s ", current->id, current->name, current->gender);

            for (int i = 0; i < 5; i++)
            {
                if (current->scores[i] < 60.0)
                {
                    printf("%-10.2f*", current->scores[i]);
                }
                else
                {
                    printf("%-10.2f ", current->scores[i]);
                }
            }
            printf("\n");
        }

        current = current->next;
    }

    if (totalFailingStudents == 0)
    {
        printf("No failing students found!\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("Total %d students have failing courses. (* indicates failing score)\n", totalFailingStudents);
    }
}

void exportGradesToCSV(Student *head)
{
    printf("Are you sure you want to export grades to CSV? (y/n, or -1 to return to main menu): ");
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("Returning to main menu.\n");
        return;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("Export cancelled.\n");
        return;
    }

    FILE *file = fopen("students.csv", "w");
    if (file == NULL)
    {
        printf("Cannot open students.csv for writing!\n");
        return;
    }
    fprintf(file, "ID,Name,Gender,Course1,Course2,Course3,Course4,Course5,Total,Average,Rank\n");
    Student *current = head;
    while (current != NULL)
    {
        fprintf(file, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
                current->id, current->name, current->gender,
                current->scores[0], current->scores[1], current->scores[2], current->scores[3], current->scores[4],
                current->total, current->average, current->rank);
        current = current->next;
    }
    fclose(file);
    printf("Exported to students.csv successfully!\n");
}

void registerUser()
{
    while (1)
    {
        char username[30];
        char password[20];
        User *users = NULL;
        int count = loadUsers(&users);

        printf("Please enter new username (or -1 to return to main menu): ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                printf("Username already exists!\n");
                free(users);
                continue;
            }
        }

        printf("Please enter new password (or -1 to return to main menu): ");
        scanf("%s", password);
        if (strcmp(password, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(users);
            return;
        }

        users = (User *)realloc(users, sizeof(User) * (count + 1));
        strcpy(users[count].username, username);
        strcpy(users[count].password, password);
        saveUsers(users, count + 1);
        printf("User registered successfully!\n");
        free(users);
        return;
    }
}

void deleteUser()
{
    while (1)
    {
        char username[30];
        User *users = NULL;
        int count = loadUsers(&users);
        if (count <= 1)
        {
            printf("At least one user must remain!\n");
            free(users);
            return;
        }

        printf("Please enter username to delete (or -1 to return to main menu): ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(users);
            return;
        }

        printf("Are you sure you want to delete user %s? (y/n, or -1 to return to previous menu): ", username);
        char confirm[10];
        scanf("%s", confirm);
        if (strcmp(confirm, "-1") == 0)
        {
            printf("Returning to previous menu.\n");
            free(users);
            return;
        }
        if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
        {
            printf("Deletion cancelled.\n");
            free(users);
            return;
        }

        int found = 0;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                found = 1;
                for (int j = i; j < count - 1; j++)
                {
                    users[j] = users[j + 1];
                }
                break;
            }
        }
        if (!found)
        {
            printf("Username not found!\n");
            free(users);
            continue;
        }
        saveUsers(users, count - 1);
        printf("User deleted successfully!\n");
        free(users);
        return;
    }
}

void changePassword()
{
    while (1)
    {
        char username[30];
        char oldPassword[20];
        char newPassword[20];
        User *users = NULL;
        int count = loadUsers(&users);
        int found = 0;

        printf("Please enter your username (or -1 to return to main menu): ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(users);
            return;
        }

        printf("Please enter your old password (or -1 to return to main menu): ");
        scanf("%s", oldPassword);
        if (strcmp(oldPassword, "-1") == 0)
        {
            printf("Returning to main menu.\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, oldPassword) == 0)
            {
                printf("Please enter your new password (or -1 to return to main menu): ");
                scanf("%s", newPassword);
                if (strcmp(newPassword, "-1") == 0)
                {
                    printf("Returning to main menu.\n");
                    free(users);
                    return;
                }
                strcpy(users[i].password, newPassword);
                saveUsers(users, count);
                printf("Password changed successfully!\n");
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("Username or old password incorrect!\n");
            free(users);
            continue;
        }
        free(users);
        return;
    }
}

void viewCurrentUser(char currentUser[])
{
    if (strcmp(currentUser, "") == 0)
    {
        printf("No user is currently logged in.\n");
        return;
    }
    printf("Current logged in user: %s\n", currentUser);
}