#pragma once

#include <iostream>

unsigned char* LoadFile(char* filePath){
	unsigned char* fileContent = nullptr;
	FILE* file = fopen(filePath,"rb");
	if (file)
	{
		fseek(file,0,SEEK_END);
		int fileLength = ftell(file);
		printf("file length : %d\n",fileLength);
		if (fileLength > 0)
		{
			rewind(file);
			fileContent = new unsigned char[fileLength+1];
			fileContent[fileLength] = '\0';
			fread(fileContent,sizeof(unsigned char),fileLength,file);
		}
		fclose(file);
	}
	else
	{
		printf("open file failed\n");
	}
	return fileContent;
}