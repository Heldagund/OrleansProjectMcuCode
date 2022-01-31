#include "MySerial.h"

SerialPort::SerialPort() : hCom(INVALID_HANDLE_VALUE)
{
}

bool SerialPort::OpenPort(char* portName, LPDWORD pErrorCode)
{
	hCom = CreateFileA(portName, 
					  GENERIC_READ|GENERIC_WRITE,  // 访问模式
		              0,             // 共享模式
		              NULL,             // 安全性设置
		              OPEN_EXISTING,      
					  0,
					  0);

	if (hCom == INVALID_HANDLE_VALUE) //打开串口失败
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
	if (CloseHandle(hCom))   //成功关闭串口
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
	DWORD byteSent = 0;     //成功写入的字节数
	do
	{
		if (WriteFile(hCom, pDataToSend, byteToSend, &byteSent, NULL))   //成功写入
		{
			byteToSend -= byteSent;
			pDataToSend += byteSent;
		}
		else
		{
			*pErrorCode = GetLastError();
			return false;
		}
	} while (byteToSend > 0);     //循环直至所有数据全部发送完毕
	return true;
}

bool SerialPort::ReadData(char* pDataToReceive, int byteToReceive, LPDWORD pErrorCode)
{
	DWORD byteReceived;   //收到的字节数
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
		if (byteToReceive > 0)     //如果收到字节数小于要求
		{
			Sleep(10);       //等待数据到来
		}
	} while (byteToReceive > 0);  //循环直至收到指定数量的数据
	return true;
}