#pragma once
#include <vector>
#include <istream>
#include <ostream>

template<typename T> class Triangle
{
	size_t maxRows;
	std::vector<std::vector<T>> rows;

public:
	Triangle() :
		Triangle(101) {}

	Triangle(size_t maxRows) :
		maxRows(maxRows), rows{ std::vector<T>{ T{} } }{}

	Triangle(const Triangle &other) :
		maxRows(other.maxRows), rows(other.rows) {}

	Triangle& operator=(const Triangle &other)
	{
		if (this == &other)return *this;

		maxRows = other.maxRows;
		rows = other.rows;
		return *this;
	}

	void clear() { rows.clear(); }

	size_t getRows()const { return rows.size(); }

	T& getAt(size_t i, size_t j)
	{
		if (i >= rows.size() || j >= rows[i].size()) i = j = 0;
		return rows[i][j];
	}

	const T& getAt(size_t i, size_t j)const
	{
		if (i >= rows.size() || j >= rows[i].size()) i = j = 0;
		return rows[i][j];
	}

	bool isSymmetric()const
	{
		bool symmetric = true;

		for (size_t i = 0; symmetric && i < rows.size(); i++)
		{
			for (int j = 0; symmetric &&j < rows[i].size() / 2; j++)
			{
				symmetric &= (rows[i][j] == rows[i][rows[i].size() - 1 - j]);
			}
		}

		return symmetric;
	}

	void readRow(std::istream &stream)
	{
		if (rows.size() >= maxRows) {/*throw std::runtime_error("...")*/ }

		size_t lastRowSize =
			(rows.empty()) ? 0 : rows.back().size();

		rows.emplace_back(lastRowSize + 1, T{});
		for (T& cell : rows.back())stream >> cell; 
	}

	void rotate()
	{
		Triangle rotated = *this;

		int targetRow = 0, targetColumn = 0;

		for (int startRow = getRows() - 1; startRow >= 0; startRow--)
		{
			const int column = startRow;

			for (int row = startRow; row < getRows(); row++)
			{
				rotated.rows[row][column]=rows[targetRow][targetColumn];

				if (targetColumn + 1 < rows[targetRow].size())targetColumn++;
				else { targetRow++; targetColumn = 0; }
			}
		}

		*this = rotated;
	}

};

template<typename T>
std::istream& operator >> (std::istream &stream, Triangle<T> &triangle)
{
	triangle.readRow(stream);
	return stream;
}

template<typename T>
std::ostream& operator << (std::ostream &stream, const Triangle<T> &triangle)
{
	for (size_t targetRow = 0; targetRow < triangle.getRows(); targetRow++)
	{
		for (size_t j = 0; j < triangle.getRows() / 2 - targetRow / 2; j++)stream << "  ";
		for (size_t j = 0; j <= targetRow; j++)
		{
			stream << triangle.getAt(targetRow, j) << ' ';
		}
		stream << std::endl;
	}
	return stream;
}