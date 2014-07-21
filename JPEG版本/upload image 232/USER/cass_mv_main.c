#include "cass/configuration_control.h"
#include "cass/configuration_system.h"
#include "cass/configuration_address.h"

extern uint8 raw_data[]; 
extern uint8 out_data[]; 

MATCHStruct configuration0_MATCH0;

extern uint8 cass_mv_boarder_width;
extern uint8 cass_mv_boarder_height;
extern uint8 cass_mv_feature_point_number;

//feather.h
//begin
extern image_data_packet_desc_type image_data_packet_desc;
//end


extern uint8 cass_mv_binary_threshold;

extern uint8 cass_mv_gussian_length;

extern uint8 cass_mv_laplace_length;

extern uint8 cass_mv_match_percent;

extern uint8 cass_mv_erode_strenth;

//configuration_ControlFuns.c
//begin
// 输入
uint8 FILEINControl(char *filename)
{
#ifdef CASS_MV_PC_VERSION
	FILE *source;
   	int  x,y,scanline;
	int width,height,fileSize,dwSize,current;
	
	memset(raw_data,0,360000);
	memset(out_data,0,360000);
	
	if((source=fopen((char *)filename,"r"))==NULL)
    	{
        	printf("can't open the source file!\n");
        	return 0;
    	}
	
	/*读取bmp文件头和信息头存到数组里*/
	fseek(source,0,SEEK_SET);
	fread(bmpinfoarray,sizeof(unsigned char),54,source);
	fseek(source,54,SEEK_SET);
	fread(plltee,sizeof(unsigned char),1024,source);
	
	fseek(source,2,0);
	fread(&fileSize,sizeof(long),1,source);//读取bmp数据区大小
	
	fseek(source,14,0);
	fread(&dwSize,sizeof(long),1,source);

          bmpinfo.width=bmpinfoarray[18];
          bmpinfo.height=bmpinfoarray[22];
          bmpinfo.fileSize=fileSize;
          bmpinfo.dwSize=dwSize;

          width=bmpinfoarray[18];
          height=bmpinfoarray[22];

          if(width%4)//bmp宽度不为4要补0
          	scanline=(width*8+31)/32*4;
          else
          	scanline=width;

 	for(x=0;x<height;x++)//读取bmp进数组		
	{	
		for(y=0;y<scanline;y++)
		{
			current=14+dwSize+1024+x*scanline+y;
			fseek(source,current,0); 
			raw_data[(height-1-x)*scanline+y]=fgetc(source);
		}	
	}		
	fclose(source);
#else

#endif
	return 1;
}

// 识别处理函数
// uint8 MATCHControl(MATCHStruct *MATCH)
// {
// #ifdef CASS_MV_PC_VERSION
// 	int8 tempx,tempy,tempvalue,tempdot;
// 	uint16 sum=0,sumvalue=0,x=0;
// 	int scanline;
// 	int width=bmpinfoarray[18];

// 	if(width%4)
// 		scanline=(width*8+31)/32*4;
// 	else
// 		scanline=width;
// 	
// 	while(x<256&&(image_data_packet_desc.point[x].w != 0))
// 	{
// 		tempx=image_data_packet_desc.point[x].x;
// 		tempy=image_data_packet_desc.point[x].y;
// 		tempdot=image_data_packet_desc.point[x].v;
// 		tempvalue=image_data_packet_desc.point[x].w;
// 		if(tempvalue < 0)
// 			tempvalue = - tempvalue;

// 		sum += tempvalue;
// 		if(raw_data[tempx*scanline+tempy]==tempdot)
// 		{
// 			sumvalue+=tempvalue;
// 		}

// 		x++;
// 	}

// 	if(sum)
//         	cass_mv_match_percent =( sumvalue * 100/sum);
// 	else
// 		cass_mv_match_percent  = 0;

// #else
//   uint8 tempx,tempy,tempvalue,tempdot;
// 	uint16 sum=0,sumvalue=0,x=0;

// 	while(x<256&&(image_data_packet_desc.point[x].w != 0))
// 	{
// 		tempx=image_data_packet_desc.point[x].x;
// 		tempy=image_data_packet_desc.point[x].y;
// 		tempdot=image_data_packet_desc.point[x].v;
// 		tempvalue=image_data_packet_desc.point[x].w;
// 		if(tempvalue <0)
// 			tempvalue = - tempvalue;
// 		sum += tempvalue;

// 		if(raw_data[tempx*cass_mv_boarder_width+tempy]==tempdot)
// 		{
// 			sumvalue+=tempvalue;
// 		}
// 		x++;
// 	}
// 	
// 	if(sum)
//         	cass_mv_match_percent =( sumvalue * 100/sum);
// 	else
// 		cass_mv_match_percent  = 0;
// #endif
//           return 1;
//           }
// 输出
	void FILEOUTControl(char *folderName)
	{
		
	}
          
        
//end


static void Configuration0_InitParam(void)
{
     
}  
static void Configuration0_mainLoop(void)
{
    FILEINControl("IMG_0271.bmp");
    MATCHControl(&configuration0_MATCH0);
    FILEOUTControl("null");

}
  
static void Configuration0_mainPage(void)
{
    Configuration0_mainLoop();

}
  
static void Configuration0_Control()
{
    Configuration0_mainPage();

}

/********************************************************
�ṩ��CASSƽ̨�Ľӿں���
*********************************************************/
int cass_mv_main_entry(void)
{
    Configuration0_Control();
    return 1;
}
