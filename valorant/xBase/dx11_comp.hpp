#pragma once

namespace dxcomp {
	extern ID3D11Device* _d3d_device;
	extern IDXGIDevice* _dxgi_device;
	extern ID3D11DeviceContext* _d3d_device_context;
	extern IDXGISwapChain1* _dxgi_swapchain;
	extern ID3D11RenderTargetView* _d3d_render_target_view;
	extern IDXGIFactory2* _dxgi_factory;

	extern ID3D11BlendState* _d3d_blend_state;

	extern IDCompositionDevice* compositionDevice;
	extern IDCompositionTarget* compositionTarget;
	extern IDCompositionVisual* compositionVisual;

	extern D3D_FEATURE_LEVEL _d3d_featureLevel;
	extern HWND _handle;

	extern uint64_t local_thread;
	extern uint64_t remote_thread;

	extern ImVec4 clear_color;

	extern HWND		create_window();
	extern HRESULT	d3d_create_render_target();
	extern void		init_rendering();
	extern HRESULT	d3d_create_device();

	extern void begin_scene();
	extern void end_scene();

	extern void handle_input();

	float draw_text(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center);
	void draw_line(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness);
	void draw_circle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments);
	void draw_rect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness);
	void draw_corner_box(float x, float y, float w, float h, float thickness, bool team);
	void draw_health_bar(float health, float x, float y, float height, uint32_t color);
}