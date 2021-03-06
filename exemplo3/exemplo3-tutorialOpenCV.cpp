// det_mov.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

/**
 * Edson Moreira C�zar
 *
 * Programa respons�vel por detectar cenas em v�deos,
 * utiliza para compara��o vetores com pixels convertidos 
 * de RGB para HSV, para esse fim exibe as imagens em HSV
**/

int _tmain(int argc, _TCHAR* argv[])
{
	// Captura o v�deo:
    CvCapture* capture = cvCaptureFromFile("C:\\VideoOut_arc12.avi");

	// Captura a imagem:
	IplImage* image = cvLoadImage("C:\\VideoOut_arc12123.jpg",-1);

	//Cria as janelas para exibi��o
	cvNamedWindow("img_video", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("img_compare",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("img_final_probalistic",CV_WINDOW_AUTOSIZE);

	//verifica se o v�deo tem o n�mero m�nimo de frames
    int numFrames = (int) cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);
	if (numFrames < 20)
	{
		printf("O v�deo deve ter mais que 100 frames\n\7");
		exit(0);
	}

	//inicializando as imagens
	IplImage* img_video=0;
	IplImage* img_hsv_image=0;
	IplImage* img_hsv_video=0;
	
	//Vari�veis de probabilidade para c�lculo de imagem
	int quantidadeTotalDePixels = 0;
	int propabilidadeImagemAnterior = 0;

	if(cvGrabFrame(capture))
	{
		img_video = cvCreateImage( cvGetSize( cvRetrieveFrame(capture) ), IPL_DEPTH_8U, 3 ); 
		img_hsv_image = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3 ); 
		img_hsv_video = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3 ); 
	}

	//V�riaveis dos vetores de pixels
	CvScalar vectorPixelVideo;
	CvScalar vectorPixelImage;
	IplImage* img_max_probabilidade=0;

	//Carrega o v�deo frame a frame e exibe
	while (cvGrabFrame(capture))
    {    
		
	if(image){
		//Converte de RGB para HSV
		cvCvtColor( image, img_hsv_image, CV_BGR2HSV );
		//Exibe a imagem de compara��o em HSV
		cvShowImage("img_compare",img_hsv_image);
		
		if(cvWaitKey(10) >= 0){
			break;
		}
	}

		//Captura a imagem
		img_video = cvRetrieveFrame(capture);

		//Converte de RGB para HSV
		cvCvtColor( img_video, img_hsv_video, CV_BGR2HSV );

		//Exibe o v�deo em HSV
		cvShowImage( "img_video", img_video );

		int propabilidadeImagem = 0;

		//Compara��o de imagens
		for( int w = 0; w < img_hsv_image->width; w++ )
		{   
		  for( int h = 0; h < img_hsv_image->height; h++ )
		  {
			  vectorPixelImage=cvGet2D(img_hsv_image,h,w); // get the (i,j) pixel value
			  vectorPixelVideo=cvGet2D(img_hsv_video,h,w);

			  if(vectorPixelImage.val[0] == vectorPixelVideo.val[0])
			  {
				propabilidadeImagem++;
			  }
		  }
		}

		//Caso seja identificada a cena id�ntica
		if(propabilidadeImagem  == quantidadeTotalDePixels)
		{
			printf("Cena identificada !");
			Sleep(10000);
		}

		//Caso n�o seja identificada a cena id�ntica
		//Obtem-se a cena atrav�s da maior probabilidade
		if(propabilidadeImagem  > propabilidadeImagemAnterior)
		{
					propabilidadeImagemAnterior = propabilidadeImagem;
					img_max_probabilidade = img_video;
					cvShowImage( "img_final_probalistic", img_max_probabilidade );
		}

	    if( cvWaitKey(5) >= 0 )
		  break;
    }

	//Exibe a imagem com maior probabilidade de acerto
	printf("Cena com maior probabilidade de identifica��o ! \n");
	printf("N�mero de acertos de pixels : \n %i",propabilidadeImagemAnterior);
	Sleep(20000);

	
    //Apaga os valores para exibi��o de imagens em tela
	cvReleaseCapture(&capture);
	cvDestroyWindow("img_video");
	cvDestroyWindow("img_compare");
	cvDestroyWindow("img_final_probalistic");

  return 0;
}
