#include "mazePath.h"

Status pass(const mazeType &maze, const posType &curPos);
void footPrint(mazeType &maze, const posType &curPos, int curStep);
posType nextPos(const posType &curPos, int di);
void markPrint(mazeType &maze, const posType &curPos);

Status mazePath(mazeType &maze, posType startPos, posType endPos)
{
    mazeLinkStack mazeStack;
    posType curPos = startPos;  //�趨����ǰλ�á�Ϊ�����λ�á�
    int curStep = 1;            //̽����һ��

    initStack(mazeStack);
    
    do
    {
        if(pass(maze, curPos)) //��ǰλ�ÿ���ͨ��������δ���ߵ�����ͨ����
        {
            printf("pass curStep: %d\n", curStep);
            footPrint(maze, curPos, curStep);  //�����㼣
           
            elemType elem;
            elem.ord = curStep;
            elem.seat = curPos;
            elem.di = 1;

            push(mazeStack, elem);  //����·��

            if(curPos == endPos)    //�����յ�(����)
            {
                puts("we arrive maze end");
                return TRUE;
            }   //if
            
            //��һ��λ���ǵ�ǰλ�õĶ���
            curPos = nextPos(curPos, 1);
            curStep++;
        }   //if
        else    //��ǰλ�ò���ͨ��
        {
            if(!stackEmpty(mazeStack))
            {
                elemType popElem;

                pop(mazeStack, popElem);

                while(4 == popElem.di && !stackEmpty(mazeStack))
                {
                    markPrint(maze, popElem.seat); //���²���ͨ���ı�ǣ����˻�һ��
                    pop(mazeStack, popElem);
                }   //while

                if(popElem.di < 4)
                {
                    popElem.di++;       //��һ������̽��
                    push(mazeStack, popElem);
                    
                    //�趨��ǰλ���Ǹ��·����ϵ����ڿ�
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

//' 'Ϊ�߹����ҿ���ͨ���ı��
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

//'*'Ϊ�߹����Ҳ���ͨ���ı��
void markPrint(mazeType &maze, const posType &curPos)
{
    maze.pMap[curPos.row][curPos.col] = '*';
}
