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
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkInteractorStyleJoystickActor.h"


#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkWindowToImageFilter.h"
#include "vtkBMPWriter.h"
#include "vtkCubeSource.h"
#include "vtkPolyDataMapper.h"

#include "MyVtkTool.h"
#include <string>
using namespace std;


// using by: vol2img volumeDataFile cameraDataFile
// reslute file is volumeDataFile.png


int main(int argc, char *argv[])
{
	vtkRenderer *renderer = MyVtkTool::volumeRenderer(MyVtkTool::getImageDataFromFile(argv[1]));
	renderer->SetActiveCamera(MyVtkTool::getCameraFromFile(argv[2]));
	
	////添加外边框以更清楚每个块的位置
	//vtkCubeSource *cube = vtkCubeSource::New();
	////cube->SetBounds(0,0,0,50,50,50);
	//cube->SetCenter(25, 25, 25);
	//cube->SetXLength(10);
	//cube->SetYLength(10);
	//cube->SetZLength(50);
	//vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
	//cubeMapper->SetInputConnection(cube->GetOutputPort());
	//vtkActor *cubeActor = vtkActor::New();
	//cubeActor->SetMapper(cubeMapper);
	//renderer->AddActor(cubeActor);

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renWin->Render();
	string str(argv[1]);
	MyVtkTool::write2image(renWin, str+".png" );
	
	renderer->Delete();
	renWin->Delete();

	return 0;
}
