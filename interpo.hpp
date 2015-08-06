#pragma once
#include <iostream>
#include <typeinfo>

#include "CImg.h"

////! Byte swap short
//short swap_short(short val)
//{
//	return (val << 8) | ((val >> 8) & 0xFF);
//}

template<typename T> T *RGBtoPlanar(T *origin, int imgsize){
	T *tmp = new T[imgsize * 3];
	for (int i = 0; i < imgsize; i++){
		tmp[imgsize * 0 + i] = origin[i * 3 + 0];
		tmp[imgsize * 1 + i] = origin[i * 3 + 1];
		tmp[imgsize * 2 + i] = origin[i * 3 + 2];
	}
	return tmp;
}

template<typename T> T *FileRead(const char*FileName, int XMAX, int YMAX, int col_t = 1){
	FILE *pFile = std::fopen(FileName, "rb");
	if (NULL == pFile){
		std::cout << "read failed1" << std::endl;
		exit(0);
	}
	int imgsize = XMAX*YMAX*col_t;
	T *pImg = new T[imgsize];
	short *type;
	unsigned short *type2;
	bool readok = (imgsize == fread(pImg, sizeof(T), imgsize, pFile));
	if (!readok){
		std::cout << "read failed2" << std::endl;
		exit(0);
	}
	std::fclose(pFile);
	
	//大小端转换以及强度最大最小值映射
	if (typeid(pImg) == typeid(type) || typeid(pImg)==typeid(type2)) {
		int low = 1000, high = 0;
		for (int i = 0; i < imgsize; i++){
			pImg[i] = (pImg[i] << 8) | ((pImg[i] >> 8) & 0xFF);
			low = pImg[i] < low ? pImg[i] : low;
			high = pImg[i]>high ? pImg[i] : high;
		}
		for (int i = 0; i < imgsize; i++){
			pImg[i] = (pImg[i] - low) / ((high - low) / 255);
		}
	}
	//else if (col_t == 3){
	//	T *pImg3 = new T[imgsize];
	//	for (int i = 0; i < XMAX*YMAX; i++){
	//		pImg3[XMAX*YMAX * 0 + i] = pImg[i * 3 + 0];
	//		pImg3[XMAX*YMAX * 1 + i] = pImg[i * 3 + 1];
	//		pImg3[XMAX*YMAX * 2 + i] = pImg[i * 3 + 2];
	//	}
	//	return pImg3;
	//}

	return pImg;
}

//template<typename T> cimg_library::CImg<unsigned char> FileRead(const char *FileName, int XMAX, int YMAX,int no){
//	FILE *pFile = std::fopen(FileName, "rb");
//	if (NULL == pFile){
//		std::cout << "read failed1" << std::endl;
//		exit(0);
//	}
//	int imgsize = XMAX*YMAX;
//	T *pImg = new T[imgsize];
//	short *type;
//	bool readok = (imgsize == fread(pImg, sizeof(T), imgsize, pFile));
//	if (!readok){
//		std::cout << "read failed2" << std::endl;
//		exit(0);
//	}
//	std::fclose(pFile);
//
//	if (typeid(pImg)==typeid(type)) {
//		int low = 0, high = 0;
//		for (int i = 0; i < imgsize; i++){
//			pImg[i] = swap_short(pImg[i]);
//			low = pImg[i] < low ? pImg[i] : low;
//			high = pImg[i]>high ? pImg[i] : high;
//		}
//		for (int i = 0; i < imgsize; i++){
//			pImg[i] = (pImg[i] - low + 1) / ((high - low + 1) / 255);
//		}
//	}
//	cimg_library::CImg<unsigned char> raw(pImg, XMAX, YMAX);
//	return raw;
//}

bool is_mode_on(bool mode, cimg_library::CImgDisplay *disp){
	if (mode){
		disp->show();
		return true;
	}
	else{
		disp->close();
		return false;
	}
}

bool image_zoomer(cimg_library::CImg<unsigned char> raw, cimg_library::CImgDisplay *disp, int XMAX, int YMAX, int disp_size = 500){
	int factor = 20, mx = 0, my = 0, bx = 0, by = 0, begin = 0;
	bool redraw = false;
	const unsigned char red[] = { 255, 0, 0 }, black[] = { 0, 0, 0 }, white[] = { 255, 255, 255 };

	// Demo selection menu
	//---------------------
	cimg_library::CImg<unsigned char> background(250, 200,1,1,0);
	background.draw_text(1, 1, 
							"  Nearest Interpolation\n"
							"  Linear Interpolation\n"
							"  Cubic Interpolation\n"
							"  Lanczos Interpolation",
							white, 0, 1, 20);
	cimg_library::CImgDisplay back; back.display(background);
	bool Nmode = false, Lmode = false, Cmode = false, Lamode = false;

	cimg_library::CImgDisplay dispN(disp_size, disp_size, "Nearest Image", 0), dispL(disp_size, disp_size, "Linear Image", 0), dispC(disp_size, disp_size, "Cubic Image", 0), dispLa(disp_size, disp_size, "Lanczos Image", 0);
	dispN.move(550, 30); dispL.move(650, 100); dispC.move(750, 170); dispLa.move(850, 240);
	while (!disp->is_closed() && !back.is_closed() && !disp->is_keyESC() && !back.is_keyESC()){
		if (disp->mouse_x() >= 0){
			mx = disp->mouse_x(); my = disp->mouse_y(); redraw = true;
		}
		if (back.mouse_x() >= 0){
			bx = back.mouse_x(); by = back.mouse_y(); 
			cimg_library::CImg<unsigned char> visu;
			visu = background.get_crop(0, 0, back.width(), back.height());
			if (Nmode) visu.draw_text(1, 1, " *", white, 0, 1, 20);
			if (Lmode) visu.draw_text(1, 21, " *", white, 0, 1, 20);
			if (Cmode) visu.draw_text(1, 41, " *", white, 0, 1, 20);
			if (Lamode) visu.draw_text(1, 61, " *", white, 0, 1, 20);
			visu.draw_line(0, 1 + 20 * (1 + by / 20), back.width(), 1 + 20 * (1 + by / 20), white).display(back);
		}
		is_mode_on(Nmode, &dispN);
		is_mode_on(Lmode, &dispL);
		is_mode_on(Cmode, &dispC);
		is_mode_on(Lamode, &dispLa);
		
		if (redraw){
			int zoomsize = disp_size / (2 * factor);
			const int
				x0 = mx - factor, y0 = my - factor,
				x1 = mx + factor, y1 = my + factor;
			(+raw).draw_rectangle(x0, y0, x1, y1, black, 1.0f, ~0U).display(*disp);
			cimg_library::CImg<unsigned char> visu;
			
			if (!dispN.is_closed()){
				begin = GetTickCount();
				visu = raw.get_crop(x0, y0, x1, y1).resize(dispN, 1);
				visu.draw_text(2, 2, "Nearest time: %d ms\nZoom size: %d", white, 0, 1, 13, GetTickCount() - begin, zoomsize).display(dispN);
			}
			if (!dispL.is_closed()){
				begin = GetTickCount();
				visu = raw.get_crop(x0, y0, x1, y1).resize(dispL, 3);
				visu.draw_text(2, 2, "Linear time: %d ms\nZoom size: %d", white, 0, 1, 13, GetTickCount() - begin, zoomsize).display(dispL);
			}
			if (!dispC.is_closed()){
				begin = GetTickCount();
				visu = raw.get_crop(x0, y0, x1, y1).resize(disp_size,disp_size,1,-100,5);
				visu.draw_text(2, 2, "Cubic time: %d ms\nZoom size: %d", white, 0, 1, 13, GetTickCount() - begin, zoomsize).display(dispC);
			}
			if (!dispLa.is_closed()){
				begin = GetTickCount();
				visu = raw.get_crop(x0, y0, x1, y1).resize(dispLa, 6);
				visu.draw_text(2, 2, "Lanzcos time: %d ms\nZoom size: %d", white, 0, 1, 13, GetTickCount() - begin, zoomsize).display(dispLa);
			}
		}
		if (disp->button() & 1){
			factor = (int)(factor / 1.5f);
			if (factor < 3) factor = 3;
			disp->set_button(); redraw = true;
		}
		if (disp->button() & 2){
			factor = (int)(factor*1.5f);
			if (factor>100) factor = 100;
			disp->set_button(); redraw = true;
		}
		if (back.button() & 1){
			if (by > 0 && by <= 19){
				Nmode = !Nmode;
				back.set_button();
				redraw = true;
			}
			if (by > 19 && by <= 39){
				Lmode = !Lmode;
				back.set_button();
				redraw = true;
			}
			if (by > 39 && by <= 59){
				Cmode = !Cmode;
				back.set_button();
				redraw = true;
			}
			if (by > 59 && by <= 79){
				Lamode = !Lamode;
				back.set_button();
				redraw = true;
			}
			if (dispN.is_keyESC()){ Nmode = false; dispN.set_button(); redraw = true; }
			if (dispL.is_keyESC()){ Lmode = false; dispL.set_button(); redraw = true; }
			if (dispC.is_keyESC()){ Cmode = false; dispC.set_button(); redraw = true; }
			if (dispLa.is_keyESC()){ Lamode = false; dispLa.set_button(); redraw = true; }
		}
		if (disp->is_resized()) disp->resize(disp);
		if (back.is_resized()) back.resize(back);
		if (dispN.is_resized()) dispN.resize(dispN);
		if (dispL.is_resized()) dispL.resize(dispL);
		if (dispC.is_resized()) dispC.resize(dispC);
		if (dispLa.is_resized()) dispLa.resize(dispLa);
		cimg_library::CImgDisplay::wait(*disp,back,dispN,dispL,dispC,dispLa);
	}
	return true;
}

template<typename T> cimg_library::CImg<T> imgZooming(cimg_library::CImg<T> raw, int mouse_x, int mouse_y, int factor_x, int disp_x,int disp_y){
	int begin = GetTickCount();
	const unsigned char white[] = { 255, 255, 255 };

	//set zoomed img
	const int factor_y = factor_x*(float(disp_y) / float(disp_x)) + 0.5;
	const int
		x0 = mouse_x - factor_x, y0 = mouse_y - factor_y,
		x1 = mouse_x + factor_x, y1 = mouse_y + factor_y;
	cimg_library::CImg<T> visu;
	visu = raw.get_crop(x0, y0, x1, y1).resize(disp_x, disp_y, 1, raw.spectrum(), 5);
	visu.draw_text(2, 2, "Cubic Zoomed time: %d ms", white, 0, 1, 13, GetTickCount() - begin);
	return visu;
}

//!图像放大函数，返回值为放大后的区域的图像。--V1.0
/**
	参数origin为需要放大的原图像数组
	XMAX为原图像的长
	YMAX为原图像的宽
	mouse_x为放大区域中心点x值
	mouse_y为放大区域中心点y值
	factor为放大区域（长方形）的长/2
	col_t为输入图像的色彩通道，1为灰度图，3为RGB真彩
	disp_x为显示区域的长
	disp_y为显示区域的宽

	需要注意的是使用CImg库时的数据格式的排列以及大小问题，理论上前端发来什么类型的数据就会返回什么数据：
	1、16bit图像需要进行大小端转换以及灰度区间映射（到0~255，之后返回值改成short同样要做映射）
	2、raw格式的RGB图像，读到数组中貌似是以RGB方式排列的，但是CImg处理RGB的时候需要转换成R...G...B...（貌似）
**/
template<typename T> T *imgZooming(T *origin, int XMAX, int YMAX, int mouse_x, int mouse_y, int factor_x, int col_t = 1, int disp_x = 400, int disp_y = 300){
	int begin = GetTickCount();
	const unsigned char white[] = { 255, 255, 255 };
	T *tmp = new T[XMAX*YMAX*col_t];

	if (col_t == 3){
		for (int i = 0; i < XMAX*YMAX; i++){
			tmp[XMAX*YMAX * 0 + i] = origin[i * 3 + 0];
			tmp[XMAX*YMAX * 1 + i] = origin[i * 3 + 1];
			tmp[XMAX*YMAX * 2 + i] = origin[i * 3 + 2];
		}
			
	}
	else{
		for (int i = 0; i < XMAX*YMAX*col_t; i++){
			tmp[i] = origin[i];
		}
	}

	cimg_library::CImg<T> raw(tmp, XMAX, YMAX, 1, col_t);

	//set zoomed img
	const int factor_y = factor_x*(float(disp_y) / float(disp_x)) + 0.5;
	const int
		x0 = mouse_x - factor_x, y0 = mouse_y - factor_y,
		x1 = mouse_x + factor_x, y1 = mouse_y + factor_y;
	cimg_library::CImg<T> visu;
	visu = raw.get_crop(x0, y0, x1, y1).resize(disp_x, disp_y, 1, col_t, 5);
	visu.draw_text(2, 2, "Cubic Zoomed time: %d ms", white, 0, 1, 13, GetTickCount() - begin);

	//read the img into an array and return
	//the question is how to read
	T *zoomed_img = new T  [disp_x*disp_y*col_t];
	int i = 0;
	if (col_t == 1){
		cimg_forXY(visu, x, y){
			zoomed_img[i] = visu.atXY(x, y);
			i++;
		}
	}
	else if (col_t == 3){
		cimg_forXY(visu, x, y){
			zoomed_img[i * 3+0] = visu.atXYZC(x, y, 1, 0);
			zoomed_img[i * 3+1] = visu.atXYZC(x, y, 1, 1);
			zoomed_img[i * 3+2] = visu.atXYZC(x, y, 1, 2);
			i++;
		}
		//for (i = 0; i < disp_x*disp_y; i++){
		//	zoomed_img[disp_x*disp_y * 0 + i] = zoomed_tmp[i * 3 + 0];
		//	zoomed_img[disp_x*disp_y * 1 + i] = zoomed_tmp[i * 3 + 1];
		//	zoomed_img[disp_x*disp_y * 2 + i] = zoomed_tmp[i * 3 + 2];
		//}
	}
	return  zoomed_img;
}
