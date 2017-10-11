





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

int main()
{

    vtkPNGReader *reader1 = vtkPNGReader::New();
    reader1->SetFileName("surf0.txt.png");
    reader1->Update();
    vtkPNGReader *reader2 = vtkPNGReader::New();
    reader2->SetFileName("surf1.txt.png");
    reader2->Update();
    vtkPNGReader *reader3 = vtkPNGReader::New();
    reader3->SetFileName("surf2.txt.png");
    reader3->Update();


    vtkImageData *imgData1 = reader1->GetOutput();
    vtkImageData *imgData2 = reader2->GetOutput();
    vtkImageData *imgData3 = reader3->GetOutput();

    vtkImageBlend *blend = vtkImageBlend::New();
    blend->AddInputData(imgData1);
    blend->SetOpacity(0,0.5);
    blend->AddInputData(imgData2);
    blend->SetOpacity(1,0.5);
    blend->AddInputData(imgData3);
    blend->SetOpacity(2,0.5);
	blend->Update();

	vtkSmartPointer<vtkPNGWriter> writer =
		vtkSmartPointer<vtkPNGWriter>::New();
	writer->SetFileName("out.png");
	writer->SetInputConnection(blend->GetOutputPort());
	writer->Write();

 //   vtkImageViewer2 *imageViewer =vtkImageViewer2::New();  
 //   imageViewer->SetInputConnection(blend->GetOutputPort());  
      
   // vtkRenderWindowInteractor *Interactor =vtkRenderWindowInteractor::New();  
    //imageViewer->SetupInteractor(Interactor);  
 //   imageViewer->Render();  
      
    // imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);  
    // imageViewer->GetRenderWindow()->SetWindowName("Display");  
    // Interactor->Initialize();  
    // Interactor->Start();  

    return 0;
}