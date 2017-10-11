/*========================================

基于vtkPolyData，绘制时除了输入点坐标，还需要通过setVerts指定点绘制信息。

simplePoints.txt的内容为简单的 xyz，如：
20 20 20
20 20 30
20 20 40
20 30 30

参考vtkSimplePointsReader，此类位于legacy文件夹下，不建议直接使用
==========================================*/



// VTK includes
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyle.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCellArray.h"
#include "vtkInteractorStyleTrackballCamera.h"



int main(int argc, char *argv[])
{
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *cells = vtkCellArray::New();

	if (argc != 2) 
	{
		cerr << "input argv error" << endl;
		return 1;
	}

	ifstream fs(argv[1]);
	if (!fs)
	{
		cerr << "file open error!" << endl;
		return 1;
	}
	//ifstream fs("simplePoints.txt");
	vtkIdType idtype;
	double x, y, z;
	while(fs>>x>>y>>z){
		//插入点坐标，此处可改为其它的xyz
		idtype = points->InsertNextPoint(x,y,z);  
		cells->InsertNextCell(1, &idtype);
	}


	// 渲染机制未知，需要同时设置点坐标与点坐标对应的verts
	// verts中的id必须与点坐标对应
	vtkPolyData *polyData = vtkPolyData::New();
	polyData->SetPoints(points);
	polyData->SetVerts(cells);

	//下面为正常的可视化流程，可设置的点云颜色、大小等已注释
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polyData);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	//设置颜色与点大小
	//actor->GetProperty()->SetColor(0.0, 0.0, 0.0);  
	//actor->GetProperty()->SetPointSize(2);


	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddActor(actor);
	// 设置背景颜色
	// renderer->SetBackground(1, 1, 1);

	vtkRenderWindow *renderWindow = vtkRenderWindow::New();
	renderWindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renderWindow);

	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();

	points->Delete();
	polyData->Delete();
	mapper->Delete();
	actor->Delete();
	renderer->Delete();
	renderWindow->Delete();
	iren->Delete();
	style->Delete();

	return 0;
}
