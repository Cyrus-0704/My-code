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

// ��������
// 1. �û���¼���
int login(char currentUser[]);
int verifyUser(char *username, char *password);
void saveUsers(User *users, int count);
int loadUsers(User **users);
void exportGradesToCSV(Student *head);
void registerUser();
void deleteUser();
void changePassword();
void viewCurrentUser(char currentUser[]);

// 2. ѧ����Ϣ����
Student *createStudentList();
Student *addStudent(Student *head);
Student *deleteStudentById(Student *head, char *id);
Student *findStudentById(Student *head, char *id);
Student *findStudentByName(Student *head, char *name);
Student *modifyStudentById(Student *head, char *id);
void displayAllStudents(Student *head);
void freeStudentList(Student *head);

// 3. �ɼ�ͳ�������
void calculateTotalAndAverage(Student *student);
void calculateAllTotalAndAverage(Student *head);
void rankStudentsByTotal(Student *head);
Student *sortStudentsById(Student *head);
Student *sortStudentsByTotal(Student *head);
void displayStudentsWithScoreAbove(Student *head, float score, int course_index);
void calculateCourseAverages(Student *head, float averages[]);
void displayStudentsBelowAverage(Student *head, float averages[]);
void displayFailingStudents(Student *head);

// 4. �ļ�����
int saveStudentsToFile(Student *head, char *filename);
Student *loadStudentsFromFile(char *filename);

// 5. �˵���������
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
        printf("��¼ʧ�ܣ������˳���\n");
        return 1;
    }

    head = loadStudentsFromFile("students.txt");
    if (head == NULL)
    {
        printf("δ�ҵ�ѧ�����ݻ�����Ϊ�գ��������µ�ѧ���б�\n");
        head = createStudentList();
    }

    // ���˵�ѭ����ֱ��ѡ���˳�
    do
    {
        displayMenu();
        printf("����������ѡ��: ");
        scanf("%d", &choice);
        if (choice == 0)
        {
            char confirm[10];
            printf("ȷ��Ҫ�˳���(yes/no): ");
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
        printf("���ݱ���ɹ���\n");
    }
    else
    {
        printf("���ݱ���ʧ�ܣ�\n");
    }

    freeStudentList(head);

    printf("��лʹ�ñ�ϵͳ���ټ���\n");
    return 0;
}

int login(char currentUser[])
{
    char username[30];
    char password[20];
    int attempts = 0;

    while (attempts < 3)
    {
        printf("�������û���: ");
        scanf("%s", username);
        printf("����������: ");
        scanf("%s", password);

        if (verifyUser(username, password))
        {
            strcpy(currentUser, username);
            printf("��¼�ɹ�����ӭʹ��ѧ���ɼ�����ϵͳ��\n");
            return 1;
        }
        else
        {
            attempts++;
            printf("�û������������������ %d �γ��Ի��ᡣ\n", 3 - attempts);
        }
    }

    printf("���Դ��������꣬��¼ʧ�ܣ�\n");
    return 0;
}

int verifyUser(char *username, char *password)
{
    User *users = NULL;
    int count = loadUsers(&users);

    if (count <= 0)
    {
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            User InitialUser;
            strcpy(InitialUser.username, "admin");
            strcpy(InitialUser.password, "admin");
            saveUsers(&InitialUser, 1);
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
    printf("\n=== ѧ���ɼ�����ϵͳ ===\n");
    printf("1. ���ѧ����Ϣ\n");
    printf("2. ��ѧ�Ų���ѧ��\n");
    printf("3. ����������ѧ��\n");
    printf("4. �޸�ѧ����Ϣ\n");
    printf("5. ɾ��ѧ����Ϣ\n");
    printf("6. ��ʾ����ѧ����Ϣ\n");
    printf("7. ��ѧ������\n");
    printf("8. ���ܷ�����\n");
    printf("9. ��ʾ�γ�ͳ����Ϣ\n");
    printf("10. ��ʾ����ָ��������ѧ��\n");
    printf("11. ��ʾ������ѧ��\n");
    printf("12. ��ʾ����ƽ���ֵ�ѧ��\n");
    printf("13. �������ݵ��ļ�\n");
    printf("14. ע�����û�\n");
    printf("15. ɾ���û�\n");
    printf("16. �޸�����\n");
    printf("17. �鿴��ǰ�û�\n");
    printf("18. �����ɼ���CSV\n");
    printf("0. �˳�ϵͳ\n");
}

void processChoice(int choice, Student **head, char currentUser[])
{
    char id[20];
    char name[30];
    float averages[5];

    switch (choice)
    {
    case 0:
        printf("���ڱ������ݲ��˳�...\n");
        break;
    case 1:
        *head = addStudent(*head);
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
        break;
    case 2:
        while (1)
        {
            printf("������Ҫ���ҵ�ѧ��ѧ�ţ�����-1�������˵���: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("�������˵���\n");
                break;
            }
            findStudentById(*head, id);
        }
        break;
    case 3:
        while (1)
        {
            printf("������Ҫ���ҵ�ѧ������������-1�������˵���: ");
            scanf("%s", name);
            if (strcmp(name, "-1") == 0)
            {
                printf("�������˵���\n");
                break;
            }
            findStudentByName(*head, name);
        }
        break;
    case 4:
        while (1)
        {
            printf("������Ҫ�޸ĵ�ѧ��ѧ�ţ�����-1�������˵���: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("�������˵���\n");
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
            printf("������Ҫɾ����ѧ��ѧ�ţ�����-1�������˵���: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("�������˵���\n");
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
        printf("\n�γ�ƽ���֣�\n");
        for (int i = 0; i < 5; i++)
        {
            printf("�γ� %d: %.2f\n", i + 1, averages[i]);
        }
        break;
    case 10:
    {
        int course_index;
        printf("������γ̱�ţ�1-5������-1�������˵���: ");
        scanf("%d", &course_index);
        if (course_index == -1)
        {
            printf("�������˵���\n");
            break;
        }
        if (course_index < 1 || course_index > 5)
        {
            printf("��Ч�Ŀγ̱�ţ�\n");
            break;
        }
        displayStudentsWithScoreAbove(*head, 0, course_index - 1);
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
            printf("���ݱ���ɹ���\n");
        }
        else
        {
            printf("���ݱ���ʧ�ܣ�\n");
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
        printf("��Ч��ѡ�����������룡\n");
    }
}

Student *createStudentList()
{
    return NULL;
}

Student *addStudent(Student *head)
{
    while (1)
    {
        Student *newStudent = (Student *)malloc(sizeof(Student));
        if (newStudent == NULL)
        {
            printf("�ڴ����ʧ�ܣ�\n");
            return head;
        }

        printf("������ѧ�ţ�����-1�������˵���: ");
        scanf("%s", newStudent->id);
        if (strcmp(newStudent->id, "-1") == 0)
        {
            printf("�������˵���\n");
            free(newStudent);
            return head;
        }

        if (findStudentById(head, newStudent->id) != NULL)
        {
            printf("��ѧ���Ѵ��ڣ��޷���ӣ�\n");
            free(newStudent);
            continue;
        }

        printf("����������������-1�������˵���: ");
        scanf("%s", newStudent->name);
        if (strcmp(newStudent->name, "-1") == 0)
        {
            printf("�������˵���\n");
            free(newStudent);
            return head;
        }

        while (1)
        {
            printf("�������Ա���/Ů������-1�������˵���: ");
            scanf("%s", newStudent->gender);
            if (strcmp(newStudent->gender, "-1") == 0)
            {
                printf("�������˵���\n");
                free(newStudent);
                return head;
            }
            if (strcmp(newStudent->gender, "��") == 0 || strcmp(newStudent->gender, "Ů") == 0)
            {
                break;
            }
            else
            {
                printf("��Ч���Ա�������'��'��'Ů'��\n");
            }
        }

        printf("������5�ſγ̳ɼ���0~100������-1�������˵���:\n");
        for (int i = 0; i < 5; i++)
        {
            float score;
            while (1)
            {
                printf("�γ� %d: ", i + 1);
                scanf("%f", &score);
                if (score == -1)
                {
                    printf("�������˵���\n");
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
                    printf("��Ч�ĳɼ���������0��100֮��ķ�����\n");
                }
            }
        }

        calculateTotalAndAverage(newStudent);
        newStudent->next = NULL;

        if (head == NULL)
        {
            head = newStudent;
        }
        else
        {
            Student *current = head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newStudent;
        }

        printf("ѧ����Ϣ��ӳɹ���\n");
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
        printf("�ڴ����ʧ�ܣ�\n");
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

Student *findStudentById(Student *head, char *id)
{
    if (strcmp(id, "-1") == 0)
    {
        printf("������ȡ����\n");
        return NULL;
    }

    Student *current = head;

    while (current != NULL)
    {
        if (strcmp(current->id, id) == 0)
        {
            printf("\n�ҵ�ѧ����Ϣ��\n");
            printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                   "ѧ��", "����", "�Ա�", "�γ�1", "�γ�2", "�γ�3", "�γ�4", "�γ�5", "�ܷ�", "ƽ����", "����");
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

    printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", id);
    return NULL;
}

Student *findStudentByName(Student *head, char *name)
{
    if (strcmp(name, "-1") == 0)
    {
        printf("������ȡ����\n");
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
                printf("\n�ҵ�ѧ����Ϣ��\n");
                printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                       "ѧ��", "����", "�Ա�", "�γ�1", "�γ�2", "�γ�3", "�γ�4", "�γ�5", "�ܷ�", "ƽ����", "����");
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
        printf("δ�ҵ�����Ϊ %s ��ѧ����\n", name);
        return NULL;
    }

    printf("--------------------------------------------------------------------------------------------------------------\n");
    return head;
}

int saveStudentsToFile(Student *head, char *filename)
{
    (void)filename;
    FILE *file = fopen("students.txt", "w");
    if (file == NULL)
    {
        printf("�޷����ļ� %s ����д�룡\n", "students.txt");
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

Student *loadStudentsFromFile(char *filename)
{
    (void)filename;
    FILE *file = fopen("students.txt", "r");
    if (file == NULL)
    {
        printf("�޷����ļ� %s ���ж�ȡ��\n", "students.txt");
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
        printf("�޷����û��ļ�����д�룡\n");
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

Student *deleteStudentById(Student *head, char *id)
{
    if (head == NULL)
    {
        printf("ѧ���б�Ϊ�գ�\n");
        return NULL;
    }

    printf("ȷ��Ҫɾ��ѧ��Ϊ %s ��ѧ����(y/n������-1������һ���˵�): ", id);
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("������һ���˵���\n");
        return head;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("ɾ����ȡ����\n");
        return head;
    }

    if (strcmp(head->id, id) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("��ɾ��ѧ��Ϊ %s ��ѧ����\n", id);
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
        printf("��ɾ��ѧ��Ϊ %s ��ѧ����\n", id);
    }
    else
    {
        printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", id);
    }

    return head;
}

Student *modifyStudentById(Student *head, char *id)
{
    if (strcmp(id, "-1") == 0)
    {
        printf("������ȡ����\n");
        return head;
    }

    Student *student = findStudentById(head, id);
    if (student == NULL)
    {
        return head;
    }

    while (1)
    {
        printf("\n��ѡ��Ҫ�޸ĵ���Ϣ��\n");
        printf("1. ����\n");
        printf("2. �Ա�\n");
        printf("3. �ɼ�\n");
        printf("-1. ������һ���˵�\n");

        int choice;
        printf("������ѡ��: ");
        scanf("%d", &choice);
        if (choice == -1)
        {
            printf("������һ���˵���\n");
            return head;
        }

        switch (choice)
        {
        case 1:
            printf("������������������-1ȡ����: ");
            scanf("%s", student->name);
            if (strcmp(student->name, "-1") == 0)
            {
                printf("������ȡ����\n");
                continue;
            }
            printf("�����޸ĳɹ���\n");
            break;
        case 2:
            while (1)
            {
                printf("���������Ա���/Ů������-1ȡ����: ");
                scanf("%s", student->gender);
                if (strcmp(student->gender, "-1") == 0)
                {
                    printf("������ȡ����\n");
                    break;
                }
                if (strcmp(student->gender, "��") == 0 || strcmp(student->gender, "Ů") == 0)
                {
                    printf("�Ա��޸ĳɹ���\n");
                    break;
                }
                else
                {
                    printf("��Ч���Ա�������'��'��'Ů'��\n");
                }
            }
            break;
        case 3:
            printf("\n��ѡ��Ҫ�޸ĵĿγ̣�\n");
            printf("1. �γ�1\n");
            printf("2. �γ�2\n");
            printf("3. �γ�3\n");
            printf("4. �γ�4\n");
            printf("5. �γ�5\n");
            printf("-1. ������һ���˵�\n");

            int courseChoice;
            printf("������Ҫ�޸ĵĿγ̱�ţ�1-5������-1ȡ����: ");
            scanf("%d", &courseChoice);

            if (courseChoice == -1)
            {
                printf("������һ���˵���\n");
                continue;
            }
            else if (courseChoice >= 1 && courseChoice <= 5)
            {
                float score;
                while (1)
                {
                    printf("������γ� %d ���³ɼ���0~100������-1ȡ����: ", courseChoice);
                    scanf("%f", &score);
                    if (score == -1)
                    {
                        printf("������ȡ����\n");
                        break;
                    }
                    if (score >= 0 && score <= 100)
                    {
                        student->scores[courseChoice - 1] = score;
                        calculateTotalAndAverage(student);
                        printf("�γ� %d �ɼ��޸ĳɹ���\n", courseChoice);
                        break;
                    }
                    else
                    {
                        printf("��Ч�ĳɼ���������0��100֮��ķ�����\n");
                    }
                }
            }
            else
            {
                printf("��Ч�Ŀγ�ѡ��\n");
            }
            break;
        default:
            printf("��Ч��ѡ��\n");
        }
    }

    return head;
}

void displayAllStudents(Student *head)
{
    if (head == NULL)
    {
        printf("ѧ���б�Ϊ�գ�\n");
        return;
    }

    printf("\n����ѧ����Ϣ��\n");
    printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
           "ѧ��", "����", "�Ա�", "�γ�1", "�γ�2", "�γ�3", "�γ�4", "�γ�5", "�ܷ�", "ƽ����", "����");
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
        printf("�ڴ����ʧ�ܣ�\n");
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
        printf("�ڴ����ʧ�ܣ�\n");
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
        printf("ѧ���б�Ϊ�գ�\n");
        return;
    }

    printf("\n��������ͷ�����0-100������-1ȡ����: ");
    float minScore;
    scanf("%f", &minScore);
    if (minScore == -1)
    {
        printf("������ȡ����\n");
        return;
    }
    if (minScore < 0 || minScore > 100)
    {
        printf("��Ч�ķ�����������0��100֮��ķ�����\n");
        return;
    }

    printf("\n�γ� %d �������� %.2f ��ѧ����\n", course_index + 1, minScore);
    printf("%-10s %-15s %-5s %-8s\n", "ѧ��", "����", "�Ա�", "����");
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
        printf("û��ѧ���γ� %d �ķ������� %.2f��\n", course_index + 1, minScore);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("���� %d ��ѧ���γ� %d �ķ������� %.2f��\n", count, course_index + 1, minScore);
    }
}

void displayStudentsBelowAverage(Student *head, float averages[5])
{
    if (head == NULL)
    {
        printf("ѧ���б�Ϊ�գ�\n");
        return;
    }

    printf("\n����ƽ���ֵ�ѧ����\n");

    for (int i = 0; i < 5; i++)
    {
        printf("\n�γ� %d��ƽ���֣�%.2f����\n", i + 1, averages[i]);
        printf("%-10s %-15s %-5s %-8s\n", "ѧ��", "����", "�Ա�", "����");
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
            printf("û��ѧ������ƽ���֣�\n");
        }
        else
        {
            printf("------------------------------------------\n");
            printf("���� %d ��ѧ���γ� %d �ķ�������ƽ���֡�\n", count, i + 1);
        }
    }
}

void displayFailingStudents(Student *head)
{
    if (head == NULL)
    {
        printf("ѧ���б�Ϊ�գ�\n");
        return;
    }

    printf("\n������ѧ��������60�֣���\n");
    printf("%-10s %-15s %-5s %-11s %-11s %-11s %-11s %-11s\n",
           "ѧ��", "����", "�Ա�", "�γ�1", "�γ�2", "�γ�3", "�γ�4", "�γ�5");
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
                    printf("*%-10.2f", current->scores[i]);
                }
                else
                {
                    printf(" %-10.2f", current->scores[i]);
                }
            }
            printf("\n");
        }

        current = current->next;
    }

    if (totalFailingStudents == 0)
    {
        printf("û���ҵ��������ѧ����\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("���� %d ��ѧ���в�����γ̡���* ��ʾ������ɼ���\n", totalFailingStudents);
    }
}

void exportGradesToCSV(Student *head)
{
    printf("ȷ��Ҫ�����ɼ���CSV�ļ���(y/n������-1�������˵�): ");
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("�������˵���\n");
        return;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("������ȡ����\n");
        return;
    }

    FILE *file = fopen("students.csv", "w");
    if (file == NULL)
    {
        printf("�޷���students.csv�ļ�����д�룡\n");
        return;
    }
    fprintf(file, "ѧ��,����,�Ա�,�γ�1,�γ�2,�γ�3,�γ�4,�γ�5,�ܷ�,ƽ����,����\n");
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
    printf("�ɹ�������students.csv�ļ���\n");
}

void registerUser()
{
    while (1)
    {
        char username[30];
        char password[20];
        User *users = NULL;
        int count = loadUsers(&users);

        printf("���������û���������-1�������˵���: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("�������˵���\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                printf("�û����Ѵ��ڣ�\n");
                free(users);
                continue;
            }
        }

        printf("�����������루����-1�������˵���: ");
        scanf("%s", password);
        if (strcmp(password, "-1") == 0)
        {
            printf("�������˵���\n");
            free(users);
            return;
        }

        users = (User *)realloc(users, sizeof(User) * (count + 1));
        strcpy(users[count].username, username);
        strcpy(users[count].password, password);
        saveUsers(users, count + 1);
        printf("�û�ע��ɹ���\n");
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
            printf("���뱣������һ���û���\n");
            free(users);
            return;
        }

        printf("������Ҫɾ�����û���������-1�������˵���: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("�������˵���\n");
            free(users);
            return;
        }

        printf("ȷ��Ҫɾ���û� %s ��(y/n������-1������һ���˵�): ", username);
        char confirm[10];
        scanf("%s", confirm);
        if (strcmp(confirm, "-1") == 0)
        {
            printf("������һ���˵���\n");
            free(users);
            return;
        }
        if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
        {
            printf("ɾ����ȡ����\n");
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
            printf("δ�ҵ����û�����\n");
            free(users);
            continue;
        }
        saveUsers(users, count - 1);
        printf("�û�ɾ���ɹ���\n");
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

        printf("�����������û���������-1�������˵���: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("�������˵���\n");
            free(users);
            return;
        }

        printf("���������ľ����루����-1�������˵���: ");
        scanf("%s", oldPassword);
        if (strcmp(oldPassword, "-1") == 0)
        {
            printf("�������˵���\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, oldPassword) == 0)
            {
                printf("���������������루����-1�������˵���: ");
                scanf("%s", newPassword);
                if (strcmp(newPassword, "-1") == 0)
                {
                    printf("�������˵���\n");
                    free(users);
                    return;
                }
                strcpy(users[i].password, newPassword);
                saveUsers(users, count);
                printf("�����޸ĳɹ���\n");
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("�û�������������\n");
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
        printf("��ǰû���û���¼��\n");
        return;
    }
    printf("��ǰ��¼�û���%s\n", currentUser);
}