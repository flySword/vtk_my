[TOC]

��vtk��������Ⱦ��Ҫ�ĸ��ֹ��ܵļ�ʵ�֣������򵥵ķ�װ

exe�ļ�����debugģʽ���޴����Ż�����

## �򵥴���

### �򵥵Ļ�������

vtkʾ�����룺

- cone.cxx
- cone2.cxx
  ....



### �������ݻ���

simpleVolumeRendering.cpp

��ȡ�������ļ�volume_simple.txt������Ⱦ��ʾ����ֱ�ӵ�imageData��ȡ��������MyVtkTool.h��



### ͼ���ں�

imageBlend.cpp

�������3��ͼ������ںϣ�����vtkImageBlend����д�������ʱû�к���ͼ��Ŀհײ��֣����ºϲ���ͼ����ɫƫ�ߡ�





### CMakeLists.txt

����vtk·����vtkģ�顢

### ���ù��߼�

- ��ȡ��txt�ļ��е����ݣ��������vtkImageData
- ���ɼ򵥵Ľ��䡢����������
- ��������z����ָ�Ϊ�������
- ��renderWindows����Ⱦ��ͼд��png�ļ�
- �Զ����vtkImageData�������ã�����vtkRenderer
- ���ļ��ж�ȡ�������

MyVtkTool.cpp
MyVtkTool.h



---

## Ӧ�ó��򹤾�

### ��������Ⱦ��ʾ

����MyVtkTool�е��ļ���ȡ����Ⱦ�������м򵥵������� **��Ⱦ��ʾ**

volRend.cpp

### ֱ����Ⱦ������Ϊͼ��

���룺volume data�ļ���surf.txt�������������camera.txt��
�������Ⱦ���ͼƬ

���������ڵ�ǰ����Ƕ�����ȾΪͼƬ����Ⱦʱ���ڻ�����ʾȻ��رա�

vol2img.cpp    

### ͼ���ں�

���룺���ͼƬ�ļ�
������ںϺ��ͼƬ

imgBlend.cpp

������png��ʽͼ�񣬽���rgba�ļ�Ȩ�ںϣ����԰�ɫͼ��Ĳ��֣���

����vtk�����غϳɣ�Ч��ƫ�ͣ����ƻ�����Python�������ٶȡ���Ҫ����ȡÿ��Ԫ��ʱ��ֱ�ӻ���ָ����ô���bug��



### �������ļ����ɹ���

volDataGenerate.cpp

ʹ��MyVtkTool::generateVolData_sinSurf()���ɴ���������`1000*1000*1000`��ʱ����ʱ��ϳ���



### �Զ���������ļ���Ⱦ��ʾ

multiRender.cpp   TODO  ��ʱ����

### �򵥵�����ʾ

pointsView.cpp