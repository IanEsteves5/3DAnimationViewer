/*
 * 3D Animation Viewer
 * By Ian Esteves do Nascimento, 2013
 */

#include <string>
#include <vector>
#include <fstream>
#include "myUti.h"
#include "objModel.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

// Reads an .obj file.
int objModel::import(string fileName){
   #ifdef DEBUG
   cout << "DEBUG starting for file " << fileName << endl;
   #endif
   ifstream objFile(fileName.c_str());
   if(!objFile.is_open()) // Returns if file can't be read.
      return 1;
   #ifdef DEBUG
   cout << "Opened " << fileName << " successfully." << endl;
   #endif
   vector<vector3d> vertex; // Data for all of the vertexes.
   while(!objFile.eof()){
      string line, lineType;
      int linePos;
      getline(objFile, line);
      linePos = line.find(' ');
      if(linePos == string::npos) // Ignore empty lines.
         continue;
      lineType = line.substr(0, linePos); // First word of a line is its type.
      line = line.substr(linePos+1);
      if(lineType == "v"){ // New vertex.
         vector3d newV;
         linePos = line.find(' '); // Reads 3 values x, y and z.
         newV.setX(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         linePos = line.find(' ');
         newV.setY(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         newV.setZ(fromStr<double>(line));
         vertex.push_back(newV); // Adds vertex to the list.
         #ifdef DEBUG
         cout << "vertex " << (string)newV << endl;
         #endif
         continue;
      }
      if(lineType == "f"){ // New face.
         vector<vector3d> newF; // A face is a list of vertexes.
         #ifdef DEBUG
         cout << "face   ";
         #endif
         for(linePos = line.find(' ') ; linePos != string::npos ; linePos = line.find(' ')){ // A face can have any number of vertexes.
            int index = fromStr<int>(line.substr(0, linePos));
            newF.push_back(vertex[index-1]); // Adds vertex to the face.
            #ifdef DEBUG
            cout << index << ", ";
            #endif
            line = line.substr(linePos+1);
         }
         newF.push_back(vertex[fromStr<int>(line)-1]);
         #ifdef DEBUG
         cout << fromStr<int>(line) << endl;
         #endif
         push_back(newF); // Adds face to the objModel.
         continue;
      }
   }
   objFile.close();
   #ifdef DEBUG
   for(int i = 0 ; i < vertex.size() ; i++)
      cout << "vertex(" << i << ")=" << (string)vertex[i] << endl;
   for(int i = 0 ; i < size() ; i++){
      cout << "face(" << i << ")=";
      for(int j = 0 ; j < at(i).size() ; j++)
         cout << (string)at(i)[j];
      cout << endl;
   }
   cout << "DEBUG ending for file " << fileName << endl;
   #endif
   return 0;
}
