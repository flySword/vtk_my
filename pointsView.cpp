/*========================================

����vtkPolyData������ʱ������������꣬����Ҫͨ��setVertsָ���������Ϣ��

simplePoints.txt������Ϊ�򵥵� xyz���磺
20 20 20
20 20 30
20 20 40
20 30 30

�ο�vtkSimplePointsReader������λ��legacy�ļ����£�������ֱ��ʹ��
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
		//��������꣬�˴��ɸ�Ϊ������xyz
		idtype = points->InsertNextPoint(x,y,z);  
		cells->InsertNextCell(1, &idtype);
	}


	// ��Ⱦ����δ֪����Ҫͬʱ���õ�������������Ӧ��verts
	// verts�е�id������������Ӧ
	vtkPolyData *polyData = vtkPolyData::New();
	polyData->SetPoints(points);
	polyData->SetVerts(cells);

	//����Ϊ�����Ŀ��ӻ����̣������õĵ�����ɫ����С����ע��
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polyData);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	//������ɫ����С
	//actor->GetProperty()->SetColor(0.0, 0.0, 0.0);  
	//actor->GetProperty()->SetPointSize(2);


	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddActor(actor);
	// ���ñ�����ɫ
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
