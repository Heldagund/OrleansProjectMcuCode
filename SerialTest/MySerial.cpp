#include "MySerial.h"

SerialPort::SerialPort() : hCom(INVALID_HANDLE_VALUE)
{
}

bool SerialPort::OpenPort(char* portName, LPDWORD pErrorCode)
{
	hCom = CreateFileA(portName, 
					  GENERIC_READ|GENERIC_WRITE,  // ����ģʽ
		              0,             // ����ģʽ
		              NULL,             // ��ȫ������
		              OPEN_EXISTING,      
					  0,
					  0);

	if (hCom == INVALID_HANDLE_VALUE) //�򿪴���ʧ��
	{
		*pErrorCode = GetLastError();
		return false;       
	}
	else
	{
		return true;
	}
}

bool SerialPort::ClosePort(LPDWORD pErrorCode)
{
	if (CloseHandle(hCom))   //�ɹ��رմ���
	{
		return true;
	}
	else
	{
		*pErrorCode = GetLastError();
		return false;
	}
}

bool SerialPort::WriteData(char* pDataToSend, int byteToSend, LPDWORD pErrorCode)
{
	DWORD byteSent = 0;     //�ɹ�д����ֽ���
	do
	{
		if (WriteFile(hCom, pDataToSend, byteToSend, &byteSent, NULL))   //�ɹ�д��
		{
			byteToSend -= byteSent;
			pDataToSend += byteSent;
		}
		else
		{
			*pErrorCode = GetLastError();
			return false;
		}
	} while (byteToSend > 0);     //ѭ��ֱ����������ȫ���������
	return true;
}

bool SerialPort::ReadData(char* pDataToReceive, int byteToReceive, LPDWORD pErrorCode)
{
	DWORD byteReceived;   //�յ����ֽ���
	do
	{
		if (ReadFile(hCom, pDataToReceive, byteToReceive, &byteReceived, NULL))
		{
			byteToReceive -= byteReceived;
			pDataToReceive += byteReceived;
		}
		else
		{
			*pErrorCode = GetLastError();
			return false;
		}
		if (byteToReceive > 0)     //����յ��ֽ���С��Ҫ��
		{
			Sleep(10);       //�ȴ����ݵ���
		}
	} while (byteToReceive > 0);  //ѭ��ֱ���յ�ָ������������
	return true;
}