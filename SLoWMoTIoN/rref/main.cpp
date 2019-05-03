#include <sys/types.h>
#include "dirent.h"
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
void getFolderFile(string folderPath, vector<string> &file)
{
	DIR *fp; // create folder point
	fp = opendir(folderPath.c_str());
	struct dirent *folderp;
	int k = 0;
	while ((folderp = readdir(fp)) != NULL)
	{
		if(k >= 2)
			file.push_back(string(folderp->d_name));
		k++;
	}
	closedir(fp);
}
int getdir(string dir, vector<string> &files) {


	DIR *dp;//創立資料夾指標
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}
	while ((dirp = readdir(dp)) != NULL) {//如果dirent指標非空
		files.push_back(string(dirp->d_name));//將資料夾和檔案名放入vector
	}
	closedir(dp);//關閉資料夾指標
	return 0;
}
void t1()
{
	fstream data;
	data.open("folder\\gg.txt");
	if (!data)
	{
		cout << "open fail" << endl;
	}
	string str;
	while (data >> str)
	{
		cout << str << endl;
	}
	data.close();
}
void t2()
{
	string dir = "folder";
	vector<string> files = vector<string>();
	getFolderFile(dir, files);

	for (int i = 0; i < files.size(); i++) {
		cout << files[i] << "123" << endl;
		
	}
}
int main()
{
	cout << "sdfsdfsd" << endl;
	t2();
	system("pause");
	return 0;
}