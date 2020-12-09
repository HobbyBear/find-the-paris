#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "string.h"

struct user {
    char name[10];
    char pwd[10];
};


void clearScreen() {
    int i = 0;
    for (i = 0; i < 1000; i++) {
        printf("\n");
    };
}


boolean findUsername(struct user u) {
    FILE *fp = fopen("users.txt", "rb");
    struct user dbUser;
    while (!feof(fp)) {
        fread(&dbUser, sizeof(struct user), 1, fp);
        if (strcmp(dbUser.name, u.name) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


boolean findUser(struct user u) {
    FILE *fp = fopen("users.txt", "r");
    struct user dbUser;
    while (!feof(fp)) {
        fread(&dbUser, sizeof(struct user), 1, fp);
        if (strcmp(dbUser.name, u.name) == 0 && strcmp(dbUser.pwd, u.pwd) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


void swap(char *tempa, char *tempb) {
    int temp;
    temp = *tempa;
    *tempa = *tempb;
    *tempb = temp;
}

void generateGrids(char grids[4][4]) {
    char a[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8'};
    int i;
    int j = 0;
    srand((unsigned int) time(NULL));
    for (i = 0; i < 16; i++) {
        j = rand() % 16;
        swap(&a[i], &a[j]);
    }
    int x = 0, y = 0;
    for (i = 0; i < 16; i++) {
        grids[x][y] = a[i];
        x++;
        if (x == 4) {
            x = 0;
            y++;
        }
    }
}

struct point {
    int x;
    int y;
};

void printGrids(char grids[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%c  ", grids[i][j]);
        }
        printf("\n");
    }
}

void initInput(char input[100]) {
    int i = 0;
    for (i = 0; i < 100; i++) {
        input[i] = '*';
    }
}

int printMaskGrids(struct point p, char grids[4][4],
                   char history_grids[4][4], char input[], int count) {
    char maskGrids[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            maskGrids[i][j] = '*';
        }
    }

    // 将点击过的都显示出来，然后判断是否正确
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            maskGrids[i][j] = history_grids[i][j];
        }
    }

    maskGrids[p.x][p.y] = grids[p.x][p.y];

    printGrids(maskGrids);
    _flushall();
    if (count % 2 != 0) {
        return 1;
    }

    // 正确，那么就继续翻下一个
    if (input[grids[p.x][p.y]] == '*') {
        printf("sorry your answer is wrong!\n");
        _flushall();
        return 0;
    }
    return 1;
}

void initHistoryGrid(char pList[4][4]) {
    int i = 0, j = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            pList[i][j] = '*';
        }
    }
}

struct history_grade {
    char grids[4][4];
    char history_grids[4][4];
    int num; // 翻开的元素数量
    int totalCount; // 翻牌的次数
    int errorCount; // 错误的次数
    char name[10];
    int is_del;
};

void write_db(struct history_grade grade) {
    FILE *fp = fopen("grade.txt", "a+");
    fprintf(fp, "一共答对%d个元素,错误%d次，翻排%d次,是否删除:%d\n", grade.num, grade.errorCount, grade.totalCount, grade.is_del);
    int i = 0, j = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            fprintf(fp, "%c ", grade.grids[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "答题记录属于用户%s\n", grade.name);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            fprintf(fp, "%c ", grade.history_grids[i][j]);
        }
        fprintf(fp, "\n");
    }
    fflush(fp);
    fclose(fp);
}

int read_db(struct history_grade grade_list[100]) {
    FILE *fp = fopen("grade.txt", "r");
    rewind(fp);
    int size = 0;
    while (1) {
        struct history_grade grade;
        if (fscanf(fp, "一共答对%d个元素,错误%d次，翻排%d次,是否删除:%d\n", &grade.num, &grade.errorCount, &grade.totalCount,
                   &grade.is_del) == EOF) {
            break;
        }
        int i = 0, j = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                fscanf(fp, "%c ", &grade.grids[i][j]);
            }
            fscanf(fp, "\n");
        }
        fscanf(fp, "答题记录属于用户%s\n", grade.name);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                fscanf(fp, "%c ", &grade.history_grids[i][j]);
            }
            fscanf(fp, "\n");
        }
        grade_list[size] = grade;
        size++;
    }
    fclose(fp);
    return size;
}


struct history_grade
genGrade(int num, int totalNum, int errCount, char grids[4][4], char history_grids[4][4], char name[10]) {
    struct history_grade grade;
    grade.num = num;
    grade.totalCount = totalNum;
    grade.errorCount = errCount;
    grade.is_del = 0;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            grade.grids[i][j] = grids[i][j];
            grade.history_grids[i][j] = history_grids[i][j];
        }
    }
    for (i = 0; i < 10; i++) {
        grade.name[i] = name[i];
    }
    return grade;
}


void printHistoryGrade(char name[]) {
    FILE *fp = fopen("grade.txt", "r");
    rewind(fp);
    int num = -1;
    while (1) {
        struct history_grade grade;
        if (fscanf(fp, "一共答对%d个元素,错误%d次，翻排%d次,是否删除:%d\n", &grade.num, &grade.errorCount, &grade.totalCount,
                   &grade.is_del) == EOF) {
            break;
        }
        num++;
        int i = 0, j = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                fscanf(fp, "%c ", &grade.grids[i][j]);
            }
            fscanf(fp, "\n");
        }
        fscanf(fp, "答题记录属于用户%s\n", grade.name);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                fscanf(fp, "%c ", &grade.history_grids[i][j]);
            }
            fscanf(fp, "\n");
        }
        if (grade.is_del == 1) {
            continue;
        }
        if (strcmp(name,grade.name) !=0){
            continue;
        }
        printf("record id is %d\n", num);
        printf("open num count: %d,total open num: %d,err open count: %d\n", grade.num,
               grade.totalCount, grade.errorCount);
        printf("standard answer is\n-----------------------------\n");
        printGrids(grade.grids);
        printf("your answer is\n-----------------------------\n");
        printGrids(grade.history_grids);
    }
    fclose(fp);
}

int delHistoryGrade(struct history_grade history_grade_list[100], int size, int delNum, char name[10]) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if (i == delNum) {
            history_grade_list[i].is_del = 1;
            remove("grade.txt");
            for (i = 0; i < size; i++) {
                write_db(history_grade_list[i]);
            }
            return read_db(history_grade_list);
        }
    }
    return size;
}

int main() {
    FILE *fp;

    if ((fp = fopen("users.txt", "rb")) == NULL && (fp = fopen("users.txt", "ab+")) == NULL) {
        printf("无法建立文件！\n");
        exit(0);
    }
    fclose(fp);

    struct history_grade history_grade_list[100];
    int history_grade_list_size = 0;
    struct user u;
    start:
    printf("welcome to find the paris.\n");
    printf("1. press 1 to login.\n");
    printf("2.press 2 to register.\n");
    _flushall();
    int logOrReg = 0;
    scanf("%d", &logOrReg);
    switch (logOrReg) {
        case 1:
            printf("please input your name!\n");
            _flushall();
            scanf("%s", u.name);
            printf("please input your password.\n");
            _flushall();
            scanf("%s", u.pwd);
            if (!findUser(u)) {
                printf("your password or username is wrong!!!\n\n");
                goto start;
            }
            printf("ok !you success login in \n");
            history_grade_list_size = read_db(history_grade_list);
            printf("历史记录有%d条\n", history_grade_list_size);
            _flushall();
            Sleep(2);
            break;
        case 2:
            printf("please input your name!\n");
            _flushall();
            scanf("%s", u.name);
            printf("please input your password.\n");
            _flushall();
            scanf("%s", u.pwd);
            if (findUsername(u)) {
                printf("there is already same name existed!!!\n\n");
                goto start;
            }
            fp = fopen("users.txt", "ab+");
            fwrite(&u, sizeof(struct user), 1, fp);
            fclose(fp);
            printf("ok !you success register \n");
            _flushall();
            Sleep(1);
            goto start;
        default:
            printf("please input one or tow !\n");
            goto start;
    }


    menuBar:
    printf("please select your interests\n");
    printf("1,play game\n");
    printf("2,view game history\n");
    printf("3, exit\n");
    _flushall();

    int menu = 0;
    scanf("%d", &menu);
    switch (menu) {
        case 2:
        history:
            printHistoryGrade(u.name);
            printf("you can input num to del record or exit\n");
            printf("1,del record.\n2,back to menu\n");
            _flushall();
            int command = 0;
            scanf("%d", &command);
            switch (command) {
                case 2:
                    goto menuBar;
                case 1:
                    printf("please input the record id to delete\n");
                    _flushall();
                    int delNum = 0;
                    scanf("%d", &delNum);
                    history_grade_list_size = delHistoryGrade(history_grade_list, history_grade_list_size, delNum,
                                                              u.name);
                    printf("delete success\n");
                    goto history;
                default:
                    printf("please input one or two!\n");
                    _flushall();
                    goto menuBar;
            }
        case 1:
            printf("there will show 8 X 8 grids ,please remember more about it!\n");
            char grids[4][4];
            generateGrids(grids);
            printGrids(grids);
//            Sleep(3000);
//            clearScreen();
            printf("please input the point you want to see.\n");
            char input[100];
            initInput(input);
            char history_grids[4][4];
            initHistoryGrid(history_grids);
            int num = 0; // 翻开的元素数量
            int totalCount = 0; // 翻牌的次数
            int errorCount = 0; // 错误的次数
        input_x:
            printf("please input your x\n");
            _flushall();
            int x = 0;
            scanf("%d", &x);
            if (x < 0 || x >= 4) {
                printf("the num ranges 0 from 3 ,please input again!\n");
                goto input_x;
            }
        input_y:
            printf("please input your y\n");
            _flushall();
            int y = 0;
            scanf("%d", &y);
            if (y < 0 || y >= 4) {
                printf("the num ranges 0 from 3 ,please input again!\n");
                goto input_y;;
            }
            struct point p;
            p.y = y;
            p.x = x;
            num++;
            totalCount++;
            if (history_grids[x][y] != '*') {
                printf("the point has been input!\n");
                _flushall();
                goto input_x;
            }
            boolean succ = printMaskGrids(p, grids, history_grids, input, num);
            if (succ) {
                history_grids[x][y] = grids[x][y];
                input[grids[p.x][p.y]] = grids[p.x][p.y];
                if (num == 16) {
                    write_db(genGrade(num, totalCount, errorCount, grids, history_grids, u.name));
                    history_grade_list_size++;
                }
                goto input_x;;
            } else {
                num--;
                errorCount++;
                printGrids(history_grids);
                if (errorCount >= 3) {
                    printf("the fail count is more than 3!\n");
                    write_db(genGrade(num, totalCount, errorCount, grids, history_grids, u.name));
                    history_grade_list_size++;
                    goto menuBar;
                }
                goto input_x;
            }

        case 3:
            printf("closing......\n");
            Sleep(3);
            exit(0);
        default:
            printf("please input one or two!\n");
            goto menuBar;
    }

}

