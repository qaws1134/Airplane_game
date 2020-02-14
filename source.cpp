#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
//========================================
#define MAXBULLET 3  								//플레이어의 총 연발 갯수
#define MAXENEMY 15  								//적 최대수
#define MAXENEMYBULLET 4 							//적 총알 최대수
#define MAXPLAYER 10  								//플레이어 최대수
#define delay(n) Sleep(n)							// n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))		// 난수 발생기 초기화
#define random(n) (rand() % (n))					//0~n까지의 난수 발생
#define SAVE_FILE ".\\savefile\\rank.txt"			//세이브 파일 생성 
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
int px = 30;  //플레이어 이동을 위한 x좌표 변수 
int bx = -1, by;	//총알의 x좌표와 y좌표
int mx = -1, my;	//미사일의 x좌표와 y좌표
int plyer = 0;		//플레이어의 번호
int del = 0;		//재시작시 총알과 적군을 초기화해주는 변수
int restart = 0;		//재시작했는지 판단하는 변수
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
}Bullet[MAXBULLET];		//총알과 미사일을 위한 구조체 변수
//========================================
struct
{						
	BOOL exist;
	int type;
	int x, y;
	int dest;
	int nFrame;
	int nStay;
}Enemy[MAXENEMY];		//적군 구조체

char *arEnemy[] = { " -<>- ", " □*□ ", " =ㅇ=  ", " -ㅇ- " };	//적의 모양

struct
{  
	BOOL exist;
	int x, y;
	int nFrame;
	int nStay;
} EnemyBullet[MAXENEMYBULLET];	//총알 구조체

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
		setcursortype(NOCURSOR);		//커서를 없애주는 함수
		LoadingStage();					//로딩화면 구현
		
		if (GetAsyncKeyState(0x31))  //눌린키가 1일경우
		{
			system("mode con:cols=80 lines=40");	//콘솔창의 크기를 조정
			HeightLine();							//세로줄을 삽입
			randomize();							//srand 매크로문
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
		else if (GetAsyncKeyState(0x32)) //눌린키가 2일경우
		{
			RankDraw();//랭크
		}
		else if (GetAsyncKeyState(0x33)) //눌린키가 3인경우
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);		//색을 정해주는 함수
}

//========================================
void clrscr() // 화면을 모두 지운다.
{
	system("cls");
}
//========================================
void gotoxy(int x, int y) 				// 커서를 x,y좌표로 이동시킨다.
{
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}
//========================================
int wherex()							// 커서의 x 좌표를 조사한다.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.X;
}
//========================================
int wherey()							// 커서의 y좌표를 조사한다.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.Y;
}
//========================================
void setcursortype(CURSOR_TYPE c)		// 커서를 숨기거나 다시 표시한다.
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
void LoadingStage()				//로딩화면을 구현
{
	int i;
	delay(30);
	for (i = 20; i < 50; i += 2)		
	{
		SetColor(WHITE);
		gotoxy(i, 4);  printf("─");
		delay(30);
	}

	gotoxy(50, 4);  printf("┒");
	for (i = 5; i < 10; i++) {
		SetColor(WHITE);
		gotoxy(50, i); printf("┃");
		delay(50);
	}
	gotoxy(50, 10); printf("┚");
	for (i = 48; i > 20; i -= 2)
	{
		SetColor(WHITE);
		gotoxy(i, 10); printf("─");
		delay(30);
	}
	gotoxy(20, 10); printf("┖");

	for (i = 9; i > 4; i--) {
		SetColor(WHITE);
		gotoxy(20, i); printf("┃");
		delay(50);
	}

	gotoxy(20, 4); printf("┎");
	delay(150);

	gotoxy(25, 7); printf("P"); delay(50);
	gotoxy(29, 7); printf("I"); delay(50);
	gotoxy(33, 7); printf("A"); delay(50);
	gotoxy(37, 7); printf("N"); delay(50);
	gotoxy(41, 7); printf("E"); delay(50);
	gotoxy(45, 7); printf("S"); delay(50);

	gotoxy(44, 13); printf("1.실행 ");
	gotoxy(44, 16); printf("2.랭킹 ");
	gotoxy(44, 19); printf("3.종료 ");

	_getch();

}
//========================================
void HeightLine()
{

	int i;
	for (i = 0; i < 40; i++)
	{
		SetColor(WHITE);
		gotoxy(55, i); printf("┃");
	}
}
//========================================
void DrawPlayer()
{
	SetColor(GREEN);
	gotoxy(px, 39); printf("<<◎>>");
}
//========================================
void ErasePlayer()
{
	gotoxy(px, 39); printf("       ");
}
//========================================
void PlayerMoveAction()		//플레이어를 움직이게 해줄 함수
{
	int i = 0;
	char ch;
	if (GetAsyncKeyState(VK_LEFT) < 0 && px > 1)	//왼쪽방향키가 눌렸을때
	{
		ErasePlayer();
		px--;
		DrawPlayer();
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0 && px < 47)	//오른쪽방향키가 눌렸을때
	{
		ErasePlayer();
		px++;
		DrawPlayer();
	}

	if (_kbhit())		//입력받은 타자를 검사하는 키
	{
		ch = _getch();
		switch (ch)
		{
		case ' ':		//스페이스바일때 총알 발사
			for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {};
			if (i != MAXBULLET)
			{
				Bullet[i].x = px + 3;
				Bullet[i].y = 38;
				Bullet[i].exist = TRUE;
			}
			break;
		case 'D ':		//D일때 미사일 발사
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
void DrawBullet(int i)		//총알을 그려주는 함수
{
	SetColor(GREEN);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("*");
}
//========================================
void EraseBullet(int i)		//총알을 지워주는 함수
{
	gotoxy(Bullet[i].x, Bullet[i].y); printf(" ");
}
//========================================
void PlayerBulletAction()		//총알의 움직임을 처리하는 함수
{
	int i;

	if (del == 1)
	{
		for (i = 0; i < MAXBULLET; i++)		//재시작시 플래이어의 총알을 없애줌
		{
			Bullet[i].exist = FALSE;
		}
	}

	for (i = 0; i < MAXBULLET; i++)
	{
		if (Bullet[i].exist == TRUE)
		{
			EraseBullet(i);
			if (Bullet[i].y == 0)		//화면 맨위까지가면 총알을 지움
			{
				Bullet[i].exist = FALSE;
			}
			else						//아니면 y값 ++을 하고 위로 한칸내려 그림
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
	gotoxy(mx, my - 2); printf("▲▲");
	gotoxy(mx, my - 1); printf("■■");
	gotoxy(mx, my);     printf("■■");
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

	if (random(100) == 0)		//1/100 확률로 적이 생성됨
	{
		for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; }

		if (i != MAXENEMY)
		{
			if (random(2) == 0)
			{
				Enemy[i].x = 5;
				Enemy[i].dest = 1;		//왼쪽에서 오른쪽으로 이동
			}
			else
			{
				Enemy[i].x = 45;
				Enemy[i].dest = -1;		//오른쪽에서 왼쪽으로 이동
			}
			while (1)
			{
				Enemy[i].y = random(10) + 1;		//적 비행기 위치(y값)를 랜덤하게 생성
				for (Found = FALSE, j = 0; j < MAXENEMY; j++)		//비행기가 없는 위치를 골라서 생성함
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
			Enemy[i].nFrame = Enemy[i].nStay = random(6) + 1;				// 적 비행기의 속도
			Enemy[i].type = random(sizeof(arEnemy) / sizeof(arEnemy[0]));	// 적 비행기의 생존 여부 -1이면 죽음
			Enemy[i].exist = TRUE;
		}
	}
}
//========================================
void EnemyMoveAction()		//적군이동과 출력을 담당한 함수
{
	int i= 0;


	if (del == 1)
	{
		for (i = 0; i < MAXENEMY; i++)		//재시작시 적군을 없애줌
		{
			Enemy[i].exist = FALSE;
		}
	}

	for (i = 0; i < MAXENEMY; i++)
	{
		if (Enemy[i].exist == FALSE)	//FALSE인경우 삭제
			continue;

		if (--Enemy[i].nStay == 0)		//속도가 0인경우
		{
			if (Enemy[i].type == -1)	//죽어있을경우 메모리상에서 사라지게함
			{
				gotoxy(Enemy[i].x - 3, Enemy[i].y);     printf("          ");
				gotoxy(Enemy[i].x - 3, Enemy[i].y - 1); printf("          ");
				gotoxy(Enemy[i].x - 3, Enemy[i].y - 2); printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}
			Enemy[i].nStay = Enemy[i].nFrame;

			if (Enemy[i].x > 47 || Enemy[i].x < 4)	//화면 끝으로 거의가면 점점 사라지는걸 구현
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

				EnemyBulletAction(i);				//총알을 발사하게함
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
			EnemyBullet[j].x = Enemy[i].x + 2;	//적군 총알의 좌표를 구함 가운데에서 나오도록함
			EnemyBullet[j].y = Enemy[i].y + 1;	//적군 총알의 좌표를 구함 아래에서 나오도록함
			EnemyBullet[j].nFrame = EnemyBullet[j].nStay = Enemy[i].nFrame * 2;		//총알의 속도를 구현함
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

	if (del == 1)								//재시작시 적의 총알을 재거해줌
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
				if (random(2))					//총알이 플레이어를 따라옴
				{
					if (EnemyBullet[i].x < px)	//플레이어의 좌표값판단 구해서 x값을 ++이나 --를 해준다
						EnemyBullet[i].x++;
					else
						EnemyBullet[i].x--;
				}
				EnemyBullet[i].y++;				//총알을 내려오게함
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

		if (Enemy[i].y == Bullet[i].y && abs((Enemy[i].x) - Bullet[i].x) <= 7)	//적 비행기에 총알이 맞는 위치
		{	
			gotoxy(Bullet[i].x, Bullet[i].y);  printf("    ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x -2, Enemy[i].y);
			printf("    +10점    ");
			rank[plyer].Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}

		if (Enemy[i].y == my && abs((Enemy[i].x+1) - mx) <= 7)		//적 비행기에 미사일이 맞는 위치
		{
			gotoxy(mx, my);
			printf("     ");
			mx = -1;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x-2, Enemy[i].y);
			printf("    +10점    ");
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

		if (EnemyBullet[i].y == 39 && abs(EnemyBullet[i].x - (px+2)) <= 6)  //플레이어가 맞는 범위
		{	
			SetColor(RED);
			gotoxy(px, 35); printf("ㅡㅡㅡ   ㅣ");
			gotoxy(px, 36); printf("ㅣ  ㅣ ㅡㅣ");
			gotoxy(px, 37); printf("ㅡㅡㅡ   ㅣ");
			gotoxy(px, 39); printf("     ㅇ    ");
			delay(2000);
			clrscr();
			gotoxy(33, 9);
			printf("┌─────┐");
			gotoxy(33, 10);
			printf("│ 게임오버 │");
			gotoxy(33, 11);
			printf("└─────┘");
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
	printf("닉네임:");
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
		rank[i].Score = 0; //점수가 0점인 상태로 파일을 저장하면 시간,점수,이름이 모두 초기화됨   
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

	if (savefile == NULL) { //오류 or 파일 없을시 새로 생성 
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
	savefile = fopen(".\\savefile\\rank.txt", "wt");            //지정한 rank.txt에 데이터를 저장함

	for (i = 0; i<10; i++) {
		if (rank[i].Score == 0) //점수가 0점이면
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
	printf("DELETE :초기화");
	gotoxy(50, 30);
	SetColor(WHITE);
	printf("ESC :나가기");

	for (i = 0; i<10; i++) {
		if (i == 0) SetColor(RED);
		gotoxy(x, (i + 1) * 3 + 2);
		printf("[%d위]", i + 1);
		gotoxy(x + 8, (i + 1) * 3 + 2);
		printf("닉네임: %s", rank[i].name);
		gotoxy(x + 26, (i + 1) * 3 + 2);
		printf("점수: %d", rank[i].Score);
		SetColor(7);
		Sleep(200);
	}

	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;      //뒤로가기
		if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
			RankReset();               //랭킹을 초기화
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
	printf("◎──────────────────────────────────◎");

	for (i = 5; i<34; i++) {
		gotoxy(right, i);
		printf("│");
	}

	for (i = 5; i<34; i++) {
		gotoxy(left, i);
		printf("│");
	}

	gotoxy(0, 33);
	printf("◎──────────────────────────────────◎");
}
//========================================

void DrawInfo()
{
	SetColor
	(WHITE);
	gotoxy(58, 3);   printf("┏━━━━━━━━┓");
	gotoxy(58, 4);   printf("┃  P L A N E S   ┃");
	gotoxy(58, 5);   printf("┗━━━━━━━━┛");

	gotoxy(62, 10);   printf("점수 : %d", rank[plyer].Score);

	gotoxy(60, 20);   printf("LEFT ← → RIGHT");
	gotoxy(60, 25); printf("총 : 스페이스바");
	gotoxy(60, 27); printf("미사일 : D");
}
//========================================
