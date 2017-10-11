#include "MyVtkTool.h"
#include "vtkUnsignedCharArray.h"
#include "vtkInformation.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkVolume.h"

#include <math.h>


using namespace std;

MyVtkTool::MyVtkTool()
{
}

MyVtkTool::~MyVtkTool()
{
}

void MyVtkTool::generateVolData_gradual(std::string filename, int xDim, int yDim, int zDim)
{
	//vector<vector<vector<int> > > volume;
	//volume.resize(z);

	ofstream fout(filename);
	fout << "origin 0 0 0" << endl;
	fout << "dimensions " << xDim << " " << yDim << " " << zDim << endl;
	int max = xDim + yDim + zDim;

	for (int i = 0; i < zDim; i++)
	{
		//volume[i].resize(y);
		for (int j = 0; j < yDim; j++)
		{
			//volume[i][j].resize(x);
			for (int k = 0; k < xDim; k++)
			{
				//volume[i][j][k] = i + j + k;
				fout << (i + j + k) * 100 / max << endl;
			}
		}
	}
}

void MyVtkTool::generateVolData_sinSurf(std::string filename, int xDim, int yDim, int zDim)
{

	ofstream fout(filename);
	fout << "origin 0 0 0" << endl;
	fout << "dimensions " << xDim << " " << yDim << " " << zDim << endl;
	int tmInt;
	int max = 100;

	#pragma omp parallel for  
	for (int i = 0; i < zDim; i++)
	{
		for (int j = 0; j < yDim; j++)
		{
			for (int k = 0; k < xDim; k++)
			{
				tmInt = 100 * pow(2, (-0.02) * pow((float)(i - xDim/2.0*sin(k*3.14/xDim) - yDim/2.0*sin(j*3.14/yDim)), 2));
				fout << tmInt << endl;
			}
		}
	}
}

vtkImageData *MyVtkTool::getImageDataFromFile(std::string filename)
{

	ifstream fin(filename);
	string str_tm;
	double xOrigin, yOrigin, zOrigin;
	fin >> str_tm >> xOrigin >> yOrigin >> zOrigin;

	int xDim, yDim, zDim;
	fin >> str_tm >> xDim >> yDim >> zDim;

	vtkImageData *imageData = vtkImageData::New();
	imageData->SetDimensions(xDim, yDim, zDim);
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
	imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);
	imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
	imageData->SetSpacing(1.0, 1.0, 1.0);
	imageData->SetOrigin(xOrigin, yOrigin, zOrigin);

	// warning, cin>> (char) will cause ascll coding
	unsigned short tm;
	unsigned char *ptr = (unsigned char *)imageData->GetScalarPointer();
	for (int i = 0; i < xDim * yDim * zDim; i++)
	{
		fin >> tm;
		*ptr++ = tm;
	}
	//info->Delete();

	return imageData;
}

void MyVtkTool::volumeSplitByZ(std::string filename, int splitNum)
{
	ifstream fin(filename);
	string str_tm;
	double xOrigin, yOrigin, zOrigin;
	fin >> str_tm >> xOrigin >> yOrigin >> zOrigin;
	int xDim, yDim, zDim;
	fin >> str_tm >> xDim >> yDim >> zDim;

	string suffix, trimSuffix;
	if (filename.find_last_of('.') == -1 || filename.find_last_of('.') > 10000)
	{
		suffix = "";
		trimSuffix = filename;
	}
	else
	{
		suffix = filename.substr(filename.find_last_of('.') + 1);
		trimSuffix = filename.substr(0, filename.find_last_of('.'));
	}

	//cout << filename.find_last_of('.') << endl;
	//cout << filename.substr(0, filename.find_last_of('.')) << endl;
	//cout << filename.substr(filename.find_last_of('.') + 1) << endl;

	int zSplitSize = zDim / splitNum;

	for (int fileNum = 0; fileNum < splitNum; fileNum++)
	{

		int startZ = fileNum * zSplitSize;
		int endZ;
		if (fileNum == splitNum - 1)
			endZ = zDim;
		else
			endZ = (fileNum + 1) * zSplitSize;

		ofstream fout(trimSuffix + to_string(fileNum) + "." + suffix);
 
		fout << "origin 0 0 " << startZ <<endl;
		fout << "dimensions " << xDim << " " << yDim << " " << endZ - startZ << endl;

		int tm;
		for (int i = startZ; i < endZ; i++)
		{
			for (int j = 0; j < yDim; j++)
			{
				for (int k = 0; k < xDim; k++)
				{
					fin >> tm;
					fout << tm << endl;
				}
			}
		}
		fout.close();
	}
}



void MyVtkTool::write2image(vtkRenderWindow *renWin, string filename)
{
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
		vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renWin);
	//windowToImageFilter->SetMagnification(3);		 //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOn();		 // read from the back buffer
	windowToImageFilter->ShouldRerenderOff();
	//windowToImageFilter->Update();

	vtkSmartPointer<vtkPNGWriter> writer =
		vtkSmartPointer<vtkPNGWriter>::New();
	writer->SetFileName(filename.c_str());
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();
}


vtkRenderer *MyVtkTool::volumeRenderer(vtkImageData *imageData)
{

	//设定体数据中不同标量值的透明度
	vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	//标量值为 20 的点透明
	opacityTransferFunction->AddPoint(0, 0);
	opacityTransferFunction->AddPoint(100, 0.4);
	//opacityTransferFunction->AddPoint(8, 0);

	//设定不同标量值的颜色
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint(0.0, 0.1, 0.91, 0.0);
	colorTransferFunction->AddRGBPoint(50.0, 0.99, 0.99, 0.1);
	colorTransferFunction->AddRGBPoint(100.0, 0.9, 0.1, 0.1);

	//设定体数据的属性
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);

	//显示阴影 与阴影相关参数
	volumeProperty->ShadeOff();
	volumeProperty->SetDiffuse(0.7);  //散射光系数
	volumeProperty->SetAmbient(0.8);  //环境光系数
	volumeProperty->SetSpecular(0.5); //镜面光系数（反射？）
	volumeProperty->SetSpecularPower(10.0);

	//设定插值类型为线性插值
	volumeProperty->SetInterpolationTypeToLinear();

	////////设定体数据映射器 设定体数据映射器 设定体数据映射器 设定体数据映射器
	//vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New();
	//vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetInputData(imageData);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// Create the renderer, render window and interactor
	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddVolume(volume);
	renderer->ResetCamera();
	renderer->SetBackground(1, 1, 1);
	return renderer;
}

vtkCamera * MyVtkTool::getCameraFromFile(std::string filename)
{
	ifstream fin(filename);
	double cameraPos[3];
	fin >> cameraPos[0] >> cameraPos[1] >> cameraPos[2];
	double focal[3];
	fin >> focal[0] >> focal[1] >> focal[2];

	vtkCamera *camera = vtkCamera::New();
	camera->SetPosition(cameraPos);
	camera->SetFocalPoint(focal);

	return camera;
}

// simple test
//int main()
//{
//	//MyVtkTool::generateVolumeData("volume_simple.txt", 50, 50, 50);
//
//	MyVtkTool::generateVolData_sinSurf("surf.txt", 50, 50, 50);
//	MyVtkTool::volumeSplitByZ("surf.txt", 3);
//	return 0;
//}