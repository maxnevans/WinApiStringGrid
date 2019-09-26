#pragma once
#include "Window.h"
#include "StringGrid.h"
#include <windows.h>
#include <gdiplus.h>

class Model
{
private:
	typedef LRESULT(*ActionHandler)(HWND, UINT, WPARAM, LPARAM, Model*);

public:
	Model();
	Model(const Model&) = delete;
	~Model();
	void Start();

private:
	void SetWindowHandlers(Window* window);
	void SetHandlerInWindow(UINT uMsg, ActionHandler handler);
	void RedrawStringGrid(Gdiplus::Graphics& graphics, Window& window, StringGrid& grid);
	void FillStringGrid(StringGrid& grid);

	static void ContentMeasureFunc(std::wstring string, int& boundingWidth, int& boundingHeight);

	static LRESULT HandlerOnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self);
	static LRESULT HandlerOnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self);
	static LRESULT HandlerOnClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self);
	static LRESULT HandlerOnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self);

private:
	static constexpr const wchar_t* WINDOW_NAME = L"Grid system";
	static constexpr const Gdiplus::ARGB DEFAULT_BG_COLOR = 0xFF62A594;
	static constexpr const int GRID_COUNT_COLS = 10;
	static constexpr const int GRID_COUNT_ROWS = 7;
	static constexpr const int GRID_DEFAULT_X = 0;
	static constexpr const int GRID_DEFAULT_Y = 0;
	static constexpr const int GRID_DEFAULT_WIDTH = 0;
	static constexpr const int GRID_DEFAULT_HEIGHT = 0;
	static constexpr const Gdiplus::ARGB GRID_BORDER_COLOR = 0xFFEEEEEE;
	static constexpr const float GRID_BORDER_THICKNESS = 1.f;
	static constexpr const Gdiplus::ARGB GRID_FONT_COLOR = 0xFFEEEEEE;
	static constexpr const wchar_t* GRID_FONT_FAMILY = L"Tahoma";
	static constexpr const int GRID_FONT_SIZE_EM = 10;
	static constexpr const StringGrid::DistributionModes GRID_COLS_DISTRIBUTION = StringGrid::DistributionModes::EQUALLY;
	static constexpr const StringGrid::DistributionModes GRID_ROWS_DISTRIBUTION = StringGrid::DistributionModes::CONTENT_BASED;
	static constexpr const int GRID_COLS_MINIMAL_WIDTH = 40;
	static constexpr const int GRID_ROWS_MINIMAL_HEIGHT = 40;

	static HWND mainWindowHwnd;

	Window* window;
	StringGrid* grid;
};

