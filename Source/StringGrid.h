#pragma once
#include <string>
#include <vector>

class StringGrid
{
public:
	typedef void (*TContentMeasureFunc)(std::wstring string, int& boundingWidth, int& bountingHeight);

	enum class DistributionModes
	{
		AUTO,
		EQUALLY,
		CONTENT_BASED,
		MANUAL
	};

private:

	struct GridCell
	{
		std::wstring string;
	};

	struct GridProperties
	{
		std::vector<DistributionModes> rowsMode;
		std::vector<DistributionModes> colsMode;
		std::vector<int> rowsHeight;
		std::vector<int> colsWidth;
	};

public:
	StringGrid(int x, int y, int width, int height,
		DistributionModes rowsDist = StringGrid::DEFAULT_ROW_DIST,
		DistributionModes colsDist = StringGrid::DEFAULT_COL_DIST,
		int colWidth = StringGrid::DEFAULT_COL_WIDTH, int colHeight = StringGrid::DEFAULT_ROW_HEIGHT);
	void SetRowsCount(int countRows);
	void SetColsCount(int countCols);
	int GetRowsCount();
	int GetColsCount();
	void SetString(int row, int col, std::wstring string);
	const std::wstring GetString(int row, int col);
	void SetX(int x);
	void SetY(int y);
	int GetX();
	int GetY();
	void SetWidth(int width);
	void SetHeight(int height);
	int GetWidth();
	int GetHeight();
	DistributionModes GetRowDistributionMode(int row);
	DistributionModes GetColDistributionMode(int col);
	void SetRowDistributionMode(int row, DistributionModes heightDistMode);
	void SetColDistributionMode(int col, DistributionModes widthDistMode);
	void SetContentMeasureCallback(TContentMeasureFunc contentMeasureCallback);
	void SetRowHeight(int row, int height);
	void SetColWidth(int col, int width);
	int GetRowHeight(int row);
	int GetColWidth(int col);
	int GetRowX1(int row);
	int GetRowX2(int row);
	int GetRowY(int row);
	int GetColX(int col);
	int GetColY1(int col);
	int GetColY2(int col);
	int GetCellX(int row, int col);
	int GetCellY(int row, int col);
	int GetCellWidth(int row, int col);
	int GetCellHeight(int row, int col);

private:
	void UpdateGridCountRows(std::vector<std::vector<GridCell>>& grid, GridProperties& gridProperties, 
		int prevCountRows, int newCountRows);
	void UpdateGridCountCols(std::vector<std::vector<GridCell>>& grid, GridProperties& gridProperties, 
		int prevCountCols, int newCountCols);
	void UpdateRowDistribution(GridProperties& gridProperties, int row, DistributionModes newHeightDistibution);
	void UpdateColDistribution(GridProperties& gridProperties, int col, DistributionModes newWidthDistibution);
	void RedistributeGridWidth(GridProperties& gridProperties, int prevWidth, int newWidth);
	void RedistributeGridHeight(GridProperties& gridProperties, int prevHeight, int newHeight);
	void RedistributeGridRows(GridProperties& gridProperties, int prevHeight, int newHeight);
	void RedistributeGridCols(GridProperties& gridProperties, int prevWidth, int newWidth);
	void CheckRowIndex(int row);
	void CheckColIndex(int col);
	void CheckContentMeasureFunc();
	void CheckCoord(int coord);
	void CheckDimention(int dimention);
	void CheckCountCols(int col);
	void CheckCountRows(int row);
	DistributionModes GetPriorDistributionOfTwo(DistributionModes a, DistributionModes b);

private:
	static constexpr const DistributionModes DEFAULT_COL_DIST = DistributionModes::AUTO;
	static constexpr const DistributionModes DEFAULT_ROW_DIST = DistributionModes::AUTO;
	static constexpr const DistributionModes AUTO_DISTRIBUTION_CONVERTION = DistributionModes::CONTENT_BASED;
	static constexpr const int DEFAULT_COL_WIDTH = 40;
	static constexpr const int DEFAULT_ROW_HEIGHT = 40;
	static constexpr const wchar_t* DEFAULT_CELL_CONTENT = L"";

	TContentMeasureFunc contentMeasureFunc;
	int width, height;
	int x, y;
	int countCols, countRows;
	std::vector<std::vector<GridCell>> gridCells;
	GridProperties gridProperties;
	DistributionModes colsDistMode;
	DistributionModes rowsDistMode;
	int cellsDefaultHeight;
	int cellsDefaultWidth;
};

