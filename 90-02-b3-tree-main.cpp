/*Ф���� ��13 1953072*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>//exit()Ҫ��
#include <sstream>
#include <fstream>
#include <direct.h>//dirent.h������
#include <windows.h>
#include  <io.h>
#include <sys/stat.h>
using namespace std;

struct argv_kind {
	char name[40];//����
	char value[200];//����ֵ
	int app_num;//�ڴ��еĳ��ִ���,��ʼΪ0
};

/*��ʽ���ݹ����*/
/*type:�Ƿ���/f*/
/*cixu:���򣺶����飬��һ�鿴�м����ļ��У��ڶ��鴦���ļ��������鴦���ļ���*/
/*digui:�ݹ�㼶*/
/*wjj����ǰ�����˵ڼ����ļ���*/
/*num�������ܹ��м����ļ���*/
void PrintFileName(int type,int cixu,int digui,char* m_pFileDirectory,int wjj,int num)//�������ΪҪ������·��,ע���޸����в��Ϸ�
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

	//int digui = 1;//�ݹ�㼶
	int i;
	BOOL bRet = FALSE;
	static int a[1000] = { 0 };//��ǰ֧·�ĸ����ļ�������һ��ȷ�����ٱ仯
	static int b[1000] = { 0 };//��ǰ֧·�Ķ����ļ������������Ŷ�ȡ���仯
	static int c[1000] = { 0 };//��ǰ֧·�Ķ����ļ����������Ŷ�ȡ���仯

	b[digui] = 0;//��ÿ��ѭ����ʼǰ���0���������ݣ�
	c[digui] = 0;//��ÿ��ѭ����ʼǰ���0���������ݣ�

	for (;;)
	{
		//cout << m_pFileDirectory << endl;
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Ŀ¼
			if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, ".."))
			{
				//.��..
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				{
					//ϵͳĿ¼????
				}
				else
				{
					if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					{
						//����Ŀ¼
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
			//�ļ�
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			{
				//ϵͳ�ļ�?????
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				{
					//�����ļ�
				}
				else
				{

					//��ͨ�ļ�

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
		cout << "û�����ļ��� " << endl;
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
	_getcwd(arg[0].value,200);	// ����:��õ�ǰ����Ŀ¼,ȱʡֵ

	/*��������֣���û����Ӧ�ԣ����򵥰�*/

	if (argc > 3)
	{
		cout << "��������" << endl;
		return -1;
	}
	/*argcΪ1���Ϊȱʡֵ*/
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

	/*�ж�·���Ƿ������ļ���*/
	int reover = 0;


	if (arg[0].app_num == 1)
	{
		if (_access(arg[0].value, 0) != 0)//�����ڵ�Ŀ¼���ļ�
		{
			reover = 1;
			char ch1[10] = { 0 };
			if (strlen(arg[0].value) >= 2)
				strncpy(ch1, arg[0].value, 2);
			ch1[2] = { '/' };
			if ((_access(ch1, 0)) != 0)//�̷�������,�õ�ǰ·��
				_getcwd(arg[0].value, 200);
		}
		else//���ڣ��ж��Ƿ�Ϊ�ļ�
		{
			struct stat s;
			if (stat(arg[0].value, &s) == 0)
			{
				if (s.st_mode & S_IFDIR)//·��
				{
				}
				else if (s.st_mode & S_IFREG)//�ļ�
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

	/*Windows API : GetVolumeInformation ��ȡ���̾�����������Ϣ*/

	char szVolumeNameBuf[MAX_PATH] = { 0 };
	DWORD dwVolumeSerialNum;
	DWORD dwMaxComponentLength;
	DWORD dwSysFlags;
	char szFileSystemBuf[MAX_PATH] = { 0 };
	DWORD dwFileSystemBuf = MAX_PATH;

	char ch[10] = { 0 };
	if(arg[0].value[1]!=':')
		_getcwd(arg[0].value, 200);
	strncpy(ch, arg[0].value,3);//....E:/��ʽ

	BOOL bGet = GetVolumeInformationA(ch,// root directory  �����ڵĸ�Ŀ¼,��:"C:\\", ���ΪNULL,��ʾ��ǰĿ¼
		szVolumeNameBuf,// volume name buffer ,�������,��ž���������
		MAX_PATH, // length of name buffer,��������������
		&dwVolumeSerialNum,// volume serial number, �����к�
		&dwMaxComponentLength, // maximum file name length,����ļ��ļ����������,���ļ�ϵ���仯
		&dwSysFlags,// file system options ,�ļ�ϵͳ��һЩ����,ͨ��ΪһЩ������
		szFileSystemBuf,// file system name buffer,˵�������ļ�ϵͳ,����NTFS,  FAT��
		MAX_PATH);// length of file system name buffer,�ļ�ϵͳ����������

	cout << "�� " << szVolumeNameBuf << " ���ļ��� PATH �б�" << endl;
	cout << "�����к�Ϊ " ;
	unsigned long t1, t2;
	t1 = dwVolumeSerialNum / 65536;//ǰ�벿��
	t2 = dwVolumeSerialNum % 65536;//��벿��
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
		cout << "��Ч��·�� - ";
		cout << &arg[0].value[2] << endl;
		cout << "û�����ļ��� " << endl;
		cout << endl;
	}

}