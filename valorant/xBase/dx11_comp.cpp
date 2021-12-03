#include "stdafx.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace dxcomp {
#define assertx(x) if ( x != S_OK ){ printf("[-] d3d error\n"); return S_FALSE; }
#define RET_CHK2(x) if ( x != S_OK ) { core::set_thread( globals::target_proc::target_wnd_hwnd, remote_thread ); return; }
	HWND create_window() {
		std::string class_name{ "helloman" };
		std::string wnd_name{ "nelloham" };

		WNDCLASSA window_class
		{
			0,
			// TOTO -> proper wnd_proc
			[](const HWND window, const UINT message, const WPARAM wparam, const LPARAM lparam) -> LRESULT
			{
				if (ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam))
					return true;

				return ::DefWindowProcA(window, message, wparam, lparam);
			},
			0,
			0,
			::GetModuleHandleW(nullptr),
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			class_name.c_str()
		};

		::RegisterClassA(&window_class);
		return ::CreateWindowExA(0, class_name.c_str(), wnd_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, window_class.hInstance, nullptr);
	}

	HRESULT d3d_create_render_target() {
		DXGI_SWAP_CHAIN_DESC sd {};
		assertx(_dxgi_swapchain->GetDesc(&sd));

		ID3D11Texture2D* _back_buffer = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc{};
		::ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
		render_target_view_desc.Format = sd.BufferDesc.Format;
		render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		assertx(_dxgi_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_back_buffer)));
		assertx(_d3d_device->CreateRenderTargetView(_back_buffer, &render_target_view_desc, &_d3d_render_target_view));

		_d3d_device_context->OMSetRenderTargets(1, &_d3d_render_target_view, nullptr);
		_back_buffer->Release();

		return S_OK;
	}

	HRESULT d3d_create_device() {
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		assertx(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &_d3d_device, &_d3d_featureLevel, &_d3d_device_context));
		assertx(_d3d_device->QueryInterface(IID_PPV_ARGS(&_dxgi_device)));
		assertx(CreateDXGIFactory2(0, __uuidof(_dxgi_factory), reinterpret_cast<void**>(&_dxgi_factory)));

		DXGI_SWAP_CHAIN_DESC1 description = {};
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.BufferUsage =  DXGI_USAGE_RENDER_TARGET_OUTPUT;
		description.BufferCount = 2;
		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		description.SampleDesc.Count = 1;
		description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

		description.Width = ::GetSystemMetrics(SM_CXSCREEN);
		description.Height = ::GetSystemMetrics(SM_CYSCREEN);

		assertx(_dxgi_factory->CreateSwapChainForComposition(_d3d_device, &description, nullptr, &_dxgi_swapchain));
		assertx(d3d_create_render_target());

		return S_OK;
	}

	void imgui_styling() {
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		style->FramePadding = ImVec2(4, 2);
		style->ItemSpacing = ImVec2(10, 2);
		style->IndentSpacing = 12;
		style->ScrollbarSize = 10;

		style->WindowRounding = 4;
		style->FrameRounding = 4;
		style->PopupRounding = 4;
		style->ScrollbarRounding = 6;
		style->GrabRounding = 4;
		style->TabRounding = 4;

		style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
		style->WindowMenuButtonPosition = ImGuiDir_Right;

		style->DisplaySafeAreaPadding = ImVec2(4, 4);
	}

	void init_rendering() {
		_handle = create_window();

		d3d_create_device();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		io.DisplaySize.x = ::GetSystemMetrics(SM_CXSCREEN);
		io.DisplaySize.y = ::GetSystemMetrics(SM_CYSCREEN);

		ImGui_ImplWin32_Init(globals::target_proc::target_wnd_hwnd);
		ImGui_ImplDX11_Init(_d3d_device, _d3d_device_context);

		imgui_styling();

		// dcomp
		// set owning thread
		core::get_thread(_handle, &local_thread);
		core::get_thread(globals::target_proc::target_wnd_hwnd, &remote_thread);
		core::set_thread(globals::target_proc::target_wnd_hwnd, local_thread);

		RET_CHK2(DCompositionCreateDevice(_dxgi_device, __uuidof(IDCompositionDevice), reinterpret_cast<void**>(&compositionDevice)))
			RET_CHK2(compositionDevice->CreateTargetForHwnd(globals::target_proc::target_wnd_hwnd, TRUE, &compositionTarget))
			RET_CHK2(compositionDevice->CreateVisual(&compositionVisual))
			RET_CHK2(compositionVisual->SetContent(_dxgi_swapchain))
			RET_CHK2(compositionTarget->SetRoot(compositionVisual))
			RET_CHK2(compositionDevice->Commit())
			RET_CHK2(compositionDevice->WaitForCommitCompletion())

		// reset owning thread
		core::set_thread(globals::target_proc::target_wnd_hwnd, remote_thread);

		if (!core::hide_dcomp(GetCurrentProcessId(), globals::target_proc::target_wnd_hwnd)) {
			printf("hiding dcomp failed!\n");
		}
	}

	void begin_scene() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void end_scene() {
		ImVec4 clear_color = ImVec4({ 0.0f, 0.0f, 0.0f, 0.f });

		ImGui::Render();
		_d3d_device_context->OMSetRenderTargets(1, &dxcomp::_d3d_render_target_view, nullptr);
		_d3d_device_context->ClearRenderTargetView(dxcomp::_d3d_render_target_view, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		_dxgi_swapchain->Present(0, 0);
	}

	void handle_input() {
		ImGuiIO& io = ImGui::GetIO();

		if (GetAsyncKeyState(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;

			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
	}

	void draw_rect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		ImGui::GetBackgroundDrawList()->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
	}

	void draw_circle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
	{
		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		ImGui::GetBackgroundDrawList()->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
	}

	void draw_line(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		ImGui::GetBackgroundDrawList()->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
	}

	void draw_corner_box(float x, float y, float w, float h, float thickness, bool team) {
		float lineW = (w / 5);
		float lineH = (h / 6);
		float lineT = 1;


		uint32_t lol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.51f, 0.36f, 0.15f, 1.00f));
		
		if (!team) {
			lol = ImColor(0, 0, 255);
		}
		else {
			lol = ImColor(0, 255, 0);
		}

		//draw inline
		draw_line(ImVec2{ x, y }, ImVec2{ x, y + lineH }, lol, thickness);
		draw_line(ImVec2{ x, y }, ImVec2{ x + lineW, y }, lol, thickness);
		draw_line(ImVec2{ x + w - lineW, y }, ImVec2{ x + w, y }, lol, thickness);
		draw_line(ImVec2{ x + w, y }, ImVec2{ x + w, y + lineH }, lol, thickness);

		draw_line(ImVec2{ x, y + h - lineH }, ImVec2{ x, y + h }, lol, thickness);
		draw_line(ImVec2{ x, y + h }, ImVec2{ x + lineW, y + h }, lol, thickness);
		draw_line(ImVec2{ x + w - lineW, y + h }, ImVec2{ x + w, y + h }, lol, thickness);
		draw_line(ImVec2{ x + w, y + h - lineH }, ImVec2{ x + w, y + h }, lol, thickness);
	}

	void draw_health_bar(float health, float x, float y, float height, uint32_t color) {
		float flBoxes = std::ceil(health / 10.f);

		float flX = x - 7 - height / 4.f; float flY = y - 1;
		float flHeight = height / 10.f;
		float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;

		draw_rect(ImVec2{ flX, flY }, ImVec2{ flX + 4, flY + height }, 0xffffffaa, 0, 0, 1);

		draw_rect(ImVec2{ flX + 1, flY + 1 }, ImVec2{ flX + 2, flY + flHeight * 10.f + 1 }, 0xff0000aa, 0, 0, 1);

		if (flBoxes > 1)
			draw_rect(ImVec2{ flX + 1, flY }, ImVec2{ flX + 2, flY + flHeight * flBoxes + 1 }, color, 0, 0, 1);

		if (height > 25) {
			for (int i = 0; i < 10; i++)
				draw_line(ImVec2{ flX, flY + i * flHeight }, ImVec2{ flX + 4, flY + i * flHeight }, 0xaabb22aa, 1);
		}
	}

	float draw_text(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
	{
		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
			if (center)
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}
			else
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}

			y = position.y + textSize.y * (i + 1);
			i++;
		}

		return y;
	}

	ID3D11Device* _d3d_device = nullptr;
	IDXGIDevice* _dxgi_device = nullptr;
	ID3D11DeviceContext* _d3d_device_context = nullptr;
	IDXGISwapChain1* _dxgi_swapchain = nullptr;
	ID3D11RenderTargetView* _d3d_render_target_view = nullptr;
	IDXGIFactory2* _dxgi_factory = nullptr;

	ID3D11BlendState* _d3d_blend_state = nullptr;

	D3D_FEATURE_LEVEL _d3d_featureLevel{};

	IDCompositionDevice* compositionDevice = nullptr;
	IDCompositionTarget* compositionTarget = nullptr;
	IDCompositionVisual* compositionVisual = nullptr;

	HWND _handle = nullptr;

	uint64_t local_thread;
	uint64_t remote_thread;
}