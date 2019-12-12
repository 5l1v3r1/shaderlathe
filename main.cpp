// dear imgui: standalone example application for SDL2 + OpenGL
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_impl_win32.h"
#include "3rdparty/imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#define GL3W_IMPLEMENTATION
#include "3rdparty/gl3w.h"    // Initialize with gl3wInit()
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Main code
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	LPCWSTR szName = L"Shader Lathe";
	WNDCLASSEXW wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, szName, NULL };
	::RegisterClassExW(&wc);
	DWORD dwExStyle = WS_EX_APPWINDOW;// | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
	RECT rect;
	int windowWidth, windowHeight, windowLeft, windowTop;
	SetRect(&rect, 0, 0, 1280, 720);
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	windowLeft = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	windowTop = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;
	HWND hwnd = CreateWindowExW(0, szName, szName, dwStyle, windowLeft, windowTop, windowWidth, windowHeight, 0, 0, 0, 0);

   

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

	PIXELFORMATDESCRIPTOR pfd;
	HDC hDC;
	HGLRC hRC;
	int pixelFormat;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	hDC = GetDC(hwnd);
	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);


	typedef HGLRC(APIENTRY* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
	static PFNWGLCREATECONTEXTATTRIBSARBPROC pfnCreateContextAttribsARB = 0;
	GLint attribs[] =
	{
		// Here we ask for OpenGL 4.5
		0x2091, 4,
		0x2092, 3,
		// Uncomment this for forward compatibility mode
		//0x2094, 0x0002,
		// Uncomment this for Compatibility profile
		// 0x9126, 0x9126,
		// We are using Core profile here
		0x9126, 0x00000001,
		0
	};
	pfnCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
	HGLRC newRC = pfnCreateContextAttribsARB(hDC, 0, attribs);
	wglMakeCurrent(0, 0);
	wglDeleteContext(hRC);
	hRC = newRC;
	wglMakeCurrent(hDC, hRC);

	bool err = gl3w_init() != 0;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

    // Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init(NULL);

	int PezInitialize(int width, int height);
	PezInitialize(1280,720);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);




	MSG msg;
	ZeroMemory(&msg, sizeof(msg));


	DWORD previousTime = GetTickCount();
	while (msg.message != WM_QUIT)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		DWORD currentTime = timeGetTime();
		DWORD deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		
		void PezRender();
			PezRender();
			

		void PezUpdate(unsigned int elapsedMilliseconds);
		PezUpdate(deltaTime);

        // Start the Dear ImGui frame
       


		

        // 3. Show another simple window.
        

        // Rendering
			ImGui::Render();
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
			//glClearColor(0, 0,0,1);
			//glClear(GL_COLOR_BUFFER_BIT);
		   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SwapBuffers(hDC);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	ExitProcess(0);
    return 0;
}


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		break;
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}