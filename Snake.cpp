#include<iostream>
#include<cstdlib>
#include<windows.h>
#include <conio.h>

using namespace std;

#define SIDE	20 //�߳�
#define MARGIN	1 //���
//ǽ
#define WALL_HEI	(SIDE+MARGIN*2)
#define WALL_WID	(SIDE+MARGIN*2)
//ǽ���ڲ�
#define WALL_IN_HEI		(SIDE-MARGIN*2)
#define WALL_IN_WID	(SIDE-MARGIN*2)
#define OFFSET	8 //����ƫ��
//��ʼ�ߵ�λ��
#define START_POS_X	(SIDE/2)
#define START_POS_Y	(SIDE/2)

short wall[WALL_HEI][WALL_WID];
COORD target;
COORD* snake;
int snakeLength;
char direct;
int speed;
int score;

void SetXY(SHORT x,SHORT y) //���ù��λ��
{
	COORD pos = {2*x, y}; //��滭����ַ�ռ2���ֽڵ�λ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void SetColor(WORD a) //������ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void ResetColor() //������ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF | 0x0); //�ڵװ���
}

void CurVisible(bool flag) //���ù���Ƿ�ɼ�
{
	CONSOLE_CURSOR_INFO cci = {sizeof(cci), flag};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void Scene() //��������
{
	int i,j;
	//��ʾ����
	for(i=MARGIN; i<WALL_HEI-MARGIN;i++)
	{
		for(j=MARGIN; j<WALL_WID-MARGIN;j++)
		{
			wall[i][j] = 1;
		}
	}
	for(i=0; i<WALL_HEI;i++)
	{
		for(j=0; j<WALL_WID;j++)
		{
			SetXY(OFFSET+i,j);
			if(1==wall[i][j])
			{
				SetColor(FOREGROUND_INTENSITY | 0x0);
				cout<<"��";
			}else
			{
				SetColor(0xf | BACKGROUND_INTENSITY);
				cout<<"��";
			}
		}
	}
	//��ʾ�ı�
	SetColor(0xf | 0x0);
	SetXY(OFFSET,j);
	cout<<"�ƶ���WSAD"; //��ʾ�ı�
	SetXY(OFFSET+10,j);
	cout<<"������0"; //��ʼ����Ϊ0
}

void ResumeBG(short x,short y) //ָ��λ�ûָ�Ϊ��������
{
	SetXY(x, y);
	SetColor(FOREGROUND_INTENSITY | 0x0);
	cout<<"��";
}

void ShowNewTarget() //��ʾһ���µ�Ŀ��λ��
{
	target.X = rand() % (WALL_IN_WID) + OFFSET + MARGIN;
	target.Y = rand() % (WALL_IN_HEI) + MARGIN;
	SetXY(target.X, target.Y);
	SetColor(FOREGROUND_RED + FOREGROUND_GREEN);
	cout<<"��";
}

void InitSnake(int len) //��ʼ���ߣ�����������
{
	snakeLength = len;
	snake = (COORD*)malloc(sizeof(COORD) * snakeLength);
	for(int i=0;i<len;i++) //��ʼʱ���߼�����һ��
	{
		snake[i].X = START_POS_X;
		snake[i].Y = START_POS_Y;
	}
}

void UpdateSnake() //�����߳���һ�������нڵ�λ��
{
	int i;
	if(snake[snakeLength-1].X==snake[snakeLength-2].X && 
		snake[snakeLength-1].Y==snake[snakeLength-2].Y)
	{
		i = snakeLength-2 ; //����ߵĳ������ӣ����һ�ڴ�ʱ���ֲ���
	}else
	{
		//�������һ��
		ResumeBG(snake[snakeLength-1].X,snake[snakeLength-1].Y);
		i = snakeLength-1;
	}
	//���³���һ�����λ��
	while(i>0) 
	{
		snake[i].X = snake[i-1].X;
		snake[i].Y = snake[i-1].Y;
		i--;
	}
}

void SnakeDirection(char c) //�ı��趨���򣬼��ı���ͷ��λ��
{
	if('w'==c || 's'==c || 'a'==c || 'd'==c)
	{
		UpdateSnake();
	}
	//���µ�һ�ڵ�λ��
	switch(c) //������������¼����߽����ֲ���
	{
	case 'w':snake[0].Y--;break;
	case 's':snake[0].Y++;break;
	case 'a':snake[0].X--;break;
	case 'd':snake[0].X++;break;
	}
}

void ShowSnake() //��ʾ��
{
	for(int i=0;i<snakeLength;i++)
	{
		SetXY(snake[i].X, snake[i].Y);
		SetColor(FOREGROUND_BLUE | BACKGROUND_GREEN);
		cout<<"��";
	}
}

void ChangeSpeed() //�ı��ٶ�
{
	speed -= speed / snakeLength;
}

void ShowNewScore() //ˢ�µ÷�
{
	score += snakeLength*2-1;
	SetXY(OFFSET+13,WALL_HEI);
	SetColor(0xf | 0x0);
	cout<<score;
}

void FindTarget() //Ѱ��Ŀ��
{
	if(snake[0].X==target.X && snake[0].Y==target.Y) //�ҵ�Ŀ��
		{
			snakeLength++;
			snake=(COORD*)realloc(snake,sizeof(COORD)*snakeLength); //���������ڴ�
			snake[snakeLength-1] = snake[snakeLength-2]; 

			ChangeSpeed();
			ShowNewScore();
			ShowNewTarget();
		}
}

bool AgainstWall() //�ж��Ƿ�ײǽ
{
	return snake[0].X == OFFSET+MARGIN-1 || snake[0].X == OFFSET+WALL_WID-1 ||
		snake[0].Y == MARGIN-1 || snake[0].Y == WALL_HEI-1;
}

void GameOver() //������Ϣ�����
{
	//��������
	SetXY(snake[0].X,snake[0].Y);
	SetColor(FOREGROUND_BLUE | BACKGROUND_RED);
	cout<<"  ";

	SetColor(0xf | 0x0);
	//�ı�˵��
	SetXY(OFFSET+WALL_WID/3, WALL_HEI/2);
	cout<<"������ - ��Ϸ����";
	free(snake);
	//�˳�
	SetXY(OFFSET, WALL_HEI+1);
	system("pause");
	exit(0);
}

int main()
{
	CurVisible(false); //���ع��
	Scene(); //����
	InitSnake(2); //��ʼ���ߣ�����>=2
	ShowSnake(); //������
	ShowNewTarget(); //Ŀ��
	direct = 'd'; //��ʼ����
	speed = 500; //��ʼ�ٶ�
	score = 0; //��ʼ����
	while(true)
	{
		if( _kbhit()) //���Ӱ���
			direct = _getch();
		SnakeDirection(direct); //ͨ�������ı��ߵķ���
		ShowSnake(); //���»�����
		FindTarget(); //Ѱ��Ŀ��
		if(AgainstWall()) //�ж�ײǽ
			GameOver(); //��ײǽ����Ϸ����
		Sleep(speed); //��Ϸ�ٶ�
	}
	return 0;
}