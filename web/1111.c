#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 本程序中使用二级指针的主要场景和原因：
 *
 * 1. 链表头指针的修改
 *    - 在processChoice函数中使用 Student **head
 *    - 原因：某些操作（如添加、删除、修改学生信息）可能会改变链表的头节点
 *    - 通过二级指针，可以在函数内部修改链表头指针的指向
 *    - 例如：*head = addStudent(*head) 可以更新链表头指针
 *
 * 2. 动态内存分配
 *    - 在loadUsers函数中使用 User **users
 *    - 原因：需要在函数内部动态分配内存并返回给调用者
 *    - 通过二级指针，可以在函数内部修改指针指向的内存地址
 *    - 如果使用一级指针，函数内部对指针的修改不会影响外部
 *
 * 3. 为什么需要二级指针：
 *    - 在C语言中，函数参数是按值传递的
 *    - 如果使用一级指针，函数内部对指针的修改不会影响外部
 *    - 使用二级指针可以修改指针本身的值，而不仅仅是修改指针指向的内容
 *    - 这对于需要修改链表头节点或动态分配内存的场景非常重要
 */

// 用户登录相关的结构体定义
typedef struct
{
    char username[30]; // 用户名，最大长度30个字符
    char password[20]; // 密码，最大长度20个字符
} User;

// 学生信息结构体定义
typedef struct Student
{
    char id[20];          // 学号，最大长度20个字符
    char name[30];        // 姓名，最大长度30个字符
    char gender[10];      // 性别，最大长度10个字符
    float scores[5];      // 5门课程的成绩数组
    float total;          // 总分
    float average;        // 平均分
    int rank;             // 排名
    struct Student *next; // 指向下一个学生节点的指针
} Student;

// 函数声明
// 1. 用户登录相关函数
int login(char currentUser[]);                              // 用户登录函数，返回登录状态，保存当前用户名
int verifyUser(const char *username, const char *password); // 验证用户名和密码
void saveUsers(User *users, int count);                     // 保存用户信息到文件
/**
 * 从文件加载用户信息
 * @param users 二级指针，用于在函数内部动态分配内存并存储用户数据
 * @return 返回加载的用户数量，如果加载失败返回0
 *
 * 使用二级指针的原因：
 * 1. 需要在函数内部动态分配内存
 * 2. 需要修改指针指向的内存地址
 * 3. 如果使用一级指针，函数内部对指针的修改不会影响外部
 */
int loadUsers(User **users);
void exportGradesToCSV(Student *head);    // 导出成绩到CSV文件
void registerUser();                      // 注册新用户
void deleteUser();                        // 删除用户
void changePassword();                    // 修改密码
void viewCurrentUser(char currentUser[]); // 查看当前登录用户

// 2. 学生信息管理
Student *createStudentList();                                  // 创建学生链表
Student *addStudent(Student *head);                            // 添加学生信息
Student *deleteStudentById(Student *head, const char *id);     // 根据学号删除学生
Student *deleteStudentByName(Student *head, const char *name); // 根据姓名删除学生
Student *findStudentById(Student *head, const char *id);       // 根据学号查找学生
Student *findStudentByName(Student *head, const char *name);   // 根据姓名查找学生
Student *modifyStudentById(Student *head, const char *id);     // 根据学号修改学生信息
Student *modifyStudentByName(Student *head, const char *name); // 根据姓名修改学生信息
void displayAllStudents(Student *head);                        // 显示所有学生信息
void freeStudentList(Student *head);                           // 释放学生链表内存

// 3. 成绩统计与分析
void calculateTotalAndAverage(Student *student);                                  // 计算单个学生的总分和平均分
void calculateAllTotalAndAverage(Student *head);                                  // 计算所有学生的总分和平均分
void rankStudentsByTotal(Student *head);                                          // 根据总分对学生进行排名
Student *sortStudentsById(Student *head);                                         // 按学号排序学生
Student *sortStudentsByTotal(Student *head);                                      // 按总分排序学生
void displayStudentsWithScoreAbove(Student *head, float score, int course_index); // 显示指定课程分数高于某值的学生
void displayStudentsWithScoreBelow(Student *head, float score, int course_index); // 显示指定课程分数低于某值的学生
void calculateCourseAverages(Student *head, float averages[5]);                   // 计算各门课程的平均分
void displayStudentsBelowAverage(Student *head, float averages[5]);               // 显示低于平均分的学生
void displayFailingStudents(Student *head);                                       // 显示不及格学生

// 4. 文件操作函数
int saveStudentsToFile(Student *head, const char *filename); // 将学生信息保存到文件
Student *loadStudentsFromFile(const char *filename);         // 从文件加载学生信息

// 5. 菜单和主程序函数
void displayMenu(); // 显示主菜单
/**
 * 处理用户选择的操作
 * @param choice 用户选择的菜单选项
 * @param head 二级指针，指向学生链表的头节点
 * @param currentUser 当前登录用户的用户名
 *
 * 使用二级指针的原因：
 * 1. 某些操作（如添加、删除、修改学生信息）可能会改变链表的头节点
 * 2. 需要修改链表头指针的值
 * 3. 通过二级指针，可以在函数内部修改链表头指针的指向
 */
void processChoice(int choice, Student **head, char currentUser[]);

int main()
{
    int choice;                // 用户选择的菜单选项
    Student *head = NULL;      // 学生链表头指针
    char currentUser[30] = ""; // 当前登录用户名
    int loggedIn = 0;          // 登录状态标志
    int exitFlag = 0;          // 退出标志

    // 用户登录
    loggedIn = login(currentUser);
    if (!loggedIn)
    {
        printf("Login failed, program exiting!\n");
        return 1;
    }

    // 加载学生数据
    head = loadStudentsFromFile("students.txt");
    if (head == NULL)
    {
        printf("No student data found or data is empty, new student list will be created.\n");
        head = createStudentList();
    }

    // 主菜单循环，直到用户选择退出
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

    // 退出前保存学生数据
    if (saveStudentsToFile(head, "students.txt"))
    {
        printf("Data saved successfully!\n");
    }
    else
    {
        printf("Data saving failed!\n");
    }

    // 释放学生链表内存
    freeStudentList(head);

    printf("Thank you for using this system, goodbye!\n");
    return 0;
}

// 登录验证函数
int login(char currentUser[])
{
    char username[30]; // 存储用户输入的用户名
    char password[20]; // 存储用户输入的密码
    int attempts = 0;  // 登录尝试次数

    // 最多允许3次登录尝试
    while (attempts < 3)
    {
        printf("Please enter username: ");
        scanf("%s", username);
        printf("Please enter password: ");
        scanf("%s", password);

        // 验证用户名和密码
        if (verifyUser(username, password))
        {
            strcpy(currentUser, username); // 保存当前登录用户名
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

// 验证用户登录信息
int verifyUser(const char *username, const char *password)
{
    User *users = NULL;
    int count = loadUsers(&users); // 加载用户数据

    // 如果没有用户数据，创建默认管理员账户
    if (count <= 0)
    {
        // 检查是否为默认管理员账户
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            // 创建默认用户并保存
            User InitialUser;
            strcpy(InitialUser.username, "admin");
            strcpy(InitialUser.password, "admin");
            saveUsers(&InitialUser, 1);
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

// 保存用户信息到文件
void saveUsers(User *users, int count)
{
    FILE *file = fopen("users.txt", "w"); // 以写入模式打开用户文件
    if (file == NULL)
    {
        printf("Cannot open user file for writing!\n");
        return;
    }
    // 将用户信息写入文件
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

/**
 * 从文件加载用户信息
 * @param users 二级指针，用于在函数内部动态分配内存并存储用户数据
 * @return 返回加载的用户数量，如果加载失败返回0
 *
 * 使用二级指针的原因：
 * 1. 需要在函数内部动态分配内存
 * 2. 需要修改指针指向的内存地址
 * 3. 如果使用一级指针，函数内部对指针的修改不会影响外部
 */
int loadUsers(User **users)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    int capacity = 10;
    int count = 0;
    *users = (User *)malloc(sizeof(User) * capacity); // 通过二级指针分配内存
    if (*users == NULL)
    {
        fclose(file);
        return 0;
    }
    // 读取文件中的用户信息
    while (!feof(file))
    {
        // 如果用户数量超过容量，重新分配内存
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

/**
 * 处理用户选择的操作
 * @param choice 用户选择的菜单选项
 * @param head 二级指针，指向学生链表的头节点
 * @param currentUser 当前登录用户的用户名
 *
 * 使用二级指针的原因：
 * 1. 某些操作（如添加、删除、修改学生信息）可能会改变链表的头节点
 * 2. 需要修改链表头指针的值
 * 3. 通过二级指针，可以在函数内部修改链表头指针的指向
 */
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
        *head = addStudent(*head); // 修改链表头指针
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
        *head = modifyStudentById(*head, id); // 修改链表头指针
        calculateAllTotalAndAverage(*head);
        rankStudentsByTotal(*head);
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

// 创建空的学生链表
Student *createStudentList()
{
    return NULL; // 返回空链表
}

// 添加学生信息（使用尾插法）
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
        newStudent->next = NULL; // 新节点的next指针初始化为NULL

        // 使用尾插法添加新节点
        if (head == NULL)
        {
            // 如果链表为空，新节点作为头节点
            head = newStudent;
        }
        else
        {
            // 找到链表的最后一个节点
            Student *current = head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            // 将新节点连接到最后一个节点后面
            current->next = newStudent;
        }

        printf("学生信息添加成功！\n");
        return head;
    }
}

// 从文件加载学生数据（使用尾插法）
Student *loadStudentsFromFile(const char *filename)
{
    (void)filename;                          // 消除未使用参数警告
    FILE *file = fopen("students.txt", "r"); // 以读取模式打开文件
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行读取！\n", "students.txt");
        return NULL;
    }

    Student *head = NULL; // 链表头指针
    Student *tail = NULL; // 链表尾指针

    // 循环读取文件中的学生信息
    while (1)
    {
        Student temp;
        // 读取基本信息
        int ret = fscanf(file, "%s %s %s ", temp.id, temp.name, temp.gender);
        if (ret != 3)
            break;

        // 读取5门课程成绩
        for (int i = 0; i < 5; i++)
        {
            if (fscanf(file, "%f ", &temp.scores[i]) != 1)
                break;
        }

        // 读取总分、平均分和排名
        if (fscanf(file, "%f %f %d\n", &temp.total, &temp.average, &temp.rank) != 3)
            break;

        // 创建新节点
        Student *newStudent = (Student *)malloc(sizeof(Student));
        *newStudent = temp;
        newStudent->next = NULL;

        // 尾插法：将新节点添加到链表末尾
        if (head == NULL)
        {
            // 如果链表为空，新节点既是头节点也是尾节点
            head = newStudent;
            tail = newStudent;
        }
        else
        {
            // 如果链表不为空，将新节点添加到尾节点后面
            tail->next = newStudent;
            tail = newStudent;
        }
    }
    fclose(file);
    return head;
}

// 计算单个学生的总分和平均分
void calculateTotalAndAverage(Student *student)
{
    student->total = 0;
    // 累加5门课程的成绩
    for (int i = 0; i < 5; i++)
    {
        student->total += student->scores[i];
    }
    // 计算平均分
    student->average = student->total / 5;
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
    // 计算学生人数
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

    free(students);
}

// 根据学号查找学生
Student *findStudentById(Student *head, const char *id)
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

// 根据姓名查找学生
Student *findStudentByName(Student *head, const char *name)
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

// 显示所有学生信息
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

// 按学号排序学生
Student *sortStudentsById(Student *head)
{
    // 检查链表是否为空或只有一个节点
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    // 计算学生总数
    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // 创建学生指针数组，用于排序
    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    // 将链表中的节点指针存入数组
    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    // 使用冒泡排序按学号升序排序
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

    // 更新链表头指针
    head = students[0];

    // 释放数组内存
    free(students);

    return head;
}

// 按总分排序学生
Student *sortStudentsByTotal(Student *head)
{
    // 检查链表是否为空或只有一个节点
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    // 计算学生总数
    int count = 0;
    Student *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // 创建学生指针数组，用于排序
    Student **students = (Student **)malloc(sizeof(Student *) * count);
    if (students == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    // 将链表中的节点指针存入数组
    current = head;
    for (int i = 0; i < count; i++)
    {
        students[i] = current;
        current = current->next;
    }

    // 使用冒泡排序按总分降序排序
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (students[j]->total < students[j + 1]->total)
            {
                // 交换指针
                Student *temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // 更新学生排名
    for (int i = 0; i < count; i++)
    {
        students[i]->rank = i + 1;
    }

    // 重新链接节点
    for (int i = 0; i < count - 1; i++)
    {
        students[i]->next = students[i + 1];
    }
    students[count - 1]->next = NULL;

    // 更新链表头指针
    head = students[0];

    // 释放数组内存
    free(students);

    return head;
}

// 计算各门课程平均分
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

    // 初始化总和和学生计数
    float sums[5] = {0};
    int count = 0;

    // 累加所有学生的各科目成绩
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

    // 计算平均分
    for (int i = 0; i < 5; i++)
    {
        averages[i] = (count > 0) ? sums[i] / count : 0;
    }
}

// 显示成绩在特定值以上的学生
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

// 显示成绩在特定值以下的学生
void displayStudentsWithScoreBelow(Student *head, float score, int course_index)
{
    // 检查链表是否为空
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return;
    }

    // 获取最高分数要求
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

    // 打印表头
    printf("\nStudents with course %d score below %.2f:\n", course_index + 1, maxScore);
    printf("%-10s %-15s %-5s %-8s\n", "ID", "Name", "Gender", "Score");
    printf("------------------------------------------\n");

    // 遍历链表查找符合条件的学生
    Student *current = head;
    int count = 0; // 记录符合条件的学生数量

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

    // 显示统计结果
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

// 显示低于平均分的学生
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

// 显示不及格学生
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

// 保存学生数据到文件
int saveStudentsToFile(Student *head, const char *filename)
{
    (void)filename;                          // 消除未使用参数警告
    FILE *file = fopen("students.txt", "w"); // 以写入模式打开文件
    if (file == NULL)
    {
        printf("Cannot open file %s for writing!\n", "students.txt");
        return 0;
    }

    // 遍历链表，将每个学生的信息写入文件
    Student *current = head;
    while (current != NULL)
    {
        // 写入基本信息
        fprintf(file, "%s %s %s ", current->id, current->name, current->gender);
        // 写入5门课程成绩
        for (int i = 0; i < 5; i++)
        {
            fprintf(file, "%.2f ", current->scores[i]);
        }
        // 写入总分、平均分和排名
        fprintf(file, "%.2f %.2f %d\n", current->total, current->average, current->rank);
        current = current->next;
    }
    fclose(file);
    return 1;
}

// 从文件加载学生数据
Student *loadStudentsFromFile(const char *filename)
{
    (void)filename;                          // 消除未使用参数警告
    FILE *file = fopen("students.txt", "r"); // 以读取模式打开文件
    if (file == NULL)
    {
        printf("Cannot open file %s for reading!\n", "students.txt");
        return NULL;
    }

    Student *head = NULL; // 链表头指针
    Student *tail = NULL; // 链表尾指针

    // 循环读取文件中的学生信息
    while (1)
    {
        Student temp;
        // 读取基本信息
        int ret = fscanf(file, "%s %s %s ", temp.id, temp.name, temp.gender);
        if (ret != 3)
            break;

        // 读取5门课程成绩
        for (int i = 0; i < 5; i++)
        {
            if (fscanf(file, "%f ", &temp.scores[i]) != 1)
                break;
        }

        // 读取总分、平均分和排名
        if (fscanf(file, "%f %f %d\n", &temp.total, &temp.average, &temp.rank) != 3)
            break;

        // 创建新节点
        Student *newStudent = (Student *)malloc(sizeof(Student));
        *newStudent = temp;
        newStudent->next = NULL;

        // 将新节点添加到链表末尾
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

    // 遍历链表，释放每个节点的内存
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// 根据学号删除学生
Student *deleteStudentById(Student *head, const char *id)
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

// 根据姓名删除学生
Student *deleteStudentByName(Student *head, const char *name)
{
    // 检查链表是否为空
    if (head == NULL)
    {
        printf("Student list is empty!\n");
        return NULL;
    }

    // 确认是否删除
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

    // 先处理头节点（可能有多个同名学生）
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

    // 处理其余节点
    Student *current = head;
    int deleted = 0; // 记录删除的学生数量

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

// 根据学号修改学生信息
Student *modifyStudentById(Student *head, const char *id)
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

// 根据姓名修改学生信息
Student *modifyStudentByName(Student *head, const char *name)
{
    // 查找是否有多个同名学生
    int count = 0;
    Student *current = head;

    // 统计同名学生的数量
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            count++;
        }
        current = current->next;
    }

    // 检查是否找到学生
    if (count == 0)
    {
        printf("Student with name %s not found!\n", name);
        return head;
    }

    // 如果有多个同名学生，要求使用学号修改
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
            printf("-1. Cancel modification\n");

            int choice;
            printf("Please enter choice: ");
            scanf("%d", &choice);
            if (choice == -1)
            {
                printf("Operation cancelled.\n");
                return head;
            }

            // 根据选择修改相应信息
            switch (choice)
            {
            case 1: // 修改学号
                printf("Please enter new ID (or -1 to cancel): ");
                scanf("%s", current->id);
                if (strcmp(current->id, "-1") == 0)
                {
                    printf("Operation cancelled.\n");
                    return head;
                }
                printf("ID modified successfully!\n");
                break;

            case 2: // 修改性别
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

            case 3: // 修改所有课程成绩
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
                calculateTotalAndAverage(current); // 重新计算总分和平均分
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

// 二次确认函数
int confirmReturn()
{
    char confirm[10];
    printf("确定要取消并返回主菜单吗？(yes/no): ");
    scanf("%s", confirm);
    return strcmp(confirm, "yes") == 0;
}