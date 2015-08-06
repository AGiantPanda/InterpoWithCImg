#include <Windows.h>

#include "CImg.h"
#include "interpo.hpp"

using namespace cimg_library;

#define FILENAME "another_16bit.raw"
#define XMAX 512
#define YMAX 512
#define col 1
#define TYPE unsigned short
#define DISP_SIZE 500

int main(int argc, char* argv[])
{		
	//CImg<unsigned char> raw("lena.bmp");
	TYPE *pImg = FileRead<TYPE>(FILENAME, XMAX, YMAX, col);

	CImg<unsigned char> raw;
	if (col == 3){
		CImg<unsigned char> tmp(RGBtoPlanar<TYPE>(pImg, XMAX*YMAX), XMAX, YMAX, 1, col);
		raw = tmp;
	}
	else{
		CImg<unsigned char> tmp(pImg, XMAX, YMAX, 1, col);
		raw = tmp;
	}
	CImgDisplay disp(raw, FILENAME);
	disp.move(15, 35);

	//image_zoomer(raw, &disp, XMAX, YMAX);
	CImgDisplay dispz(DISP_SIZE, DISP_SIZE, "Cubic Zoomed img", 0);
	dispz.move(disp.width() + 35, 35);
	//CImgDisplay diss(DISP_SIZE, DISP_SIZE, "dd", 0);
	int mouse_x = 0, mouse_y = 0,factor = 185;

	//
	//unsigned char *pp = FileRead<unsigned char>("another_8bit.raw", 512, 512, 1);
	//CImg<unsigned char> sa(pp, 512, 512, 1, 1);
	//CImgDisplay dispn(DISP_SIZE, DISP_SIZE, "second", 0);
	//dispn.move(disp.width() + 100, 35);
	//
	while (!disp.is_closed() && !disp.is_keyESC()){
		if (disp.mouse_x() >= 0){
			mouse_x = disp.mouse_x(); mouse_y = disp.mouse_y();
			CImg<unsigned char> visu;
			if (disp.button())
			{
				while (disp.button() & 1){
					factor -= 1;
					if (factor < 3)factor = 3;
					//if (col == 3){
					//	CImg<unsigned char> tmp(RGBtoPlanar<TYPE>(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE*DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
					//	visu = tmp; visu.display(dispz);
					//}
					//else{
					//	CImg<unsigned char> tmp(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
					//	visu = tmp; visu.display(dispz);
					//}
					visu = imgZooming(raw, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispz);
					//visu = imgZooming(sa, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispn);

					//上面这两个语句应该是多余操作，因为没有找到如何对已经初始化的CImg类型变量重新设置长宽图像等参数的函数，不过这个不影响，主要是返回数组的函数可用
					//imgZooming(raw, mouse_x, mouse_y, factor, 300, 400).display(dispz);
				}
				while (disp.button() & 2)
				{
					factor += 1;
					if (factor > 100)factor = 100;
					//if (col == 3){
					//	CImg<unsigned char> tmp(RGBtoPlanar<TYPE>(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE*DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
					//	visu = tmp; visu.display(dispz);
					//}
					//else{
					//	CImg<unsigned char> tmp(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
					//	visu = tmp; visu.display(dispz);
					//}
					visu = imgZooming(raw, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispz);
					//visu = imgZooming(sa, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispn);

					//imgZooming(raw, mouse_x, mouse_y, factor, 300, 400).display(dispz);
				}
				disp.set_button();
			}
			else{
				//if (col == 3){
				//	CImg<unsigned char> tmp(RGBtoPlanar<TYPE>(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE*DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
				//	visu = tmp; visu.display(dispz);
				//}
				//else{
				//	CImg<unsigned char> tmp(imgZooming<TYPE>(pImg, XMAX, YMAX, mouse_x, mouse_y, factor, col, DISP_SIZE, DISP_SIZE), DISP_SIZE, DISP_SIZE, 1, col);
				//	visu = tmp; visu.display(dispz);
				//}
				//imgZooming(raw, mouse_x, mouse_y, factor, 300, 400).display(dispz);
				visu = imgZooming(raw, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispz);
				//visu = imgZooming(sa, mouse_x, mouse_y, factor, DISP_SIZE, DISP_SIZE).display(dispn);

			}
		}
		CImgDisplay::wait(disp);
	}
	return 0;
}