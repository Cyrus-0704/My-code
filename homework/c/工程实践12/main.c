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
int verifyUser(char *username, char *password);
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
Student *deleteStudentById(Student *head, char *id);
Student *findStudentById(Student *head, char *id);
Student *findStudentByName(Student *head, char *name);
Student *modifyStudentById(Student *head, char *id);
void displayAllStudents(Student *head);
void freeStudentList(Student *head);

// 3. 成绩统计与分析
void calculateTotalAndAverage(Student *student);
void calculateAllTotalAndAverage(Student *head);
void rankStudentsByTotal(Student *head);
Student *sortStudentsById(Student *head);
Student *sortStudentsByTotal(Student *head);
void displayStudentsWithScoreAbove(Student *head, float score, int course_index);
void calculateCourseAverages(Student *head, float averages[]);
void displayStudentsBelowAverage(Student *head, float averages[]);
void displayFailingStudents(Student *head);

// 4. 文件操作
int saveStudentsToFile(Student *head, char *filename);
Student *loadStudentsFromFile(char *filename);

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
        printf("登录失败，程序退出！\n");
        return 1;
    }

    head = loadStudentsFromFile("students.txt");
    if (head == NULL)
    {
        printf("未找到学生数据或数据为空，将创建新的学生列表。\n");
        head = createStudentList();
    }

    // 主菜单循环，直到选择退出
    do
    {
        displayMenu();
        printf("请输入您的选择: ");
        scanf("%d", &choice);
        if (choice == 0)
        {
            char confirm[10];
            printf("确定要退出吗？(yes/no): ");
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
        printf("数据保存成功！\n");
    }
    else
    {
        printf("数据保存失败！\n");
    }

    freeStudentList(head);

    printf("感谢使用本系统，再见！\n");
    return 0;
}

int login(char currentUser[])
{
    char username[30];
    char password[20];
    int attempts = 0;

    while (attempts < 3)
    {
        printf("请输入用户名: ");
        scanf("%s", username);
        printf("请输入密码: ");
        scanf("%s", password);

        if (verifyUser(username, password))
        {
            strcpy(currentUser, username);
            printf("登录成功！欢迎使用学生成绩管理系统。\n");
            return 1;
        }
        else
        {
            attempts++;
            printf("用户名或密码错误，您还有 %d 次尝试机会。\n", 3 - attempts);
        }
    }

    printf("尝试次数已用完，登录失败！\n");
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
    printf("\n=== 学生成绩管理系统 ===\n");
    printf("1. 添加学生信息\n");
    printf("2. 按学号查找学生\n");
    printf("3. 按姓名查找学生\n");
    printf("4. 修改学生信息\n");
    printf("5. 删除学生信息\n");
    printf("6. 显示所有学生信息\n");
    printf("7. 按学号排序\n");
    printf("8. 按总分排序\n");
    printf("9. 显示课程统计信息\n");
    printf("10. 显示高于指定分数的学生\n");
    printf("11. 显示不及格学生\n");
    printf("12. 显示低于平均分的学生\n");
    printf("13. 保存数据到文件\n");
    printf("14. 注册新用户\n");
    printf("15. 删除用户\n");
    printf("16. 修改密码\n");
    printf("17. 查看当前用户\n");
    printf("18. 导出成绩到CSV\n");
    printf("0. 退出系统\n");
}

void processChoice(int choice, Student **head, char currentUser[])
{
    char id[20];
    char name[30];
    float averages[5];

    switch (choice)
    {
    case 0:
        printf("正在保存数据并退出...\n");
        break;
    case 1:
        *head = addStudent(*head);
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
        break;
    case 2:
        while (1)
        {
            printf("请输入要查找的学生学号（输入-1返回主菜单）: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("返回主菜单。\n");
                break;
            }
            findStudentById(*head, id);
        }
        break;
    case 3:
        while (1)
        {
            printf("请输入要查找的学生姓名（输入-1返回主菜单）: ");
            scanf("%s", name);
            if (strcmp(name, "-1") == 0)
            {
                printf("返回主菜单。\n");
                break;
            }
            findStudentByName(*head, name);
        }
        break;
    case 4:
        while (1)
        {
            printf("请输入要修改的学生学号（输入-1返回主菜单）: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("返回主菜单。\n");
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
            printf("请输入要删除的学生学号（输入-1返回主菜单）: ");
            scanf("%s", id);
            if (strcmp(id, "-1") == 0)
            {
                printf("返回主菜单。\n");
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
        printf("\n课程平均分：\n");
        for (int i = 0; i < 5; i++)
        {
            printf("课程 %d: %.2f\n", i + 1, averages[i]);
        }
        break;
    case 10:
    {
        int course_index;
        printf("请输入课程编号（1-5，输入-1返回主菜单）: ");
        scanf("%d", &course_index);
        if (course_index == -1)
        {
            printf("返回主菜单。\n");
            break;
        }
        if (course_index < 1 || course_index > 5)
        {
            printf("无效的课程编号！\n");
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
            printf("数据保存成功！\n");
        }
        else
        {
            printf("数据保存失败！\n");
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
        printf("无效的选择，请重新输入！\n");
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
            printf("内存分配失败！\n");
            return head;
        }

        printf("请输入学号（输入-1返回主菜单）: ");
        scanf("%s", newStudent->id);
        if (strcmp(newStudent->id, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(newStudent);
            return head;
        }

        if (findStudentById(head, newStudent->id) != NULL)
        {
            printf("该学号已存在，无法添加！\n");
            free(newStudent);
            continue;
        }

        printf("请输入姓名（输入-1返回主菜单）: ");
        scanf("%s", newStudent->name);
        if (strcmp(newStudent->name, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(newStudent);
            return head;
        }

        while (1)
        {
            printf("请输入性别（男/女，输入-1返回主菜单）: ");
            scanf("%s", newStudent->gender);
            if (strcmp(newStudent->gender, "-1") == 0)
            {
                printf("返回主菜单。\n");
                free(newStudent);
                return head;
            }
            if (strcmp(newStudent->gender, "男") == 0 || strcmp(newStudent->gender, "女") == 0)
            {
                break;
            }
            else
            {
                printf("无效的性别！请输入'男'或'女'。\n");
            }
        }

        printf("请输入5门课程成绩（0~100，输入-1返回主菜单）:\n");
        for (int i = 0; i < 5; i++)
        {
            float score;
            while (1)
            {
                printf("课程 %d: ", i + 1);
                scanf("%f", &score);
                if (score == -1)
                {
                    printf("返回主菜单。\n");
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
                    printf("无效的成绩！请输入0到100之间的分数。\n");
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

        printf("学生信息添加成功！\n");
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
        printf("内存分配失败！\n");
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
        printf("操作已取消。\n");
        return NULL;
    }

    Student *current = head;

    while (current != NULL)
    {
        if (strcmp(current->id, id) == 0)
        {
            printf("\n找到学生信息：\n");
            printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                   "学号", "姓名", "性别", "课程1", "课程2", "课程3", "课程4", "课程5", "总分", "平均分", "排名");
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

    printf("未找到学号为 %s 的学生！\n", id);
    return NULL;
}

Student *findStudentByName(Student *head, char *name)
{
    if (strcmp(name, "-1") == 0)
    {
        printf("操作已取消。\n");
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
                printf("\n找到学生信息：\n");
                printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
                       "学号", "姓名", "性别", "课程1", "课程2", "课程3", "课程4", "课程5", "总分", "平均分", "排名");
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
        printf("未找到姓名为 %s 的学生！\n", name);
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
        printf("无法打开文件 %s 进行写入！\n", "students.txt");
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
        printf("无法打开文件 %s 进行读取！\n", "students.txt");
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
        printf("无法打开用户文件进行写入！\n");
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
        printf("学生列表为空！\n");
        return NULL;
    }

    printf("确定要删除学号为 %s 的学生吗？(y/n，输入-1返回上一级菜单): ", id);
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("返回上一级菜单。\n");
        return head;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("删除已取消。\n");
        return head;
    }

    if (strcmp(head->id, id) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("已删除学号为 %s 的学生！\n", id);
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
        printf("已删除学号为 %s 的学生！\n", id);
    }
    else
    {
        printf("未找到学号为 %s 的学生！\n", id);
    }

    return head;
}

Student *modifyStudentById(Student *head, char *id)
{
    if (strcmp(id, "-1") == 0)
    {
        printf("操作已取消。\n");
        return head;
    }

    Student *student = findStudentById(head, id);
    if (student == NULL)
    {
        return head;
    }

    while (1)
    {
        printf("\n请选择要修改的信息：\n");
        printf("1. 姓名\n");
        printf("2. 性别\n");
        printf("3. 成绩\n");
        printf("-1. 返回上一级菜单\n");

        int choice;
        printf("请输入选择: ");
        scanf("%d", &choice);
        if (choice == -1)
        {
            printf("返回上一级菜单。\n");
            return head;
        }

        switch (choice)
        {
        case 1:
            printf("请输入新姓名（输入-1取消）: ");
            scanf("%s", student->name);
            if (strcmp(student->name, "-1") == 0)
            {
                printf("操作已取消。\n");
                continue;
            }
            printf("姓名修改成功！\n");
            break;
        case 2:
            while (1)
            {
                printf("请输入新性别（男/女，输入-1取消）: ");
                scanf("%s", student->gender);
                if (strcmp(student->gender, "-1") == 0)
                {
                    printf("操作已取消。\n");
                    break;
                }
                if (strcmp(student->gender, "男") == 0 || strcmp(student->gender, "女") == 0)
                {
                    printf("性别修改成功！\n");
                    break;
                }
                else
                {
                    printf("无效的性别！请输入'男'或'女'。\n");
                }
            }
            break;
        case 3:
            printf("\n请选择要修改的课程：\n");
            printf("1. 课程1\n");
            printf("2. 课程2\n");
            printf("3. 课程3\n");
            printf("4. 课程4\n");
            printf("5. 课程5\n");
            printf("-1. 返回上一级菜单\n");

            int courseChoice;
            printf("请输入要修改的课程编号（1-5，输入-1取消）: ");
            scanf("%d", &courseChoice);

            if (courseChoice == -1)
            {
                printf("返回上一级菜单。\n");
                continue;
            }
            else if (courseChoice >= 1 && courseChoice <= 5)
            {
                float score;
                while (1)
                {
                    printf("请输入课程 %d 的新成绩（0~100，输入-1取消）: ", courseChoice);
                    scanf("%f", &score);
                    if (score == -1)
                    {
                        printf("操作已取消。\n");
                        break;
                    }
                    if (score >= 0 && score <= 100)
                    {
                        student->scores[courseChoice - 1] = score;
                        calculateTotalAndAverage(student);
                        printf("课程 %d 成绩修改成功！\n", courseChoice);
                        break;
                    }
                    else
                    {
                        printf("无效的成绩！请输入0到100之间的分数。\n");
                    }
                }
            }
            else
            {
                printf("无效的课程选择！\n");
            }
            break;
        default:
            printf("无效的选择！\n");
        }
    }

    return head;
}

void displayAllStudents(Student *head)
{
    if (head == NULL)
    {
        printf("学生列表为空！\n");
        return;
    }

    printf("\n所有学生信息：\n");
    printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-5s\n",
           "学号", "姓名", "性别", "课程1", "课程2", "课程3", "课程4", "课程5", "总分", "平均分", "排名");
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
        printf("内存分配失败！\n");
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
        printf("内存分配失败！\n");
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
        printf("学生列表为空！\n");
        return;
    }

    printf("\n请输入最低分数（0-100，输入-1取消）: ");
    float minScore;
    scanf("%f", &minScore);
    if (minScore == -1)
    {
        printf("操作已取消。\n");
        return;
    }
    if (minScore < 0 || minScore > 100)
    {
        printf("无效的分数！请输入0到100之间的分数。\n");
        return;
    }

    printf("\n课程 %d 分数高于 %.2f 的学生：\n", course_index + 1, minScore);
    printf("%-10s %-15s %-5s %-8s\n", "学号", "姓名", "性别", "分数");
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
        printf("没有学生课程 %d 的分数高于 %.2f！\n", course_index + 1, minScore);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("共有 %d 名学生课程 %d 的分数高于 %.2f。\n", count, course_index + 1, minScore);
    }
}

void displayStudentsBelowAverage(Student *head, float averages[5])
{
    if (head == NULL)
    {
        printf("学生列表为空！\n");
        return;
    }

    printf("\n低于平均分的学生：\n");

    for (int i = 0; i < 5; i++)
    {
        printf("\n课程 %d（平均分：%.2f）：\n", i + 1, averages[i]);
        printf("%-10s %-15s %-5s %-8s\n", "学号", "姓名", "性别", "分数");
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
            printf("没有学生低于平均分！\n");
        }
        else
        {
            printf("------------------------------------------\n");
            printf("共有 %d 名学生课程 %d 的分数低于平均分。\n", count, i + 1);
        }
    }
}

void displayFailingStudents(Student *head)
{
    if (head == NULL)
    {
        printf("学生列表为空！\n");
        return;
    }

    printf("\n不及格学生（低于60分）：\n");
    printf("%-10s %-15s %-5s %-11s %-11s %-11s %-11s %-11s\n",
           "学号", "姓名", "性别", "课程1", "课程2", "课程3", "课程4", "课程5");
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
        printf("没有找到不及格的学生！\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("共有 %d 名学生有不及格课程。（* 表示不及格成绩）\n", totalFailingStudents);
    }
}

void exportGradesToCSV(Student *head)
{
    printf("确定要导出成绩到CSV文件吗？(y/n，输入-1返回主菜单): ");
    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "-1") == 0)
    {
        printf("返回主菜单。\n");
        return;
    }
    if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
    {
        printf("导出已取消。\n");
        return;
    }

    FILE *file = fopen("students.csv", "w");
    if (file == NULL)
    {
        printf("无法打开students.csv文件进行写入！\n");
        return;
    }
    fprintf(file, "学号,姓名,性别,课程1,课程2,课程3,课程4,课程5,总分,平均分,排名\n");
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
    printf("成功导出到students.csv文件！\n");
}

void registerUser()
{
    while (1)
    {
        char username[30];
        char password[20];
        User *users = NULL;
        int count = loadUsers(&users);

        printf("请输入新用户名（输入-1返回主菜单）: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                printf("用户名已存在！\n");
                free(users);
                continue;
            }
        }

        printf("请输入新密码（输入-1返回主菜单）: ");
        scanf("%s", password);
        if (strcmp(password, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(users);
            return;
        }

        users = (User *)realloc(users, sizeof(User) * (count + 1));
        strcpy(users[count].username, username);
        strcpy(users[count].password, password);
        saveUsers(users, count + 1);
        printf("用户注册成功！\n");
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
            printf("必须保留至少一个用户！\n");
            free(users);
            return;
        }

        printf("请输入要删除的用户名（输入-1返回主菜单）: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(users);
            return;
        }

        printf("确定要删除用户 %s 吗？(y/n，输入-1返回上一级菜单): ", username);
        char confirm[10];
        scanf("%s", confirm);
        if (strcmp(confirm, "-1") == 0)
        {
            printf("返回上一级菜单。\n");
            free(users);
            return;
        }
        if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0)
        {
            printf("删除已取消。\n");
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
            printf("未找到该用户名！\n");
            free(users);
            continue;
        }
        saveUsers(users, count - 1);
        printf("用户删除成功！\n");
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

        printf("请输入您的用户名（输入-1返回主菜单）: ");
        scanf("%s", username);
        if (strcmp(username, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(users);
            return;
        }

        printf("请输入您的旧密码（输入-1返回主菜单）: ");
        scanf("%s", oldPassword);
        if (strcmp(oldPassword, "-1") == 0)
        {
            printf("返回主菜单。\n");
            free(users);
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, oldPassword) == 0)
            {
                printf("请输入您的新密码（输入-1返回主菜单）: ");
                scanf("%s", newPassword);
                if (strcmp(newPassword, "-1") == 0)
                {
                    printf("返回主菜单。\n");
                    free(users);
                    return;
                }
                strcpy(users[i].password, newPassword);
                saveUsers(users, count);
                printf("密码修改成功！\n");
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("用户名或旧密码错误！\n");
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
        printf("当前没有用户登录。\n");
        return;
    }
    printf("当前登录用户：%s\n", currentUser);
}