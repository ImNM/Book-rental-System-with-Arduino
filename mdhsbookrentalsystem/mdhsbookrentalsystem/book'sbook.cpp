//고등학교 도서대출프로그램 제작을 위한 소스파일
//-이찬진
//메인윈도우엔 8개의 버튼
//차일드윈도우는 총 8개이며 버튼 한개당 차일드윈도우 한개씩 있음
//시리얼통신은 설정
//간단한 정보표시와 학교이미지 그리고 팀 정보는 홈버튼
//시리얼 통신을통해받은 uid값은 전역변수로 저장한다.
//제작만기일은 8월 21일 금요일
// Team NM, Team 모음
//이찬진 이숭우 오승준

#pragma comment(linker,"/manifestdependency:\" type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0'  processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") )

#include <Windows.h>
#include "resource.h"
#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

#define HomeEdit 100 //HMENU ID
#define BMbutton 560//button id
#define BMbutton1 561//button id
#define BMbutton2 562
#define BMbutton3 563
#define BMbutton4 564
#define SBotton1 565//설정
#define SearchButton 570

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildHomeButtonProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildStudentRegisterProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildBookRentalProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildRentalCurrentStateProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildBookManagementProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildBookStatsProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildStudentManagementProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildSetupProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WPStartpage2Proc(HWND, UINT, WPARAM, LPARAM);

void Sellectwindow(HWND);
void FontTextout(int b, int c, int x, int y);
void FontCreate(int a, HWND st3);
void GetNMSerial();

HINSTANCE g_hlnst;
LPCTSTR lpszClass = TEXT("도서대출프로그램");
HWND hWndMain, HomeButton, StudentRegister, BookRental, RentalCurrentState, BookManagement, BookStats, StudentManagement, Setup, WPStartpage2;//ShowWindow SW_HIDE or SW_SHOW createwindow hwnd class
HWND hHomeEdit;
HWND WPStartpage, WPStartpage1;

HDC hdc; //dc
PAINTSTRUCT ps; //paintstruct
HFONT hFont, OldFont; //Font
TCHAR str[256]; //문자열
//TCHAR buf[1024];

TCHAR buf[1024];
TCHAR buf1[1024];
CHAR str2[256];

int dataLength = 256;
int readResult = 0;
static HWND SEdit1, HEdit1, SREdit1, SREdit2, SREdit3, BREdit1, BREdit7, SMEdit1, RCEdit1;//설정에딧 정보표시

Serial* SP = new Serial(TEXT("\\\\.\\COM3"));//시리얼 포트 디폴트 COM3

static HBRUSH hWhiteBrush, hRedBrush, hOrangeBrush, hYellowBrush, hGreenBrush, hBlueBrush, hNavyBrush, hPurpleBrush, hDarkorangeBrush, hGoldBrush, hGrayBrush, hLightGrayBrush, hYellowLGBrush;
//------------------------------------------------------------------------------------
void FontCreate(int a, HWND st3){
	hFont = CreateFont(a, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("나눔바른고딕"));
	SendMessage(st3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));

}
//------------------------------------------------------------------------------------------------------------------------------
struct SCstudentRegister{
	char Cstudentuid[100];
	char Cstudentname[100];
	char Cstudentnumber[100];
};
struct STstudentRegister{
	TCHAR Tstudentuid[100];
	TCHAR Tstudentname[100];
	TCHAR Tstudentnumber[100];
};
struct SCstudentRegister1{
	char Cstudentuid1[100];//1은 백업용
	char Cstudentname1[100];
	char Cstudentnumber1[100];
};
struct STstudentRegister1{
	TCHAR Tstudentuid1[100];
	TCHAR Tstudentname1[100];
	TCHAR Tstudentnumber1[100];
};

//학생정보 저장 구조체
//---------------------------------------------------------------------------------------------------------------------------
struct Cbook{
	char Cbookname[100];
	char Cbookpublisher[100];
	char Cbookauthor[100];
	char Cbookrental[100];
};
struct Tbook{
	TCHAR Tbookname[100];
	TCHAR Tbookpublisher[100];
	TCHAR Tbookauthor[100];
	TCHAR Tbookrental[100];
};
struct Cbook1{//1은 백업용
	char Cbookname1[100];
	char Cbookpublisher1[100];
	char Cbookauthor1[100];
	char Cbookrental1[100];
};
struct Tbook1{
	TCHAR Tbookname1[100];
	TCHAR Tbookpublisher1[100];
	TCHAR Tbookauthor1[100];
	TCHAR Tbookrental1[100];
};
static CHAR blinks[2] = " ";
static CHAR getblinks[1] = "";
//-------------------------------------------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdParam, int nCmdShow){
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClassEx;
	g_hlnst = hInstance;

	hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	hOrangeBrush = CreateSolidBrush(RGB(255, 165, 0));
	hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
	hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	hNavyBrush = CreateSolidBrush(RGB(0, 0, 128));
	hPurpleBrush = CreateSolidBrush(RGB(128, 0, 128));
	hDarkorangeBrush = CreateSolidBrush(RGB(255, 140, 0));
	hGoldBrush = CreateSolidBrush(RGB(255, 215, 0));
	hGrayBrush = CreateSolidBrush(RGB(128, 128, 128));
	hLightGrayBrush = CreateSolidBrush(RGB(226, 226, 226));
	hYellowLGBrush = CreateSolidBrush(RGB(255, 222, 102));

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.cbClsExtra = 0;
	WndClassEx.cbWndExtra = 0;
	WndClassEx.hbrBackground = hDarkorangeBrush;
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClassEx.hIconSm = NULL;
	WndClassEx.hInstance = hInstance;
	WndClassEx.lpfnWndProc = WndProc;
	WndClassEx.lpszClassName = lpszClass;
	WndClassEx.lpszMenuName = NULL;
	WndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&WndClassEx);// 메인 윈도우 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildHomeButtonProc;
	WndClassEx.lpszClassName = TEXT("ChildHomeButton");
	RegisterClassEx(&WndClassEx);//홈버튼 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildStudentRegisterProc;
	WndClassEx.lpszClassName = TEXT("ChildStudentRegister");
	RegisterClassEx(&WndClassEx);//학생 등록 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildBookRentalProc;
	WndClassEx.lpszClassName = TEXT("ChildBookRental");
	RegisterClassEx(&WndClassEx);//도서 대출 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildRentalCurrentStateProc;
	WndClassEx.lpszClassName = TEXT("ChildRentalCurrentState");
	RegisterClassEx(&WndClassEx);//대출 현황 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildBookManagementProc;
	WndClassEx.lpszClassName = TEXT("ChildBookManagement");
	RegisterClassEx(&WndClassEx);//책 관리 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildBookStatsProc;
	WndClassEx.lpszClassName = TEXT("ChildBookStats");
	RegisterClassEx(&WndClassEx);//대출 통계 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildStudentManagementProc;
	WndClassEx.lpszClassName = TEXT("ChildStudentManagement");
	RegisterClassEx(&WndClassEx);//학생 관리 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hWhiteBrush;
	WndClassEx.lpfnWndProc = ChildSetupProc;
	WndClassEx.lpszClassName = TEXT("ChildSetup");
	RegisterClassEx(&WndClassEx);//설정 화면 클래스

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hbrBackground = hDarkorangeBrush;
	WndClassEx.lpfnWndProc = WPStartpage2Proc;
	WndClassEx.lpszClassName = TEXT("WPStartpage2");
	RegisterClassEx(&WndClassEx);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,//크기조절 없앰
		5, 5, 1020, 545, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;//hWnd 핸들값 전역변수 저장

	while (GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
//----------------------------------------------- --- 시리얼 thread 설정proc에서 오픈

DWORD WINAPI ThreadSerial(LPVOID temp)
{
	static CHAR incomingData[255] = "";			// don't forget to pre-allocate memory
	static CHAR DataSave[255] = "";
	static CHAR clenanincomingData[255] = "";
	static TCHAR TincomingData[255] = TEXT(""); //태그 유아이디
	while (SP->IsConnected()){
		CHAR incomingData[255] = "";			// don't forget to pre-allocate memory
		CHAR DataSave[255] = "";
		CHAR clenanincomingData[255] = "";
		TCHAR TincomingData[255] = TEXT(""); //태그 유아이디
		SP->ReadData(incomingData, dataLength);
		strcpy(clenanincomingData, incomingData);
		switch (strlen(clenanincomingData)){
		case 11:
			MultiByteToWideChar(CP_ACP, 0, incomingData, 9, TincomingData, 9);
			break;
		case 12:
			MultiByteToWideChar(CP_ACP, 0, incomingData, 10, TincomingData, 10);
			break;
		}
		/*if (strlen(clenanincomingData) == 11){
			MultiByteToWideChar(CP_ACP, 0, incomingData, 9, TincomingData, 9);
			}
			else if (strlen(incomingData) == 12){
			MultiByteToWideChar(CP_ACP, 0, incomingData, 10, TincomingData, 10);
			}*/

		if (strcmp(incomingData, DataSave) != 0){
			SetWindowText(SEdit1, TincomingData);//tincoimingData 계속 씀
			SetWindowText(HEdit1, TincomingData);
			SetWindowText(SREdit1, TincomingData);
			SetWindowText(BREdit1, TincomingData);
			SetWindowText(BREdit7, TincomingData);
			SetWindowText(SMEdit1, TincomingData);
			SetWindowText(RCEdit1, TincomingData);
		}
		strcpy(DataSave, incomingData);
		Sleep(500);

	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//mainProc

	int createi = 1;
	switch (iMessage){
	case WM_CREATE:
		//CreateWindow 부분  10~17 ID
		//HomeButton = CreateWindow(TEXT("ChildHomeButton"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
		//	255, 0, 1020, 545, hWnd, (HMENU)10, g_hlnst, NULL); ShowWindow(HomeButton, SW_SHOW);
		if (createi == 1){
			WPStartpage1 = CreateWindow(TEXT("static"), TEXT("Click to START"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
				0, 120, 1020, 400, hWnd, (HMENU)702, g_hlnst, NULL);
			WPStartpage = CreateWindow(TEXT("static"), TEXT("도서대출시스템"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
				0, 0, 1020, 300, hWnd, (HMENU)701, g_hlnst, NULL);
			FontCreate(80, WPStartpage);
			FontCreate(50, WPStartpage1);
		}

		//CreateWindow 부분  10~17 ID

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == WPStartpage){
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			SetBkColor((HDC)wParam, RGB(255, 140, 0));
			return (LRESULT)hDarkorangeBrush;
		}
		if ((HWND)lParam == WPStartpage1){
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			SetBkColor((HDC)wParam, RGB(255, 140, 0));
			return (LRESULT)hDarkorangeBrush;
		}
		break;
	case WM_LBUTTONDOWN:
		if (createi == 1){
			WPStartpage2 = CreateWindow(TEXT("WPStartpage2"), NULL, WS_CHILD | WS_VISIBLE,
				0, 0, 1020, 545, hWnd, (HMENU)18, g_hlnst, NULL); ShowWindow(WPStartpage2, SW_SHOW);


			DestroyWindow(WPStartpage);
			DestroyWindow(WPStartpage1);
			createi = 2;
		}
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//------------------------------------------------------------------------------------------------------------------------------
void Sellectwindow(HWND a){//래퍼 함수 showwindow SW_
	int i;
	HWND str[8] = { HomeButton, StudentRegister, BookRental, RentalCurrentState, BookManagement, BookStats, StudentManagement, Setup };
	for (i = 0; i < 8; i++){
		ShowWindow(str[i], SW_HIDE);
	}
	ShowWindow(a, SW_SHOW);
}
//------------------------------------------------------------------------------------------------------------------------------
void FontTextout(int fontsize, int string, int x, int y){//a 인스턴트,b 폰트크기,c 스티링ID,x,y


	hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("나눔바른고딕"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	LoadString(g_hlnst, string, str, 20);
	TextOut(hdc, x, y, str, lstrlen(str));
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

}
//------------------------------------------------------------------------------------------------------------------------------

FILE* StudentFile;
//버튼->화면 부분
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildHomeButtonProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//홈 화면 함수
	static HWND HBst1, HBst2, HBst3, HBst4, HBst5, HBst6, HBst7, HBst8, HBst9;
	static HWND HEdit2, HEdit3;
	HWND HButton1;
	struct SCstudentRegister CstudentRegister;
	struct STstudentRegister TstudentRegister;
	struct SCstudentRegister1 CstudentRegister1;
	struct STstudentRegister1 TstudentRegister1;
	switch (iMessage){
	case WM_CREATE:

		HBst1 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 350, 70, 20, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst2 = CreateWindow(TEXT("static"), TEXT("성명 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 390, 70, 20, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst3 = CreateWindow(TEXT("static"), TEXT("도서대출시스템"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 250, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst4 = CreateWindow(TEXT("static"), TEXT("과학탐구대회 "), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 210, 745, 40, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst5 = CreateWindow(TEXT("static"), TEXT("TEAM 모음 "), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 250, 745, 40, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst6 = CreateWindow(TEXT("static"), TEXT("태그 정보"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 295, 350, 40, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst7 = CreateWindow(TEXT("static"), TEXT("TEAM 모음"), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
			400, 450, 420, 40, hWnd, (HMENU)-1, g_hlnst, NULL);
		HBst8 = CreateWindow(TEXT("static"), TEXT("학번 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			10, 430, 70, 20, hWnd, (HMENU)-1, g_hlnst, NULL);
		FontCreate(50, HBst3);
		FontCreate(35, HBst4);
		FontCreate(35, HBst5);
		FontCreate(30, HBst6);
		FontCreate(30, HBst7);
		FontCreate(20, HBst9);

		HEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 350, 200, 25, hWnd, (HMENU)-1, g_hlnst, NULL);//태그 uid
		//SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)3, 0);
		HEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 390, 100, 25, hWnd, (HMENU)-1, g_hlnst, NULL);//성명
		HEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 430, 80, 25, hWnd, (HMENU)-1, g_hlnst, NULL);//학번

		HButton1 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			295, 350, 50, 23, hWnd, (HMENU)452, g_hlnst, NULL);

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case 452://찾기

			GetWindowText(HEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentnumber, 100, TstudentRegister.Tstudentnumber, 100);

				SetWindowText(SREdit2, TstudentRegister.Tstudentname);
				SetWindowText(SREdit3, TstudentRegister.Tstudentnumber);
				SetWindowText(HEdit2, TstudentRegister.Tstudentname);
				SetWindowText(HEdit3, TstudentRegister.Tstudentnumber);
			}

			fclose(StudentFile);
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == HBst1 | (HWND)lParam == HBst2 | (HWND)lParam == HBst8 | (HWND)lParam == HBst9) {
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		if ((HWND)lParam == HBst3 | (HWND)lParam == HBst4){
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == HBst5){
			SetTextColor((HDC)wParam, RGB(255, 165, 0));
			SetBkColor((HDC)wParam, RGB(226, 226, 226));
			return (LRESULT)hLightGrayBrush;
		}
		if ((HWND)lParam == HBst6 | (HWND)lParam == HBst7){
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return (LRESULT)hGoldBrush;
		}
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------고쳐라 0818
//파일 저장된후 시리얼 정보를 찾기 버튼으로 찾는게 나을듯 싶음
// 도서정보에 하나더 추가하고 %s로 태그uid추가 하는게.. %s /%s /%s /%s 
/*
DWORD WINAPI ThreadStudentFile(LPVOID temp){

GetWindowText(SREdit1, TstudentRegister1.Tstudentuid1, 15);
WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 15, CstudentRegister1.Cstudentuid1, 15, NULL, NULL);
StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
fscanf(StudentFile, "%s /%s /%s", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
if (feof(StudentFile)){
MessageBox(hWndMain, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("오류"), MB_OK);
break;
}
}

if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 15, TstudentRegister.Tstudentname, 15);
MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentnumber, 15, TstudentRegister.Tstudentnumber, 15);
SetWindowText(SREdit2, TstudentRegister.Tstudentname);
SetWindowText(SREdit3, TstudentRegister.Tstudentnumber);
}

fclose(StudentFile);
Sleep(500);

return 0;
}*/
//---------------------------------------------------------------------------------------------------------------
struct rental{

};
//-------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildStudentRegisterProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//학생 등록 화면  함수
	static HWND SRst1, SRst2, SRst3, SRst4, SRst5, SRst6, SRst7, SRst8, SRst9;
	static HWND SREdit2, SREdit3, SREdit4, SREdit5;
	static HWND SRButton1, SRButton2, SRButton3;
	HANDLE studentFile;
	DWORD ThreadID;
	int file_state;
	struct SCstudentRegister CstudentRegister;
	struct STstudentRegister TstudentRegister;
	struct SCstudentRegister1 CstudentRegister1;
	struct STstudentRegister1 TstudentRegister1;
	static int i = 0;

	switch (iMessage){

	case WM_CREATE:
		SRst1 = CreateWindow(TEXT("static"), TEXT("학생 등록"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 110, hWnd, (HMENU)0, g_hlnst, NULL);
		SRst2 = CreateWindow(TEXT("static"), TEXT("학생 정보"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 155, 350, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst3 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 240, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst4 = CreateWindow(TEXT("static"), TEXT("이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 275, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst5 = CreateWindow(TEXT("static"), TEXT("학번 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 320, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst6 = CreateWindow(TEXT("static"), TEXT("학생 등록"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			395, 440, 350, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst7 = CreateWindow(TEXT("static"), TEXT("이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			395, 300, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst8 = CreateWindow(TEXT("static"), TEXT("학번 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			395, 335, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SRst9 = CreateWindow(TEXT("static"), TEXT("TEAM 모음 "), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 110, 745, 40, hWnd, (HMENU)-1, g_hlnst, NULL);

		SREdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 240, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//uid
		SREdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 275, 100, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//name--
		SREdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			85, 320, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//넘버--
		SREdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			470, 300, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//이름
		SREdit5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			470, 335, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//학번

		SRButton1 = CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			600, 370, 50, 23, hWnd, (HMENU)450, g_hlnst, NULL);
		SRButton2 = CreateWindow(TEXT("button"), TEXT("저장"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			655, 370, 50, 23, hWnd, (HMENU)451, g_hlnst, NULL);
		SRButton3 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			290, 240, 50, 23, hWnd, (HMENU)452, g_hlnst, NULL);
		SendMessage(SREdit4, EM_LIMITTEXT, (WPARAM)3, 0);
		SendMessage(SREdit5, EM_LIMITTEXT, (WPARAM)5, 0);

		FontCreate(50, SRst1);
		FontCreate(40, SRst2);
		FontCreate(40, SRst6);
		FontCreate(35, SRst9);

		//studentFile = CreateThread(NULL, 0, ThreadStudentFile, NULL, 0, &ThreadID);//스레드 만듬
		//CloseHandle(ThreadStudentFile);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case 450://취소
			SetWindowText(SREdit4, NULL);
			SetWindowText(SREdit5, NULL);
			break;

		case 451://저장

			if (i == 0){
				GetWindowText(HEdit1, TstudentRegister1.Tstudentuid1, 100);
				WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
				StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

				while (!feof(StudentFile)){
					fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
						MessageBox(hWnd, TEXT("이미 학생정보가 등록되어 있습니다."), TEXT("학생등록오류"), MB_OK);
						fclose(StudentFile);
						i = 1;
						break;
					}
				}
				fclose(StudentFile);

			}

			if (i != 1){
				GetWindowText(SREdit1, TstudentRegister.Tstudentuid, 100);
				GetWindowText(SREdit4, TstudentRegister.Tstudentname, 100);
				GetWindowText(SREdit5, TstudentRegister.Tstudentnumber, 100);

				WideCharToMultiByte(CP_ACP, 0, TstudentRegister.Tstudentuid, 100, CstudentRegister.Cstudentuid, 100, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, TstudentRegister.Tstudentname, 100, CstudentRegister.Cstudentname, 100, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, TstudentRegister.Tstudentnumber, 100, CstudentRegister.Cstudentnumber, 100, NULL, NULL);
				StudentFile = fopen("student.txt", "a+");

				if (strlen(CstudentRegister.Cstudentuid) == 0 | strlen(CstudentRegister.Cstudentname) == 0 | strlen(CstudentRegister.Cstudentnumber) == 0){
					MessageBox(hWnd, TEXT("입력되지 않은 값들이 있습니다."), TEXT("오류"), MB_OK);
				}
				else if (strchr(CstudentRegister.Cstudentname, ' ') != NULL || strchr(CstudentRegister.Cstudentnumber, ' ') != NULL){///---------------------
					MessageBox(hWnd, TEXT("공백문자입력금지"), TEXT("금지"), MB_OK);
				}
				else if (StudentFile != NULL){
					MessageBox(hWnd, TEXT("성공!"), TEXT("저장"), MB_OK);
					fprintf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);

					SetWindowText(SREdit4, NULL);
					SetWindowText(SREdit5, NULL);
					fclose(StudentFile);

				}
				i = 0;
				
			}
			i = 0;
			fclose(StudentFile);
			break;

		case 452:
			GetWindowText(HEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentnumber, 100, TstudentRegister.Tstudentnumber, 100);

				SetWindowText(SREdit2, TstudentRegister.Tstudentname);
				SetWindowText(SREdit3, TstudentRegister.Tstudentnumber);
			}

			fclose(StudentFile);
			break;

		}
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == SRst1){
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == SRst2){
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return (LRESULT)hGoldBrush;
		}
		if ((HWND)lParam == SRst3 | (HWND)lParam == SRst4 | (HWND)lParam == SRst5 | (HWND)lParam == SRst7 | (HWND)lParam == SRst8){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		if ((HWND)lParam == SRst6){
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return (LRESULT)hGoldBrush;
		}
		if ((HWND)lParam == SRst9){
			SetTextColor((HDC)wParam, RGB(255, 165, 0));
			SetBkColor((HDC)wParam, RGB(226, 226, 226));
			return (LRESULT)hLightGrayBrush;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam){
			if (wParam == ' '){
				MessageBox(hWnd, TEXT("공백문자입력금지입니다."), TEXT("금지"), 0);
			}
			break;
		}

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));

}
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildBookRentalProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//도서 대출 화면  함수
	static HWND BRst1, BRst2, BRst3, BRst4, BRst5, BRst6, BRst7, BRst8, BRst9, BRst10, BRst11, BRst12, BRst13;
	static HWND BREdit2, BREdit3, BREdit4, BREdit5, BREdit6, BREdit8, BREdit9, BREdit10;
	HWND BRButton1, BRButton2, BRButton3, BRButton4, BRButton5, BRButton6, BRButton7, BRButton8;
	static TCHAR cpynull[10] = TEXT("없음  ");
	FILE* bookFile;
	int file_state;
	struct Tbook Tbook;
	struct Cbook Cbook;
	struct Cbook1 Cbook1;
	struct Tbook1 Tbook1;
	struct SCstudentRegister CstudentRegister;
	struct STstudentRegister TstudentRegister;
	struct SCstudentRegister1 CstudentRegister1;
	struct STstudentRegister1 TstudentRegister1;
	static TCHAR BOOKNAME[100];
	static TCHAR RENTALNAME[100];
	static TCHAR blank[15] = TEXT("  ");
	switch (iMessage){
	case WM_CREATE:
		BRst1 = CreateWindow(TEXT("static"), TEXT("도서 대출"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst2 = CreateWindow(TEXT("static"), TEXT("도서 대출"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 75, 350, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst3 = CreateWindow(TEXT("static"), TEXT("도서 반납"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			395, 75, 350, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst4 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			10, 150, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst5 = CreateWindow(TEXT("static"), TEXT("학생 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 190, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst6 = CreateWindow(TEXT("static"), TEXT("책 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 230, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst7 = CreateWindow(TEXT("static"), TEXT("출판사 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 270, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst8 = CreateWindow(TEXT("static"), TEXT("저자 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 310, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst9 = CreateWindow(TEXT("static"), TEXT("대출자 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 350, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);

		BRst10 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			395, 150, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst11 = CreateWindow(TEXT("static"), TEXT("학생이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			395, 190, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst12 = CreateWindow(TEXT("static"), TEXT("책 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			395, 230, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BRst13 = CreateWindow(TEXT("static"), TEXT("대출자 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			395, 270, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);



		BREdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 150, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BREdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 190, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BREdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			90, 230, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BREdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 270, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BREdit5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 310, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BREdit6 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 350, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);

		BREdit7 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			475, 150, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//uid
		BREdit8 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			475, 190, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//name
		BREdit9 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			475, 230, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//book name
		BREdit10 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			475, 270, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//rentalname

		BRButton1 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 150, 50, 23, hWnd, (HMENU)390, g_hlnst, NULL);
		BRButton2 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 230, 50, 23, hWnd, (HMENU)391, g_hlnst, NULL);
		BRButton3 = CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			245, 400, 50, 23, hWnd, (HMENU)392, g_hlnst, NULL);
		BRButton4 = CreateWindow(TEXT("button"), TEXT("대출"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 400, 50, 23, hWnd, (HMENU)393, g_hlnst, NULL);

		BRButton5 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			685, 150, 50, 23, hWnd, (HMENU)394, g_hlnst, NULL);//uid
		BRButton6 = CreateWindow(TEXT("button"), TEXT("찿기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			685, 230, 50, 23, hWnd, (HMENU)395, g_hlnst, NULL);//book name
		BRButton7 = CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			630, 350, 50, 23, hWnd, (HMENU)396, g_hlnst, NULL);//취소
		BRButton8 = CreateWindow(TEXT("button"), TEXT("반납"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			685, 350, 50, 23, hWnd, (HMENU)397, g_hlnst, NULL);//

		FontCreate(50, BRst1);
		FontCreate(40, BRst2);
		FontCreate(40, BRst3);
		return 0;
	case WM_COMMAND:
		switch ((LPARAM)wParam){
		case 390://학생정보
			GetWindowText(HEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				SetWindowText(BREdit2, TstudentRegister.Tstudentname);//학생이름
			}

			fclose(StudentFile);
			break;
		case 391://책찿기
			GetWindowText(BREdit3, Tbook1.Tbookname1, lstrlen(Tbook1.Tbookname1));
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "rb");//이진형식의 파일 열음

			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
						MessageBox(hWnd, TEXT("책이 등록되어있지 않습니다."), TEXT("오류"), MB_OK);
						break;
					}
				}
			}

			if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookpublisher, 100, Tbook.Tbookpublisher, 100);
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookauthor, 100, Tbook.Tbookauthor, 100);
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookrental, 100, Tbook.Tbookrental, 100);
				SetWindowText(BREdit4, Tbook.Tbookpublisher);
				SetWindowText(BREdit5, Tbook.Tbookauthor);
				SetWindowText(BREdit6, Tbook.Tbookrental);
			}

			fclose(bookFile);
			break;
		case 392://취소
			SetWindowText(BREdit2, NULL);
			SetWindowText(BREdit3, NULL);
			SetWindowText(BREdit4, NULL);
			SetWindowText(BREdit5, NULL);
			SetWindowText(BREdit6, NULL);
			break;
		case 393://대출 struct---------------------------------------------------------------------------------------
			//고쳐라
			GetWindowText(BREdit3, Tbook1.Tbookname1, 100);//책 이름
			GetWindowText(BREdit2, Tbook1.Tbookrental1, 100);//이름
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "r+");//이진 형식의 파일 열음
			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
						MessageBox(hWnd, TEXT("찾으신 책이 없습니다!"), TEXT("오류!"), MB_OK);
						break;
					}
				}
			}
			if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				if (strcmp(Cbook.Cbookrental, "없음") != 0){
					MessageBox(hWnd, TEXT("대출할 수 없습니다! 반납을 먼저 해주세요!"), TEXT("대출오류"), MB_OK);
				}
				else if (lstrcmp(Tbook1.Tbookrental1, TEXT("")) == 0){
					MessageBox(hWnd, TEXT("uid찾기버튼을 눌러주세요!"), TEXT("오류"), MB_OK);
				}
				else{
					int len = strlen(Cbook.Cbookrental);//---------------------------------
					fseek(bookFile, -len, SEEK_CUR);

					lstrcat(Tbook1.Tbookrental1, blank);//2칸채워놈
					WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookrental1, 100, Cbook.Cbookrental, 100, NULL, NULL);

					fputs(Cbook.Cbookrental, bookFile);
					fseek(bookFile, -2, SEEK_CUR);
					fputc('\n', bookFile);
					MessageBox(hWnd, TEXT("대출 성공!"), TEXT("대출"), MB_OK);

					SetWindowText(BREdit3, NULL);
					SetWindowText(BREdit4, NULL);
					SetWindowText(BREdit5, NULL);
					SetWindowText(BREdit6, NULL);
				}
			}
			fclose(bookFile);
			break;
		case 394: //uid학생정보 찾기버튼
			GetWindowText(BREdit7, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				SetWindowText(BREdit8, TstudentRegister.Tstudentname);//학생이름
			}

			fclose(StudentFile);
			break;
		case 395://책찾기
			GetWindowText(BREdit9, Tbook1.Tbookname1, lstrlen(Tbook1.Tbookname1));
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "rb");//이진형식의 파일 열음

			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
						MessageBox(hWnd, TEXT("책이 등록되어있지 않습니다."), TEXT("오류"), MB_OK);
						break;
					}
				}
			}

			if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookrental, 100, Tbook.Tbookrental, 100);
				SetWindowText(BREdit10, Tbook.Tbookrental);
			}

			fclose(bookFile);
			break;
		case 396://cancel
			SetWindowText(BREdit8, NULL);
			SetWindowText(BREdit9, NULL);
			SetWindowText(BREdit10, NULL);
			break;
		case 397://책반납-------------------------------------------------------------------------------
			GetWindowText(BREdit9, BOOKNAME, 100);//책 이름
			GetWindowText(BREdit8, RENTALNAME, 100);
			WideCharToMultiByte(CP_ACP, 0, BOOKNAME, 100, Cbook1.Cbookname1, 100, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, RENTALNAME, 100, Cbook1.Cbookrental1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "r+");//이진 형식의 파일 열음
			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
						MessageBox(hWnd, TEXT("찾으신 책이 없습니다!"), TEXT("오류!"), MB_OK);
						break;
					}
				}
			}
			if (strcmp(Cbook.Cbookrental, Cbook1.Cbookrental1) != 0){
				MessageBox(hWnd, TEXT("해당 책의 대출자가 아닙니다!!"), TEXT("반납오류"), MB_OK);
			}
			else if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				int len = strlen(Cbook.Cbookrental);//---------------------------------
				fseek(bookFile, -len, SEEK_CUR);
				GetWindowText(BREdit2, RENTALNAME, 100);//이름
				WideCharToMultiByte(CP_ACP, 0, cpynull, 10, Cbook.Cbookrental, 100, NULL, NULL);
				fputs(Cbook.Cbookrental, bookFile);
				fputc('\n', bookFile);
				MessageBox(hWnd, TEXT("반납 성공!"), TEXT("반납"), MB_OK);

				SetWindowText(BREdit8, NULL);
				SetWindowText(BREdit9, NULL);
				SetWindowText(BREdit10, NULL);

			}
			fclose(bookFile);
			break;
		}

	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == BRst1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == BRst2 | (HWND)lParam == BRst3){
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return(LRESULT)hGoldBrush;
		}
		if ((HWND)lParam == BRst4 | (HWND)lParam == BRst5 | (HWND)lParam == BRst6 | (HWND)lParam == BRst7 | (HWND)lParam == BRst8 | (HWND)lParam == BRst9 | (HWND)lParam == BRst10 | (HWND)lParam == BRst11 | (HWND)lParam == BRst12 | (HWND)lParam == BRst13){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return(LRESULT)hWhiteBrush;
		}
		break;
	}


	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildRentalCurrentStateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//대출 현황 화면  함수----------------------------------라스트작업
	//리스트박스이용 while문 파일끝까지 돌리고 if문자열같을때마다 sendmessage로 보내고 스레드하나더만들어서 다른숫자들어오면 리스트박스 초기화시켜버리자.8-20새벽 3:15
	static HWND RCst1, RCst2, RCst3, RCst4, RCst5, RCst6, RCst7;
	static HWND  RCEdit2, RCEdit3, RCEdit4, RCEdit5;
	static HWND RCListBox1;
	HWND RCButton1, RCButton2;
	static TCHAR cpynull[10] = TEXT("없음  ");
	FILE* bookFile;
	int file_state;
	struct Tbook Tbook;
	struct Cbook Cbook;
	struct Cbook1 Cbook1;
	struct Tbook1 Tbook1;
	struct SCstudentRegister CstudentRegister;
	struct STstudentRegister TstudentRegister;
	struct SCstudentRegister1 CstudentRegister1;
	struct STstudentRegister1 TstudentRegister1;
	static TCHAR BOOKNAME[100];
	static TCHAR RENTALNAME[100];
	static int i;
	switch (iMessage){
	case WM_CREATE:
		RCst1 = CreateWindow(TEXT("static"), TEXT("대출 현황"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst2 = CreateWindow(TEXT("static"), TEXT("태크 uid :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 130, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst3 = CreateWindow(TEXT("static"), TEXT("학생이름 :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 170, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst4 = CreateWindow(TEXT("static"), TEXT("책 이름 :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 210, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst5 = CreateWindow(TEXT("static"), TEXT("출판사 :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 250, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst6 = CreateWindow(TEXT("static"), TEXT("지은이 :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 290, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);
		RCst7 = CreateWindow(TEXT("static"), TEXT("조회 내역"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			330, 100, 80, 23, hWnd, (HMENU)0, g_hlnst, NULL);

		RCEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			90, 130, 120, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//uid
		RCEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			90, 170, 120, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//학생이름
		RCEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			90, 210, 120, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//책이름
		RCEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			90, 250, 120, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//출판사
		RCEdit5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			90, 290, 120, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//지은이

		RCButton1 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			220, 130, 50, 23, hWnd, (HMENU)190, g_hlnst, NULL);//uid
		RCButton2 = CreateWindow(TEXT("button"), TEXT("대출조회"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			220, 170, 70, 23, hWnd, (HMENU)191, g_hlnst, NULL);//와일문

		RCListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY|LBS_SORT,
			330, 130, 200, 250, hWnd, (HMENU)180, g_hlnst, NULL);

		FontCreate(50, RCst1);
		break;
	case WM_COMMAND:
		switch ((LPARAM)wParam){
		case 190://찾기학생정보에서
			GetWindowText(RCEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "rb");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				SetWindowText(RCEdit2, TstudentRegister.Tstudentname);//학생이름
			}

			fclose(StudentFile);
			break;
		case 191:
			//조회 버튼 와일문!!!!!!!!!!!!!!!!!!!!!!1-------------------------------------------------------------------짱중요
			DestroyWindow(RCListBox1);
			RCListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
				330, 130, 200, 250, hWnd, (HMENU)180, g_hlnst, NULL);
			GetWindowText(RCEdit2, Tbook1.Tbookrental1, 100);
			//MultiByteToWideChar(CP_ACP, 0, Cbook1.Cbookrental1, 15, Tbook1.Tbookrental1, 15);
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookrental1, 100, Cbook1.Cbookrental1, 100, NULL, NULL);//학생이름 이름 조사
			bookFile = fopen("data1.txt", "r");//이진형식의 파일 열음

			while (!feof(bookFile)){//파일끝날때까지
				fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (strcmp(Cbook1.Cbookrental1, Cbook.Cbookrental) == 0){
					MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookname, 100, Tbook.Tbookname, 100);//멀티바이트>유니코드
					SendMessage(RCListBox1, LB_ADDSTRING, 0, (LPARAM)Tbook.Tbookname);
				}
			}
			fclose(bookFile);
			break;
		}
		switch (LOWORD(wParam)) {
		case 180://조회
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				//if (SendMessage(RCListBox1,LB_GETCOUNT)
				i = SendMessage(RCListBox1, LB_GETCURSEL, 0, 0);
				SendMessage(RCListBox1, LB_GETTEXT, i, (LPARAM)Tbook.Tbookname);//책이름 바듬

				WideCharToMultiByte(CP_ACP, 0, Tbook.Tbookname, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
				bookFile = fopen("data1.txt", "rb");//이진형식의 파일 열음

				while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
					fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
					if (feof(bookFile)){
						break;
					}
				}

				if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
					MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookname, 100, Tbook.Tbookname, 100);
					MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookpublisher, 100, Tbook.Tbookpublisher, 100);
					MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookauthor, 100, Tbook.Tbookauthor, 100);
					SetWindowText(RCEdit3, Tbook.Tbookname);
					SetWindowText(RCEdit4, Tbook.Tbookpublisher);
					SetWindowText(RCEdit5, Tbook.Tbookauthor);
				}

				fclose(bookFile);
				break;

			}
			/*while (strcmp(Cbook1.Cbookrental1, Cbook.Cbookrental) != 0){
				fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
				if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				MessageBox(hWnd, TEXT("책이 등록되어있지 않습니다."), TEXT("오류"), MB_OK);
				break;
				}
				}
				}
				case WM_COMMAND:
				switch (LOWORD(wParam)) {
				case ID_LISTBOX:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
				i=SendMessage(hList, LB_GETCURSEL,0,0);
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);
				break;
				if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookrental, 15, Tbook.Tbookrental, 15);
				SetWindowText(BREdit10, Tbook.Tbookrental);
				}*/

			break;
		}


		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == RCst1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == RCst2 | (HWND)lParam == RCst3 | (HWND)lParam == RCst4 | (HWND)lParam == RCst5 | (HWND)lParam == RCst6 | (HWND)lParam == RCst7){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//----------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildBookManagementProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//책 관리 화면  함수 파일 입출력
	static HWND BMEdit1, BMEdit2, BMEdit3, BMEdit4, BMEdit5, BMEdit6, BMEdit7;
	static HWND BMst1, BMst2, BMst3, BMst4, BMst5, BMst6, BMst7, BMst8, BMst9, BMst10, BMst11;
	static HWND BMListBox1;
	TCHAR cpynull[10] = TEXT("없음  ");
	FILE* bookFile;
	int file_state;
	struct Tbook Tbook;
	struct Cbook Cbook;
	struct Cbook1 Cbook1;
	struct Tbook1 Tbook1;
	static CHAR Csumstr[400];
	static TCHAR tsumstr[400];
	static CHAR blinks[2] = " ";
	static int count = 0;
	static TCHAR Tcount[5] = TEXT("");
	static CHAR Ccount[5] = "";
	static int ic = 0;
	switch (iMessage){
	case WM_CREATE:
		BMst1 = CreateWindow(TEXT("static"), TEXT("책 관리"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst2 = CreateWindow(TEXT("static"), TEXT("책 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			5, 140, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst3 = CreateWindow(TEXT("static"), TEXT("출판사 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			5, 170, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst4 = CreateWindow(TEXT("static"), TEXT("저자 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			5, 200, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst5 = CreateWindow(TEXT("static"), TEXT("책 등록"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 75, 370, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst6 = CreateWindow(TEXT("static"), TEXT("책 제거"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			375, 75, 370, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst7 = CreateWindow(TEXT("static"), TEXT("책 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			395, 140, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst8 = CreateWindow(TEXT("static"), TEXT("출판사 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			395, 170, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst9 = CreateWindow(TEXT("static"), TEXT("저자 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			395, 200, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst10 = CreateWindow(TEXT("static"), TEXT("책 등록 현황"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			5, 290, 100, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		BMst11 = CreateWindow(TEXT("static"), TEXT("등록된 책 수"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			5, 350, 100, 23, hWnd, (HMENU)-1, g_hlnst, NULL);

		FontCreate(50, BMst1);
		FontCreate(40, BMst5);
		FontCreate(40, BMst6);

		CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 245, 250, 50, 20, hWnd, (HMENU)BMbutton1, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 300, 250, 50, 20, hWnd, (HMENU)BMbutton, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 675, 140, 50, 20, hWnd, (HMENU)BMbutton2, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("제거"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 675, 250, 50, 20, hWnd, (HMENU)BMbutton3, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 620, 250, 50, 20, hWnd, (HMENU)BMbutton4, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("조회"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 55, 320, 50, 23, hWnd, (HMENU)222, g_hlnst, NULL);//조회 222

		BMEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			80, 140, 200, 23, hWnd, (HMENU)501, g_hlnst, NULL);
		BMEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			80, 170, 200, 23, hWnd, (HMENU)502, g_hlnst, NULL);
		BMEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			80, 200, 200, 23, hWnd, (HMENU)503, g_hlnst, NULL);
		BMEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			470, 140, 200, 23, hWnd, (HMENU)504, g_hlnst, NULL);
		BMEdit5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			470, 170, 200, 23, hWnd, (HMENU)505, g_hlnst, NULL);
		BMEdit6 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			470, 200, 200, 23, hWnd, (HMENU)506, g_hlnst, NULL);
		BMEdit7 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | SS_RIGHT,
			55, 380, 50, 23, hWnd, (HMENU)506, g_hlnst, NULL);
		BMListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY|LBS_SORT|WS_VSCROLL,
			115, 290, 600, 200, hWnd, (HMENU)140, g_hlnst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case BMbutton://저장

			if (ic == 0){
				GetWindowText(BMEdit1, Tbook1.Tbookname1, 100);
				WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);
				bookFile = fopen("data1.txt", "rb");//이진형식의 파일 열음

				while (!feof(bookFile)){
					fscanf(bookFile, "%s /%s /%s /%s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
						MessageBox(hWnd, TEXT("이미 책정보가 등록되어 있습니다."), TEXT("학생등록오류"), MB_OK);
						fclose(bookFile);
						ic = 1;
						break;
					}
				}
				fclose(bookFile);
			}

			if (ic != 1){
				GetWindowText(BMEdit1, Tbook.Tbookname, lstrlen(Tbook.Tbookname));
				GetWindowText(BMEdit2, Tbook.Tbookpublisher, lstrlen(Tbook.Tbookpublisher));
				GetWindowText(BMEdit3, Tbook.Tbookauthor, lstrlen(Tbook.Tbookauthor));
				//GetWindowText(HEdit1, Tbook.Tbookrental, 15);
				lstrcpy(Tbook.Tbookrental, cpynull);

				WideCharToMultiByte(CP_ACP, 0, Tbook.Tbookname, 100, Cbook.Cbookname, 100, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, Tbook.Tbookpublisher, 100, Cbook.Cbookpublisher, 100, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, Tbook.Tbookauthor, 100, Cbook.Cbookauthor, 100, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, Tbook.Tbookrental, 100, Cbook.Cbookrental, 100, NULL, NULL);

				bookFile = fopen("data1.txt", "a+");
				if (strlen(Cbook.Cbookname) == 0 | strlen(Cbook.Cbookpublisher) == 0 | strlen(Cbook.Cbookauthor) == 0){
					MessageBox(hWnd, TEXT("입력되지 않은 값들이 있습니다."), TEXT("오류"), MB_OK);
				}
				else if (strchr(Cbook.Cbookname, ' ') != NULL | strchr(Cbook.Cbookpublisher, ' ') != NULL | strchr(Cbook.Cbookauthor, ' ') != NULL){///---------------------
					MessageBox(hWnd, TEXT("공백문자입력금지"), TEXT("금지"), MB_OK);
				}
				else if (bookFile != NULL){
					MessageBox(hWnd, TEXT("성공!"), TEXT("저장"), MB_OK);
					fprintf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
					//fgets(str2, strlen(str2), stdin);//-------------------------------------------------------------
					//str[strlen(str2) - 1] = '\0';

					SetWindowText(BMEdit1, NULL);
					SetWindowText(BMEdit2, NULL);
					SetWindowText(BMEdit3, NULL);
				}//-----------------
				ic = 0;
				fclose(bookFile);
			}
			ic = 0;
			break;
		case BMbutton1://취소버튼

			SetWindowText(BMEdit1, NULL);
			SetWindowText(BMEdit2, NULL);
			SetWindowText(BMEdit3, NULL);
			break;

		case BMbutton2://찾기

			GetWindowText(BMEdit4, Tbook1.Tbookname1, lstrlen(Tbook1.Tbookname1));
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "r");//이진형식의 파일 열음

			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
						MessageBox(hWnd, TEXT("책이 등록되어있지 않습니다."), TEXT("오류"), MB_OK);
						break;
					}
				}
			}

			if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookpublisher, 100, Tbook.Tbookpublisher, 100);
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookauthor, 100, Tbook.Tbookauthor, 100);
				MultiByteToWideChar(CP_ACP, 0, Cbook.Cbookrental, 100, Tbook.Tbookrental, 100);
				SetWindowText(BMEdit5, Tbook.Tbookpublisher);
				SetWindowText(BMEdit6, Tbook.Tbookauthor);
				//	SetWindowText(BMEdit7, Tbook.Tbookrental);
			}

			fclose(bookFile);
			break;
		case BMbutton4://취소

			SetWindowText(BMEdit4, NULL);
			SetWindowText(BMEdit5, NULL);
			SetWindowText(BMEdit6, NULL);
			break;
		case BMbutton3://제거버튼
			GetWindowText(BMEdit4, Tbook1.Tbookname1, lstrlen(Tbook1.Tbookname1));
			WideCharToMultiByte(CP_ACP, 0, Tbook1.Tbookname1, 100, Cbook1.Cbookname1, 100, NULL, NULL);//책 이름 조사
			bookFile = fopen("data1.txt", "r+");//이진형식의 파일 열음-------------------------------------------------------------------------------------------------------------------------fix
			while (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) != 0){
				fscanf(bookFile, "%s /%s /%s /%s\n", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (feof(bookFile)){
					break;
				}
			}
			if (strcmp(Cbook1.Cbookname1, Cbook.Cbookname) == 0){
				int len = 0;
				int i;
				len = strlen(Cbook.Cbookname) + strlen(Cbook.Cbookauthor) + strlen(Cbook.Cbookpublisher) + strlen(Cbook.Cbookrental) + 10;
				fseek(bookFile, -len, SEEK_CUR);
				for (i = 2; i < len; i++){//-----------------------------------------------------오류고침!!!!!!!!!!!!!!!!!!!! \n이 바이트2
					fputc(NULL, bookFile);
				}
				fputc('\n', bookFile);
				//fputc('\n', bookFile);
				MessageBox(hWnd, TEXT("제거 성공!"), TEXT("제거"), MB_OK);

				SetWindowText(BMEdit4, NULL);
				SetWindowText(BMEdit5, NULL);
				SetWindowText(BMEdit6, NULL);
			}
			fclose(bookFile);

			break;
		case 222://책등록현황
			DestroyWindow(BMListBox1);
			BMListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | LBS_SORT | WS_VSCROLL,
				115, 290, 600, 200, hWnd, (HMENU)140, g_hlnst, NULL);
			bookFile = fopen("data1.txt", "r");//이진형식의 파일 열음

			while (!feof(bookFile)){//파일끝날때까지

				fscanf(bookFile, "%s /%s /%s /%s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor, &Cbook.Cbookrental);
				if (!(strchr(Cbook.Cbookname, ' ') != NULL | strchr(Cbook.Cbookpublisher, ' ') != NULL)){//--------------------------------------------------------------------------
					char Csumstr[400] = "";
					sprintf(Csumstr, "책이름 : %s /출판사 : %s /저자 : %s", &Cbook.Cbookname, &Cbook.Cbookpublisher, &Cbook.Cbookauthor);

					MultiByteToWideChar(CP_ACP, 0, Csumstr, 400, tsumstr, 100);
					//문자열 합치기
					SendMessage(BMListBox1, LB_ADDSTRING, 0, (LPARAM)tsumstr);
					count++;
					strcpy(Cbook.Cbookname, blinks);//---------------------------------
					strcpy(Cbook.Cbookpublisher, blinks);
					strcpy(Cbook.Cbookauthor, blinks);
				}

			}
			sprintf(Ccount, "%d", count);
			MultiByteToWideChar(CP_ACP, 0, Ccount, 4, Tcount, 4);
			fclose(bookFile);
			SetWindowText(BMEdit7, Tcount);
			count = 0;
			break;

		}


		return 0;
	case WM_CTLCOLORSTATIC: // 스태틱의 백설정
		if ((HWND)lParam == BMst1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == BMst2 | (HWND)lParam == BMst3 | (HWND)lParam == BMst4 | (HWND)lParam == BMst7 | (HWND)lParam == BMst8 | (HWND)lParam == BMst9 | (HWND)lParam == BMst10 | (HWND)lParam == BMst11){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		if ((HWND)lParam == BMst5){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == BMst6){
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return (LRESULT)hGoldBrush;
		}
		break;
	}


	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildBookStatsProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//대출 통계 화면  함수
	static HWND BSst1;
	switch (iMessage){
	case WM_CREATE:
		BSst1 = CreateWindow(TEXT("static"), TEXT("대출 통계"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)0, g_hlnst, NULL);
		FontCreate(50, BSst1);
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == BSst1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		break;

	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildStudentManagementProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//학생 관리 화면  함수
	static HWND SMst1, SMst2, SMst3, SMst4, SMst5;
	static HWND SMEdit2, SMEdit3;
	static HWND SMButton1;
	struct SCstudentRegister CstudentRegister;
	struct STstudentRegister TstudentRegister;
	struct SCstudentRegister1 CstudentRegister1;
	struct STstudentRegister1 TstudentRegister1;
	switch (iMessage){
	case WM_CREATE:
		SMst1 = CreateWindow(TEXT("static"), TEXT("학생 관리"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)0, g_hlnst, NULL);
		SMst2 = CreateWindow(TEXT("static"), TEXT("학생 정보 삭제"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 80, 360, 40, hWnd, (HMENU)0, g_hlnst, NULL);
		SMst3 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 150, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SMst4 = CreateWindow(TEXT("static"), TEXT("학생 이름 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 190, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SMst5 = CreateWindow(TEXT("static"), TEXT("학번 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			0, 230, 80, 23, hWnd, (HMENU)-1, g_hlnst, NULL);

		SMEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 150, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//uid
		SMEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 190, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//name
		SMEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
			90, 230, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);//number

		SMButton1 = CreateWindow(TEXT("button"), TEXT("찾기"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 150, 50, 23, hWnd, (HMENU)270, g_hlnst, NULL);
		SMButton1 = CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			245, 290, 50, 23, hWnd, (HMENU)271, g_hlnst, NULL);
		SMButton1 = CreateWindow(TEXT("button"), TEXT("제거"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 290, 50, 23, hWnd, (HMENU)272, g_hlnst, NULL);
		FontCreate(50, SMst1);
		FontCreate(40, SMst2);
		break;
	case WM_COMMAND:
		switch ((LPARAM)wParam){
		case 270://찾기
			GetWindowText(SMEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);
			StudentFile = fopen("student.txt", "r");//이진형식의 파일 열음

			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
						MessageBox(hWnd, TEXT("학생정보가 등록되어있지 않습니다."), TEXT("학생찾기오류"), MB_OK);
						fclose(StudentFile);
						break;
					}
				}
			}

			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentname, 100, TstudentRegister.Tstudentname, 100);
				MultiByteToWideChar(CP_ACP, 0, CstudentRegister.Cstudentnumber, 100, TstudentRegister.Tstudentnumber, 100);
				SetWindowText(SMEdit2, TstudentRegister.Tstudentname);//학생이름
				SetWindowText(SMEdit3, TstudentRegister.Tstudentnumber);//학번
			}

			fclose(StudentFile);
			break;
		case 271://취소
			SetWindowText(SMEdit2, NULL);
			SetWindowText(SMEdit3, NULL);
			break;
		case 272://제거----------------------------------------------------------------------------------------------
			GetWindowText(SMEdit1, TstudentRegister1.Tstudentuid1, 100);
			WideCharToMultiByte(CP_ACP, 0, TstudentRegister1.Tstudentuid1, 100, CstudentRegister1.Cstudentuid1, 100, NULL, NULL);//책 이름 조사
			StudentFile = fopen("student.txt", "r+");//이진형식의 파일 열음-------------------------------------------------------------------------------------------------------------------------fix
			while (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0){
				fscanf(StudentFile, "%s /%s /%s\n", &CstudentRegister.Cstudentuid, &CstudentRegister.Cstudentname, &CstudentRegister.Cstudentnumber);
				if (feof(StudentFile)){
					if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) != 0)
						MessageBox(hWnd, TEXT("제거할 학생uid정보가 없습니다."), TEXT("오류"), NULL);
					break;
				}
			}
			if (strcmp(CstudentRegister1.Cstudentuid1, CstudentRegister.Cstudentuid) == 0){
				int len = 0;
				int i;
				len = strlen(CstudentRegister.Cstudentuid) + strlen(CstudentRegister.Cstudentname) + strlen(CstudentRegister.Cstudentnumber) + 6;
				fseek(StudentFile, -len, SEEK_CUR);
				for (i = 2; i < len; i++){//-----------------------------------------------------오류고침!!!!!!!!!!!!!!!!!!!! \n이 바이트2
					fputc(NULL, StudentFile);
				}
				fputc('\n', StudentFile);
				MessageBox(hWnd, TEXT("제거 성공!"), TEXT("제거"), MB_OK);

				SetWindowText(SMEdit2, NULL);
				SetWindowText(SMEdit3, NULL);
			}
			fclose(StudentFile);
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == SMst1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == SMst2){
			SetBkColor((HDC)wParam, RGB(255, 215, 0));
			return (LRESULT)hGoldBrush;
		}
		if ((HWND)lParam == SMst3 | (HWND)lParam == SMst4 | (HWND)lParam == SMst5){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK ChildSetupProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//설정 화면  함수 시리얼 통신 부분
	static HWND SSt1, SSt2, SSt3;
	static HWND SCb1;
	DWORD ThreadID;
	HANDLE SThread;
	TCHAR *port[2] = { TEXT("COM3"), TEXT("COM4") };
	TCHAR CPYport[5];
	switch (iMessage){
	case WM_CREATE:
		SSt1 = CreateWindow(TEXT("static"), TEXT("설정"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
			0, 0, 745, 70, hWnd, (HMENU)-1, g_hlnst, NULL);
		SSt2 = CreateWindow(TEXT("static"), TEXT("태그 UID :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 200, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SSt3 = CreateWindow(TEXT("static"), TEXT("포트 설정 :"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
			10, 250, 70, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		FontCreate(50, SSt1);

		SThread = CreateThread(NULL, 0, ThreadSerial, NULL, 0, &ThreadID);
		CloseHandle(SThread);

		SEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
			100, 200, 200, 23, hWnd, (HMENU)-1, g_hlnst, NULL);
		SCb1 = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
			100, 250, 70, 50, hWnd, (HMENU)-1, g_hlnst, NULL);
		CreateWindow(TEXT("button"), TEXT("설정"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			180, 250, 50, 23, hWnd, (HMENU)565, g_hlnst, NULL);
		for (int i = 0; i < 2; i++){
			SendMessage(SCb1, CB_ADDSTRING, 0, (LPARAM)port[i]);
		}
		if (SP->IsConnected() == 0){
			MessageBox(hWnd, TEXT("포트설정오류 디폴트값은 COM3입니다. 아두이노를 연결해주세요!."), TEXT("오류COM3"), NULL);
		}
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == SSt1){
			SetBkColor((HDC)wParam, RGB(255, 165, 0));
			return (LRESULT)hOrangeBrush;
		}
		if ((HWND)lParam == SSt2 | (HWND)lParam == SSt3){
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hWhiteBrush;
		}
		break;
	case CBN_CLOSEUP: //드롭다운 리스트 닫힐때 메세지

		GetWindowText(SCb1, CPYport, 5);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case SBotton1:
			int CPY;
			if (lstrcmp(CPYport, TEXT("COM3")) == 0){
				CPY = 1;
			}
			else if (lstrcmp(CPYport, TEXT("COM4")) == 0){
				CPY = 2;
			}
			if (CPY == 1){
				SP = new Serial(TEXT("\\\\.\\COM3"));
				if (SP->IsConnected() == 0){
					MessageBox(hWnd, TEXT("포트설정오류"), TEXT("오류COM3"), NULL);
				}
				else{
					MessageBox(hWnd, TEXT("포트설정성공"), TEXT("성공COM3"), NULL);
				}
			}
			else if (CPY == 2){
				SP = new Serial(TEXT("\\\\.\\COM4"));
				if (SP->IsConnected() == 0){
					MessageBox(hWnd, TEXT("포트설정오류"), TEXT("오류COM4"), NULL);
				}
				else{
					MessageBox(hWnd, TEXT("포트설정성공"), TEXT("성공COM4"), NULL);
				}
			}
		}

	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WPStartpage2Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){//시작화면을위한 바탕깔기
	HWND WPbutton1, WPbutton2, WPbutton3, WPbutton4, WPbutton5, WPbutton6, WPbutton7, WPbutton8;

	switch (iMessage){
	case WM_CREATE:
		HomeButton = CreateWindow(TEXT("ChildHomeButton"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)10, g_hlnst, NULL); ShowWindow(HomeButton, SW_SHOW);

		StudentRegister = CreateWindow(TEXT("ChildStudentRegister"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)11, g_hlnst, NULL); ShowWindow(StudentRegister, SW_HIDE);

		BookRental = CreateWindow(TEXT("ChildBookRental"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)12, g_hlnst, NULL); ShowWindow(BookRental, SW_HIDE);

		RentalCurrentState = CreateWindow(TEXT("ChildRentalCurrentState"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)13, g_hlnst, NULL); ShowWindow(RentalCurrentState, SW_HIDE);

		BookManagement = CreateWindow(TEXT("ChildBookManagement"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)14, g_hlnst, NULL); ShowWindow(BookManagement, SW_HIDE);

		BookStats = CreateWindow(TEXT("ChildBookStats"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)15, g_hlnst, NULL); ShowWindow(BookStats, SW_HIDE);

		StudentManagement = CreateWindow(TEXT("ChildStudentManagement"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)16, g_hlnst, NULL); ShowWindow(StudentManagement, SW_HIDE);

		Setup = CreateWindow(TEXT("ChildSetup"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			255, 5, 745, 495, hWnd, (HMENU)17, g_hlnst, NULL); ShowWindow(Setup, SW_HIDE);


		//버튼생성 부분 1~8 ID
		WPbutton1 = CreateWindow(TEXT("button"), TEXT("홈"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 5, 5, 120, 120, hWnd, (HMENU)1, g_hlnst, NULL);
		WPbutton2 = CreateWindow(TEXT("button"), TEXT("도서 대출"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 5, 130, 120, 120, hWnd, (HMENU)3, g_hlnst, NULL);
		WPbutton3 = CreateWindow(TEXT("button"), TEXT("책 관리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 5, 255, 120, 120, hWnd, (HMENU)5, g_hlnst, NULL);
		WPbutton4 = CreateWindow(TEXT("button"), TEXT("학생 관리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON // 4
			, 5, 380, 120, 120, hWnd, (HMENU)7, g_hlnst, NULL);
		WPbutton5 = CreateWindow(TEXT("button"), TEXT("학생 등록"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 130, 5, 120, 120, hWnd, (HMENU)2, g_hlnst, NULL);
		WPbutton6 = CreateWindow(TEXT("button"), TEXT("대출 현황"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 130, 130, 120, 120, hWnd, (HMENU)4, g_hlnst, NULL);
		WPbutton7 = CreateWindow(TEXT("button"), TEXT("대출 통계"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 130, 255, 120, 120, hWnd, (HMENU)6, g_hlnst, NULL);
		WPbutton8 = CreateWindow(TEXT("button"), TEXT("설정"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 130, 380, 120, 120, hWnd, (HMENU)8, g_hlnst, NULL);

		FontCreate(25, WPbutton1);
		FontCreate(25, WPbutton2);
		FontCreate(25, WPbutton3);
		FontCreate(25, WPbutton4);
		FontCreate(25, WPbutton5);
		FontCreate(25, WPbutton6);
		FontCreate(25, WPbutton7);
		FontCreate(25, WPbutton8);
		break;
	case WM_COMMAND: //버튼누르면 화면띄울 부분
		switch (LOWORD(wParam)){
		case 1:
			Sellectwindow(HomeButton);
			break;
		case 2:
			Sellectwindow(StudentRegister);
			break;
		case 3:
			Sellectwindow(BookRental);
			break;
		case 4:
			Sellectwindow(RentalCurrentState);
			break;
		case 5:
			Sellectwindow(BookManagement);
			break;
		case 6:
			Sellectwindow(BookStats);
			break;
		case 7:
			Sellectwindow(StudentManagement);
			break;
		case 8:
			Sellectwindow(Setup);
			break;
		}
		return 0;
	case WM_CHAR:
		CHAR getblinks[2] = "";
		getblinks[2] = (TCHAR)wParam;
		if (strcmp(getblinks, blinks) == 0){
			MessageBox(hWnd, TEXT("공백문자입력금지!"), TEXT("위험!!"), 0);
		}
		break;

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}