#include "mazePath.h"

Status pass(const mazeType &maze, const posType &curPos);
void footPrint(mazeType &maze, const posType &curPos, int curStep);
posType nextPos(const posType &curPos, int di);
void markPrint(mazeType &maze, const posType &curPos);

Status mazePath(mazeType &maze, posType startPos, posType endPos)
{
    mazeLinkStack mazeStack;
    posType curPos = startPos;  //设定“当前位置”为“入口位置”
    int curStep = 1;            //探索第一步

    initStack(mazeStack);
    
    do
    {
        if(pass(maze, curPos)) //当前位置可以通过，即是未曾走到过的通道块
        {
            printf("pass curStep: %d\n", curStep);
            footPrint(maze, curPos, curStep);  //留下足迹
           
            elemType elem;
            elem.ord = curStep;
            elem.seat = curPos;
            elem.di = 1;

            push(mazeStack, elem);  //加入路径

            if(curPos == endPos)    //到达终点(出口)
            {
                puts("we arrive maze end");
                return TRUE;
            }   //if
            
            //下一个位置是当前位置的东邻
            curPos = nextPos(curPos, 1);
            curStep++;
        }   //if
        else    //当前位置不能通过
        {
            if(!stackEmpty(mazeStack))
            {
                elemType popElem;

                pop(mazeStack, popElem);

                while(4 == popElem.di && !stackEmpty(mazeStack))
                {
                    markPrint(maze, popElem.seat); //留下不能通过的标记，并退回一步
                    pop(mazeStack, popElem);
                }   //while

                if(popElem.di < 4)
                {
                    popElem.di++;       //换一个方向探索
                    push(mazeStack, popElem);
                    
                    //设定当前位置是该新方向上的相邻块
                    curPos = nextPos(popElem.seat, popElem.di); 
                }   //if
            }   //if
        }   //else

    }while(!stackEmpty(mazeStack));

    return FALSE;

}

Status pass(const mazeType &maze, const posType &curPos)
{
    if('#' == maze.pMap[curPos.row][curPos.col])
    {
        return TRUE;
    }

    return FALSE;
}

//' '为走过并且可以通过的标记
void footPrint(mazeType &maze, const posType &curPos, int curStep)
{
    //printf("pos : %d, %d; %d -- %c\n", 
    //        curPos.row, curPos.col, curStep, curStep + '0');
    maze.pMap[curPos.row][curPos.col] = curStep + '0';
}

posType nextPos(const posType &curPos, int di)
{
    posType nextPos = curPos;

    switch(di)
    {
        case 1:
            nextPos.col = curPos.col - 1;
            break;
        case 2:
            nextPos.row = curPos.row - 1;
            break;
        case 3:
            nextPos.col = curPos.col + 1;
            break;
        case 4:
            nextPos.row = curPos.row + 1;
            break;
        default:
            break;
    }

    return nextPos;
}

//'*'为走过并且不能通过的标记
void markPrint(mazeType &maze, const posType &curPos)
{
    maze.pMap[curPos.row][curPos.col] = '*';
}
