#ifndef LOOPQUEUE_H_
#define LOOPQUEUE_H_

#include "MicroDefine.h"

#define MAXSIZE 100     //�����г���

typedef int qElemType;

typedef struct qNode
{
    qElemType *base;    //��ʼ���Ķ�̬����洢�ռ�
    int front;          //ͷָ�룬�����в��գ�ָ�����ͷԪ��
    int rear;           //βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ��

}sqQueue;

//����һ���ն���q
Status initQueue(sqQueue &q);

//���ٶ���q��q���ٴ���
Status destroyQueue(sqQueue &q);

//��q��Ϊ�ն���
Status clearQueue(sqQueue &q);

//������qΪ�ն��У��򷵻�TRUE�����򷵻�FALSE
Status queueEmpty(const sqQueue &q);

//����q��Ԫ�ظ�������Ϊ���еĳ���
int queueLength(const sqQueue &q);

//���жӲ��գ�����e���ص�ͷԪ�أ�������OK�����򷵻�ERROR
Status getHead(const sqQueue &q, qElemType &e);

//����Ԫ��eΪq���µĶ�βԪ��
Status enQueue(sqQueue &q, qElemType e);

//�����в�Ϊ�գ���ɾ��q�Ķ�ͷԪ�أ���e������ֵ��������OK
Status deQueue(sqQueue &q, qElemType e);

//�Ӷ�ͷ����β���ζԶ���q�е�ÿ��Ԫ�ص��ú���visit()��
//һ��visitʧ�ܣ������ʧ��
Status queueTraverse(const sqQueue &q, Status (*visit)());

#endif


