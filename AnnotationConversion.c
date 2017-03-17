#define _CRT_SECURE_NO_WARNINGS 1

#include "AnnotationConversion.h"

void AnnotationConversion(FILE *pfIn,FILE *pfOut)
{
	enum STATE state = NULL_STATE;
	while (state != END_STATE)
	{
		switch (state)
		{
		case NULL_STATE:
			DO_NULL_STATE(pfIn,pfOut,&state);
			break;
		case C_STATE:
			DO_C_STATE(pfIn,pfOut,&state);
			break;
		case CPP_STATE:
			DO_CPP_STATE(pfIn,pfOut,&state);
			break;
		default:
			break;
		}
	}
}

void DO_NULL_STATE(FILE *pfIn,FILE *pfOut,enum STATE *state)
{
	int first = 0;
	int second = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '/':
		{
			second = fgetc(pfIn);
			switch(second)
			{
			case '/':
				fputc(first,pfOut);						/*���������/����cpp״̬,������/������CPP����*/
				fputc(second,pfOut);
				*state = CPP_STATE;
				break;
			case'*':
				fputc(first,pfOut);
				fputc('/',pfOut);						//�����/*��c״̬ ����C��CPP����
				*state = C_STATE;
				break;
			default:
				fputc(first,pfOut);						//�����һ���ǣ��ڶ������ǣ��ǾͲ���c/cpp״̬�����������
				fputc(second,pfOut);	
				break;
			}
		}	
		break;
	case EOF:											//�����ļ���β���˳�
		*state = END_STATE;
		break;
	default:											//������������ǣ���ɶ���ɶ
		fputc(first,pfOut);
		break;
	}
}

void DO_C_STATE(FILE *pfIn,FILE *pfOut,enum STATE *state)		//����ж���C��ע�ͣ�ҪתΪCPP
{
	int first = 0;
	int second = 0;
	int third = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '*':
		second = fgetc(pfIn);
		switch(second)
		{
		case '/':
			third = fgetc(pfIn);
			switch(third)
			{
			case '\n':
				ungetc(third,pfIn);							//����Ҫ�жϵ�����������������ǻ��У���Ӧ�÷��ص��������������µ���null_state
				*state = NULL_STATE;
				break;
			default:	
				fputc('\n',pfOut);							//����������ַ����ػ��У��ٰ��ַ�����ȥ����NULL_STATE
				ungetc(third,pfIn);
				*state = NULL_STATE;
				break;
			}
			break;
		default:
			fputc(first,pfOut);
			ungetc(second,pfIn);
			break;
		}
		break;
	case EOF:
		break;
	case '\n':												//����ǻ��У���ô���в����//
		fputc('\n',pfOut);
		fputc('/',pfOut);
		fputc('/',pfOut);	
		break;
	default:
		fputc(first,pfOut);
		break;
	}
}

void DO_CPP_STATE(FILE *pfIn,FILE *pfOut,enum STATE *state)			//cpp�ĺ����������ֻ�����Ƿ�Ϊ���л����ļ�������
{	
	int first = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '\n':														//����ȡ�����У���һ����һ����ע�ͽ���������NULL_STATE��
		fputc(first,pfOut);
		*state = NULL_STATE;
		break;
	case EOF:
		*state = END_STATE;											//��������������ֱ�ӽ�����
		break;
	default:
		fputc(first, pfOut);										//�������˻������ⶫ������ɶ���ɶ
		break;
	}
}