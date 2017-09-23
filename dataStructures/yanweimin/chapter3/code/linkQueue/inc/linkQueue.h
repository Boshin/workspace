#ifndef LINKQUEUE_H_
#define LINKQUEUE_H_

#include "MicroDefine.h"

typedef int qElemType;

typedef struct qNode
{
    qElemType data;
    struct qNode *next;
}qNode, *queuePtr;

typedef struct
{
    queuePtr front;    //��ͷָ��
    queuePtr rear;     //��βָ��
}linkQueue;

//����һ���ն���q
Status initQueue(linkQueue &q);

//���ٶ���q��q���ٴ���
Status destroyQueue(linkQueue &q);

//��q��Ϊ�ն���
Status clearQueue(linkQueue &q);

//������qΪ�ն��У��򷵻�TRUE�����򷵻�FALSE
Status queueEmpty(const linkQueue &q);

//����q��Ԫ�ظ�������Ϊ���еĳ���
int queueLength(const linkQueue &q);

//���жӲ��գ�����e���ص�ͷԪ�أ�������OK�����򷵻�ERROR
Status getHead(const linkQueue &q, qElemType &e);

//����Ԫ��eΪq���µĶ�βԪ��
Status enQueue(linkQueue &q, qElemType e);

//�����в�Ϊ�գ���ɾ��q�Ķ�ͷԪ�أ���e������ֵ��������OK
Status deQueue(linkQueue &q, qElemType e);

//�Ӷ�ͷ����β���ζԶ���q�е�ÿ��Ԫ�ص��ú���visit()��
//һ��visitʧ�ܣ������ʧ��
Status queueTraverse(const linkQueue &q, Status (*visit)());

#endif
