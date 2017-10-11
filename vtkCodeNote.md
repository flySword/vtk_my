[TOC]

对vtk体数据渲染需要的各种功能的简单实现，包括简单的封装

exe文件基于debug模式与无代码优化编译

## 简单代码

### 简单的绘制流程

vtk示例代码：

- cone.cxx
- cone2.cxx
  ....



### 简单体数据绘制

simpleVolumeRendering.cpp

读取简单数据文件volume_simple.txt进行渲染显示，有直接的imageData读取，不依赖MyVtkTool.h。



### 图像融合

imageBlend.cpp

对输入的3个图像进行融合，基于vtkImageBlend类进行处理，处理时没有忽略图像的空白部分，导致合并的图像颜色偏高。





### CMakeLists.txt

设置vtk路径、vtk模块、

### 自用工具集

- 读取简单txt文件中的数据，结果存入vtkImageData
- 生成简单的渐变、曲面体数据
- 体数据以z坐标分割为多块数据
- 将renderWindows的渲染截图写入png文件
- 对读入的vtkImageData进行设置，返回vtkRenderer
- 从文件中读取相机参数

MyVtkTool.cpp
MyVtkTool.h



---

## 应用程序工具

### 体数据渲染显示

调用MyVtkTool中的文件读取与渲染参数进行简单的体数据 **渲染显示**

volRend.cpp

### 直接渲染体数据为图像

输入：volume data文件（surf.txt）、相机参数（camera.txt）
输出：渲染后的图片

将体数据在当前相机角度下渲染为图片，渲染时窗口会先显示然后关闭。

vol2img.cpp    

### 图像融合

输入：多个图片文件
输出：融合后的图片

imgBlend.cpp

输入多个png格式图像，进行rgba的加权融合（忽略白色图像的部分）。

基于vtk的像素合成，效率偏低，估计还不如Python的运行速度。主要是在取每个元素时，直接基于指针调用存在bug。



### 体数据文件生成功能

volDataGenerate.cpp

使用MyVtkTool::generateVolData_sinSurf()生成大数据量（`1000*1000*1000`）时计算时间较长。



### 对多个体数据文件渲染显示

multiRender.cpp   TODO  暂时不管

### 简单点云显示

pointsView.cpp