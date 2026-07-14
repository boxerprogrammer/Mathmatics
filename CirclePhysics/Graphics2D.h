#pragma once
#include<vector>
class Graphics2D
{
public:
	static void DrawGraph(int x, int y, 
					const std::vector<int>& handles,
					int ps, bool transFlag);
};

