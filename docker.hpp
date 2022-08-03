#include <iostream>
#include <string>
#include <vector>

typedef struct column
{
	std::string name;
	int index;
	std::vector<std::string> values;
} column;

std::string removeVersion(std::string str);

std::string removeTrailingSpaces(std::string str);

std::vector<column> createColumns();

void populateColumns(std::vector<column> &columns);

std::string getContainerId(std::string containerName);