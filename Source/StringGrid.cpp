#include "StringGrid.h"
#include "Exception.h"
#include <algorithm>

StringGrid::StringGrid(int x, int y, int width, int height, DistributionModes rowsDist, 
	DistributionModes colsDist, int colWidth, int rowHeight)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->countCols = 0;
	this->countRows = 0;
	this->contentMeasureFunc = nullptr;
	this->colsDistMode = colsDist;
	this->rowsDistMode = rowsDist;
	this->cellsDefaultWidth = colWidth;
	this->cellsDefaultHeight = rowHeight;
}

void StringGrid::SetRowsCount(int countRows)
{
	this->CheckCountRows(countRows);

	if (this->countRows == countRows)
		return;

	this->UpdateGridCountRows(this->gridCells, this->gridProperties, this->countRows, countRows);
	this->countRows = countRows;
}

void StringGrid::SetColsCount(int countCols)
{
	this->CheckCountCols(countCols);

	if (this->countCols == countCols)
		return;

	this->UpdateGridCountCols(this->gridCells, this->gridProperties, this->countCols, countCols);
	this->countCols = countCols;
}

int StringGrid::GetRowsCount()
{
	return this->countRows;
}

int StringGrid::GetColsCount()
{
	return this->countCols;
}

void StringGrid::SetString(int row, int col, std::wstring string)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);
	this->CheckContentMeasureFunc();

	if (this->gridCells[row][col].string == string)
		return;

	// TODO: CALCULATE HEIGHT AND WIDTH BASED ON MODE

	this->gridCells[row][col].string = string;

	int boundingWidth = this->gridProperties.colsWidth[col];
	int boundingHeight = 0;
	this->contentMeasureFunc(string, boundingWidth, boundingHeight);

	this->gridProperties.rowsHeight[row] = std::max(this->gridProperties.rowsHeight[row], boundingHeight);
}

const std::wstring StringGrid::GetString(int row, int col)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);

	return this->gridCells[row][col].string;
}

void StringGrid::SetX(int x)
{
	if (this->x == x)
		return;

	this->x = x;
}

void StringGrid::SetY(int y)
{
	if (this->y == y)
		return;

	this->y = y;
}

int StringGrid::GetX()
{
	return this->x;
}

int StringGrid::GetY()
{
	return this->y;
}

void StringGrid::SetWidth(int width)
{
	this->CheckDimention(width);

	if (width == this->width)
		return;

	this->RedistributeGridWidth(this->gridProperties, this->width, width);
	this->RedistributeGridHeight(this->gridProperties, this->height, this->height);
	this->width = width;
}

void StringGrid::SetHeight(int height)
{
	this->CheckDimention(height);

	if (height == this->height)
		return;

	this->RedistributeGridHeight(this->gridProperties, this->height, height);
	this->RedistributeGridWidth(this->gridProperties, this->width, this->width);
	this->height = height;
}

int StringGrid::GetWidth()
{
	return this->width;
}

int StringGrid::GetHeight()
{
	return this->height;
}

StringGrid::DistributionModes StringGrid::GetRowDistributionMode(int row)
{
	this->CheckRowIndex(row);
	
	return this->gridProperties.rowsMode[row];
}

StringGrid::DistributionModes StringGrid::GetColDistributionMode(int col)
{
	this->CheckColIndex(col);

	return this->gridProperties.colsMode[col];
}

void StringGrid::SetRowDistributionMode(int row, DistributionModes heightDistMode)
{
	this->CheckRowIndex(row);

	if (this->gridProperties.rowsMode[row] == heightDistMode)
		return;

	this->UpdateRowDistribution(this->gridProperties, row, heightDistMode);
	this->RedistributeGridRows(this->gridProperties, this->height, this->height);
}

void StringGrid::SetColDistributionMode(int col, DistributionModes widthDistMode)
{
	this->CheckColIndex(col);

	if (this->gridProperties.colsMode[col] == widthDistMode)
		return;

	this->UpdateColDistribution(this->gridProperties, col, widthDistMode);
	this->RedistributeGridCols(this->gridProperties, this->width, this->width);
}

void StringGrid::SetContentMeasureCallback(TContentMeasureFunc contentMeasureCallback)
{
	this->contentMeasureFunc = contentMeasureCallback;
}

void StringGrid::SetRowHeight(int row, int height)
{
	throw Exception(L"not impelemented");
}

void StringGrid::SetColWidth(int col, int width)
{
	throw Exception(L"not impelemented");
}

int StringGrid::GetRowHeight(int row)
{
	this->CheckRowIndex(row);

	return this->gridProperties.rowsHeight[row];
}

int StringGrid::GetColWidth(int col)
{
	this->CheckRowIndex(col);

	return this->gridProperties.colsWidth[col];
}

int StringGrid::GetRowX1(int row)
{
	this->CheckRowIndex(row);

	return this->x;
}

int StringGrid::GetRowX2(int row)
{
	this->CheckRowIndex(row);

	return this->x + this->width;
}

int StringGrid::GetRowY(int row)
{
	this->CheckRowIndex(row);

	int rowY = this->y;

	for (int i = 0; i <= row; i++)
	{
		rowY += this->gridProperties.rowsHeight[i];
	}

	return rowY;
}

int StringGrid::GetColX(int col)
{
	this->CheckColIndex(col);

	int colX = this->x;

	for (int i = 0; i <= col; i++)
	{
		colX += this->gridProperties.colsWidth[i];
	}

	return colX;
}

int StringGrid::GetColY1(int col)
{
	this->CheckColIndex(col);

	return this->y;
}

int StringGrid::GetColY2(int col)
{
	this->CheckColIndex(col);

	return this->y + this->height;
}

int StringGrid::GetCellX(int row, int col)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);

	int cellX = this->x;

	for (int i = 0; i < col; i++)
	{
		cellX += this->gridProperties.colsWidth[i];
	}

	return cellX;
}

int StringGrid::GetCellY(int row, int col)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);

	int cellY = this->y;

	for (int i = 0; i < row; i++)
	{
		cellY += this->gridProperties.rowsHeight[i];
	}

	return cellY;
}

int StringGrid::GetCellWidth(int row, int col)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);

	return this->gridProperties.colsWidth[col];
}

int StringGrid::GetCellHeight(int row, int col)
{
	this->CheckRowIndex(row);
	this->CheckColIndex(col);

	return this->gridProperties.rowsHeight[row];
}

void StringGrid::UpdateGridCountRows(std::vector<std::vector<GridCell>>& grid, GridProperties& gridProperties, int prevCountRows, int newCountRows)
{
	grid.resize(newCountRows, 
		std::vector<GridCell>(this->countCols, GridCell{ std::wstring(StringGrid::DEFAULT_CELL_CONTENT) }));

	gridProperties.rowsMode.resize(newCountRows, this->rowsDistMode);
	gridProperties.rowsHeight.resize(newCountRows, this->cellsDefaultHeight);
}

void StringGrid::UpdateGridCountCols(std::vector<std::vector<GridCell>>& grid, GridProperties& gridProperties, int prevCountCols, int newCountCols)
{
	for (auto& row : grid)
	{
		row.resize(newCountCols, GridCell{ std::wstring(StringGrid::DEFAULT_CELL_CONTENT) });
	}

	gridProperties.colsMode.resize(newCountCols, this->colsDistMode);
	gridProperties.colsWidth.resize(newCountCols, this->cellsDefaultWidth);
}

void StringGrid::UpdateRowDistribution(GridProperties& gridProperties, 
	int row, DistributionModes newHeightDistibution)
{
	gridProperties.rowsMode[row] = newHeightDistibution;
}

void StringGrid::UpdateColDistribution(GridProperties& gridProperties, 
	int col, DistributionModes newWidthDistibution)
{
	gridProperties.colsMode[col] = newWidthDistibution;
}

void StringGrid::RedistributeGridWidth(GridProperties& gridProperties, int prevWidth, int newWidth)
{
	int countFixedWidth = 0;
	int countEquallyCols = 0;

	for (size_t i = 0; i < gridProperties.colsMode.size(); i++)
	{
		if (gridProperties.colsMode[i] == DistributionModes::EQUALLY)
			countEquallyCols++;
		else
			throw Exception(L"this kind of redistribution for width is not implemented in StringGrid");
	}

	int toBeDistributed = newWidth - countFixedWidth;
	int newEqualColsWidth = std::max(static_cast<int>(toBeDistributed / static_cast<float>(countEquallyCols)),
		this->cellsDefaultWidth);

	for (size_t i = 0; i < gridProperties.colsMode.size(); i++)
	{
		if (gridProperties.colsMode[i] == DistributionModes::EQUALLY)
			gridProperties.colsWidth[i] = newEqualColsWidth;
	}
}

void StringGrid::RedistributeGridHeight(GridProperties& gridProperties, int prevHeight, int newHeight)
{
	for (size_t row = 0; row < gridProperties.rowsMode.size(); row++)
	{
		if (gridProperties.rowsMode[row] == DistributionModes::CONTENT_BASED)
		{
			int newContentBasedRowHeight = this->cellsDefaultHeight;

			for (size_t col = 0; col < this->gridCells[row].size(); col++)
			{
				int boundingWidth = gridProperties.colsWidth[col];
				int boundingHeight = 0;
				this->contentMeasureFunc(this->gridCells[row][col].string, boundingWidth, boundingHeight);

				newContentBasedRowHeight = std::max(newContentBasedRowHeight, boundingHeight);
			}
				

			gridProperties.rowsHeight[row] = newContentBasedRowHeight;
		}
		else
			throw Exception(L"this kind of redistribution for width is not implemented in StringGrid");
	}
}

void StringGrid::RedistributeGridRows(GridProperties& gridProperties, int prevHeight, int newHeight)
{
	this->RedistributeGridHeight(gridProperties, prevHeight, newHeight);
}

void StringGrid::RedistributeGridCols(GridProperties& gridProperties, int prevWidth, int newWidth)
{
	this->RedistributeGridWidth(gridProperties, prevWidth, newWidth);
}

void StringGrid::CheckRowIndex(int row)
{
	if (row < 0 || row >= this->countRows)
	{
		throw Exception(std::wstring(L"row with index ")
			+ std::to_wstring(row) + std::wstring(L" does not exists in StringGrid"));
	}
}

void StringGrid::CheckColIndex(int col)
{
	if (col < 0 || col >= this->countCols)
	{
		throw Exception(std::wstring(L"col with index ")
			+ std::to_wstring(col) + std::wstring(L" does not exists in StringGrid"));
	}
}

void StringGrid::CheckContentMeasureFunc()
{
	if (this->contentMeasureFunc == nullptr)
		throw Exception(L"content measure function is not provided for StringGrid");
}

void StringGrid::CheckCoord(int coord)
{
	if (coord < 0)
		throw Exception(L"coord can not be negative in StringGrid");
}

void StringGrid::CheckDimention(int dimention)
{
	if (dimention < 0)
		throw Exception(L"dimention can not be negative in StringGrid");
}

void StringGrid::CheckCountCols(int col)
{
	if (col < 0)
		throw Exception(L"count cols can not be negative in StringGrid");
}

void StringGrid::CheckCountRows(int row)
{
	if (row < 0)
		throw Exception(L"count rows can not be negative in StringGrid");
}

StringGrid::DistributionModes StringGrid::GetPriorDistributionOfTwo(DistributionModes a, DistributionModes b)
{
	if (a == DistributionModes::MANUAL || b == DistributionModes::MANUAL)
		return DistributionModes::MANUAL;

	if (a == DistributionModes::CONTENT_BASED || b == DistributionModes::CONTENT_BASED)
		return DistributionModes::CONTENT_BASED;

	if (a == DistributionModes::EQUALLY || b == DistributionModes::EQUALLY)
		return DistributionModes::EQUALLY;

	return DistributionModes::AUTO;
}
