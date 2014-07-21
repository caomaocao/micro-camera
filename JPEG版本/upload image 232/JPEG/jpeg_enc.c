
#include <stdio.h>
#include <stdlib.h>

#include "jinclude.h"
#include "jcapi.h"


//r g b  r g b  240*16
//unsigned char buf_BMP[320*240*3] = {0};

//unsigned char JPG_enc_buf[60000];//jpeg 输出缓冲
extern unsigned int pt_buf;//缓冲区指针

jpeg_compress_info info1;

JQUANT_TBL  JQUANT_TBL_2[2];

JHUFF_TBL  JHUFF_TBL_4[4];

unsigned char dcttab[3][512];//
/*
int Load_File(char filename[])
{
	FILE *source;
   	int  x,y,z,current;
		
	if((source=fopen((char *)filename,"rb+"))==NULL)
    	{
        	printf("can't open the source file!\n");
        	return 0;
    	}
	
	fseek(source,54,SEEK_SET);
	
	z=0;
	for(x=0;x<240;x++)//读取bmp进数组		
	{	
		for(y=0;y<320;y++)
		{
			current=54+(x*320+y)*3;
			fseek(source,current,0); 
			buf_BMP[((240-1-x)*320+y)*3]=fgetc(source);
			buf_BMP[((240-1-x)*320+y)*3+1]=fgetc(source);
			buf_BMP[((240-1-x)*320+y)*3+2]=fgetc(source);
		}	
	}

	
	fclose(source);
	return 1;
}	
*/

////输入区缓冲,这个是为宽度240的图片大小设置的，如果要更大的图片，就需要更大的缓冲11520 = 240x16x3

int jpeg_encoder(unsigned char *buf_BMP, unsigned char *JPG_enc_buf) 
{
  int width, height;
  JSAMPLE *image;//图像源数据指针

  jpeg_compress_info *cinfo;
  
  pt_buf = 0;

  //Load_File("1.bmp");

  image = buf_BMP;//(JSAMPLE *)jutl_read_bitmap(src, &width, &height);
  width = 320;//图像的宽度
  height = 240;//图像的高度

  //file = fopen("1.jpg", "wb+");

  cinfo = jpeg_create_compress();
  if (!cinfo) 
  {
		return;
     //printf("error in create cinfo, malloc faild!\n");
  }
  cinfo->image_width = width;
  cinfo->image_height= height;
  cinfo->output = (char *)JPG_enc_buf;//fopen("test.jpg", "wb");
  jpeg_set_default(cinfo);  
  
  jpeg_start_compress(cinfo);
  
  while (cinfo->next_line < cinfo->image_height) 
  {
    //printf("%d\n", cinfo->next_line);
    //jpeg_write_scanline(cinfo, &image[(cinfo->next_line*cinfo->image_width*3)%11520]);
	  jpeg_write_scanline(cinfo, &image[cinfo->next_line*cinfo->image_width*3]);
    //因为输入区ram很小，所以用了%11520做了循环重复，正常的程序自己做处理
   
  }
  jpeg_finish_compress(cinfo);
  
  jpeg_destory_compress(cinfo);
  //getchar();
  return pt_buf;
}


