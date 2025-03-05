#pragma once
#include "./Engine/GameObject.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "./Source/Screen.h"
#include "./globals.h"

using std::vector;


constexpr int STAGE_WIDTH = (Screen::WIDTH / CHA_WIDTH) % 2 ? (Screen::WIDTH / CHA_WIDTH) : (Screen::WIDTH / CHA_WIDTH) - 1;
constexpr int STAGE_HEIGHT = (Screen::HEIGHT / CHA_HEIGHT) % 2 ? (Screen::HEIGHT / CHA_HEIGHT) : (Screen::HEIGHT / CHA_HEIGHT) - 1;


class Stage :
    public GameObject
{
	vector<vector<STAGE_OBJ>> stageData;
	vector<vector<int>> stageDataSearch;
	vector<Rect> stageRects;
	const Point dirs[4]{ {0,1},{0,-1},{1,0},{-1,0} };

public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	STAGE_OBJ GetStageData(int x, int y) {return stageData[y][x];}
	vector<Rect> GetStageRects() { return stageRects; }
	vector<vector<STAGE_OBJ>>& GetStageGrid() { return stageData; }
	void setStageRects();

	void BFS(Point Start, Point Goal);
	void Dijkstra(std::pair<int, int> sp);

	vector<Point> restore(int tx, int ty);
	vector<Point> restore(Point start, Point goal);
};

