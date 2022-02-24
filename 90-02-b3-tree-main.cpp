/*肖鹏飞 信13 1953072*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>//exit()要用
#include <sstream>
#include <fstream>
#include <direct.h>//dirent.h不包含
#include <windows.h>
#include  <io.h>
#include <sys/stat.h>
using namespace std;

struct argv_kind {
	char name[40];//名称
	char value[200];//数据值
	int app_num;//在串中的出现次数,起始为0
};

/*方式：递归遍历*/
/*type:是否有/f*/
/*cixu:次序：读三遍，第一遍看有几个文件夹，第二遍处理文件，第三遍处理文件夹*/
/*digui:递归层级*/
/*wjj：当前读到了第几个文件夹*/
/*num：本层总共有几个文件夹*/
void PrintFileName(int type,int cixu,int digui,char* m_pFileDirectory,int wjj,int num)//传入参数为要遍历的路径,注意修改其中不合法
{
	char pLogFileDirectory[MAX_PATH] = { 0 };
	strcpy_s(pLogFileDirectory, MAX_PATH, m_pFileDirectory);
	int iLen = strlen(pLogFileDirectory);
	pLogFileDirectory[iLen] = '*';
	pLogFileDirectory[iLen + 1] = 0;

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(pLogFileDirectory, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		int iErrno = GetLastError();
		return;
	}

	//int digui = 1;//递归层级
	int i;
	BOOL bRet = FALSE;
	static int a[1000] = { 0 };//当前支路的各层文件夹数，一旦确定不再变化
	static int b[1000] = { 0 };//当前支路的读到文件夹数，会随着读取而变化
	static int c[1000] = { 0 };//当前支路的读到文件数，会随着读取而变化

	b[digui] = 0;//在每次循环开始前变回0（本次数据）
	c[digui] = 0;//在每次循环开始前变回0（本次数据）

	for (;;)
	{
		//cout << m_pFileDirectory << endl;
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//目录
			if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, ".."))
			{
				//.或..
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				{
					//系统目录????
				}
				else
				{
					if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					{
						//隐藏目录
					}
					else
					{
						if (cixu == 0)
						{
							num++;
						}


						if (cixu == 2)
						{
					
							wjj++;
							b[digui] = wjj;
							
							for (i = 1; i <= digui; i++)
							{

								if (i != digui)
								{
									if (a[i] == 0)
										cout << "    ";
									else if (b[i] < a[i])
										cout << "|   ";
									else//b=a
										cout << "    ";
								}
								else//i==digui
								{
									if (wjj != num)
									{
										cout << "+---";
									}
									else
										cout << "\\---";
								}
							}

							cout << findFileData.cFileName << endl;
							//int len = strlen(m_pFileDirectory);
							char file1[400] = { 0 };
							strcpy(file1, m_pFileDirectory);
							strcat(file1, findFileData.cFileName);
							strcat(file1, "/");
							PrintFileName(type,0, digui + 1, file1,0,0);
						}
					}
				}
			}
		}
		else
		{
			//文件
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			{
				//系统文件?????
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				{
					//隐藏文件
				}
				else
				{

					//普通文件

					if (cixu == 1)
					{
						c[digui] += 1;
						if (type == 1)
						{
							for (i = 1; i <= digui; i++)
							{
								if (a[i] == 0)
									cout << "    ";
								else if (b[i] < a[i])
									cout << "|   ";
								else//b=a
									cout << "    ";
							}

							cout << findFileData.cFileName << endl;
						}
					}
				}
			}
		}

		bRet = FindNextFileA(hFind, &findFileData);
		if (!bRet)
		{
			if (cixu == 1)
			{
				if (type == 1)
				{
					if (c[digui] != 0)
					{
						for (i = 1; i <= digui; i++)
						{
							if (a[i] == 0)
								cout << "    ";
							else if (b[i] < a[i])
								cout << "|   ";
							else//b=a
								cout << "    ";

						}
						cout << endl;
					}
				}

				PrintFileName(type,2, digui, m_pFileDirectory,0,num);
				break;
			}
			else if (cixu == 0)
			{
				a[digui] = num;//
				PrintFileName(type,1, digui, m_pFileDirectory, 0, num);
				break;
			}
			else
				break;
		}
	}

	if (cixu==2&&digui == 1 && a[1] == 0)
	{
		cout << "没有子文件夹 " << endl;
		cout << endl;
	}
	FindClose(hFind);
}



int main(int argc, char* argv[])
{
	argv_kind arg[3]{
		{"path","",0},
		{"/f","",0}
	};
	_getcwd(arg[0].value,200);	// 功能:获得当前工作目录,缺省值

	/*错误处理不充分，且没有适应性，但简单啊*/

	if (argc > 3)
	{
		cout << "参数过多" << endl;
		return -1;
	}
	/*argc为1则均为缺省值*/
	if (argc == 2)
	{
		if (strcmp("/f", argv[1]) == 0 || strcmp("/F", argv[1]) == 0)
			arg[1].app_num = 1;
		else
		{
			arg[0].app_num = 1;
			strcpy(arg[0].value,argv[1]);
		}
	}
	if (argc == 3)
	{
		arg[0].app_num = 1;
		arg[1].app_num = 1;
		if (strcmp("/f", argv[1]) == 0 || strcmp("/F", argv[1]) == 0)	
			strcpy(arg[0].value, argv[2]);
		else
			strcpy(arg[0].value, argv[1]);
	}

	/*判断路径是否有子文件夹*/
	int reover = 0;


	if (arg[0].app_num == 1)
	{
		if (_access(arg[0].value, 0) != 0)//不存在的目录或文件
		{
			reover = 1;
			char ch1[10] = { 0 };
			if (strlen(arg[0].value) >= 2)
				strncpy(ch1, arg[0].value, 2);
			ch1[2] = { '/' };
			if ((_access(ch1, 0)) != 0)//盘符不存在,用当前路径
				_getcwd(arg[0].value, 200);
		}
		else//存在，判断是否为文件
		{
			struct stat s;
			if (stat(arg[0].value, &s) == 0)
			{
				if (s.st_mode & S_IFDIR)//路径
				{
				}
				else if (s.st_mode & S_IFREG)//文件
				{
					reover = 1;
				}
				else
				{
					reover = 1;
				}
			}
		}
	}

	/*Windows API : GetVolumeInformation 获取磁盘卷（驱动器）信息*/

	char szVolumeNameBuf[MAX_PATH] = { 0 };
	DWORD dwVolumeSerialNum;
	DWORD dwMaxComponentLength;
	DWORD dwSysFlags;
	char szFileSystemBuf[MAX_PATH] = { 0 };
	DWORD dwFileSystemBuf = MAX_PATH;

	char ch[10] = { 0 };
	if(arg[0].value[1]!=':')
		_getcwd(arg[0].value, 200);
	strncpy(ch, arg[0].value,3);//....E:/形式

	BOOL bGet = GetVolumeInformationA(ch,// root directory  卷所在的根目录,如:"C:\\", 如果为NULL,表示当前目录
		szVolumeNameBuf,// volume name buffer ,输出参数,存放卷名缓冲区
		MAX_PATH, // length of name buffer,卷名缓冲区长度
		&dwVolumeSerialNum,// volume serial number, 卷序列号
		&dwMaxComponentLength, // maximum file name length,最大文件文件名组件长度,随文件系而变化
		&dwSysFlags,// file system options ,文件系统的一些属性,通常为一些宏的组合
		szFileSystemBuf,// file system name buffer,说明何种文件系统,例如NTFS,  FAT等
		MAX_PATH);// length of file system name buffer,文件系统缓冲区长度

	cout << "卷 " << szVolumeNameBuf << " 的文件夹 PATH 列表" << endl;
	cout << "卷序列号为 " ;
	unsigned long t1, t2;
	t1 = dwVolumeSerialNum / 65536;//前半部分
	t2 = dwVolumeSerialNum % 65536;//后半部分
	cout << hex << setiosflags(ios::uppercase) << t1 << "-";
	cout << hex << setiosflags(ios::uppercase) << t2 << endl;;

	_strupr(arg[0].value);
	cout << arg[0].value << endl;


	if (reover == 0)
	{
		strcat(arg[0].value, "/");
		PrintFileName(arg[1].app_num, 0, 1, arg[0].value, 0, 0);
	}
	else
	{
		cout << "无效的路径 - ";
		cout << &arg[0].value[2] << endl;
		cout << "没有子文件夹 " << endl;
		cout << endl;
	}

}