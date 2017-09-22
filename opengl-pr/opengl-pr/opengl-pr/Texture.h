#pragma once

unsigned char* DecodeImageBmp(unsigned char* fileContent , int width , int height){
	unsigned char* imageData = nullptr;
	printf("width:%d,height:%d\n",width,height);
	printf("unsigned short:%x\n", *(unsigned short*)fileContent);
	//С�˷�ʽ
	//��ͷ�����ֽ�
	//�ж��Ƿ���BMP��ʽ
	if (0x4D42 == *(unsigned short*)fileContent)
	{
		printf("this picture is BMP\n");
		int pixelDataOffset = *((int*)(fileContent + 8));
		printf("pixelDataOffset:%d\n",pixelDataOffset);
	}
	else
	{
		printf("this picture is not BMP\n");
	}
	return imageData;
}