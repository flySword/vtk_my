#include <iostream>
#include <stdio.h>

#include "vtkImageData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPointData.h"
#include "vtkType.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPoints.h"
#include "vtkUniformGrid.h"
#include "vtkInformation.h"
#include "vtkImageDataStreamer.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkStructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkStructuredPointsReader.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"



int main(int argc, char *argv[])
{


	int xDim, yDim, zDim;
	ifstream fin("volume_simple.txt");
	fin >> xDim >> yDim >> zDim;

	// warning!!!!  
	// 此处读入数据为char型，因此读入的1存储为ascii码的1，实际数据为49！！！！
	unsigned char *data_src = new unsigned char[xDim*yDim*zDim];
	int tm;
	for (int i = 0; i < xDim*yDim*zDim; i++) {
		fin >> tm;
		data_src[i] = tm;
	}
	vtkUnsignedCharArray *array = vtkUnsignedCharArray::New();
	array->SetVoidArray(data_src, xDim*yDim*zDim, 0);


	vtkImageData *imageData = vtkImageData::New();
	imageData->GetPointData()->SetScalars(array);
	imageData->SetDimensions(xDim, yDim, zDim);
	vtkInformation *info = vtkInformation::New();
	imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);
	imageData->SetSpacing(1.0, 1.0, 1.0);
	imageData->SetOrigin(0.0, 0.0, 0.0);


	//设定体数据中不同标量值的透明度 
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
	//标量值为 20 的点透明
	opacityTransferFunction->AddPoint(0, 0);
	opacityTransferFunction->AddPoint(100, 0.5);
	//opacityTransferFunction->AddPoint(8, 0);


	//opacityTransferFunction->AddPoint(4, 0);
	//设定不同标量值的颜色 
	vtkColorTransferFunction *colorTransferFunction =
		vtkColorTransferFunction::New();
	colorTransferFunction->AddRGBPoint(0.0, 0.2, 0.3, 0.7);
	colorTransferFunction->AddRGBPoint(50.0, 0.8, 0.8, 0.8);
	colorTransferFunction->AddRGBPoint(100.0, 0.7, 0.1, 0.1);

	//设定体数据的属性 
	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);

	//显示阴影 与阴影相关参数
	volumeProperty->ShadeOn();
	volumeProperty->SetDiffuse(0.7);//散射光系数
	volumeProperty->SetAmbient(0.8);//环境光系数
	volumeProperty->SetSpecular(0.5);//镜面光系数（反射？）
	volumeProperty->SetSpecularPower(30.0);

	//设定插值类型为线性插值 
	volumeProperty->SetInterpolationTypeToLinear();


	////////设定体数据映射器 设定体数据映射器 设定体数据映射器 设定体数据映射器                   
	//vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New(); 
	//vtkFixedPointVolumeRayCastMapper *volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
	vtkSmartVolumeMapper *volumeMapper = vtkSmartVolumeMapper::New();

	volumeMapper->SetInputData(imageData);

	//vtkVolumeRayCastCompositeFunction *compositeFunction = vtkVolumeRayCastCompositeFunction::New();
	//volumeMapper->SetVolumeRayCastFunction(compositeFunction);



	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	// Create the renderer, render window and interactor
	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddVolume(volume);
	renderer->ResetCamera();
	renderer->SetBackground(1, 1, 1);

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renWin->SetPosition(100, 100);
	renWin->SetSize(300, 300);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	// interact with data
	renWin->Render();
	iren->Start();


	delete data_src;
	opacityTransferFunction->Delete();
	colorTransferFunction->Delete();
	volume->Delete();
	volumeMapper->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}