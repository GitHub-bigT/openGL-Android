#include <fstream>
#include <string>

class TextFile{
public:
	char* readFile(char* filePath){
		char* txt = nullptr;
		std::string fileContent;
		std::ifstream fileContentStream(filePath, std::ios::in);
		if (fileContentStream.is_open()){
			std::string line = "";
			while (std::getline(fileContentStream, line)){
				fileContent += "\n" + line;
			}
			fileContentStream.close();

			char* t = (char*)fileContent.c_str();
			txt = (char*)malloc(fileContent.length() * sizeof(char));
			strcpy(txt, t);
		}

		return txt;
	}
};