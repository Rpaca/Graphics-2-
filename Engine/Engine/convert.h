#pragma once
#include <iostream>
#include <fstream>
using namespace std;

typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	float x, y, z;
}VertexType2;


typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);
int Converting(char*);
