#include <iostream>
#include <string>
#include <vector>
#include "docker.hpp"

std::string removeVersion(std::string str)
{
	size_t pos = str.find(":");
	if (pos != std::string::npos)
	{
		str = str.substr(0, pos);
	}
	return str;
}

std::string removeTrailingSpaces(std::string str) 
{
	int i = str.length() - 1;
	while (str[i] == ' ')
	{
		i--;
	}
	return str.substr(0, i + 1);
}

std::vector<column> createColumns()
{
	std::vector<column> columns;
	std::string columnsLine;
	std::getline(std::cin, columnsLine);
	std::string columnName = "";
	int columnIndex = 0;
	bool pastFirstColumn = false;
	for (int i = 0; i < columnsLine.size(); i++)
	{
		char c = columnsLine[i];
		if (c == ' ')
		{
			if (pastFirstColumn)
			{
				if (columnName != "")
				{
					column col = {columnName, columnIndex, std::vector<std::string>()};
					columns.push_back(col);
				}
				columnName = "";
			}
			else
			{
				columnName += " ";
				pastFirstColumn = true;
			}
		}
		else
		{
			if (columnName == "")
				columnIndex = i;
			columnName += c;
		}
	}
	return columns;
}

void populateColumns(std::vector<column> &columns)
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line == "")
			break;
		std::string value = "";
		for (int i = 0; i < columns.size(); i++) {
			int endIndex = i == columns.size() - 1 ? line.size() : columns[i + 1].index;
			value = line.substr(columns[i].index, endIndex - columns[i].index);
			columns[i].values.push_back(removeTrailingSpaces(value));
		}
	}
}

std::string getContainerId(std::string containerName)
{
	std::string line;
	std::vector<column> columns = createColumns();
	populateColumns(columns);

	for (int i = 0; i < columns[1].values.size(); i++)
	{
		if (columns[1].values[i].rfind(containerName, 0) == 0)
		{
			std::cout << columns[0].values[i] << std::endl;
			return 0;
		}
	}

	std::cout << std::endl;
	return 0;
}