#include "pch.h"
#include "JPSPathFinder.h"
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <time.h>
#include <stdarg.h>



int main()
{
	jpspath::CollisionMap::SharedPtr spMap = std::make_shared<jpspath::CollisionMap>();
	if (!spMap)
		throw std::bad_alloc();

	//====================================================
	// CREATE MAP 
	//====================================================
	int32_t GridWidth = 300, GridHeight = 300;
	spMap->Create(GridWidth, GridHeight);

	srand(32);

	// Start, End Position (시작점, 도착점)
	int32_t Sx = 0, Sy = 0;
	int32_t Ex = GridWidth - 1, Ey = GridHeight - 1;

	// Make a Maze (충돌 미로를 만든다.)
	for (int32_t y = 0; y < GridHeight; ++y)
	{
		for (int32_t x = 0; x < GridWidth; ++x)
		{
			// Don't Mark Collision at Start, End Position (시작,도착점은 충돌처리 하지 않는다.)
			if (x == Sx && y == Sy)	continue;
			if (x == Ex && y == Ey) continue;

			// Randomly Mark Collision at Position (랜덤하게 충돌처리를 한다.)
			if (rand() % 50 == 0) spMap->SetAt(x, y);
		}
	}

	for (int32_t x = 3; x < GridWidth-3; ++x)
	{
		// Randomly Mark Collision at Position (랜덤하게 충돌처리를 한다.)
		spMap->SetAt(x, 150);
	}

	//====================================================
	// Find PATH
	//====================================================
	std::list<jpspath::Coord> ResultNodes;	// Result for JPS

	jpspath::Path	jps;
	// SET
	jps.Init(spMap);
	// SEARCH
	jps.Search(Sx, Sy, Ex, Ey, ResultNodes);
	// STRAIGHT PATH 
	//jps.PullingString(ResultNodes);
	//====================================================
	// SAVE RESULT MAP TO FILE FOR DEBUG
	//====================================================
	std::string results(GridHeight * (GridWidth + 1) + 1, ' ');

	// Mark Collision With '@' ('@' 로 충돌맵을 표시합니다.)
	for (int32_t y = 0; y < GridHeight; y++)
	{
		for (int32_t x = 0; x < GridWidth; x++)
		{
			results[(GridHeight - 1 - y) * (GridWidth + 1) + x] = !spMap->IsCollision(x, y) ? ' ' : '@';
		}
		results[(GridHeight - 1 - y) * (GridWidth + 1) + GridWidth] = '\n';
	}

	if (ResultNodes.size() > 0) 
	{
		for (auto it = ResultNodes.begin(); it != ResultNodes.end(); ++it)
		{
			auto& coord = (*it);
			std::cout << coord.m_x << ", " << coord.m_y << " \n";

		}
	}

	// Mark Path Nodes With '#' ('#'로 찾은 경로를 표시합니다.)
	if (ResultNodes.size() > 0)
	{
		std::cout << "==============\n";

		auto iterS = ResultNodes.begin();
		auto iterE = ResultNodes.end();
		auto iterP = iterS;	++iterS;
		for (; iterS != iterE; iterP = iterS, ++iterS)
		{
			auto& PreCoord = (*iterP);
			auto& CurCoord = (*iterS);

			std::cout << "(" << PreCoord.m_x << ", " << PreCoord.m_y << ")- ";
			std::cout << CurCoord.m_x << ", " << CurCoord.m_y << " \n";

			int32_t x = PreCoord.m_x;
			int32_t y = PreCoord.m_y;
			int32_t dx = jpspath::clamp<int32_t>(CurCoord.m_x - PreCoord.m_x, -1, 1);
			int32_t dy = jpspath::clamp<int32_t>(CurCoord.m_y - PreCoord.m_y, -1, 1);

			for (int32_t v = y, u = x; ; v += dy, u += dx)
			{
				results[(GridHeight - 1 - v) * (GridWidth + 1) + u] = '#';

				// std::cout << "\t\t" << u << ", " << v << " \n";


				if (u == CurCoord.m_x && v == CurCoord.m_y)
					break;

				int32_t deltax = jpspath::clamp<int32_t>(CurCoord.m_x - u, -1, 1);
				int32_t deltay = jpspath::clamp<int32_t>(CurCoord.m_y - v, -1, 1);
				if (deltax != dx || deltay != dy)
				{
					std::cout << "INVALID NODE\n";
					break;
				}
			}
			results[(GridHeight - 1 - CurCoord.m_y) * (GridWidth + 1) + CurCoord.m_x] = '#';
		}

		// Mark Start & End Position ('S', 'E' 로 시작점 도착점을 표시합니다.)
		auto	iterStart	= ResultNodes.begin();
		auto	iterEnd		= ResultNodes.rbegin();
		auto&	startCoord	= (*iterStart);
		auto&	endCoord	= (*iterEnd);
		results[(GridHeight - 1 - startCoord.m_y) * (GridWidth + 1) + startCoord.m_x] = 'S';
		results[(GridHeight - 1 - endCoord.m_y) * (GridWidth + 1) + endCoord.m_x]	  = 'E';
	}

	std::cout << "==============\n";

	//====================================================
	// SAVE FILE
	//====================================================
	if(results.size() > 0)
	{
		FILE* pFile = fopen("result_jps(b).txt", "wt");
		if (pFile != NULL)
		{
			fwrite(results.c_str(), sizeof(char), results.size(), pFile);
			fclose(pFile);
		}
	}

}
