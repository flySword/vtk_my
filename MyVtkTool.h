#pragma once

#ifndef MY_VTK_TOOL
#define MY_VTK_TOOL

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

class MyVtkTool
{
public:
	MyVtkTool();
	~MyVtkTool();

	// generte a gradual volume data file named <filename>
	static void generateVolData_gradual(std::string filename, int xDim, int yDim, int zDim);

	//  generte a sinuous surface volume data file named <filename>
	static void generateVolData_sinSurf(std::string filename, int xDim, int yDim ,int zDim);	

	// read the simple txt file named <filename>
	// store the data in a vtkImageData as return value
	static vtkImageData* getImageDataFromFile(std::string filename);

	// split the volume file to <splitNum> pieces by z direction
	static void volumeSplitByZ(std::string filename, int splitNum);

	// write current window screenshot to a image  
	static void write2image(vtkRenderWindow *renWin, std::string filename);

	// renderring setting for volume data
	static vtkRenderer *volumeRenderer(vtkImageData *imageData);

	static vtkCamera *getCameraFromFile(std::string file);


	virtual void renderActor() = 0;

};




#endif // MY_VTK_TOOL



