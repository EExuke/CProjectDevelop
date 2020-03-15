#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")

#define CLIENTSEND_EXIT 1
#define CLIENTSEND_TRAN 2
#define CLIENTSEND_LIST 3

#define SERVERSEND_SELFID 1
#define SERVERSEND_NEWUSR 2
#define SERVERSEND_SHOWMSG 3
#define SERVERSEND_ONLINE 4

FILE *ioutfileServer;
FILE *ioutfileClient;

struct CReceivePackage
{
	int iType;			//�����Ϣ����
	int iToID;			//���Ŀ���û�ID
	int iFromID;		//���ԭ�û�ID
	char cBuffer[1024];	//�����Ϣ����
};

struct CSendPackage
{
	int iType;			//��Ϣ����
	int iCurConn;		//��ǰ�����û�����
	char cBuffer[1024];	//�����Ϣ����
};

struct CUserSocketInfo
{
	int ID;				//�û���ID
	char cDstIP[64];	//�û���IP��ַ,��չʹ��
	int iPort;			//�û�Ӧ�ó���˿���չʹ��
	SOCKET sUserSocket;	//������
};

struct CUser
{
	int ID;				//�û���ID
	char cDstIP[64];	//�û���IP��ַ ��չʱʹ��
};

struct CUser usr[20];
int bSend=0;//�Ƿ���Է�����Ϣ
int iMyself;//�Լ���id
int iNew=0;//�����û���

struct CUserSocketInfo usrinfo[20]; 


void ExitSystem()
{
	if(ioutfileServer!=NULL)
		fclose(ioutfileServer);
	if(ioutfileClient!=NULL)
		fclose(ioutfileClient);
	WSACleanup();
	exit(0);
}

DWORD WINAPI threadproClient(LPVOID pParam)
{
	SOCKET hsock=(SOCKET)pParam;
	char cRecvBuffer[1024];
	char cShowBuffer[1024];
	int num=0;                                                                                                                                                                                                                                                                                                                                                                                  
	if(hsock!=INVALID_SOCKET)
		printf("start:\n");
	while(1)
	{
		num = recv(hsock,cRecvBuffer,1024,0);
		if(num >= 0)
		{		
			cRecvBuffer[num]='\0';
			sprintf(cShowBuffer,"to me : %s\n",cRecvBuffer);
			printf("%s",cShowBuffer);
			fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileClient);
			fflush(ioutfileClient);
			if(strcmp("exit",cRecvBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
	return 0;
}

DWORD WINAPI threadproServer(LPVOID pParam)
{
	SOCKET hsock=(SOCKET)pParam;
	char cRecvBuffer[1024];
	char cShowBuffer[1024];
	int num=0;
	if(hsock!=INVALID_SOCKET)
		printf("start:\n");
	while(1)
	{
		num = recv(hsock,cRecvBuffer,1024,0);
		if(num >= 0)
		{		
			cRecvBuffer[num]='\0';
			sprintf(cShowBuffer,"to me : %s\n",cRecvBuffer);
			printf("%s",cShowBuffer);
			fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileServer);
			fflush(ioutfileServer);
			if(strcmp("exit",cRecvBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
	return 0;
}


void CreateServer()
{
	SOCKET m_SockServer;
	struct sockaddr_in serveraddr;//���ص�ַ��Ϣ
	struct sockaddr_in serveraddrfrom;//���ӵĵ�ַ��Ϣ

	int iPort=4600;//�趨Ϊ�̶��˿�
	int iBindResult=-1;//�󶨽��
	int iWhileCount=200;

	struct hostent* localHost;
	char* localIP;

	SOCKET m_Server;

	char cWelcomBuffer[]="Welcome to you\0";

	int len=sizeof(struct sockaddr);
	int iWhileListenCount=10;

	DWORD nThreadId = 0;
	int ires;//���͵ķ���ֵ

	char cSendBuffer[1024];//������Ϣ����
	char cShowBuffer[1024];//������Ϣ����

	ioutfileServer= fopen("MessageServer.txt","a");//�򿪼�¼��Ϣ���ļ�
	m_SockServer = socket ( AF_INET,SOCK_STREAM,  0);
	
	printf("�����󶨵Ķ˿ں�(����1024)��");
	scanf("%d",&iPort);
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
	//���������ַ��Ϣ
	serveraddr.sin_family = AF_INET;					
	serveraddr.sin_port = htons(iPort);	//�˿�			
	serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//��ַ
	//�󶨵�ַ��Ϣ
	iBindResult=bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
	//����˿ڲ��ܱ��󶨣��������ö˿�
	while(iBindResult!=0 && iWhileCount > 0)
	{
		printf("��ʧ�ܣ��������룺");
		scanf("%d",iPort);
		//���������ַ��Ϣ
		serveraddr.sin_family = AF_INET;					
		serveraddr.sin_port = htons(iPort);//�˿�	
		serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//IP
		//�󶨵�ַ��Ϣ
		iBindResult = bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
		iWhileCount--;
		if(iWhileCount<=0)
		{
			printf("�˿ڰ�ʧ��,�������г���\n");
			exit(0);
		}
	}

	while(iWhileListenCount>0)
	{
		printf("start listen\n");
		listen(m_SockServer,0);//����ֵ�жϵ��������Ƿ�ʱ
		m_Server=accept(m_SockServer,(struct sockaddr*)&serveraddrfrom,&len);
			if(m_Server!=INVALID_SOCKET)
			{
				//�����ӳɹ������ͻ�ӭ��Ϣ
				send(m_Server,cWelcomBuffer,sizeof(cWelcomBuffer),0);
				//����������Ϣ���߳�
				CreateThread(NULL,0,threadproServer,(LPVOID)m_Server,0,&nThreadId );
				break;
			}
		printf(".");
		iWhileListenCount--;
		if(iWhileListenCount<=0)
		{
			printf("\n��������ʧ��\n");
			exit(0);
		}
	}
	
		
	while(1)
	{
		memset(cSendBuffer,0,1024);
		scanf("%s",cSendBuffer);//������Ϣ
		if(strlen(cSendBuffer)>0)//������Ϣ����Ϊ��
		{
			ires = send(m_Server,cSendBuffer,strlen(cSendBuffer),0);//������Ϣ
			if(ires<0)
			{
				printf("����ʧ��");
			}
			else
			{
				sprintf(cShowBuffer,"Send to : %s\n",cSendBuffer);
				printf("%s",cShowBuffer);
				fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileServer);//����Ϣд����־
			}
			if(strcmp("exit",cSendBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
}
int CheckIP(char *cIP)
{
	char IPAddress[128];//IP��ַ�ַ���
	char IPNumber[4];//IP��ַ��ÿ�����ֵ
	int iSubIP=0;//IP��ַ��4��֮һ
	int iDot=0;//IP��ַ��'.'�ĸ���
	int iResult=0;
	int iIPResult=1;
	int i;//ѭ�����Ʊ���
	memset(IPNumber,0,4);
	strncpy(IPAddress,cIP,128);
	for(i=0;i<128;i++)
	{
		if(IPAddress[i]=='.')
		{
			iDot++;
			iSubIP=0;
			if(atoi(IPNumber)>255)
				iIPResult = 0;
			memset(IPNumber,0,4);
		}
		else
		{
			IPNumber[iSubIP++]=IPAddress[i];
		}
		if(iDot==3 && iIPResult!=0)
			iResult= 1;
	}
	return iResult;
}
void CreateClient()
{
	SOCKET m_SockClient;
	struct sockaddr_in clientaddr;
	char cServerIP[128];
	int iWhileIP=10;//ѭ������
	int iCnnRes;//���ӽ��
	DWORD nThreadId = 0;//�߳�IDֵ
	char cSendBuffer[1024];//���ͻ���
	char cShowBuffer[1024];//��ʾ����
	char cRecvBuffer[1024];//���ջ���
	int num;//���յ��ַ�����
	int ires;//������Ϣ�Ľ��
	int iIPRes;//���IP�Ƿ���ȷ
	m_SockClient = socket ( AF_INET,SOCK_STREAM, 0 );
	printf("�������������ַ��");
	scanf("%s",cServerIP);
	//IP��ַ�ж�
	if(strlen(cServerIP)==0)
		strcpy(cServerIP,"127.0.0.1");//û�������ַ��ʹ�ûػ���ַ
	else
	{
		iIPRes=CheckIP(cServerIP);
		while(!iIPRes && iWhileIP>0)
		{
			printf("�����������������ַ��\n");
			scanf("%s",cServerIP);//��������IP��ַ
			iIPRes=CheckIP(cServerIP);//���IP�ĺϷ���
			iWhileIP--;
			if(iWhileIP<=0)
			{
				printf("�����������\n");
				exit(0);
			}
		}
	}

	ioutfileClient= fopen("MessageServerClient.txt","a");//�򿪼�¼��Ϣ���ļ�
	clientaddr.sin_family = AF_INET;
	//�ͻ�������������Ķ˿ںã�Ӧ�úͷ���˰󶨵�һ��			
	clientaddr.sin_port = htons(4600);
	clientaddr.sin_addr.S_un.S_addr = inet_addr(cServerIP);

	iCnnRes = connect(m_SockClient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(iCnnRes==0)//���ӳɹ�
	{
		num = recv(m_SockClient,cRecvBuffer,1024,0);//������Ϣ
		if( num > 0 )
		{
			printf("Receive form server : %s\n",cRecvBuffer);
			//����������Ϣ���߳�
			CreateThread(NULL,0,threadproClient,(LPVOID)m_SockClient,0,&nThreadId );
		}
		while(1)
		{
			memset(cSendBuffer,0,1024);
			scanf("%s",cSendBuffer);	
			if(strlen(cSendBuffer)>0)
			{
				ires=send(m_SockClient,cSendBuffer,strlen(cSendBuffer),0);
				if(ires<0)
				{
					printf("����ʧ��\n");
				}
				else
				{
					sprintf(cShowBuffer,"Send to : %s\n",cSendBuffer);//����Ҫ��ʾ���ַ���
					printf("%s",cShowBuffer);
					fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileClient);//��¼������Ϣ
					fflush(ioutfileClient);
				}
				if(strcmp("exit",cSendBuffer)==0)
				{
					ExitSystem();
				}
			}
		}
	}//iCnnRes
	else
	{
		//	printf("%s",inet_addr(cServerIP));
		printf("���Ӳ���ȷ\n");
	}
}

DWORD WINAPI threadTranServer(LPVOID pParam) 
{
	SOCKET hsock=(SOCKET)pParam;//��ȡSOCKET���
	SOCKET sTmp;//��ʱ����û���SOCKET���
	char cRecvBuffer[1024];//������Ϣ�Ļ���
	int num=0;//���͵��ַ���
	int m,j;//ѭ�����Ʊ���
	//char cTmp[2];//��ʱ����û�ID
	int ires;
	struct CSendPackage sp;//����
	struct CReceivePackage *p;
	if(hsock!=INVALID_SOCKET)
		printf("start:%d\n",hsock);
	while(1)
	{
		num=recv(hsock,cRecvBuffer,1024,0);//���շ��͹�������Ϣ
		if(num>=0)
		{		
			p = (struct CReceivePackage*)cRecvBuffer;
			switch(p->iType)
			{
			case CLIENTSEND_TRAN://����Ϣ������ת
				for(m=0;m<2;m++)
				{
					if(usrinfo[m].ID==p->iToID)
					{
						//���
						sTmp=usrinfo[m].sUserSocket;
						memset(&sp,0,sizeof(sp));
						sp.iType=SERVERSEND_SHOWMSG;
						strcpy(sp.cBuffer,p->cBuffer);
						ires = send(sTmp,(char*)&sp,sizeof(sp),0);//��������
						if(ires<0)
							printf("����ʧ��\n");
					}
				}
				break;
			case CLIENTSEND_LIST://���������û�
				memset(&sp,0,sizeof(sp));
				for(j=0;j<2;j++)
				{
					if(usrinfo[j].ID!=p->iFromID && usrinfo[j].ID!=0)
					{
						sp.cBuffer[j]=usrinfo[j].ID;
					}
				}
				sp.iType=SERVERSEND_ONLINE;
				send(hsock,(char*)&sp,sizeof(sp),0);
				break;
			case CLIENTSEND_EXIT:
				printf("�˳�ϵͳ\n");
				return 0;//�����߳�
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI NotyifyProc(LPVOID pParam)
{
	struct CSendPackage sp;//���Ͱ�
	SOCKET sTemp;//�����û���socket���
	int *p;//�������̷߳��͹�����IDֵ
	int j;//ѭ�����Ʊ���
	p=(int*)pParam;//���û�ID
	
	for(j=0;j<2;j++)//ȥ���µ�¼��,�Ѿ����ӵ�
	{
		if(usrinfo[j].ID !=  (*p))
		{
			sTemp=usrinfo[j].sUserSocket;
			sp.iType=SERVERSEND_NEWUSR;//������֪ͨ
			sprintf(sp.cBuffer,"%d\n",(*p));
			send(sTemp,(char*)&sp,sizeof(sp),0);//�������û�����֪ͨ
		}
	}
	return 0;
}

void CreateTranServer()
{
	SOCKET m_SockServer;//��ʼ������SOCKET���
	struct sockaddr_in serveraddr;//���ڰ󶨵ĵ�ַ��Ϣ
	struct sockaddr_in serveraddrfrom;//���յ������ӵĵ�ַ��Ϣ
	int iRes;//��ȡ�󶨵Ľ��
	SOCKET m_Server;//�ѽ������ӵ�SOCKET���
	struct hostent* localHost;//��������ָ��
	char* localIP;//����IP��ַ
	struct CSendPackage sp;//���Ͱ�
	int iMaxConnect=20;//�����������Ӹ���
	int iConnect=0;//�������ӵĸ���
	DWORD nThreadId = 0;//��ȡ�̵߳�IDֵ
	char cWarnBuffer[]="It is voer Max connect\0";//�����ַ���
	int len=sizeof(struct sockaddr);
	int id;//�·���Ŀͻ�ID
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);//��ȡ����IP
	serveraddr.sin_family = AF_INET;					
	serveraddr.sin_port = htons(4600);//���ð󶨵Ķ˿ں�			
	serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//���ñ���IP
	//�����׽���
	m_SockServer = socket ( AF_INET,SOCK_STREAM,  0);
	if(m_SockServer == INVALID_SOCKET)
	{
		printf("�����׽���ʧ��\n");
		exit(0);
	}
	//�󶨱���IP��ַ
	iRes=bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
	if(iRes < 0)
	{
		printf("�����׽���ʧ��\n");
		exit(0);
	}
	//������ѭ��
	while(1)
	{
		listen(m_SockServer,0);//��ʼ����
		m_Server=accept(m_SockServer,(struct sockaddr*)&serveraddrfrom,&len);//��������
		if(m_Server!=INVALID_SOCKET)
		{
			printf("�����û���¼");//�Է��ѵ�¼ 
			if(iConnect < iMaxConnect)
			{
				//����������Ϣ�߳�
				CreateThread(NULL,0,threadTranServer,(LPVOID)m_Server,0,&nThreadId );
				//���������û�����Ϣ
				usrinfo[iConnect].ID=iConnect+1;//����û�ID
				usrinfo[iConnect].sUserSocket=m_Server;
				usrinfo[iConnect].iPort=0;//��Ŷ˿ڣ���չ��
				//����������Ϣ
				sp.iType=SERVERSEND_SELFID;//��ȡ��IDֵ��������Ϣ
				sp.iCurConn=iConnect;//���߸���
				id=iConnect+1;
				sprintf(sp.cBuffer,"%d\0",id);
				send(m_Server,(char*)&sp,sizeof(sp),0);//���Ϳͻ��˵�IDֵ
				//֪ͨ�����ͻ���
				if(iConnect>0)
					CreateThread(NULL,0,NotyifyProc,(LPVOID)&id,0,&nThreadId );
				iConnect++;
			}
			else
				send(m_Server,cWarnBuffer,sizeof(cWarnBuffer),0);//�ѳ������������
		}
	}
	WSACleanup();
}



DWORD WINAPI threadTranClient(LPVOID pParam) 
{
	SOCKET hsock=(SOCKET)pParam;
	int i;//ѭ�����Ʊ���
	char cRecvBuffer[2048];//������Ϣ�Ļ���
	int	num;//������Ϣ���ַ���
	//char cTmp[2];//��ʱ��������û�ID
	struct CReceivePackage sp;//����˵Ľ��հ��ǣ��ͻ��˵ķ��Ͱ�
	struct CSendPackage *p;//����˵ķ��Ͱ��ǣ��ͻ��˵Ľ��հ�
	int iTemp;//��ʱ��Ž��յ���IDֵ
	while(1)
	{
		num = recv(hsock,cRecvBuffer,2048,0);//������Ϣ
		if(num>=0) 
		{
			p = (struct CSendPackage*)cRecvBuffer;
			if(p->iType==SERVERSEND_SELFID)
			{
				iMyself=atoi(p->cBuffer);
				sp.iType=CLIENTSEND_LIST;//����������Ա�б�
				send(hsock,(char*)&sp,sizeof(sp),0);
			}
			if(p->iType==SERVERSEND_NEWUSR)//��¼�û�ID
			{
				iTemp = atoi(p->cBuffer);
				usr[iNew++].ID=iTemp;//iNew��ʾ�ж��ٸ����û���¼ 
				printf("�����û���¼,������������\n");
				bSend=1;//���Է�����Ϣ����
			}
			if(p->iType==SERVERSEND_SHOWMSG)//��ʾ���ܵ���Ϣ
			{
				printf("rec:%s\n",p->cBuffer);
			}
			if(p->iType==SERVERSEND_ONLINE)//��ȡ�����б�
			{
				for(i=0;i<2;i++)
					{
						if(p->cBuffer[i]!=iMyself && p->cBuffer[i]!=0)
						{
							usr[iNew++].ID=p->cBuffer[i];
							printf("���û�����,������������\n");
							bSend=1;//���Է�����Ϣ����
						}
					}
					if(!bSend)
						printf("�����б�Ϊ��\n");
			}
		}
	}
	return 0;
}

void ExitTranSystem()
{
	WSACleanup();
	exit(0);
}
void CreateTranClient()
{
	SOCKET m_SockClient;//�������ӵ�socket
	struct sockaddr_in clientaddr;//Ŀ��ĵ�ַ��Ϣ
	int iRes;//����ִ�����
	char cSendBuffer[1024];//������Ϣ�Ļ���
	DWORD nThreadId = 0;//�����̵߳�IDֵ
	struct CReceivePackage sp;//�����ṹ
	char IPBuffer[128];
	printf("���������IP��ַ\n");
	scanf("%s",IPBuffer);

	clientaddr.sin_family = AF_INET;				
	clientaddr.sin_port = htons(4600);//���ӵĶ˿ں�
	clientaddr.sin_addr.S_un.S_addr = inet_addr(IPBuffer);
	m_SockClient = socket ( AF_INET,SOCK_STREAM, 0 );//����socket
	//���������˵�����
	iRes = connect(m_SockClient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(iRes < 0)
	{
		printf("���Ӵ���\n");
		exit(0);
	}
	//����������Ϣ���߳�
	CreateThread(NULL,0,threadTranClient,(LPVOID)m_SockClient,0,&nThreadId );
	while(1)//���յ��Լ�ID
	{
		memset(cSendBuffer,0,1024);
		scanf("%s",cSendBuffer);//���뷢������
		if(bSend)
		{
			if(sizeof(cSendBuffer)>0)
			{
				memset(&sp,0,sizeof(sp));
				strcpy(sp.cBuffer,cSendBuffer);
				sp.iToID=usr[0].ID;//��������ǹ̶���
				sp.iFromID=iMyself;//�Լ�
				sp.iType=CLIENTSEND_TRAN;
				send(m_SockClient,(char*)&sp,sizeof(sp),0);//������Ϣ
			}
			if(strcmp("exit",cSendBuffer)==0)
			{
				memset(&sp,0,sizeof(sp));
				strcpy(sp.cBuffer,"�˳�");//���÷�����Ϣ���ı�����
				sp.iFromID=iMyself;
				sp.iType=CLIENTSEND_EXIT;//�˳�
				send(m_SockClient,(char*)&sp,sizeof(sp),0);//������Ϣ
				ExitTranSystem();
			}
		}
		else
			printf("û�н��ն���,����ʧ��\n");
	Sleep(10);
	}
}

int main(void)
{
	int iSel=0;
	WSADATA wsd;									
	WSAStartup(MAKEWORD(2,2),&wsd);
	do
	{
		printf("ѡ��������ͣ�\n");
		printf("��Ե�����: 1\n");
		printf("��Ե�ͻ���: 2\n");
		printf("��������ת�����: 3\n");
		printf("��������ת�ͻ���: 4\n");
		scanf("%d",&iSel);
	}while(iSel<0 || iSel >4);
	switch(iSel)
	{
	case 1:
		CreateServer();
	
		break;
	case 2:
		CreateClient();
		break;
	case 3:
		CreateTranServer();
		break;
	case 4:
		CreateTranClient();
		break;
	}
	printf("�˳�ϵͳ\n");
	
	return 0;
}

