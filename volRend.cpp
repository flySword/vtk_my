// maybe a bug in vtk, this is a inefficient realise

#include "vtkSmartPointer.h"
#include "vtkImageReader2.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include "vtkImageMapper3D.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPNGReader.h"
#include "vtkImageViewer2.h"
#include "vtkImageBlend.h"
#include "vtkPNGWriter.h"
#include "vtkInformation.h"
#include "MyVtkTool.h"




int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "input parameters error" << endl;
	}

	vtkImageData *imageData =  MyVtkTool::getImageDataFromFile(argv[1]);
	vtkRenderer *renderer = MyVtkTool::volumeRenderer(imageData);
	vtkRenderWindow *renWin = vtkRenderWindow::New();


	renWin->AddRenderer(renderer);
	renWin->SetPosition(50, 50);
	renWin->Render();

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);
	iren->SetRenderWindow(renWin);
	iren->Start();

	imageData->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	style->Delete();

	return 0;

}