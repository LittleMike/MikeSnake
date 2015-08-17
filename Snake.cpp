#include<iostream>
#include<cstdlib>
#include<windows.h>
#include <conio.h>

using namespace std;

#define SIDE	20 //边长
#define MARGIN	1 //外框
//墙
#define WALL_HEI	(SIDE+MARGIN*2)
#define WALL_WID	(SIDE+MARGIN*2)
//墙的内部
#define WALL_IN_HEI		(SIDE-MARGIN*2)
#define WALL_IN_WID	(SIDE-MARGIN*2)
#define OFFSET	8 //场景偏移
//初始蛇的位置
#define START_POS_X	(SIDE/2)
#define START_POS_Y	(SIDE/2)

short wall[WALL_HEI][WALL_WID];
COORD target;
COORD* snake;
int snakeLength;
char direct;
int speed;
int score;

void SetXY(SHORT x,SHORT y) //设置光标位置
{
	COORD pos = {2*x, y}; //描绘画面的字符占2个字节的位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void SetColor(WORD a) //设置颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void ResetColor() //重置颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF | 0x0); //黑底白字
}

void CurVisible(bool flag) //设置光标是否可见
{
	CONSOLE_CURSOR_INFO cci = {sizeof(cci), flag};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void Scene() //建立场景
{
	int i,j;
	//显示背景
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
				cout<<"⊙";
			}else
			{
				SetColor(0xf | BACKGROUND_INTENSITY);
				cout<<"□";
			}
		}
	}
	//显示文本
	SetColor(0xf | 0x0);
	SetXY(OFFSET,j);
	cout<<"移动：WSAD"; //提示文本
	SetXY(OFFSET+10,j);
	cout<<"分数：0"; //初始分数为0
}

void ResumeBG(short x,short y) //指定位置恢复为背景内容
{
	SetXY(x, y);
	SetColor(FOREGROUND_INTENSITY | 0x0);
	cout<<"⊙";
}

void ShowNewTarget() //显示一个新的目标位置
{
	target.X = rand() % (WALL_IN_WID) + OFFSET + MARGIN;
	target.Y = rand() % (WALL_IN_HEI) + MARGIN;
	SetXY(target.X, target.Y);
	SetColor(FOREGROUND_RED + FOREGROUND_GREEN);
	cout<<"●";
}

void InitSnake(int len) //初始化蛇，参数：长度
{
	snakeLength = len;
	snake = (COORD*)malloc(sizeof(COORD) * snakeLength);
	for(int i=0;i<len;i++) //初始时，蛇集中于一点
	{
		snake[i].X = START_POS_X;
		snake[i].Y = START_POS_Y;
	}
}

void UpdateSnake() //更新蛇除第一节外所有节的位置
{
	int i;
	if(snake[snakeLength-1].X==snake[snakeLength-2].X && 
		snake[snakeLength-1].Y==snake[snakeLength-2].Y)
	{
		i = snakeLength-2 ; //如果蛇的长度增加，最后一节此时保持不动
	}else
	{
		//擦除最后一节
		ResumeBG(snake[snakeLength-1].X,snake[snakeLength-1].Y);
		i = snakeLength-1;
	}
	//更新除第一节外的位置
	while(i>0) 
	{
		snake[i].X = snake[i-1].X;
		snake[i].Y = snake[i-1].Y;
		i--;
	}
}

void SnakeDirection(char c) //改变设定方向，即改变蛇头的位置
{
	if('w'==c || 's'==c || 'a'==c || 'd'==c)
	{
		UpdateSnake();
	}
	//更新第一节的位置
	switch(c) //如果按键非以下键，蛇将保持不动
	{
	case 'w':snake[0].Y--;break;
	case 's':snake[0].Y++;break;
	case 'a':snake[0].X--;break;
	case 'd':snake[0].X++;break;
	}
}

void ShowSnake() //显示蛇
{
	for(int i=0;i<snakeLength;i++)
	{
		SetXY(snake[i].X, snake[i].Y);
		SetColor(FOREGROUND_BLUE | BACKGROUND_GREEN);
		cout<<"¤";
	}
}

void ChangeSpeed() //改变速度
{
	speed -= speed / snakeLength;
}

void ShowNewScore() //刷新得分
{
	score += snakeLength*2-1;
	SetXY(OFFSET+13,WALL_HEI);
	SetColor(0xf | 0x0);
	cout<<score;
}

void FindTarget() //寻找目标
{
	if(snake[0].X==target.X && snake[0].Y==target.Y) //找到目标
		{
			snakeLength++;
			snake=(COORD*)realloc(snake,sizeof(COORD)*snakeLength); //重新申请内存
			snake[snakeLength-1] = snake[snakeLength-2]; 

			ChangeSpeed();
			ShowNewScore();
			ShowNewTarget();
		}
}

bool AgainstWall() //判断是否撞墙
{
	return snake[0].X == OFFSET+MARGIN-1 || snake[0].X == OFFSET+WALL_WID-1 ||
		snake[0].Y == MARGIN-1 || snake[0].Y == WALL_HEI-1;
}

void GameOver() //结束信息与操作
{
	//结束表现
	SetXY(snake[0].X,snake[0].Y);
	SetColor(FOREGROUND_BLUE | BACKGROUND_RED);
	cout<<"  ";

	SetColor(0xf | 0x0);
	//文本说明
	SetXY(OFFSET+WALL_WID/3, WALL_HEI/2);
	cout<<"蛇死了 - 游戏结束";
	free(snake);
	//退出
	SetXY(OFFSET, WALL_HEI+1);
	system("pause");
	exit(0);
}

int main()
{
	CurVisible(false); //隐藏光标
	Scene(); //场景
	InitSnake(2); //初始化蛇，长度>=2
	ShowSnake(); //绘制蛇
	ShowNewTarget(); //目标
	direct = 'd'; //初始方向
	speed = 500; //初始速度
	score = 0; //初始分数
	while(true)
	{
		if( _kbhit()) //监视按键
			direct = _getch();
		SnakeDirection(direct); //通过按键改变蛇的方向
		ShowSnake(); //重新绘制蛇
		FindTarget(); //寻找目标
		if(AgainstWall()) //判断撞墙
			GameOver(); //若撞墙，游戏结束
		Sleep(speed); //游戏速度
	}
	return 0;
}