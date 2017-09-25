#pragma once

unsigned char* DecodeImageBmp(unsigned char* fileContent , int &width , int &height){

	printf("unsigned short:%x\n", *(unsigned short*)fileContent);
	//小端方式
	//0~1位（开头两个字节）代表是否是位图文件
	if (0x4D42 == *(unsigned short*)fileContent)
	{
		printf("this picture is BMP\n");
		//第11~14位代表：从文件头到位图数据需要偏移xxx个字节
		int pixelDataOffset = *((int*)(fileContent + 10));
		printf("pixelDataOffset:%d\n",pixelDataOffset);
		//宽高
		width = *((int*)(fileContent + 18));
		height = *((int*)(fileContent + 22));
		//取出来的是bgr bgr bgr ...
		unsigned char* imageData = fileContent + pixelDataOffset;
		for (int i = 0; i < width * height * 3; i+=3)
		{
			unsigned char temp = imageData[i];
			imageData[i] = imageData[i+2];
			imageData[i] = temp;
		}
	}
	else
	{
		printf("this picture is not BMP\n");
		return nullptr;
	}
	
}