#include <stdio.h>
#define MAX_STACK_SIZE 100
#define INVALID_KEY -1
#define EXIT_ROW 11
#define EXIT_COL 15

int maze[13][17] = {
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

typedef struct {
    short int row;
    short int col;
    short int dir;
} element;

int mark[13][17] = { 0 };  //크기의 통일성을 위해 미로 둘러싼 테두리도 그냥 있는셈 침

element stack[MAX_STACK_SIZE];
int top = -1;

void stackFull() {
    fprintf(stderr, "no more space in the queue\n");
    exit(1);
}
element stackEmpty() { //스택이 비어있는데 pop이 호출되었을 경우를 처리하기 위한 함수. 그래서 반환형식이 element인것
    element invalidElement = { INVALID_KEY, INVALID_KEY, INVALID_KEY };
    return invalidElement;
}
void push(element item) {
    if (top >= MAX_STACK_SIZE - 1) {
        stackFull(); 
    }
    else {
        stack[++top] = item;
    }
}
element pop() {
    if (top == -1)
        return stackEmpty(); 
    return stack[top--];
}

typedef struct {
    short int vert;
    short int horiz;
} offsets;
offsets move[8] = { {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1} };
// E, SE, S, SW, W, NW, N, NE(동쪽부터 시계방향으로)
//동쪽, 남쪽 방향이 출구방향이므로 먼저 고려하는 것이 조금 더 유리할지도

void path(void) {

    int i, row, col, nextRow, nextCol, dir, found = 0; //초기화
    element position = { 1,1,0 }; 

    mark[1][1] = 1;//벽 고려하면 미로는 1,1부터 시작

    push(position);

    while (top > -1 && !found) {
        position = pop();
        row = position.row; col = position.col; dir = position.dir;
        while (dir < 8 && !found) {

            /*디버그용 코드
            printf("%d\n", top);
            printf("%2d%5d%5d\n", row, col, dir);
            */

            nextRow = row + move[dir].vert;
            nextCol = col + move[dir].horiz;
            if (nextRow == EXIT_ROW && nextCol == EXIT_COL)
                found = 1;
            else if (!maze[nextRow][nextCol]/*벽(1)이 아니고 통로(0)인지*/ && !mark[nextRow][nextCol]/*not visited인지*/) {
                mark[nextRow][nextCol] = 1; //visited로 바꿔줌
                position.row = row; position.col = col;
                position.dir = ++dir; //나중에 돌아올거 대비해서 어느방향까지 가봤는지를 표시하기 위함
                push(position); //현재좌표 스택에 넣고
                row = nextRow; col = nextCol; dir = 0; //다음좌표료 이동
            }
            else ++dir; //지금방향으로 이동불가능한 경우, 다음 방향 검토
        }
    }
    if (found) {
        printf("The path is: \n");
        printf("row col\n");
        printf("%d\n", top);
        for (i = 0; i <= top; i++) {
            printf("%2d%5d\n", stack[i].row, stack[i].col);
        }
        printf("%2d%5d\n", row, col);
        printf("%2d%5d\n", EXIT_ROW, EXIT_COL);
    }
    else printf("The maze does not have a path.\n");
}
int main()
{
    path();
}

/*
* 미로를 1(벽)이 둘러싼다고 가정
* 11x15 미로이므로 벽까지 생각하면 13x17
* 스택은 어차피 하나만 사용하므로 전역변수로 선언함(스택의 top도.)
* 후진(왔던길 다시)을 제외하고는 모든방향 이동이 불가능한 경우 내부 while문 탈출해(조건: dir<8 불충족) 스택에서 새로운 원소 꺼냄(pop)
* 방향도 스택에 같이 저장하는 이유 : 다시 돌아오게 될 경우 이미 점검한 방향은 제외하고 그 다음부터 점검해야함 (시간 복잡도 감소시키지 않을까?)
* => ++dir로 저장 
*/