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



// input multiple files, and output a image named "blend.png"
int main(int argc, char *argv[])
{

	vtkImageData **imgDatas = new vtkImageData *[argc - 1];

	//vtkSmartPointer<vtkImageBlend> imgBlend = vtkSmartPointer<vtkImageBlend>::New();
	for (int i = 1; i != argc; i++)
	{
		vtkPNGReader *reader = vtkPNGReader::New();
		reader->SetFileName(argv[i]);
		//vtkSmartPointer<vtkImageData> imgData = vtkSmartPointer<vtkImageData>::New();
		imgDatas[i - 1] = reader->GetOutput();
		reader->Update();
	}

	int *dims = new int[3];
	imgDatas[0]->GetDimensions(dims);

	// image data attributes
	cout << dims[0] << " " << dims[1] << " " << dims[2] << endl;
	cout << imgDatas[0]->GetNumberOfScalarComponents() << endl;
	cout << (imgDatas[0]->GetScalarType() == VTK_UNSIGNED_CHAR) << endl;

	//
	vtkImageData *imgBlend = vtkImageData::New();
	imgBlend->SetDimensions(dims);
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();

	imgBlend->SetScalarType(VTK_UNSIGNED_CHAR, info);
	imgBlend->SetNumberOfScalarComponents(4, info);
	imgBlend->AllocateScalars(VTK_UNSIGNED_CHAR, 4);
	unsigned char *ptrBlend = (unsigned char *)imgBlend->GetScalarPointer();


	for (int i = 0; i < dims[1]; i++)
	{
		for (int j = 0; j < dims[0]; j++)
		{
			int cnt = 0;
			int r = 0;
			int g = 0;
			int b = 0;
			int a = 0;
			for (int idx = 0; idx != argc - 1; idx++)
			{
				if (imgDatas[idx]->GetScalarType() == VTK_UNSIGNED_CHAR)
				{
					unsigned char rt, bt, gt, at;
					rt = *(unsigned char *)imgDatas[idx]->GetScalarPointer(j, i, 0);  
					gt = *((unsigned char *)imgDatas[idx]->GetScalarPointer(j, i, 0) + 1);
					bt = *((unsigned char *)imgDatas[idx]->GetScalarPointer(j, i, 0) + 2);
					at = *((unsigned char *)imgDatas[idx]->GetScalarPointer(j, i, 0) + 3);
					// there is a efficiency problem, 
					// but it seems a bug in the direct data reading through the scalarPointer
					// 

					if (rt == 255 && gt == 255 && bt == 255)
					{
						continue;
					}
					else
					{
						r += rt;
						g += gt;
						b += bt;
						a += at;

						cnt++;
					}
				}

			}

			if (cnt != 0)
			{
				ptrBlend[i*dims[0]*4 + j*4] = r / cnt;
				ptrBlend[i*dims[0]*4 + j*4 + 1] = g / cnt;
				ptrBlend[i*dims[0]*4 + j*4 + 2] = b / cnt;
				ptrBlend[i*dims[0]*4 + j*4 + 3] = a / cnt;
			}
			else
			{
				ptrBlend[i*dims[0]*4 + j*4] = 255;
				ptrBlend[i*dims[0]*4 + j*4 + 1] = 255;
				ptrBlend[i*dims[0]*4 + j*4 + 2] = 255;
				ptrBlend[i*dims[0]*4 + j*4 + 3] = 255;
			}

		}
	}



	vtkSmartPointer<vtkPNGWriter> writer =
		vtkSmartPointer<vtkPNGWriter>::New();
	writer->SetFileName("blend.png");
	writer->SetInputData(imgBlend);
	writer->Write();
	//

	return 0;
}