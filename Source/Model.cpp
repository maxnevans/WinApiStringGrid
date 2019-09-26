#include "Model.h"

HWND Model::mainWindowHwnd = NULL;

Model::Model()
{
	this->window = new Window(Model::WINDOW_NAME);
	this->grid = new StringGrid(Model::GRID_DEFAULT_X, Model::GRID_DEFAULT_Y, 
		Model::GRID_DEFAULT_WIDTH, Model::GRID_DEFAULT_HEIGHT, 
		Model::GRID_ROWS_DISTRIBUTION, Model::GRID_COLS_DISTRIBUTION,
		Model::GRID_COLS_MINIMAL_WIDTH, Model::GRID_ROWS_MINIMAL_HEIGHT);
}
Model::~Model()
{
	delete this->window;
	this->window = nullptr;
}

void Model::Start()
{
	this->SetWindowHandlers(this->window);
	this->window->Create();
	Model::mainWindowHwnd = window->GetHandle();
}

void Model::SetWindowHandlers(Window* window)
{
	this->SetHandlerInWindow(WM_CREATE, Model::HandlerOnCreate);
	this->SetHandlerInWindow(WM_PAINT, Model::HandlerOnPaint);
	this->SetHandlerInWindow(WM_CLOSE, Model::HandlerOnClose);
	this->SetHandlerInWindow(WM_DESTROY, Model::HandlerOnDestroy);
}

void Model::SetHandlerInWindow(UINT uMsg, ActionHandler handler)
{
	window->SetActionHandler(uMsg, reinterpret_cast<Window::ActionHandler>(handler), reinterpret_cast<void*>(this));
}

void Model::RedrawStringGrid(Gdiplus::Graphics& graphics, Window& window, StringGrid& grid)
{
	using namespace Gdiplus;

	grid.SetWidth(window.GetClientWidth());
	grid.SetHeight(window.GetClientHeight());

	const int countRows = grid.GetRowsCount();
	const int countCols = grid.GetColsCount();

	Gdiplus::Pen pen(Color::Color(Model::GRID_BORDER_COLOR), Model::GRID_BORDER_THICKNESS);
	Gdiplus::FontFamily fontFamily(Model::GRID_FONT_FAMILY);
	Gdiplus::Font font(&fontFamily, Model::GRID_FONT_SIZE_EM);
	Gdiplus::SolidBrush brush(Color::Color(Model::GRID_FONT_COLOR));
	Gdiplus::StringFormat stringFormat;

	stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

	for (int col = 0; col < countCols - 1; col++)
	{
		const int colX = grid.GetColX(col);
		const int colY1 = grid.GetColY1(col);
		const int colY2 = grid.GetColY2(col);

		graphics.DrawLine(&pen, colX, colY1, colX, colY2);
	}

	for (int row = 0; row < countRows - 1; row++)
	{
		const int rowX1 = grid.GetRowX1(row);
		const int rowX2 = grid.GetRowX2(row);
		const int rowY = grid.GetRowY(row);

		graphics.DrawLine(&pen, rowX1, rowY, rowX2, rowY);
	}
	
	for (int col = 0; col < countCols; col++)
	{

		for (int row = 0; row < countRows; row++)
		{
			std::wstring string = grid.GetString(row, col);
			Gdiplus::RectF rect(
				static_cast<REAL>(grid.GetCellX(row, col)), 
				static_cast<REAL>(grid.GetCellY(row, col)), 
				static_cast<REAL>(grid.GetCellWidth(row, col)), 
				static_cast<REAL>(grid.GetCellHeight(row, col))
			);

			graphics.DrawString(grid.GetString(row, col).c_str(), -1, &font, rect, &stringFormat, &brush);
		}
	}
}

void Model::FillStringGrid(StringGrid& grid)
{
	grid.SetContentMeasureCallback(Model::ContentMeasureFunc);
	grid.SetRowsCount(Model::GRID_COUNT_ROWS);
	grid.SetColsCount(Model::GRID_COUNT_COLS);
	
	for (int i = 0; i < Model::GRID_COUNT_COLS; i++)
	{
		grid.SetColDistributionMode(i, StringGrid::DistributionModes::EQUALLY);
	}

	for (int i = 0; i < Model::GRID_COUNT_ROWS; i++)
	{
		grid.SetRowDistributionMode(i, StringGrid::DistributionModes::CONTENT_BASED);
	}
	
	for (int col = 0; col < Model::GRID_COUNT_COLS; col++)
	{
		for (int row = 0; row < Model::GRID_COUNT_ROWS; row++)
		{
			grid.SetString(row, col, 
				std::wstring(L"Row : ") + std::to_wstring(row) + L"\n"
				+ std::wstring(L"Col: " + std::to_wstring(col)));
		}
	}
}

void Model::ContentMeasureFunc(std::wstring string, int& boundingWidth, int& boundingHeight)
{
	Gdiplus::Pen pen(Gdiplus::Color::Color(Model::GRID_BORDER_COLOR), Model::GRID_BORDER_THICKNESS);
	Gdiplus::FontFamily fontFamily(Model::GRID_FONT_FAMILY);
	Gdiplus::Font font(&fontFamily, Model::GRID_FONT_SIZE_EM);
	Gdiplus::RectF layoutRect(0.f, 0.f, 
		static_cast<Gdiplus::REAL>(boundingWidth),
		static_cast<Gdiplus::REAL>(boundingHeight)
	);
	Gdiplus::RectF boundingRect;

	HDC hdc = GetDC(Model::mainWindowHwnd);
	Gdiplus::Graphics* graphics = new Gdiplus::Graphics(hdc);

	graphics->MeasureString(string.c_str(), -1, &font, layoutRect, &boundingRect);

	delete graphics;
	graphics = nullptr;

	ReleaseDC(Model::mainWindowHwnd, hdc);
	
	boundingWidth = static_cast<int>(boundingRect.Width);
	boundingHeight = static_cast<int>(boundingRect.Height);
}

LRESULT Model::HandlerOnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self)
{
	self->FillStringGrid(*self->grid);
	InvalidateRect(hWnd, NULL, FALSE);
	return 0;
}

LRESULT Model::HandlerOnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self)
{
	using namespace Gdiplus;

	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, self->window->GetClientWidth(), self->window->GetClientHeight());
	SelectObject(hdcMem, hBmp);
	Graphics graphics(hdcMem);

	SolidBrush sb(Color::Color(self->DEFAULT_BG_COLOR));
	graphics.FillRectangle(&sb, 0, 0, self->window->GetClientWidth(), self->window->GetClientHeight());

	self->RedrawStringGrid(graphics, *self->window, *self->grid);

	BitBlt(hdc, 0, 0, self->window->GetClientWidth(), self->window->GetClientHeight(), hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
	DeleteObject(hBmp);
	EndPaint(hWnd, &ps);

	return 0;
}

LRESULT Model::HandlerOnClose(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self)
{
	DestroyWindow(hWnd);
	return 0;
}

LRESULT Model::HandlerOnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Model* self)
{
	PostQuitMessage(0);
	return 0;
}
