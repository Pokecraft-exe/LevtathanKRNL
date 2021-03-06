#pragma once
#include "cstring.h"
class File {
private:
	char* data;
public:
	char* name;
	char* extention;
	char* Read() {
		return data;
	}
	void Write(char* newData) {
 		data = newData;
	}
	void Append(char* newData) {
		data = strncat(data,newData,strlen(newData));
	}
	void Rename(char* NewName, char* NewExtention) {
		name = NewName;
		extention = NewExtention;
	}
};

class Directory {
private:
	Directory* childDir;
	int childdir;
	int childfile;
	File childFile[100];
	char* fileName;
	int currentDirIndex = -1;
	File emptyFile;
public:
	Directory(char* Name) {
		fileName = Name;
	}
	
	void cd(char* Name) {
		if (Name == "..")
			currentDirIndex = -1; // -1 is root
		else {
			if (currentDirIndex == -1) {
				for (int i = 0; i < 100; i++)
					if (childDir[i].fileName == Name)
						currentDirIndex = i;
			} else
				childDir[currentDirIndex].cd(Name);
		}
	}

	File *open(char* Name, char* ext) {
		if (currentDirIndex == -1) {
			for (int i = 0; i < 100; i++) {
				if (childFile[i].name == Name &&
					childFile[i].extention == ext) {
					return &childFile[i];
				}
			}
			return &emptyFile;
		} else
			return childDir[currentDirIndex].open( Name, ext);
	}

	void mkdir(char* Name) {
		if (currentDirIndex == -1){
			childDir[childdir+1]=Directory( Name);
			childdir++;
        }
		else
			childDir[currentDirIndex].mkdir( Name);
	}

	void mkfile(char*  Name, char* ext) {
		File newfile;
		newfile.name =  Name;
		newfile.extention = ext;
		if (currentDirIndex == -1){
			childFile[childfile+1]=newfile;
			childfile++;
        }
		else
			childDir[currentDirIndex].mkfile( Name, ext);
	}

	void ls() {
		if (currentDirIndex == -1) {
			puts(">>> ");puts(IntToStr(childdir));puts(" directory(s) found in ");puts(fileName);puts(":\n\r");
			puts("::: ");puts(IntToStr(childfile));puts(" file(s) found in ");puts(fileName);puts(":\r\n");
			write_serial('h');
			for (int i; i<=childdir;i++) {
				puts("> ");puts(childDir[i].fileName);puts("\r\n");
			}
			write_serial('i');
			for (int i;i<=childfile;i++) {
				puts(": ");puts(childFile[i].name);puts(".");puts(childFile[i].extention);puts("\n\r");
			}
			write_serial('b');
		} else childDir[currentDirIndex].ls();
	}
};

extern Directory Partition("RAM://");

void initRAMDISK(){
	puts("Partition on RAM://\n\r");
	Partition.ls();
	write_serial('c');
}