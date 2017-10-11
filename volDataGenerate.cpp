#include "MyVtkTool.h"
#include <string>
using namespace std;


int main(int argc, char *argv[])
{
	if (argc != 5) {
		cout << "error number of parameters" << endl;
		return 2;
	}

	int xDim = atoi(argv[1]);
	int yDim = atoi(argv[2]);
	int zDim = atoi(argv[3]);
	int splitSize = atoi(argv[4]);

	MyVtkTool::generateVolData_sinSurf("volData.txt", xDim, yDim, zDim);
	cout << "volData generate over" << endl;
	cout << "split volData ......" << endl;
	MyVtkTool::volumeSplitByZ("volData.txt", splitSize);
	
	return 0; 
}