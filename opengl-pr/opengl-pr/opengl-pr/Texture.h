#pragma once

unsigned char* DecodeImageBmp(unsigned char* fileContent , int &width , int &height){

	printf("unsigned short:%x\n", *(unsigned short*)fileContent);
	//С�˷�ʽ
	//0~1λ����ͷ�����ֽڣ������Ƿ���λͼ�ļ�
	if (0x4D42 == *(unsigned short*)fileContent)
	{
		printf("this picture is BMP\n");
		//��11~14λ�������ļ�ͷ��λͼ������Ҫƫ��xxx���ֽ�
		int pixelDataOffset = *((int*)(fileContent + 10));
		printf("pixelDataOffset:%d\n",pixelDataOffset);
		//���
		width = *((int*)(fileContent + 18));
		height = *((int*)(fileContent + 22));
		//ȡ��������bgr bgr bgr ...
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