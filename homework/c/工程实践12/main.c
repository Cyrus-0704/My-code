#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30
#define MAX_PASSWORD_LEN 20
#define COURSE_NUM 5
#define MAX_GENDER_LEN 10

// 用户登录信息结构
typedef struct
{
    char username[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
} User;

// 学生信息结构
typedef struct Student
{
    char id[20];                 // 学号
    char name[MAX_NAME_LEN];     // 姓名
    char gender[MAX_GENDER_LEN]; // 性别
    float scores[COURSE_NUM];    // 5门课程成绩
    float total;                 // 总分
    float average;               // 平均分
    int rank;                    // 名次
    struct Student *next;        // 链表下一节点指针
} Student;

// 函数声明
// 1. 用户登录相关
int login(char currentUser[]);
int verifyUser(const char *username, const char *password);
void saveUsers(User *users, int count);
int loadUsers(User **users);
void exportGradesToCSV(Student *head);

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
void calculateCourseAverages(Student *head, float averages[COURSE_NUM]);
void displayStudentsBelowAverage(Student *head, float averages[COURSE_NUM]);
void displayExcellentStudents(Student *head);
void displayFailingStudents(Student *head);

// 4. 文件操作
int saveStudentsToFile(Student *head, const char *filename);
Student *loadStudentsFromFile(const char *filename);

// 5. 菜单和主程序
void displayMenu();
void processChoice(int choice, Student **head, char currentUser[]);

// Register new user
void registerUser()
{
    char username[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    User *users = NULL;
    int count = loadUsers(&users);
    printf("Please enter new username: ");
    scanf("%s", username);
    // Check if username exists
    for (int i = 0; i < count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Username already exists!\n");
            free(users);
            return;
        }
    }
    printf("Please enter new password: ");
    scanf("%s", password);
    users = (User *)realloc(users, sizeof(User) * (count + 1));
    strcpy(users[count].username, username);
    strcpy(users[count].password, password);
    saveUsers(users, count + 1);
    printf("User registered successfully!\n");
    free(users);
}

// Delete user
void deleteUser()
{
    char username[MAX_NAME_LEN];
    User *users = NULL;
    int count = loadUsers(&users);
    if (count <= 1)
    {
        printf("At least one user must remain!\n");
        free(users);
        return;
    }
    printf("Please enter username to delete: ");
    scanf("%s", username);
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
        return;
    }
    saveUsers(users, count - 1);
    printf("User deleted successfully!\n");
    free(users);
}

// Change password
void changePassword()
{
    char username[MAX_NAME_LEN];
    char oldPassword[MAX_PASSWORD_LEN];
    char newPassword[MAX_PASSWORD_LEN];
    User *users = NULL;
    int count = loadUsers(&users);
    int found = 0;
    printf("Please enter your username: ");
    scanf("%s", username);
    printf("Please enter your old password: ");
    scanf("%s", oldPassword);
    for (int i = 0; i < count; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, oldPassword) == 0)
        {
            printf("Please enter your new password: ");
            scanf("%s", newPassword);
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
    }
    free(users);
}

// 查看当前登录用户
void viewCurrentUser(char currentUser[])
{
    printf("Current logged in user: %s\n", currentUser); // 当前登录用户：%s
}


int main()
{
    int choice;
    Student *head = NULL;
    char currentUser[MAX_NAME_LEN] = ""; // 当前登录用户名，局部变量
    int loggedIn = 0;
    int exitFlag = 0;

    // 验证用户登录，三次机会
    loggedIn = login(currentUser);
    if (!loggedIn)
    {
        printf("Login failed, program exiting!\n");
        return 1;
    }

    // 从文件加载学生数据，若无数据则新建链表
    head = loadStudentsFromFile("students.txt");
    if (head == NULL)
    {
        printf("No student data found or data is empty, new student list will be created.\n"); // 未找到学生数据或数据为空，将创建新的学生列表。
        head = createStudentList();
    }

    // 主菜单循环，直到用户选择退出
    do
    {
        displayMenu();                        // 显示菜单
        printf("Please enter your choice: "); // 请输入您的选择：
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
            processChoice(choice, &head, currentUser); // 传递currentUser
        }
    } while (!exitFlag);

    // 退出前保存学生数据
    if (saveStudentsToFile(head, "students.txt"))
    {
        printf("Data saved successfully!\n"); // 数据保存成功！
    }
    else
    {
        printf("Data saving failed!\n"); // 数据保存失败！
    }

    // 释放链表内存
    freeStudentList(head);

    printf("Thank you for using this system, goodbye!\n"); // 感谢使用本系统，再见！
    return 0;
}

// 登录验证函数
int login(char currentUser[])
{
    char username[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS)
    {
        printf("Please enter username: ");
        scanf("%s", username);
        printf("Please enter password: ");
        scanf("%s", password);

        if (verifyUser(username, password))
        {
            strcpy(currentUser, username); // 保存当前登录用户名
            printf("Login successful! Welcome to the student grade management system.\n");
            return 1;
        }
        else
        {
            attempts++;
            printf("Username or password incorrect, you have %d more attempts.\n", MAX_ATTEMPTS - attempts);
        }
    }

    printf("Attempts exceeded, login failed!\n");
    return 0;
}

// 验证用户名和密码
int verifyUser(const char *username, const char *password)
{
    User *users = NULL;
    int count = loadUsers(&users);

    if (count <= 0)
    {
        // 如果没有用户数据，创建默认管理员账户
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            // 创建默认用户并保存
            User defaultUser;
            strcpy(defaultUser.username, "admin");
            strcpy(defaultUser.password, "admin");
            saveUsers(&defaultUser, 1);
            return 1;
        }
        return 0;
    }

    // 遍历用户列表验证用户名和密码
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

// 显示主菜单
void displayMenu()
{
    printf("\n=== Student Grade Management System ===\n");        // 学生成绩管理系统
    printf("1. Add student information\n");                       // 添加学生信息
    printf("2. Find student by ID\n");                            // 按学号查找学生
    printf("3. Find student by name\n");                          // 按姓名查找学生
    printf("4. Modify student information\n");                    // 修改学生信息
    printf("5. Delete student information\n");                    // 删除学生信息
    printf("6. Display all student information\n");               // 显示所有学生信息
    printf("7. Sort by ID\n");                                    // 按学号排序
    printf("8. Sort by total score\n");                           // 按总分排序
    printf("9. Display course statistics\n");                     // 显示课程统计
    printf("10. Display excellent students (above 90 points)\n"); // 显示优秀学生（90分以上）
    printf("11. Display failing students\n");                     // 显示不及格学生
    printf("12. Display students below average\n");               // 显示低于平均分的学生
    printf("13. Save data to file\n");                            // 保存数据到文件
    printf("14. Register new user\n");                            // 注册新用户
    printf("15. Delete user\n");                                  // 删除用户
    printf("16. Change password\n");                              // 修改密码
    printf("17. View current user\n");                            // 查看当前登录用户
    printf("18. Export grades to CSV\n");                         // 导出成绩为CSV
    printf("0. Exit system\n");                                   // 退出系统
}

// 处理用户选择
void processChoice(int choice, Student **head, char currentUser[])
{
    char id[20];
    char name[MAX_NAME_LEN];
    float averages[COURSE_NUM];

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
        printf("Please enter the ID of the student to find: ");
        scanf("%s", id);
        findStudentById(*head, id);
        break;
    case 3:
        printf("Please enter the name of the student to find: ");
        scanf("%s", name);
        findStudentByName(*head, name);
        break;
    case 4:
        printf("Please enter the ID of the student to modify: ");
        scanf("%s", id);
        *head = modifyStudentById(*head, id);
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
        break;
    case 5:
        printf("Please enter the ID of the student to delete: ");
        scanf("%s", id);
        *head = deleteStudentById(*head, id);
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
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
        for (int i = 0; i < COURSE_NUM; i++)
        {
            printf("Course %d: %.2f\n", i + 1, averages[i]);
        }
        break;
    case 10:
        displayExcellentStudents(*head);
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

// 创建空的学生链表
Student *createStudentList()
{
    return NULL;
}

// 辅助函数：二次确认
int confirmReturn()
{
    char confirm[10];
    printf("Are you sure you want to cancel and return to main menu? (yes/no): ");
    scanf("%s", confirm);
    return strcmp(confirm, "yes") == 0;
}

// 添加学生信息
Student *addStudent(Student *head)
{
    Student *newStudent = (Student *)malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    printf("Please enter ID: "); // 请输入学号：
    scanf("%s", newStudent->id);
    if (strcmp(newStudent->id, "0") == 0)
    {
        if (confirmReturn())
        {
            free(newStudent);
            return head;
        }
    }

    // 检查学号是否已存在
    if (findStudentById(head, newStudent->id) != NULL)
    {
        printf("This ID already exists, cannot add!\n");
        free(newStudent);
        return head;
    }

    printf("Please enter name (or 0 to return to main menu): ");
    scanf("%s", newStudent->name);
    if (strcmp(newStudent->name, "0") == 0)
    {
        if (confirmReturn())
        {
            free(newStudent);
            return head;
        }
    }

    // 性别输入合法性判断，只允许male或female
    while (1)
    {
        printf("Please enter gender (male/female, or 0 to return): ");
        scanf("%s", newStudent->gender);
        if (strcmp(newStudent->gender, "0") == 0)
        {
            if (confirmReturn())
            {
                free(newStudent);
                return head;
            }
            else
            {
                continue;
            }
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

    printf("Please enter 5 course scores (0~100, or -1 to return):\n");
    for (int i = 0; i < COURSE_NUM; i++)
    {
        float score;
        while (1)
        {
            printf("Course %d: ", i + 1);
            scanf("%f", &score);
            if (score == -1)
            {
                if (confirmReturn())
                {
                    free(newStudent);
                    return head;
                }
                else
                {
                    continue;
                }
            }
            if (score >= 0 && score <= 100)
            {
                newStudent->scores[i] = score;
                break;
            }
            else
            {
                printf("Invalid score! Please enter a value between 0 and 100.\n"); // 分数无效！请输入0到100之间的值。
            }
        }
    }

    // 计算总分和平均分
    calculateTotalAndAverage(newStudent);

    // 将新节点插入链表头部
    newStudent->next = head;
    head = newStudent;

    printf("Student information added successfully!\n"); // 学生信息添加成功！
    return head;
}

// 计算单个学生的总分和平均分
void calculateTotalAndAverage(Student *student)
{
    student->total = 0;
    for (int i = 0; i < COURSE_NUM; i++)
    {
        student->total += student->scores[i];
    }
    student->average = student->total / COURSE_NUM;
}

// 计算所有学生的总分和平均分
void calculateAllTotalAndAverage(Student *head)
{
    Student *current = head;
    while (current != NULL)
    {
        calculateTotalAndAverage(current);
        current = current->next;
    }
}

// 根据总分为学生排名
void rankStudentsByTotal(Student *head)
{
    // 先计算学生人数
    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // 如果没有学生，直接返回
    if (count == 0)
    {
        return;
    }

    // 创建学生指针数组，方便排序
    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // 填充数组
    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    // 按总分降序排序
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

    // 设置名次
    for (int i = 0; i < count; i++)
    {
        students[i]->rank = i + 1;
    }

    free(students);
}

// 根据学号查找学生
Student *findStudentById(Student *head, const char *id)
{
    Student *current = head;

    while (current != NULL)
    {
        if (strcmp(current->id, id) == 0)
        {
            printf("Student information found:\n");                                                // 找到学生信息：
            printf("ID: %s, Name: %s, Gender: %s\n", current->id, current->name, current->gender); // 学号: %s, 姓名: %s, 性别: %s
            printf("Scores: ");                                                                    // 成绩：
            for (int i = 0; i < COURSE_NUM; i++)
            {
                printf("%.2f ", current->scores[i]);
            }
            printf("\nTotal: %.2f, Average: %.2f, Rank: %d\n", current->total, current->average, current->rank); // 总分: %.2f, 平均分: %.2f, 名次: %d
            return current;
        }
        current = current->next;
    }

    printf("Student with ID %s not found!\n", id); // 未找到学号为%s的学生！
    return NULL;
}

// 根据姓名查找学生
Student *findStudentByName(Student *head, const char *name)
{
    Student *current = head;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            if (!found)
            {
                printf("Student information found:\n");
                found = 1;
            }
            printf("ID: %s, Name: %s, Gender: %s\n", current->id, current->name, current->gender);
            printf("Scores: ");
            for (int i = 0; i < COURSE_NUM; i++)
            {
                printf("%.2f ", current->scores[i]);
            }
            printf("\nTotal: %.2f, Average: %.2f, Rank: %d\n\n", current->total, current->average, current->rank);
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Student with name %s not found!\n", name); // 未找到姓名为%s的学生！
        return NULL;
    }

    return head;
}

// 保存学生数据到文件（文本格式）
int saveStudentsToFile(Student *head, const char *filename)
{
    (void)filename; // 消除未使用参数警告
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
        for (int i = 0; i < COURSE_NUM; i++)
        {
            fprintf(file, "%.2f ", current->scores[i]);
        }
        fprintf(file, "%.2f %.2f %d\n", current->total, current->average, current->rank);
        current = current->next;
    }
    fclose(file);
    return 1;
}

// 从文件加载学生数据（文本格式）
Student *loadStudentsFromFile(const char *filename)
{
    (void)filename; // 消除未使用参数警告
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
        for (int i = 0; i < COURSE_NUM; i++)
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

// 释放学生链表内存
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

// 保存用户信息（文本格式）
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

// 加载用户信息（文本格式）
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

// 根据学号删除学生
Student *deleteStudentById(Student *head, const char *id)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n"); // 学生列表为空！
        printf("Student list is empty!\n");
        return NULL;
    }

    // 如果要删除的是头节点
    if (strcmp(head->id, id) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("Student with ID %s deleted!\n", id); // 已删除学号为%s的学生！
        return head;
    }

    // 查找要删除的节点及其前一个节点
    Student *current = head;
    while (current->next != NULL && strcmp(current->next->id, id) != 0)
    {
        current = current->next;
    }

    // 如果找到了要删除的节点
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

// 根据姓名删除学生
Student *deleteStudentByName(Student *head, const char *name)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return NULL;
    }

    // 先处理头节点
    while (head != NULL && strcmp(head->name, name) == 0)
    {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("Student with name %s deleted!\n", name); // 已删除姓名为%s的学生！
    }

    if (head == NULL)
    {
        return NULL;
    }

    // 处理其余节点
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
        printf("Deleted %d students with name %s!\n", deleted, name); // 已删除%d个姓名为%s的学生！
    }
    else
    {
        printf("Student with name %s not found!\n", name);
    }

    return head;
}

// 根据学号修改学生信息
Student *modifyStudentById(Student *head, const char *id)
{
    Student *student = findStudentById(head, id);
    if (student == NULL)
    {
        return head;
    }

    printf("\nPlease choose the information to modify:\n");
    printf("1. Name\n");
    printf("2. Gender\n");
    printf("3. Scores\n");
    printf("0. Cancel modification\n");

    int choice;
    printf("Please enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 0:
        return head;
    case 1:
        printf("Please enter new name (or 0 to cancel): ");
        scanf("%s", student->name);
        if (strcmp(student->name, "0") == 0)
        {
            if (confirmReturn())
                return head;
        }
        printf("Name modified successfully!\n"); // 姓名修改成功！
        break;
    case 2:
        // 修改性别时也加合法性判断
        while (1)
        {
            printf("Please enter new gender (male/female, or 0 to cancel): ");
            scanf("%s", student->gender);
            if (strcmp(student->gender, "0") == 0)
            {
                if (confirmReturn())
                    return head;
                else
                    continue;
            }
            if (strcmp(student->gender, "male") == 0 || strcmp(student->gender, "female") == 0)
            {
                printf("Gender modified successfully!\n"); // 性别修改成功！
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
        for (int i = 0; i < COURSE_NUM; i++)
        {
            float score;
            while (1)
            {
                printf("Course %d: ", i + 1);
                scanf("%f", &score);
                if (score == -1)
                {
                    if (confirmReturn())
                        return head;
                    else
                        continue;
                }
                if (score >= 0 && score <= 100)
                {
                    student->scores[i] = score;
                    break;
                }
                else
                {
                    printf("Invalid score! Please enter a value between 0 and 100, or -1 to cancel.\n");
                }
            }
        }
        printf("Scores modified successfully!\n"); // 成绩修改成功！
        break;
    default:
        printf("Invalid choice!\n"); // 无效的选择！
    }

    return head;
}

// 根据姓名修改学生信息
Student *modifyStudentByName(Student *head, const char *name)
{
    // 查找是否有多个同名学生
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

    // 只有一个学生时，可以直接修改
    current = head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            printf("\nPlease choose the information to modify:\n");
            printf("1. ID\n");
            printf("2. Gender\n");
            printf("3. Scores\n");
            printf("0. Cancel modification\n");

            int choice;
            printf("Please enter choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 0:
                return head;
            case 1:
                printf("Please enter new ID (or 0 to cancel): ");
                scanf("%s", current->id);
                if (strcmp(current->id, "0") == 0)
                {
                    if (confirmReturn())
                        return head;
                }
                printf("ID modified successfully!\n"); // 学号修改成功！
                break;
            case 2:
                while (1)
                {
                    printf("Please enter new gender (male/female, or 0 to cancel): ");
                    scanf("%s", current->gender);
                    if (strcmp(current->gender, "0") == 0)
                    {
                        if (confirmReturn())
                            return head;
                        else
                            continue;
                    }
                    if (strcmp(current->gender, "male") == 0 || strcmp(current->gender, "female") == 0)
                    {
                        printf("Gender modified successfully!\n"); // 性别修改成功！
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
                for (int i = 0; i < COURSE_NUM; i++)
                {
                    float score;
                    while (1)
                    {
                        printf("Course %d: ", i + 1);
                        scanf("%f", &score);
                        if (score == -1)
                        {
                            if (confirmReturn())
                                return head;
                            else
                                continue;
                        }
                        if (score >= 0 && score <= 100)
                        {
                            current->scores[i] = score;
                            break;
                        }
                        else
                        {
                            printf("Invalid score! Please enter a value between 0 and 100, or -1 to cancel.\n");
                        }
                    }
                }
                printf("Scores modified successfully!\n"); // 成绩修改成功！
                break;
            default:
                printf("Invalid choice!\n"); // 无效的选择！
            }
            break;
        }
        current = current->next;
    }

    return head;
}

// 显示所有学生信息
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

// 按学号排序
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

    // 创建学生指针数组
    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    // 填充数组
    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    // 按学号排序（冒泡排序）
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(students[j]->id, students[j + 1]->id) > 0)
            {
                // 交换指针
                Student *temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // 重新链接节点
    for (int i = 0; i < count - 1; i++)
    {
        students[i]->next = students[i + 1];
    }
    students[count - 1]->next = NULL;

    // 保存新的链表头
    head = students[0];

    // 释放数组
    free(students);

    return head;
}

// 按总分排序
Student *sortStudentsByTotal(Student *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    // 先计算学生人数
    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // 创建学生指针数组，方便排序
    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    // 填充数组
    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    // 按总分降序排序（冒泡排序）
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

    // 设置名次
    for (int i = 0; i < count; i++)
    {
        students[i]->rank = i + 1;
    }

    // 重新链接节点，按总分排序
    for (int i = 0; i < count - 1; i++)
    {
        students[i]->next = students[i + 1];
    }
    students[count - 1]->next = NULL;

    // 保存新的链表头
    head = students[0];

    // 释放数组
    free(students);

    return head;
}

// 计算各门课程平均分
void calculateCourseAverages(Student *head, float averages[COURSE_NUM])
{
    if (head == NULL)
    {
        for (int i = 0; i < COURSE_NUM; i++)
        {
            averages[i] = 0.0;
        }
        return;
    }

    // 初始化总和和学生计数
    float sums[COURSE_NUM] = {0};
    int count = 0;

    // 累加所有学生的各科目成绩
    Student *current = head;
    while (current != NULL)
    {
        for (int i = 0; i < COURSE_NUM; i++)
        {
            sums[i] += current->scores[i];
        }
        count++;
        current = current->next;
    }

    // 计算平均分
    for (int i = 0; i < COURSE_NUM; i++)
    {
        averages[i] = (count > 0) ? sums[i] / count : 0;
    }
}

// 显示成绩在特定值以上的学生
void displayStudentsWithScoreAbove(Student *head, float score, int course_index)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nStudents with course %d score above %.2f:\n", course_index + 1, score);
    printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
    printf("------------------------------------------\n");

    Student *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (current->scores[course_index] >= score)
        {
            printf("%-10s %-15s %-5s %-8.2f\n",
                   current->id, current->name, current->gender, current->scores[course_index]);
            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("No student's course %d score above %.2f!\n", course_index + 1, score);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("Total %d students' course %d score above %.2f.\n", count, course_index + 1, score);
    }
}

// 显示成绩在特定值以下的学生
void displayStudentsWithScoreBelow(Student *head, float score, int course_index)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nStudents with course %d score below %.2f:\n", course_index + 1, score);
    printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
    printf("------------------------------------------\n");

    Student *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (current->scores[course_index] < score)
        {
            printf("%-10s %-15s %-5s %-8.2f\n",
                   current->id, current->name, current->gender, current->scores[course_index]);
            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("No student's course %d score below %.2f!\n", course_index + 1, score);
    }
    else
    {
        printf("------------------------------------------\n");
        printf("Total %d students' course %d score below %.2f.\n", count, course_index + 1, score);
    }
}

// 显示低于平均分的学生
void displayStudentsBelowAverage(Student *head, float averages[COURSE_NUM])
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nStudents below average:\n");

    for (int i = 0; i < COURSE_NUM; i++)
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

// 显示优秀学生（90分以上）
void displayExcellentStudents(Student *head)
{
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    printf("\nExcellent students (above 90 points):\n");

    for (int i = 0; i < COURSE_NUM; i++)
    {
        printf("\nCourse %d:\n", i + 1);
        displayStudentsWithScoreAbove(head, 90.0, i);
    }
}

// 显示不及格学生（60分以下）
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

        // 先检查是否有不及格课程
        for (int i = 0; i < COURSE_NUM; i++)
        {
            if (current->scores[i] < 60.0)
            {
                hasFailingScore = 1;
                break;
            }
        }

        // 如果有不及格课程，显示学生信息
        if (hasFailingScore)
        {
            totalFailingStudents++;
            printf("%-10s %-15s %-5s ", current->id, current->name, current->gender);

            // 显示所有课程成绩，不及格的标记出来
            for (int i = 0; i < COURSE_NUM; i++)
            {
                if (current->scores[i] < 60.0)
                {
                    printf("%-10.2f*", current->scores[i]); // 不及格成绩加*标记
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

// 导出成绩为CSV文件
void exportGradesToCSV(Student *head)
{
    FILE *file = fopen("students.csv", "w");
    if (file == NULL)
    {
        printf("Cannot open students.csv for writing!\n"); // 无法打开students.csv进行写入！
        return;
    }
    // 写表头
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
    printf("Exported to students.csv successfully!\n"); // 成绩已成功导出到students.csv！
}

// 这里只实现了部分核心功能，其他函数可以根据需要补充实现
