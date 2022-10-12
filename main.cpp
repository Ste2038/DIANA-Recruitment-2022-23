#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <bits/stdc++.h>

#define MAX_PUNTI 100

using namespace std;

class Point {
public:
    int id;
    float x, y;
    bool isVisited;

    Point(){}

    Point(int id, float x, float y){
        this->id = id;
        this->x = x;
        this->y = y;
        this->isVisited = false;
    }
};

Point * importPoints(string filePath, int &nPoints);
void buildPath(int *sol, float distance, int index, Point *points, int nPoints);
int *cloneArray(int* array, int n);

int *bestSolution;
float bestDistance = 99999999999;

int main() {
    int nPoints;
    Point *points = importPoints("../points.txt", nPoints);

    cout << nPoints << " points entered" << endl;

    int i;
    for(i = 0; i < nPoints; i++) {
        cout << points[i].id << ' ' << points[i].x  << ' ' << points[i].y << endl;
    }


    //start calculations
    int sol[nPoints];
    sol[0] = 0; //fix the first point so it doesn't try shifted or inverted solutions
    buildPath(sol, 0, 1, points, nPoints);

    cout << "[ ";
    for(i = 0; i < nPoints; i++)
        cout << bestSolution[i] << ' ';
    cout << "] " << bestDistance << " m" << endl;

    return 0;
}

Point * importPoints(string filePath, int &nPoints){
    fstream pointsFile;
    string line;
    int i;
    static Point points[MAX_PUNTI];

    // file opening;
    pointsFile.open(filePath, ios::in);
    if(!pointsFile.is_open()){
        std::cout << "Error file not opened!" << std::endl;
        return NULL;
    }

    // read points number
    getline(pointsFile, line);
    nPoints = stoi(line);

    // read single point
    for(i = 0; i < nPoints; i++){
        getline(pointsFile, line);

        stringstream ss(line);
        string coordinate;
        int j = 1;

        while (!ss.eof() && j < 3) {
            getline(ss, coordinate, ',');

            if(j == 1)
                points[i].x = stof(coordinate);
            else if(j == 2)
                points[i].y = stof(coordinate);

            j++;
        }

        points[i].id = i;
    }

    pointsFile.close();
    return points;
}

void buildPath(int *sol, float distance, int index, Point *points, int nPoints){
    if(index == nPoints){
        int i;
        distance += sqrt(pow(points[sol[index - 1]].x - points[0].x, 2) + pow(points[sol[index - 1]].y - points[0].y, 2));

        if(distance < bestDistance){
            bestDistance = distance;
            bestSolution = cloneArray(sol, nPoints);
        }
        return;
    }

    int i_point; //avoid the point 0, start from 1
    for(i_point = 1; i_point < nPoints; i_point++){
        if(!points[i_point].isVisited) {
            //calculate distance
            float thisDistance = sqrt(pow(points[sol[index - 1]].x - points[i_point].x, 2) + pow(points[sol[index - 1]].y - points[i_point].y, 2));

            sol[index] = i_point;
            points[i_point].isVisited = true;

            buildPath(sol, distance + thisDistance, index + 1, points, nPoints);
            points[i_point].isVisited = false;
        }
    }
}

int *cloneArray(int* array, int n){
    static int newArray[MAX_PUNTI];

    for(int i = 0; i < n; i++)
        newArray[i] = array[i];

    return newArray;
}