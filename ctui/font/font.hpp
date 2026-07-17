#pragma once

#include <string>
#include <vector>

// consts
int const LINECOUNT = 10;
int const FONTSIZE = 5;
int const SPACECOUNT = 1;


void generateFont([[maybe_unused]]std::string text, int size, char fontChar, std::vector<std::vector<std::string>>& outVec);
