//�C���N���[�h
#include <Windows.h>
#include<stdlib.h>
#include"DirectX_11/Direct3D.h"
#include"DirectX_11/Direct2D.h"
#include"Engine/Camera.h"
#include"Engine/Quad.h"
#include"Engine/Sprite.h"
#include"Engine/Transform.h"
#include"Engine/ImageManager.h"
#include"DebugUI.h"
#include"Engine/Input.h"
#include"Engine/RootJob.h"
#include"ImGui/imgui.h"
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include"Engine/Audio.h"
#include"Text.h"


#pragma comment(lib, "winmm.lib")


//�萔�錾
LPCWSTR WIN_CLASS_NAME = L"SampleGame";
LPCWSTR WIN_TITLE_NAME = L"�T���v���Q�[��";


const int WINDOW_WIDTH = 1920;   //�E�B���h�E��
const int WINDOW_HEIGHT = 1080;	 //�E�B���h�E����
int pxelUnit;
//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

RootJob* pRootJob;

//�G���g���[�|�C���g
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�N���X(�݌v)�쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL;                     //���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i�i���j
	RegisterClassEx(&wc);  //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����
	//�E�B���h�E�쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		WIN_TITLE_NAME,     //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		winW,                 //�E�B���h�E��
		winH,                 //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);


	//�E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);

	CoInitialize(nullptr);

	if (FAILED(Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(D2D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd)))
	{
		PostQuitMessage(0);
	}


	DebugUI::Initialize(hWnd, Direct3D::GetDevice(), Direct3D::GetContext());
	Input::Initialize(hWnd);
	Camera::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Audio::Initialize();

	pRootJob = new RootJob;
	pRootJob->Initialize();
	
	//���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ�
		else
		{
			//FPS����
			static DWORD countFps = 0;					 //��ʍX�V��
			static DWORD startTime = timeGetTime();		 //�N�������擾(�~���b)							//�~���b��10^-3
			DWORD nowTime = timeGetTime();				 //���̎����擾(�~���b)							//1000�~���b��1�b
			static DWORD lastUpdateTime = nowTime;		 //�Ō�ɍX�V��������(�~���b) : Update
			static DWORD lastFixedUpdateTime = nowTime;  //�Ō�ɍX�V��������(�~���b) : FixedUpdate

			//���̎����ƋN�������̍���1000�~���b��������
			//countFps��startTime�����Z�b�g
			if (nowTime - startTime >= 1000)
			{
				WCHAR str[16];
				wsprintf(str, L"%u", countFps);
				SetWindowText(hWnd, str);
				countFps = 0;
				startTime = nowTime;//
			}

			//���̎����ƍŌ�ɍX�V���������̍�*60��1000�ȏ�ł����
			if ((nowTime - lastUpdateTime) * 60.0f >= 1000.0f)
			{
#if false
				DebugUI::StartImGui();
#endif
				//�Q�[���̏���
				Input::Update();
				Camera::Update();

				lastUpdateTime = nowTime;
				countFps++;
				pRootJob->UpdateSub();
#if false
				DebugUI::Debug(pRootJob->FindChild("SceneManager"));
				DebugUI::Log();
				ImGui::Render();
#endif 
				//�`�揈��
				Direct3D::BeginDraw();
				pRootJob->DrawSub();
				pRootJob->SecondDrawSub();

				ImageManager::Draw();
				D2D::BeginDraw();
				Text t;
				TEXT_RECT re = { 0,0,100,100 };
				t.Load("YO!", "Gabliora", re, RIGHT_TOP);
				t.Draw();
				D2D::EndDraw();
#if false		
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

				Direct3D::EndDraw();
			}
		}
	}

	Audio::Releace();
	DebugUI::CleanUp();
	ImageManager::AllRelease();
	pRootJob->ReleaseSub();
	Input::Release();
	D2D::Release();
	Direct3D::Release();

	CoUninitialize();
	return 0;
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//�E�B���h�E�v���V�[�W��(�Ȃ񂩂�������Ă΂��֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam))
	{
		return true;
	}

	switch (msg)
	{

	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

