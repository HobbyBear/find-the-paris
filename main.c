#include <stdio.h>
#include <windows.h>
#include <time.h>

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

boolean findUsername(FILE *fp, struct user u) {
    struct user dbUser;
    while (!feof(fp)) {
        fread(&dbUser, sizeof(struct user), 1, fp);
        if (strcmp(dbUser.name, u.name) != 0) {
            return 1;
        }
    }
    return 0;
}


boolean findUser(FILE *fp, struct user u) {
    struct user dbUser;
    while (!feof(fp)) {
        fread(&dbUser, sizeof(struct user), 1, fp);
        if (strcmp(dbUser.name, u.name) != 0 && strcmp(dbUser.pwd, u.pwd) != 0) {
            return 1;
        }
    }
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
    _flushall();
}

void initInput(char input[100]) {
    int i = 0;
    for (i = 0; i < 100; i++) {
        input[i] = '*';
    }
}

int printMaskGrids(struct point p, char grids[4][4],
                   struct point *pList[], char input[], int count) {
    char maskGrids[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            maskGrids[i][j] = '*';
        }
    }

    // 将点击过的都显示出来，然后判断是否正确
    for (i = 0; i < 16; i++) {
        if (pList[i] != NULL) {
            maskGrids[pList[i]->x][pList[i]->y] =
                    grids[pList[i]->x][pList[i]->y];
        }
    }
    maskGrids[p.x][p.y] = grids[p.x][p.y];

    printGrids(maskGrids);

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

void initPointList(struct point *pList[16]) {
    int i = 0;
    for (i = 0; i < 16; i++) {
        pList[i] = NULL;
    }
}

void printMaskGridsOnFail(char grids[4][4], struct point *pList[16]) {
    char maskGrids[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            maskGrids[i][j] = '*';
        }
    }

    // 将点击过的都显示出来，然后判断是否正确
    for (i = 0; i < 16; i++) {
        if (pList[i] != NULL) {
            maskGrids[pList[i]->x][pList[i]->y] =
                    grids[pList[i]->x][pList[i]->y];
        }
    }
}

int main() {

    FILE *fp;

    if ((fp = fopen("user.txt", "rb")) == NULL && (fp = fopen("users.txt", "wb+")) == NULL) {
        printf("无法建立文件！\n");
        exit(0);
    }

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
            if (!findUser(fp, u)) {
                printf("your password or username is wrong!!!\n\n");
                goto start;
            }
            printf("ok !you success login in \n");
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
            if (findUsername(fp, u)) {
                printf("there is already same name existed!!!\n\n");
                goto start;
            }
            fwrite(&u, sizeof(struct user), 1, fp);
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
        case 1:
            break;
        case 2:
            printf("there will show 8 X 8 grids ,please remember more about it!\n");
            char grids[4][4];
            generateGrids(grids);
            printGrids(grids);
//            Sleep(3000);
//            clearScreen();
            printf("please input the point you want to see.\n");
            char input[100];
            initInput(input);
            struct point *pList[16];
            initPointList(pList);
            int num = 0; // 翻开的元素数量
            int totalCount = 0; // 翻牌的次数
            int errorCount = 0; // 错误的次数
            struct point p;
            while (num <= 16 && errorCount <= 3) {
                printf("please input your x\n");
                _flushall();
                int x = 0;
                scanf("%d", &x);
                if (x < 0 || x >= 4) {
                    printf("the num ranges 0 from 3 ,please input again!\n");
                    continue;
                }
                printf("please input your y\n");
                _flushall();
                int y = 0;
                scanf("%d", &y);
                if (y < 0 || y >= 4) {
                    printf("the num ranges 0 from 3 ,please input again!\n");
                    continue;
                }
                p.y = y;
                p.x = x;
                num++;
                totalCount++;
                boolean succ = printMaskGrids(p, grids, pList, input, num);
                if (succ) {
                    pList[num] = &p;
                    input[grids[p.x][p.y]] = grids[p.x][p.y];
                    // todo 记录成绩
                    continue;
                } else {
                    num--;
                    errorCount++;
                    printMaskGridsOnFail(grids, pList);
                    if (errorCount >= 3) {
                        printf("the fail count is more than 3!\n");
                        // todo 记录成绩
                        continue;
                    }
                    continue;
                }
            }
            if (errorCount >= 3) {
                goto menuBar;
            }
        case 3:
            printf("closing......\n");
            Sleep(3);
            exit(0);
        default:
            printf("please input one or two!\n");
            goto menuBar;
    }


    return 0;
}

