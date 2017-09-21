#pragma once
#include <iostream>

unsigned char* LoadFile(const char* filepath){
	unsigned char* fileContent = nullptr;
	FILE* file =  fopen(filepath, "rb");
	if (file)
	{
		//文件有多大
		fseek(file,0,SEEK_END);
		int fileLength = ftell(file);
		printf("file length:%d\n",fileLength);
		if (fileLength>0)
		{
			//将文件指针重新指向文件开头
			rewind(file);
			fileContent = new unsigned char[fileLength + 1];
			fileContent[fileLength] = '\0';
			fread(fileContent,sizeof(unsigned char),fileLength,file);
		}
		else
		{
			printf("file content null");
		}
		fclose(file);
	}
	return fileContent;
}
