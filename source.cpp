#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
//========================================
#define MAXBULLET 3  								//�÷��̾��� �� ���� ����
#define MAXENEMY 15  								//�� �ִ��
#define MAXENEMYBULLET 4 							//�� �Ѿ� �ִ��
#define MAXPLAYER 10  								//�÷��̾� �ִ��
#define delay(n) Sleep(n)							// n/1000�ʸ�ŭ �ð� ����
#define randomize() srand((unsigned)time(NULL))		// ���� �߻��� �ʱ�ȭ
#define random(n) (rand() % (n))					//0~n������ ���� �߻�
#define SAVE_FILE ".\\savefile\\rank.txt"			//���̺� ���� ���� 
//========================================
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void clrscr();	 
void gotoxy(int x, int y); 
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);
//========================================                                                                                                                                             
void SetColer(int color);	
//========================================
void Flush_buffer(void);
//========================================
void LoadingStage(); 
//========================================
void HeightLine();
//========================================
int px = 30;  //�÷��̾� �̵��� ���� x��ǥ ���� 
int bx = -1, by;	//�Ѿ��� x��ǥ�� y��ǥ
int mx = -1, my;	//�̻����� x��ǥ�� y��ǥ
int plyer = 0;		//�÷��̾��� ��ȣ
int del = 0;		//����۽� �Ѿ˰� ������ �ʱ�ȭ���ִ� ����
int restart = 0;		//������ߴ��� �Ǵ��ϴ� ����
//========================================
void DrawPlayer();	//
//========================================
void ErasePlayer();
//========================================
void PlayerMoveAction();
//========================================
void DrawEnemyBullet();
//========================================
struct
{
	BOOL exist;
	int x, y;
}Bullet[MAXBULLET];		//�Ѿ˰� �̻����� ���� ����ü ����
//========================================
struct
{						
	BOOL exist;
	int type;
	int x, y;
	int dest;
	int nFrame;
	int nStay;
}Enemy[MAXENEMY];		//���� ����ü

char *arEnemy[] = { " -<>- ", " ��*�� ", " =��=  ", " -��- " };	//���� ���

struct
{  
	BOOL exist;
	int x, y;
	int nFrame;
	int nStay;
} EnemyBullet[MAXENEMYBULLET];	//�Ѿ� ����ü

//========================================
void DrawBullet(int i);
//========================================
void EraseBullet(int i);
//========================================
void PlayerBulletAction();
//========================================
void DrawMissile();
//========================================
void EraseMissile();
//========================================
void PlayerMissileAction();
//========================================
void DrawEnemy();
//========================================
void EnemyMoveAction();
//========================================
void EnemyBulletAction(int i);
//========================================
void EraseEnemyBullet(int i);
//========================================
void MoveEnemyBullet( );
//========================================
void DrawInfo();
//========================================
void EnemyCrash();
//=======================================
void PlayerCrash();
//========================================
void ShowRank();
//========================================
struct info {
	char name[10];
	int Score;
};

struct info rank[10];

void AddRank(int Score);
void RankDraw(void);
void FrameDraw(void);
void FileLoad(void);
void FileSave(void);
void RankSort(void);
void RankReset(void);
//========================================

enum
{
	BlACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DARK_YELLOW,
	GRAY,
	DARK_GARY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

int main()
{
	while (1) {
		setcursortype(NOCURSOR);		//Ŀ���� �����ִ� �Լ�
		LoadingStage();					//�ε�ȭ�� ����
		
		if (GetAsyncKeyState(0x31))  //����Ű�� 1�ϰ��
		{
			system("mode con:cols=80 lines=40");	//�ܼ�â�� ũ�⸦ ����
			HeightLine();							//�������� ����
			randomize();							//srand ��ũ�ι�
			while (1)
			{
				HeightLine();
				DrawInfo();

				PlayerMoveAction();
				PlayerBulletAction();
				PlayerMissileAction();

				DrawEnemy();
				EnemyMoveAction();
				MoveEnemyBullet();

				EnemyCrash();
				PlayerCrash();
				delay(10);
				if (restart == 1)
				{
					del = 1;
					system("cls");
					restart = 0;
					break;
				}
			}
			plyer++;
			_getch();
		}
		else if (GetAsyncKeyState(0x32)) //����Ű�� 2�ϰ��
		{
			RankDraw();//��ũ
		}
		else if (GetAsyncKeyState(0x33)) //����Ű�� 3�ΰ��
		{
			gotoxy(30, 25);
			return 0;
		}
	}
}
//========================================
void Flush_buffer(void)
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
//========================================
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);		//���� �����ִ� �Լ�
}

//========================================
void clrscr() // ȭ���� ��� �����.
{
	system("cls");
}
//========================================
void gotoxy(int x, int y) 				// Ŀ���� x,y��ǥ�� �̵���Ų��.
{
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}
//========================================
int wherex()							// Ŀ���� x ��ǥ�� �����Ѵ�.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.X;
}
//========================================
int wherey()							// Ŀ���� y��ǥ�� �����Ѵ�.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.Y;
}
//========================================
void setcursortype(CURSOR_TYPE c)		// Ŀ���� ����ų� �ٽ� ǥ���Ѵ�.
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize=1;
		CurInfo.bVisible=FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize=100;
		CurInfo.bVisible=TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize=20;
		CurInfo.bVisible=TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}
//========================================
void LoadingStage()				//�ε�ȭ���� ����
{
	int i;
	delay(30);
	for (i = 20; i < 50; i += 2)		
	{
		SetColor(WHITE);
		gotoxy(i, 4);  printf("��");
		delay(30);
	}

	gotoxy(50, 4);  printf("��");
	for (i = 5; i < 10; i++) {
		SetColor(WHITE);
		gotoxy(50, i); printf("��");
		delay(50);
	}
	gotoxy(50, 10); printf("��");
	for (i = 48; i > 20; i -= 2)
	{
		SetColor(WHITE);
		gotoxy(i, 10); printf("��");
		delay(30);
	}
	gotoxy(20, 10); printf("��");

	for (i = 9; i > 4; i--) {
		SetColor(WHITE);
		gotoxy(20, i); printf("��");
		delay(50);
	}

	gotoxy(20, 4); printf("��");
	delay(150);

	gotoxy(25, 7); printf("P"); delay(50);
	gotoxy(29, 7); printf("I"); delay(50);
	gotoxy(33, 7); printf("A"); delay(50);
	gotoxy(37, 7); printf("N"); delay(50);
	gotoxy(41, 7); printf("E"); delay(50);
	gotoxy(45, 7); printf("S"); delay(50);

	gotoxy(44, 13); printf("1.���� ");
	gotoxy(44, 16); printf("2.��ŷ ");
	gotoxy(44, 19); printf("3.���� ");

	_getch();

}
//========================================
void HeightLine()
{

	int i;
	for (i = 0; i < 40; i++)
	{
		SetColor(WHITE);
		gotoxy(55, i); printf("��");
	}
}
//========================================
void DrawPlayer()
{
	SetColor(GREEN);
	gotoxy(px, 39); printf("<<��>>");
}
//========================================
void ErasePlayer()
{
	gotoxy(px, 39); printf("       ");
}
//========================================
void PlayerMoveAction()		//�÷��̾ �����̰� ���� �Լ�
{
	int i = 0;
	char ch;
	if (GetAsyncKeyState(VK_LEFT) < 0 && px > 1)	//���ʹ���Ű�� ��������
	{
		ErasePlayer();
		px--;
		DrawPlayer();
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0 && px < 47)	//�����ʹ���Ű�� ��������
	{
		ErasePlayer();
		px++;
		DrawPlayer();
	}

	if (_kbhit())		//�Է¹��� Ÿ�ڸ� �˻��ϴ� Ű
	{
		ch = _getch();
		switch (ch)
		{
		case ' ':		//�����̽����϶� �Ѿ� �߻�
			for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {};
			if (i != MAXBULLET)
			{
				Bullet[i].x = px + 3;
				Bullet[i].y = 38;
				Bullet[i].exist = TRUE;
			}
			break;
		case 'D ':		//D�϶� �̻��� �߻�
			if (mx == -1)
			{
				mx = px + 3;
				my = 38;
			}
			break;
		case 'd':
			if (mx == -1)
			{
				mx = px + 3;
				my = 38;
			}
			break;

		}
	}
}
//========================================
void DrawBullet(int i)		//�Ѿ��� �׷��ִ� �Լ�
{
	SetColor(GREEN);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("*");
}
//========================================
void EraseBullet(int i)		//�Ѿ��� �����ִ� �Լ�
{
	gotoxy(Bullet[i].x, Bullet[i].y); printf(" ");
}
//========================================
void PlayerBulletAction()		//�Ѿ��� �������� ó���ϴ� �Լ�
{
	int i;

	if (del == 1)
	{
		for (i = 0; i < MAXBULLET; i++)		//����۽� �÷��̾��� �Ѿ��� ������
		{
			Bullet[i].exist = FALSE;
		}
	}

	for (i = 0; i < MAXBULLET; i++)
	{
		if (Bullet[i].exist == TRUE)
		{
			EraseBullet(i);
			if (Bullet[i].y == 0)		//ȭ�� ������������ �Ѿ��� ����
			{
				Bullet[i].exist = FALSE;
			}
			else						//�ƴϸ� y�� ++�� �ϰ� ���� ��ĭ���� �׸�
			{
				Bullet[i].y--;
				DrawBullet(i);
			}

		}
	}
}
//========================================

void DrawMissile()
{
	SetColor(YELLOW);
	gotoxy(mx, my - 2); printf("���");
	gotoxy(mx, my - 1); printf("���");
	gotoxy(mx, my);     printf("���");
}

//========================================

void EraseMissile()
{
	gotoxy(mx, my - 2); printf("     ");
	gotoxy(mx, my - 1); printf("     ");
	gotoxy(mx, my);     printf("     ");
}

//========================================

void PlayerMissileAction()
{
	if (mx != -1)
	{
		EraseMissile();
		if (my - 2 == 0)
		{
			mx = -1;
		}
		else
		{
			my--;
			DrawMissile();
		}
	}
}

//========================================

void DrawEnemy()
{
	BOOL Found;
	int i, j;

	if (random(100) == 0)		//1/100 Ȯ���� ���� ������
	{
		for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; }

		if (i != MAXENEMY)
		{
			if (random(2) == 0)
			{
				Enemy[i].x = 5;
				Enemy[i].dest = 1;		//���ʿ��� ���������� �̵�
			}
			else
			{
				Enemy[i].x = 45;
				Enemy[i].dest = -1;		//�����ʿ��� �������� �̵�
			}
			while (1)
			{
				Enemy[i].y = random(10) + 1;		//�� ����� ��ġ(y��)�� �����ϰ� ����
				for (Found = FALSE, j = 0; j < MAXENEMY; j++)		//����Ⱑ ���� ��ġ�� ��� ������
				{
					if (Enemy[i].exist == TRUE && Enemy[j].y == Enemy[i].y)
					{
						Found = TRUE;
						break;
					}
				}
				if (Found == FALSE)
					break;
			}
			Enemy[i].nFrame = Enemy[i].nStay = random(6) + 1;				// �� ������� �ӵ�
			Enemy[i].type = random(sizeof(arEnemy) / sizeof(arEnemy[0]));	// �� ������� ���� ���� -1�̸� ����
			Enemy[i].exist = TRUE;
		}
	}
}
//========================================
void EnemyMoveAction()		//�����̵��� ����� ����� �Լ�
{
	int i= 0;


	if (del == 1)
	{
		for (i = 0; i < MAXENEMY; i++)		//����۽� ������ ������
		{
			Enemy[i].exist = FALSE;
		}
	}

	for (i = 0; i < MAXENEMY; i++)
	{
		if (Enemy[i].exist == FALSE)	//FALSE�ΰ�� ����
			continue;

		if (--Enemy[i].nStay == 0)		//�ӵ��� 0�ΰ��
		{
			if (Enemy[i].type == -1)	//�׾�������� �޸𸮻󿡼� ���������
			{
				gotoxy(Enemy[i].x - 3, Enemy[i].y);     printf("          ");
				gotoxy(Enemy[i].x - 3, Enemy[i].y - 1); printf("          ");
				gotoxy(Enemy[i].x - 3, Enemy[i].y - 2); printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}
			Enemy[i].nStay = Enemy[i].nFrame;

			if (Enemy[i].x > 47 || Enemy[i].x < 4)	//ȭ�� ������ ���ǰ��� ���� ������°� ����
			{
				Enemy[i].exist = FALSE;
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf("         ");
			}
			else
			{
				Enemy[i].x += Enemy[i].dest;			
				SetColor(RED);
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf(arEnemy[Enemy[i].type]);

				EnemyBulletAction(i);				//�Ѿ��� �߻��ϰ���
			}
		}
	}
}

//========================================

void EnemyBulletAction(int i)
{
	int j;

	if (random(20) == 0)
	{
		for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) { ; }
		if (j != MAXENEMYBULLET)				
		{
			EnemyBullet[j].x = Enemy[i].x + 2;	//���� �Ѿ��� ��ǥ�� ���� ������� ����������
			EnemyBullet[j].y = Enemy[i].y + 1;	//���� �Ѿ��� ��ǥ�� ���� �Ʒ����� ����������
			EnemyBullet[j].nFrame = EnemyBullet[j].nStay = Enemy[i].nFrame * 2;		//�Ѿ��� �ӵ��� ������
			EnemyBullet[j].exist = TRUE;
		}
	}
}

//========================================

void DrawEnemyBullet(int i)
{
	SetColor(RED);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("*");
}

//========================================

void EraseEnemyBullet(int i)
{
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf(" ");
}


//========================================
void MoveEnemyBullet()
{
	int i;

	if (del == 1)								//����۽� ���� �Ѿ��� �������
	{
		for (i = 0; i < MAXBULLET; i++)
		{
			EnemyBullet[i].exist = FALSE;
		}
	}
	del = 0;


	for (i = 0; i < MAXBULLET; i++)
	{
		if (EnemyBullet[i].exist == FALSE)
			continue;

		if (--EnemyBullet[i].nStay == 0)
		{
			EnemyBullet[i].nStay = EnemyBullet[i].nFrame;
			EraseEnemyBullet(i);

			if (EnemyBullet[i].y >38)
			{
				EnemyBullet[i].exist = FALSE;
			}
			else
			{
				if (random(2))					//�Ѿ��� �÷��̾ �����
				{
					if (EnemyBullet[i].x < px)	//�÷��̾��� ��ǥ���Ǵ� ���ؼ� x���� ++�̳� --�� ���ش�
						EnemyBullet[i].x++;
					else
						EnemyBullet[i].x--;
				}
				EnemyBullet[i].y++;				//�Ѿ��� ����������
				DrawEnemyBullet(i);
			}
		}
	}



}

//========================================
void EnemyCrash()
{
	int i;

	for (i = 0; i < MAXENEMY; i++)
	{
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;

		if (Enemy[i].y == Bullet[i].y && abs((Enemy[i].x) - Bullet[i].x) <= 7)	//�� ����⿡ �Ѿ��� �´� ��ġ
		{	
			gotoxy(Bullet[i].x, Bullet[i].y);  printf("    ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x -2, Enemy[i].y);
			printf("    +10��    ");
			rank[plyer].Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}

		if (Enemy[i].y == my && abs((Enemy[i].x+1) - mx) <= 7)		//�� ����⿡ �̻����� �´� ��ġ
		{
			gotoxy(mx, my);
			printf("     ");
			mx = -1;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x-2, Enemy[i].y);
			printf("    +10��    ");
			rank[plyer].Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}
	}
}
//========================================
void PlayerCrash()
{

	int i;

	for (i = 0; i < MAXENEMYBULLET; i++)
	{
		if (EnemyBullet[i].exist == FALSE)
			continue;

		if (EnemyBullet[i].y == 39 && abs(EnemyBullet[i].x - (px+2)) <= 6)  //�÷��̾ �´� ����
		{	
			SetColor(RED);
			gotoxy(px, 35); printf("�ѤѤ�   ��");
			gotoxy(px, 36); printf("��  �� �Ѥ�");
			gotoxy(px, 37); printf("�ѤѤ�   ��");
			gotoxy(px, 39); printf("     ��    ");
			delay(2000);
			clrscr();
			gotoxy(33, 9);
			printf("��������������");
			gotoxy(33, 10);
			printf("�� ���ӿ��� ��");
			gotoxy(33, 11);
			printf("��������������");
			delay(2000);
			AddRank(rank[plyer].Score);
			restart = 1;
			px = 30;
		}
	}
}


//========================================

void AddRank(int Score)
{
	Flush_buffer();
	gotoxy(33, 13);
	printf("�г���:");
	gotoxy(41, 13);
	//scanf("%s",rank[10].name);
	fgets(rank[plyer].name, 10, stdin);

	rank[plyer].Score = Score;
	RankSort();
	FileSave();
}

//========================================

void RankReset(void)
{
	int i;
	for (i = 0; i<10; i++)
		rank[i].Score = 0; //������ 0���� ���·� ������ �����ϸ� �ð�,����,�̸��� ��� �ʱ�ȭ��   
	FileSave();
}

//========================================

void RankSort(void)
{
	int i, j, cnt = 0;
	struct info temp;

	for (i = 0; i<10; i++)
	{
		for (j = 0; j<11; j++)
		{
			if (rank[j].Score < rank[j + 1].Score)
			{
				temp = rank[j];
				rank[j] = rank[j + 1];
				rank[j + 1] = temp;
			}
		}
	}
	FileSave();
}

//========================================

void FileLoad(void)
{
	int i;
	FILE *savefile;

	savefile = fopen(".\\savefile\\rank.txt", "rt");

	if (savefile == NULL) { //���� or ���� ������ ���� ���� 
		savefile = fopen(".\\savefile\\rank.txt", "a");
		fclose(savefile);
		return;
	}


	for (i = 0; i<10; i++)
		fscanf(savefile, "%d %s\n", &rank[i].Score, &rank[i].name);

	fclose(savefile);
}

//========================================

void FileSave(void)
{
	int i;
	FILE *savefile;
	savefile = fopen(".\\savefile\\rank.txt", "wt");            //������ rank.txt�� �����͸� ������

	for (i = 0; i<10; i++) {
		if (rank[i].Score == 0) //������ 0���̸�
			fprintf(savefile, "0 ---\n");
		else
			fprintf(savefile, "%d %s\n", rank[i].Score, rank[i].name);
	}
	fclose(savefile);
}

//========================================

void RankDraw(void)
{
	const int x = 8;
	int i;

	FrameDraw();
	FileLoad();
	RankSort();

	gotoxy(50, 28);
	SetColor(RED);
	printf("DELETE :�ʱ�ȭ");
	gotoxy(50, 30);
	SetColor(WHITE);
	printf("ESC :������");

	for (i = 0; i<10; i++) {
		if (i == 0) SetColor(RED);
		gotoxy(x, (i + 1) * 3 + 2);
		printf("[%d��]", i + 1);
		gotoxy(x + 8, (i + 1) * 3 + 2);
		printf("�г���: %s", rank[i].name);
		gotoxy(x + 26, (i + 1) * 3 + 2);
		printf("����: %d", rank[i].Score);
		SetColor(7);
		Sleep(200);
	}

	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;      //�ڷΰ���
		if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
			RankReset();               //��ŷ�� �ʱ�ȭ
			break;
		}
	}
	system("cls");
}

//========================================

void FrameDraw(void)
{
	int i;
	const int right = 0, left = 70;
	system("cls");
	gotoxy(0, 4);
	printf("�ݦ���������������������������������������������������������������������");

	for (i = 5; i<34; i++) {
		gotoxy(right, i);
		printf("��");
	}

	for (i = 5; i<34; i++) {
		gotoxy(left, i);
		printf("��");
	}

	gotoxy(0, 33);
	printf("�ݦ���������������������������������������������������������������������");
}
//========================================

void DrawInfo()
{
	SetColor
	(WHITE);
	gotoxy(58, 3);   printf("��������������������");
	gotoxy(58, 4);   printf("��  P L A N E S   ��");
	gotoxy(58, 5);   printf("��������������������");

	gotoxy(62, 10);   printf("���� : %d", rank[plyer].Score);

	gotoxy(60, 20);   printf("LEFT �� �� RIGHT");
	gotoxy(60, 25); printf("�� : �����̽���");
	gotoxy(60, 27); printf("�̻��� : D");
}
//========================================
