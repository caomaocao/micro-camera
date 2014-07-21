int Save_File(int a)
{
	FILE *dst;
	unsigned char temp;
	int i=0,j=0;
	double current;
	int scanline;
	char na[20];
	char bmp[20]=".bmp";
	unsigned char *temppoint;
	unsigned char *temppointA=plltee;
	
	temppoint=&bmpinfoarray[0];
	itoa(a,na,10);
	strcat(na,bmp);
	if((dst=fopen(na,"wb+"))==NULL)
	{
		printf("file cannot open\n");
		return 0;
	}
	
	if(bmpinfo.width%4)//bmp宽度不为4要补0
		scanline=(bmpinfo.width*8+31)/32*4;
	else
		scanline=bmpinfo.width;

	fseek(dst,0,SEEK_SET);//写bmp信息头
	i=0;
	while(i<54)
	{
		fwrite(temppoint,sizeof(unsigned char),1,dst);			
		temppoint++;
		i++;
	}
	fseek(dst,54,SEEK_SET);//写bmp调色板
	while(j<1024)
	{
		fwrite(temppointA,sizeof(unsigned char),1,dst);	
		temppointA++;
		j++;
	}
	
	fseek(dst,1078,SEEK_SET);//写数据区
	for(i=0;i<bmpinfo.height;i++)
		for(j=0;j<scanline;j++)
		{			
			temp=out_data[i*scanline+j];
			current=14+40+1024+(bmpinfo.height-i-1)*scanline+j;
			fseek(dst,current,SEEK_SET);
			fwrite(&temp,sizeof(unsigned char),1,dst);			
		}
	fclose(dst);
	for(i=0;i<bmpinfo.height;i++)
	{
		for(j=0;j<bmpinfo.width;j++)
		{
			raw_data[i*scanline+j]=out_data[i*scanline+j];
		}
	}
	return 1;
}