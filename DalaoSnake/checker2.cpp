#include <windows.h>
#include "resource.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include<mmsystem.h>
using namespace std;
#pragma comment (lib,"winmm.lib");
//��Ϸ���Ϳ�������ȸ߶�
#define  GAME_WIDE             70     //��Ϸ�����70*10
#define  GAME_HEIGHT           60     //��Ϸ���߶�60*10
#define  CTRL_WIDE             30     //���������30*10
#define  CTRL_HEIGHT           60     //�������߶�60*10
#define  BOUNDSIZE             10     //�߽���10   
//�ߵ���Ϣ
#define   SNAKESIZE            10     //�ߵĿ�߶�Ϊ20��С����
#define   SNAKEMAX            100     //���������ֵΪ100
#define   SNAKESPEED          500     //��ʼ���ߵ��ٶ�Ϊ500ms
#define   TIMERID_1             1     //��ʱ��ID
#define   PAUSEID               1     //��ͣ��ťID
#define   STARTID               2     //��ʼ��ťID 
#define   SPEEDUPID             3     //���ٰ�ťID
#define   SPEEDDOWNID           4     //���ٰ�ťID
vector<POINT> vSnakePoint;       //����һ���ߵ�����
vector<POINT> vSnakePoint2;
	
UCHAR ucSnakeHead = 4;                //��ʼ����ͷΪ4��ʵ������vSnakePoint[3];
UCHAR ucSnakeTail = 0;                //��ʼ����βΪ0
UCHAR ucSnakekLen = 4;                //��ʼ��������Ϊ4
UCHAR ucSnakeHead2 = 4;                //��ʼ����ͷΪ4��ʵ������vSnakePoint[3];
UCHAR ucSnakeTail2 = 0;                //��ʼ����βΪ0
UCHAR ucSnakekLen2 = 4;
UINT uiSnakeSpeed = SNAKESPEED;       //��ʼ���ߵ��ٶ�Ϊ500ms
UINT uiScore = 0;//��ʼ������Ϊ0
UINT uiScore2 = 0;

POINT ptDirectHead = { 1,0 }; 
POINT ptDirectHead2 = { -1, 0 };	//��ͷ�ķ���,��ʼ��Ϊ����
POINT ptFood = { 20,20 };             //ʳ������

BOOL bIsLoose = TRUE; //�ж��Ƿ�����
BOOL bIsLoose2 = TRUE;
BOOL bIsWin = FALSE;                  //�ж��Ƿ�Ӯ��
BOOL bIsPause = FALSE;                //�ж��Ƿ���ͣ��
BOOL bIsStart = FALSE;                //�ж��Ƿ�ʼ��
BOOL bFlagStart = FALSE;              //�ж��Ƿ��ܿ�ʼ
HACCEL hAccel;
HBITMAP hBit1,hBit2,hBit3,hBit4,hBit5,hBit6; /* handle of bitmaps */
//HINSTANCE hInst; /* handle to this instance */

FILE* file;
HWND hIdeaDlg = NULL;


static int state = 2;//1����2����3����4����
static int state1 = 1,choice = 0;
HANDLE hfile;
TCHAR buf[100][100];//���ļ�


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  CALLBACK IdeaDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szAppName[] = TEXT("Snake by DaLao");
//HINSTANCE hInst = nullptr;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	//hInst = hInstance;
	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	//hInst = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	
	hwnd = CreateWindow(szAppName, szAppName,
		WS_DLGFRAME | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

//��ʼ����
VOID InitSnake()
{
	//���������
	vSnakePoint.clear();
	//�����������ռ�
	vSnakePoint.resize(SNAKEMAX);
	//��Ϸ�������ٴγ�ʼ��
	ucSnakeHead = 4;//��ʼ����ͷΪ4��ʵ������vSnakePoint[3];
	ucSnakeTail = 0;//��ʼ����βΪ0
	ucSnakekLen = 4;//��ʼ��������Ϊ4
	uiSnakeSpeed = SNAKESPEED;//��ʼ���ߵ��ٶ�Ϊ200ms
	ptDirectHead = { 1,0 };//��ʼ����ͷ����
	ptFood = { 20,20 };//��ʼ��ʳ������
	uiScore = 0;//��ʼ������
				//��ʼ��ÿһ�ڵ���������
	uiScore2 = 0;
	for (int i = 0; i < ucSnakekLen; i++)
	{
		vSnakePoint[i].x = i;
		vSnakePoint[i].y = 0;
	}
}


VOID InitSnake2()
{
	//���������
	vSnakePoint2.clear();
	//�����������ռ�
	vSnakePoint2.resize(SNAKEMAX << 4);
	//��Ϸ�������ٴγ�ʼ��
	ucSnakeHead2 = 4;//��ʼ����ͷΪ4��ʵ������vSnakePoint[3];
	ucSnakeTail2 = 0;//��ʼ����βΪ0
	ucSnakekLen2 = 4;//��ʼ��������Ϊ4
	uiSnakeSpeed = SNAKESPEED;//��ʼ���ߵ��ٶ�Ϊ200ms
	ptDirectHead2 = { -1, 0 };//��ʼ����ͷ����
							  //��ʼ��ÿһ�ڵ���������
	for (int i = 0; i < ucSnakekLen2; i++)
	{
		vSnakePoint2[i].x = 69 - i;
		vSnakePoint2[i].y = 59;
	}
}

//��ȡ�߽��λ��
//index��һ��ƫ����β��ƫ����
POINT &GetSnakeNode(int index)
{
	int i = ucSnakeTail + index;//����ĳ���
	if (i >= SNAKEMAX)
	{
		i -= SNAKEMAX;
	}
	return vSnakePoint[i];
}


POINT &GetSnakeNode2(int index)
{
	int i = ucSnakeTail2 + index;//����ĳ���
	if (i >= SNAKEMAX)
	{
		i -= SNAKEMAX;
	}
	return vSnakePoint2[i];
}

//������
VOID DrawSnake(HDC hdc,HWND hwnd)
{
	HDC memDC1,memDC2,memDC3;
	char str[30] = "saa";
	//static HBRUSH hBrush = CreateSolidBrush(RGB(0, 243, 222));
	hdc = GetDC(hwnd);
	switch (state)
	{
	case 1:
		hBit1 = (HBITMAP)LoadImage(NULL, "headblueL.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit2 = (HBITMAP)LoadImage(NULL, "bodyblue1.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit3 = (HBITMAP)LoadImage(NULL, "tailblueL.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 2:
		hBit1 = (HBITMAP)LoadImage(NULL, "headblueR.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit2 = (HBITMAP)LoadImage(NULL, "bodyblue1.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit3 = (HBITMAP)LoadImage(NULL, "tailblueR.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 3:
		hBit1 = (HBITMAP)LoadImage(NULL, "headblueU.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit2 = (HBITMAP)LoadImage(NULL, "bodyblue.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit3 = (HBITMAP)LoadImage(NULL, "tailblueU.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 4:
		hBit1 = (HBITMAP)LoadImage(NULL, "headblueD.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit2 = (HBITMAP)LoadImage(NULL, "bodyblue.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit3 = (HBITMAP)LoadImage(NULL, "tailblueD.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	}
	memDC1 = CreateCompatibleDC(hdc);
	memDC2 = CreateCompatibleDC(hdc);
	memDC3 = CreateCompatibleDC(hdc);
	//SelectObject(hdc, hBrush);
	SelectObject(memDC1, hBit1);
	SelectObject(memDC2, hBit2);
	SelectObject(memDC3, hBit3);
	//��ǰ�߽��
	POINT ptCurrentNode = { 0 };

	for (int i = 0; i < ucSnakekLen; i++)
	{
		ptCurrentNode = GetSnakeNode(i);
		//Rectangle(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE,
			//(ptCurrentNode.x + 1)*SNAKESIZE + BOUNDSIZE, (ptCurrentNode.y + 1)*SNAKESIZE + BOUNDSIZE);
		if(i==0)
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE,10, 10,memDC3, 0, 0, SRCCOPY);
		else if(i==ucSnakekLen-1)
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE, 10, 10, memDC1, 0, 0, SRCCOPY);
		else
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE, 10, 10, memDC2, 0, 0, SRCCOPY);

	}

}


VOID DrawSnake2(HDC hdc,HWND hwnd)
{
	HDC memDC4, memDC5, memDC6;
	char str[30] = "saa";
	//static HBRUSH hBrush = CreateSolidBrush(RGB(0, 243, 222));
	hdc = GetDC(hwnd);
	switch (state1)
	{
	case 1:
		hBit4 = (HBITMAP)LoadImage(NULL, "headredL.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit5 = (HBITMAP)LoadImage(NULL, "bodyred.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit6 = (HBITMAP)LoadImage(NULL, "tailredL.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 2:
		hBit4 = (HBITMAP)LoadImage(NULL, "headredR.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit5 = (HBITMAP)LoadImage(NULL, "bodyred.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit6 = (HBITMAP)LoadImage(NULL, "tailredR.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 3:
		hBit4 = (HBITMAP)LoadImage(NULL, "headredU.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit5 = (HBITMAP)LoadImage(NULL, "bodyred1.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit6 = (HBITMAP)LoadImage(NULL, "tailredU.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	case 4:
		hBit4 = (HBITMAP)LoadImage(NULL, "headredD.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit5 = (HBITMAP)LoadImage(NULL, "bodyred1.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		hBit6 = (HBITMAP)LoadImage(NULL, "tailredD.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
		break;
	}
	memDC4 = CreateCompatibleDC(hdc);
	memDC5 = CreateCompatibleDC(hdc);
	memDC6 = CreateCompatibleDC(hdc);
	//SelectObject(hdc, hBrush);
	SelectObject(memDC4, hBit4);
	SelectObject(memDC5, hBit5);
	SelectObject(memDC6, hBit6);
	//��ǰ�߽��
	POINT ptCurrentNode = { 0 };

	for (int i = 0; i < ucSnakekLen2; i++)
	{
		ptCurrentNode = GetSnakeNode2(i);
		//Rectangle(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE,
		//(ptCurrentNode.x + 1)*SNAKESIZE + BOUNDSIZE, (ptCurrentNode.y + 1)*SNAKESIZE + BOUNDSIZE);
		if (i == 0)
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE, 10, 10, memDC6, 0, 0, SRCCOPY);
		else if (i == ucSnakekLen2 - 1)
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE, 10, 10, memDC4, 0, 0, SRCCOPY);
		else
			BitBlt(hdc, ptCurrentNode.x*SNAKESIZE + BOUNDSIZE, ptCurrentNode.y*SNAKESIZE + BOUNDSIZE, 10, 10, memDC5, 0, 0, SRCCOPY);

	}
}

//����ʳ������
void SetFood()
{
	POINT ptNow = { 0 };
	int x = 0, y = 0, i = 0;
	while (true)
	{
		srand(time(0));//���������
		x = rand() % GAME_WIDE; //����ʳ���������
		y = rand() % GAME_HEIGHT;
		for (i = 0; i < ucSnakekLen; i++)
		{
			ptNow = GetSnakeNode(i);
			if (ptNow.x == x && ptNow.y == y)
				break;//���ʳ���������������������ʳ��
		}//���ʳ�ﲻ���������˳�ѭ��
		if (ucSnakekLen == i) break;
	}
	ptFood.x = x;
	ptFood.y = y;
}

//�ж����Ƿ�ײǽ
BOOL IsStrikeWall(POINT ptHead)
{
	if (ptHead.x < 0 || ptHead.x >= GAME_WIDE
		|| ptHead.y < 0 || ptHead.y >= GAME_HEIGHT)
	{
		bIsLoose = FALSE;//ײǽ����
		return FALSE;
	}
	return TRUE;
}

BOOL IsStrikeWall2(POINT ptHead)
{
	if (ptHead.x < 0 || ptHead.x >= GAME_WIDE
		|| ptHead.y < 0 || ptHead.y >= GAME_HEIGHT)
	{
		bIsLoose2 = FALSE;//ײǽ����
		return FALSE;
	}
	return TRUE;
}

//�ж��Ƿ�ײ���Լ�
BOOL IsStrikeSelf(POINT ptHead)
{
	POINT ptNow = { 0 };
	for (int i = 0; i < ucSnakekLen - 1; i++)
	{
		ptNow = GetSnakeNode(i);
		if (ptHead.x == ptNow.x && ptHead.y == ptNow.y)
		{
			bIsLoose = FALSE;//ײ�Լ�����
			return FALSE;
		}

	}
	return TRUE;
}

BOOL IsStrikeSelf2(POINT ptHead)
{
	POINT ptNow = { 0 };
	for (int i = 0; i < ucSnakekLen2 - 1; i++)
	{
		ptNow = GetSnakeNode2(i);
		if (ptHead.x == ptNow.x && ptHead.y == ptNow.y)
		{
			bIsLoose2 = FALSE;//ײ�Լ�����
			return FALSE;
		}

	}
	return TRUE;
}


BOOL IsStrike(POINT ptHead){
	POINT ptNow = { 0 };
	for (int i = 0; i < ucSnakekLen2; i++){
		ptNow = GetSnakeNode2(i);
		if (ptHead.x == ptNow.x && ptHead.y == ptNow.y){
			bIsLoose = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL IsStrike2(POINT ptHead2){

	POINT ptNow = { 0 };

	for (int i = 0; i < ucSnakekLen; i++){
		ptNow = GetSnakeNode(i);
		if (ptHead2.x == ptNow.x && ptHead2.y == ptNow.y){
			bIsLoose2 = FALSE;
			return FALSE;
		}
	}
	return TRUE;
}
//�ж��Ƿ�Ե�ʳ��
BOOL IsEat(POINT ptHead)
{
	return (ptHead.x == ptFood.x && ptHead.y == ptFood.y) ? TRUE : FALSE;
}

//��ʾ����
void ShowScore(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	HPEN hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));
	SelectObject(hdc, hPen);
	int iLength = 0;
	TCHAR szBuffer[20] = { 0 };
	iLength = wsprintf(szBuffer, "Player1 Score: %u", uiScore);
	TextOut(hdc, BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 100,
		BOUNDSIZE + 200, szBuffer, iLength);
	if (choice == 2){
		iLength = wsprintf(szBuffer, "Player2 Score: %u", uiScore2);
		TextOut(hdc, BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 100,
			BOUNDSIZE + 300, szBuffer, iLength);
	}
	ReleaseDC(hwnd, hdc);
	DeleteObject(hPen);
}

//�ж��Ƿ񳬳���Χ
void IsOverTake()
{
	if (SNAKEMAX == ucSnakeHead)  ucSnakeHead = 0;   //������Χ����ѭ��  
	if (SNAKEMAX == ucSnakeTail)  ucSnakeTail = 0;
	//if (SNAKEMAX == ucSnakekLen)  bIsWin = TRUE;//Ӯ��
	if (SNAKEMAX == ucSnakeHead2) ucSnakeHead2 = 0;
	if (SNAKEMAX == ucSnakeTail2) ucSnakeTail2 = 0;
	//if (SNAKEMAX == ucSnakekLen2) bIsWin = TRUE;
}


//�����ߵĳ���
void AddSnakeLen(POINT ptHead)
{
	vSnakePoint[ucSnakeHead++] = ptHead;//��ͷ����
	ucSnakekLen++;//����+1
	IsOverTake();//�ж��Ƿ񳬳���Χ
}

void AddSnakeLen2(POINT ptHead)
{
	vSnakePoint2[ucSnakeHead2++] = ptHead;//��ͷ����
	ucSnakekLen2++;//����+1
	IsOverTake();//�ж��Ƿ񳬳���Χ
}


//���߶�����
BOOL  SnakeMove()
{
	BOOL bRet = FALSE;
	do
	{
		POINT ptNewNode = { 0 };//�µ���ͷ
		ptNewNode.x = GetSnakeNode(ucSnakekLen - 1).x + ptDirectHead.x;
		ptNewNode.y = GetSnakeNode(ucSnakekLen - 1).y + ptDirectHead.y;
		if (!IsStrikeWall(ptNewNode))//�ж��µ���ͷ�Ƿ�ײǽ
			break;
		if (!IsStrikeSelf(ptNewNode))//�ж����Ƿ�ײ���Լ�
			break;
		if (!IsStrike(ptNewNode))
			break;
		if (IsEat(ptNewNode))//�ж��Ƿ�Ե�ʳ��
		{
			SetFood();//��������ʳ������
			uiScore += 10;//�Ե�һ��ʳ���10��
			AddSnakeLen(ptNewNode);//�����ߵĳ���
			break;
		}
		vSnakePoint[ucSnakeHead].x = ptNewNode.x;//��ͷ�ƶ�
		vSnakePoint[ucSnakeHead++].y = ptNewNode.y;
		ucSnakeTail++;
		IsOverTake();
		bRet = TRUE;
	} while (FALSE);
	return bRet;
}


//���߶�����
BOOL  SnakeMove2()
{
	BOOL bRet = FALSE;
	do
	{
		POINT ptNewNode = { 0 };//�µ���ͷ
		ptNewNode.x = GetSnakeNode2(ucSnakekLen2 - 1).x + ptDirectHead2.x;
		ptNewNode.y = GetSnakeNode2(ucSnakekLen2 - 1).y + ptDirectHead2.y;
		if (!IsStrikeWall2(ptNewNode))//�ж��µ���ͷ�Ƿ�ײǽ
			break;
		if (!IsStrikeSelf2(ptNewNode))//�ж����Ƿ�ײ���Լ�
			break;
		if (!IsStrike2(ptNewNode))
			break;
		if (IsEat(ptNewNode))//�ж��Ƿ�Ե�ʳ��
		{
			SetFood();//��������ʳ������
			if(choice == 2) uiScore2 += 10;//�Ե�һ��ʳ���10��
			AddSnakeLen2(ptNewNode);//�����ߵĳ���
			break;
		}

		vSnakePoint2[ucSnakeHead2].x = ptNewNode.x;//��ͷ�ƶ�
		vSnakePoint2[ucSnakeHead2++].y = ptNewNode.y;
		ucSnakeTail2++;
		IsOverTake();
		bRet = TRUE;
	} while (FALSE);
	return bRet;
}

//��ʳ��
void DrawFood(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);
	//��Բʳ��
	Ellipse(hdc, BOUNDSIZE + ptFood.x*SNAKESIZE, BOUNDSIZE + ptFood.y*SNAKESIZE
		, BOUNDSIZE + (ptFood.x + 1)*SNAKESIZE, BOUNDSIZE + (ptFood.y + 1)*SNAKESIZE);
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBrush);
}

void showrank(HWND hwnd){

	HDC hdc = GetDC(hwnd);

	int i = 0, col = 200;
	TCHAR S[1000] = { 0 };
	int k = 0;
	TCHAR temp[20] = "Score  Name\n\n$";
	for (int i = 0; temp[i] != '$'; i++){
		S[k++] = temp[i];
	}
	file = fopen("Rank.txt", "r");
	for (int i = 0; i < 5; i++){
		fgets(buf[i], 100, file);
		//TextOut(hdc, 200, col, buf[i], strlen(buf[i])-1);
		//col += 20;
		for (int j = 0; j<strlen(buf[i]); j++)
			S[k++] = buf[i][j];
		//S[k++] = '\n';
	}
	MessageBox(hwnd,S, TEXT("Ranklist"), MB_OK);
}

void setrank(){
	SetFilePointer(hfile, 0, 0, FILE_BEGIN);
	



}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect = { 0 };
	static int  ixWind = 0, iyWind = 0;//���ڴ�С����
	static int ixClient = 0, iyClient = 0;//�ͻ�����С����
	static int started = 0;;
	static int x1 = 0;   //���λ��
	static int y1 = 0;
	static HBITMAP      background,background1, bmp, mask;
	static HWND  hPause = nullptr;//��ͣ��ť���
	static HWND  hStart = nullptr;//��ʼ��ť���
	static HWND  hSpeedUp = nullptr;//���ٰ�ť���
	static HWND  hSpeedDown = nullptr;//���ٰ�ť���
	HMENU hmenu;
	DWORD dwError = 0;//������Ϣ
	HDC DC, memDC;
	static HINSTANCE hInst;
	DWORD dwreadsize = 0;
	switch (message)
	{
	case WM_CREATE:
	{
		DC = GetDC(hwnd);
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		//��ȡ���ڴ�С
		GetWindowRect(hwnd, &rect);
		ixWind = rect.right - rect.left;
		iyWind = rect.bottom - rect.top;
		//��ȡ�ͻ�����С
		GetClientRect(hwnd, &rect);
		ixClient = rect.right - rect.left;
		iyClient = rect.bottom - rect.top;
		//�޸Ĵ��ڴ�С
		int nWideth = (BOUNDSIZE) * 3 + (GAME_WIDE )*SNAKESIZE + (ixWind - ixClient);
		int nHeight = (BOUNDSIZE*2) + GAME_HEIGHT*SNAKESIZE+ (iyWind - iyClient);
		MoveWindow(hwnd, 50, 50, nWideth, nHeight, TRUE);
		//��ʼ������
		InitSnake();
		InitSnake2();
		//����һ����ͣ��ť,��100��50
		hPause = CreateWindowW(L"button", L"Pause", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 30, BOUNDSIZE + 20,
			100, 50, hwnd, (HMENU)PAUSEID, hInst, 0);
		//����һ����ʼ��ť,��100��50
		hStart = CreateWindowW(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 170, BOUNDSIZE + 20,
			100, 50, hwnd, (HMENU)STARTID, hInst, 0);
		//����һ�����ٰ�ť,��100��50
		hSpeedUp = CreateWindowW(L"button", L"SpeedUp", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 30, BOUNDSIZE + 90,
			100, 50, hwnd, (HMENU)SPEEDUPID, hInst, 0);
		//����һ�����ٰ�ť,��100��50
		hSpeedDown = CreateWindowW(L"button", L"SpeedDown", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE + 170, BOUNDSIZE + 90,
			100, 50, hwnd, (HMENU)SPEEDDOWNID, hInst, 0);

		//PlaySound((LPCTSTR)IDR_WAVE1, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		PlaySound(TEXT("Mass.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//background = (HBITMAP)LoadImage(NULL, "bk.bmp", IMAGE_BITMAP, BOUNDSIZE + GAME_WIDE*SNAKESIZE, BOUNDSIZE + GAME_HEIGHT*SNAKESIZE, LR_LOADFROMFILE);
		memDC = CreateCompatibleDC(DC);
		ReleaseDC(hwnd,DC);
	}

	break;
	case WM_TIMER:

		switch (wParam)
		{
		case TIMERID_1:
		{   //�ƶ���
			SnakeMove();
			if(choice == 2) SnakeMove2();
			//�ж��Ƿ�����
			if (FALSE == bIsLoose)
			{
				KillTimer(hwnd, TIMERID_1);
				MessageBox(hwnd, "Player1 looooooooooooooose!", "", MB_OK);
				InitSnake();//��ʼ����
				state = 2;
				InitSnake2();
				state1 = 1;
				choice = 0;
				int nWideth = BOUNDSIZE * 3 + (GAME_WIDE)*SNAKESIZE + (ixWind - ixClient);
				int nHeight = BOUNDSIZE * 2 + GAME_HEIGHT*SNAKESIZE + (iyWind - iyClient);
				MoveWindow(hwnd, 50, 50, nWideth, nHeight, TRUE);
				bIsLoose = TRUE;
				SetWindowTextW(hStart, L"Start"); //������˾Ͱ�ReStart��ΪStart
				bIsStart = FALSE;//���ÿ�ʼ
				bFlagStart = FALSE;
				started = 0;
			}
			if (FALSE == bIsLoose2)
			{
				KillTimer(hwnd, TIMERID_1);
				MessageBox(hwnd, "Player2 looooooooooooooose!", "", MB_OK);
				InitSnake();//��ʼ����
				state = 2;
				InitSnake2();
				state1 = 1;
				choice = 0;
				int nWideth = BOUNDSIZE * 3 + (GAME_WIDE)*SNAKESIZE + (ixWind - ixClient);
				int nHeight = BOUNDSIZE * 2 + GAME_HEIGHT*SNAKESIZE + (iyWind - iyClient);
				MoveWindow(hwnd, 50, 50, nWideth, nHeight, TRUE);
				bIsLoose2 = TRUE;
				SetWindowTextW(hStart, L"Start"); //������˾Ͱ�ReStart��ΪStart
				bIsStart = FALSE;//���ÿ�ʼ
				bFlagStart = FALSE;
				started = 0;
			}
			if (TRUE == bIsWin)//�ж��Ƿ�Ӯ��
			{
				KillTimer(hwnd, TIMERID_1);
				MessageBox(hwnd, "you Win!", "", MB_OK);
				InitSnake();//��ʼ����
				InitSnake2();
				bIsWin = FALSE;
			}
			//���»���
			RECT rect = { 0 };
			SetRect(&rect, 0, 0, BOUNDSIZE + GAME_WIDE*SNAKESIZE,
				BOUNDSIZE + GAME_HEIGHT*SNAKESIZE);
			InvalidateRect(hwnd, &rect, TRUE);
		}
		break;
		default:
			break;
		}

		break;
	case WM_SETFOCUS:
		SetFocus(hwnd);
		break;
	case WM_KEYDOWN://���Ʒ���

		if (FALSE == bIsLoose || FALSE==bIsLoose2 || TRUE == bIsPause)
			return 0;

		switch (wParam)
		{
		case VK_UP:
			state = 3;
			if (ptDirectHead.x != 0 && ptDirectHead.y != 1)
			{
				ptDirectHead.x = 0;
				ptDirectHead.y = -1;
			}
			break;
		case VK_DOWN:
			state = 4;
			if (ptDirectHead.x != 0 && ptDirectHead.y != -1)
			{
				ptDirectHead.x = 0;
				ptDirectHead.y = 1;
			}
			break;
		case VK_LEFT:
			state = 1;
			if (ptDirectHead.y != 0 && ptDirectHead.x != 1)
			{
				ptDirectHead.x = -1;
				ptDirectHead.y = 0;
			}
			break;
		case VK_RIGHT:
			state = 2;
			if (ptDirectHead.y != 0 && ptDirectHead.x != -1)
			{
				ptDirectHead.x = 1;
				ptDirectHead.y = 0;
			}
			break;
		case 87://W
			state1 = 3;
			if (choice == 2){
				if (ptDirectHead2.x != 0 && ptDirectHead2.y != 1)
				{
					ptDirectHead2.x = 0;
					ptDirectHead2.y = -1;

				}
				
			}
			break;
		case 83://S
			state1 = 4;
			if (choice == 2){
				if (ptDirectHead2.x != 0 && ptDirectHead2.y != -1)
				{
					ptDirectHead2.x = 0;
					ptDirectHead2.y = 1;

				}
			}

			break;
		case 65://A
			state1 = 1;
			if (choice == 2){
				if (ptDirectHead2.y != 0 && ptDirectHead2.x != 1)
				{
					ptDirectHead2.x = -1;
					ptDirectHead2.y = 0;

				}
			}

			break;
		case 68://D
			state1 = 2;
			if (choice == 2){
				if (ptDirectHead2.y != 0 && ptDirectHead2.x != -1)
				{
					ptDirectHead2.x = 1;
					ptDirectHead2.y = 0;

				}
			}

			break;
		default:
			break;
		}

		break;
	case WM_LBUTTONUP:
		x1 = LOWORD(lParam);	// ��귢��ʱ��x����λ��
		y1 = HIWORD(lParam);
		if (choice == 0) {
			if (x1>250 && x1<450 && y1>300 && y1<540) {

				if (y1 < 360) {
					choice = 1;
					SendMessage(hwnd, WM_COMMAND, STARTID, 0);
				}
				else if (y1 < 420) {
					choice = 2;
					SendMessage(hwnd, WM_COMMAND, STARTID, 0);
				}
				else if (y1 < 480) {
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTDLG),
						hwnd, AboutDlgProc);
				}
				else {
					choice = 4;
					KillTimer(hwnd, TIMERID_1);
					PostQuitMessage(0);
				}
			}
		}
		else {
		}
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		hmenu = GetMenu(hwnd);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case ID_Speedup:
		case SPEEDUPID://�ٶȼ�
			if (uiSnakeSpeed >= 10 && bIsStart == TRUE)
			{
				if (100 >= uiSnakeSpeed)
					uiSnakeSpeed -= 10;
				else
					uiSnakeSpeed -= 100;
				SetTimer(hwnd, TIMERID_1, uiSnakeSpeed, nullptr);//��������             
			}
			SendMessage(hwnd, WM_SETFOCUS, 0, 0);
			break;
		case ID_Speeddown:
		case SPEEDDOWNID://�ٶȼ�
		{

			if (uiSnakeSpeed <= 1000 && bIsStart == TRUE)
			{
				if (100 > uiSnakeSpeed)
					uiSnakeSpeed += 10;
				else
					uiSnakeSpeed += 100;
				SetTimer(hwnd, TIMERID_1, uiSnakeSpeed, nullptr);//��������             
			}
			SendMessage(hwnd, WM_SETFOCUS, 0, 0);
		}
		break;
		case ID_Start:
		case STARTID://��ʼ
		{
			if (FALSE == bIsStart && FALSE == bFlagStart)
			{
				started = 1;
				int nWideth = BOUNDSIZE * 3 + (GAME_WIDE + CTRL_WIDE)*SNAKESIZE + (ixWind - ixClient);
				int nHeight = BOUNDSIZE * 2 + GAME_HEIGHT*SNAKESIZE + (iyWind - iyClient);
				MoveWindow(hwnd, 50, 50, nWideth, nHeight, TRUE);
				SetTimer(hwnd, TIMERID_1, uiSnakeSpeed, nullptr);//���ö�ʱ��
				bIsStart = TRUE;//����Ϊ�Ѿ���ʼ
				bFlagStart = TRUE;//��һ�ο�ʼ�����ΪTREU ��ʾ���ٽ����������
				SetWindowTextW(hStart, L"ReStart");
				SendMessage(hwnd, WM_SETFOCUS, 0, 0);//�����뽹�㻹����Ϸ��
			}
			else
			{
				if (TRUE == bIsPause)
				{//����Ѿ���ͣ�ˣ��������¿�ʼҪ�ǵð���ͣ��ť�ָ�ԭ��
					if (!SetWindowTextW(hPause, L"Pause"))
						dwError = GetLastError();
					bIsPause = FALSE;
				}
				InitSnake();//��ʼ�������¿�ʼ
				InitSnake2();
				SetTimer(hwnd, TIMERID_1, uiSnakeSpeed, nullptr);
				SendMessage(hwnd, WM_SETFOCUS, 0, 0);//�ѽ��㻹����Ϸ��
			}
		}
		break;
		case ID_Pause:
		case PAUSEID://��ͣ
			if (TRUE == bFlagStart)//��Ϸ���е�ʱ�������ͣ
			{
				if (FALSE == bIsPause)//���û����ͣ������ͣ
				{
					KillTimer(hwnd, TIMERID_1);//�ص���ʱ��������ͣ��
					bIsPause = TRUE;          //��״̬����Ϊ��ͣ
					if (!SetWindowText(hPause, "Go on"))//���İ�ť����
						dwError = GetLastError();

				}
				else
				{//���¼�����ť�����´�����ʱ��
					SetTimer(hwnd, TIMERID_1, uiSnakeSpeed, nullptr);
					bIsPause = FALSE;//��״̬����Ϊû�б���ͣ
					if (!SetWindowText(hPause, "Pause"))//���İ�ť����
						dwError = GetLastError();
					SendMessage(hwnd, WM_SETFOCUS, 0, 0);//�ѽ��㻹����Ϸ��
				}
			}
			break;


		case ID_Rank:
			//showrank(hwnd);
			
			//hIdeaDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTDLG), hwnd, AboutDlgProc);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTDLG),
				hwnd, AboutDlgProc);
			break;
		case ID_MUSICOFF:
			PlaySound(NULL, NULL, SND_FILENAME);
			break;
		case ID_MUSICON:
			PlaySound(TEXT("Mass.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		case WM_SETFOCUS:
			SetFocus(hwnd);
			break;
		case ID_About:
			MessageBox(hwnd, TEXT("����:\n14121802\t�����\n14121811\tĲ����\n14121840\t����"), TEXT("����"), MB_OK);
			break;
		case ID_Rules:
			MessageBox(hwnd, TEXT("����ģʽ:\n����С��ȥ�Ե���ɫ��ʳ�ÿ�Ե�һ��С�ߵĳ��ȼ�һ\n��С�ߴ�����ǽ�ڻ�����������Ϸʧ��\n˫��ģʽ:\n��Ϸ�����ϽǺ����½ǻ�ֱ�����һ����ɫ�ͺ�ɫС�ߣ�ÿ�γ����ڻ�ˢ��һ��ʳ���������͵���ģʽһ��\n������С�ߵ�ͷ����������һ������ǰ��ʧ��"), TEXT("��Ϸ����"), MB_OK);
			break;
		case ID_Tips:
			MessageBox(hwnd, TEXT("����ģʽ:\n���������ֱ�����ߵ���������\n˫��ģʽ:\n���������ֱ�������ߵ���������\nWSAD���ƺ��ߵ���������"), TEXT("����˵��"), MB_OK);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC memDC,hdc = BeginPaint(hwnd, &ps);
		HDC backgroundhdc,backgroundhdc1, bmphdc, maskhdc;
		static HBRUSH hBrush1 = CreateSolidBrush(RGB(58, 206, 66));
		static HBRUSH hBrush2 = CreateSolidBrush(RGB(80, 87, 196));
		if (started == 1) {
			
			//����Ϸ��
			Rectangle(hdc, BOUNDSIZE, BOUNDSIZE, BOUNDSIZE + GAME_WIDE*SNAKESIZE,
				BOUNDSIZE + GAME_HEIGHT*SNAKESIZE);

			Rectangle(hdc, BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE, BOUNDSIZE,
				BOUNDSIZE * 2 + (GAME_WIDE + CTRL_WIDE)*SNAKESIZE, BOUNDSIZE + CTRL_HEIGHT*SNAKESIZE);
			//����������ɫ
			SetRect(&rect, BOUNDSIZE * 2 + GAME_WIDE*SNAKESIZE, BOUNDSIZE,
				BOUNDSIZE * 2 + (GAME_WIDE + CTRL_WIDE)*SNAKESIZE,
				BOUNDSIZE + CTRL_HEIGHT*SNAKESIZE);
			
			DC = GetDC(hwnd);
			memDC = CreateCompatibleDC(DC);

			background = (HBITMAP)LoadImage(NULL, "bk.bmp", IMAGE_BITMAP, BOUNDSIZE + GAME_WIDE*SNAKESIZE, BOUNDSIZE + GAME_HEIGHT*SNAKESIZE, LR_LOADFROMFILE);
			SelectObject(memDC, background);
			BitBlt(DC, BOUNDSIZE, BOUNDSIZE,  GAME_WIDE*SNAKESIZE, GAME_HEIGHT*SNAKESIZE, memDC, 0, 0, SRCCOPY);
			
			FillRect(hdc, &rect, hBrush2);
			//������
			DrawSnake(hdc, hwnd);
			if(choice==2) DrawSnake2(hdc, hwnd);
			//��ʳ��
			DrawFood(hwnd);
			//��ʾ����
			ShowScore(hwnd);
			DeleteDC(memDC);
			ReleaseDC(hwnd, DC);
			EndPaint(hwnd, &ps);
		}
		else {
			background = (HBITMAP)LoadImage(NULL, "Clouds.bmp", IMAGE_BITMAP, ixWind, iyWind, LR_LOADFROMFILE);
			backgroundhdc = CreateCompatibleDC(hdc);
			SelectObject(backgroundhdc, background);


			//------------------- background  ------------------------------------


			bmp = (HBITMAP)LoadImage(NULL, "logo.bmp", IMAGE_BITMAP, 400, 200, LR_LOADFROMFILE);
			bmphdc = CreateCompatibleDC(hdc);
			SelectObject(bmphdc, bmp);

			mask = (HBITMAP)LoadImage(NULL, "logoo.bmp", IMAGE_BITMAP, 400, 200, LR_LOADFROMFILE);
			maskhdc = CreateCompatibleDC(hdc);
			SelectObject(maskhdc, mask);

			BitBlt(hdc, 0, 0, ixWind, iyWind, backgroundhdc, 0, 0, SRCCOPY);
			BitBlt(hdc, 150, 50, 550, 250, maskhdc, 0, 0, SRCPAINT);
			BitBlt(hdc, 150, 50, 550, 250, bmphdc, 0, 0, SRCAND);

			DeleteDC(backgroundhdc);
			DeleteDC(maskhdc);
			DeleteDC(bmphdc);

			//------------------- logo  ------------------------------------
			bmp = (HBITMAP)LoadImage(NULL, "danren.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			bmphdc = CreateCompatibleDC(hdc);
			SelectObject(bmphdc, bmp);

			mask = (HBITMAP)LoadImage(NULL, "danrenBP.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			maskhdc = CreateCompatibleDC(hdc);
			SelectObject(maskhdc, mask);

			BitBlt(hdc, 0, 0, ixWind, iyWind, backgroundhdc, 0, 0, SRCCOPY);
			BitBlt(hdc, 250, 300, 450, 350, maskhdc, 0, 0, SRCPAINT);
			BitBlt(hdc, 250, 300, 450, 350, bmphdc, 0, 0, SRCAND);

			DeleteDC(backgroundhdc);
			DeleteDC(maskhdc);
			DeleteDC(bmphdc);

			//------------------- ������Ϸ  ------------------------------------
			bmp = (HBITMAP)LoadImage(NULL, "duoren.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			bmphdc = CreateCompatibleDC(hdc);
			SelectObject(bmphdc, bmp);

			mask = (HBITMAP)LoadImage(NULL, "duorenBP.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			maskhdc = CreateCompatibleDC(hdc);
			SelectObject(maskhdc, mask);

			BitBlt(hdc, 250, 360, 450, 410, maskhdc, 0, 0, SRCPAINT);
			BitBlt(hdc, 250, 360, 450, 410, bmphdc, 0, 0, SRCAND);

			DeleteDC(maskhdc);
			DeleteDC(bmphdc);
			//------------------- ������Ϸ  ------------------------------------

			bmp = (HBITMAP)LoadImage(NULL, "xuanxiang.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			bmphdc = CreateCompatibleDC(hdc);
			SelectObject(bmphdc, bmp);

			mask = (HBITMAP)LoadImage(NULL, "xuanxiangBP.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			maskhdc = CreateCompatibleDC(hdc);
			SelectObject(maskhdc, mask);

			BitBlt(hdc, 250, 420, 450, 470, maskhdc, 0, 0, SRCPAINT);
			BitBlt(hdc, 250, 420, 450, 470, bmphdc, 0, 0, SRCAND);

			DeleteDC(maskhdc);
			DeleteDC(bmphdc);
			//------------------- ѡ��  ------------------------------------

			bmp = (HBITMAP)LoadImage(NULL, "tuichu.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			bmphdc = CreateCompatibleDC(hdc);
			SelectObject(bmphdc, bmp);

			mask = (HBITMAP)LoadImage(NULL, "tuichuBP.bmp", IMAGE_BITMAP, 200, 50, LR_LOADFROMFILE);
			maskhdc = CreateCompatibleDC(hdc);
			SelectObject(maskhdc, mask);

			BitBlt(hdc, 250, 480, 450, 530, maskhdc, 0, 0, SRCPAINT);
			BitBlt(hdc, 250, 480, 450, 530, bmphdc, 0, 0, SRCAND);

			DeleteDC(maskhdc);
			DeleteDC(bmphdc);
			//------------------- �˳���Ϸ  ------------------------------------
		}
	}
	break;
	case WM_DESTROY:
		KillTimer(hwnd, TIMERID_1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*
BOOL  CALLBACK IdeaDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	HDC hdc = GetDC(hDlg);
	
	TextOut(hdc, 200, 200, buf[0], strlen(buf[0]) - 1);
	switch (LOWORD(wParam))
	{
	case IDOK:

		break;
	
	case IDCANCEL:

		break; 
	}
	return FALSE;
}
*/




BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){

	HDC hdc = GetDC(hDlg);

	int i = 0, col = 110;
	TCHAR S[1000] = { 0 };
	int k = 0;
	switch (message){
		case WM_PAINT:

			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case IDC_SHOW:
		
					file = fopen("rank.txt", "r");
					for (int i = 0; i < 5; i++){
						fgets(buf[i], 50, file);
						TextOut(hdc, 150, col, buf[i], strlen(buf[i]) - 1);
						col += 20;
						/*for (int j = 0; j<strlen(buf[i]); j++)
						S[k++] = buf[i][j];
						S[k++] = '\n';*/
					}
					fclose(file);
					//EndDialog(hDlg, FALSE);
					return TRUE;
				case IDOK:
					EndDialog(hDlg, TRUE);
					return TRUE;
				case IDCANCEL:	
					EndDialog(hDlg, TRUE);
					return TRUE;
			}


	}


	

	return FALSE;

}