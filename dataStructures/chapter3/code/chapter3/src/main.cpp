#include "linearStack.h"
#include "linkStack.h"
#include "digitConv.h"
#include "verifyBrackets.h"
#include "lineEditor.h"

#include "mazePath.h"
#include "hanoi.h"

void linkStack()
{
    struct linkStack stack;
    int popElem = -1;

    initStack(stack);

    push(stack, 100);
    push(stack, 90);
    push(stack, 80);
    push(stack, 70);
    printf("stack size: %d\n", stackLength(stack));
    pop(stack, popElem);
    printf("popELem: %d\n", popElem);

    printf("%d\n", stackEmpty(stack));

    destoryStack(stack);
    printf("%d\n", stackEmpty(stack));
}

void digitConv()
{
    dec2oct();
    dec2hex();
    dec2binary();
    oct2dec();
}

void verifyBraketsTest()
{
    verifyBrackets();
}

void lineEditorTest()
{
    lineEditor();
}

void mazePathTest()
{
    char mazeMap[10][10] =
    {
        {'|', '|', '|', '|', '|', '|', '|', '|', '|', '|'},
        {'|', '#', '#', '|', '#', '#', '#', '|', '#', '|'},
        {'|', '#', '#', '|', '#', '#', '#', '|', '#', '|'},
        {'|', '#', '#', '#', '#', '|', '|', '#', '#', '|'},
        {'|', '#', '|', '|', '|', '#', '#', '#', '#', '|'},
        {'|', '#', '#', '#', '|', '#', '#', '#', '#', '|'},
        {'|', '#', '|', '#', '#', '#', '|', '#', '#', '|'},
        {'|', '#', '|', '|', '|', '#', '|', '|', '#', '|'},
        {'|', '|', '#', '#', '#', '#', '#', '#', '#', '|'},
        {'|', '|', '|', '|', '|', '|', '|', '|', '|', '|'}
    };

    mazeType maze;
    posType startPos;
    posType endPos;
    
    maze.row = 10;
    maze.col = 10;
    maze.pMap = mazeMap;

    startPos.row = 1;
    startPos.col = 1;

    endPos.row = 8;
    endPos.col = 8;

    mazePath(maze, startPos, endPos);

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            printf("%c ", mazeMap[i][j]);
        }

        puts("");
    }

    puts("\nover");
}

void hanoiTest()
{
    hanoi(4, 'a', 'b', 'c');
}

int main(int argc, char *argv[])
{
    //linkStack();
    //digitConv();
    //verifyBraketsTest();
    //lineEditorTest();
    //mazePathTest();
    hanoiTest();

    return 0;
}

