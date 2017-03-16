#pragma once

#include <opencv2\opencv.hpp>
#include "./include/CameraApi.h"

//Ϊ500W��ҵ�����װ���࣬����ǰȷ��MindVisionCAMSDK.lib����
//@Author��VShawn
//@URL��http://www.cnblogs.com/singlex/
class MindVisionCAM
{
	//��������
public:
	//�Ƿ�����˳�ʼ��
	bool HasInited = false;
	//��ɫģʽ��CV_8U CV_8UC3
	int ColorType = CV_8UC3;
	//�ع�ʱ��
	double ExposureTimeMS = 50;
	//ģ������
	double AnalogGain = 2;
public:
	MindVisionCAM();
	~MindVisionCAM();
	bool Init();
	void Release()
	{
		if (HasInited)
		{
			HasInited = false;
			StopCapture();
			CameraStop(m_hCamera);
			//���˳�����ǰ�ر����(����ʼ�����ǳ���Ҫ�����ֱ�ӹرճ������
			//����ʼ������������п��ܻᱨ�ڴ����)

			CameraUnInit(m_hCamera);
			CameraAlignFree(m_pFrameBuffer);
		}
	};


	//��ʼ�ɼ������ú���������
	void StartCapture(){ CameraPlay(m_hCamera); isCapturing = true; };
	//ֹͣ�ɼ�
	void StopCapture(){ CameraPause(m_hCamera); isCapturing = false; };
	//ȡ��֡
	void GetFrame(cv::Mat& img);
	cv::Mat Grub() { cv::Mat m; GetFrame(m); return m; }
private:
	UINT            m_threadID;		//ͼ��ץȡ�̵߳�ID
	HANDLE          m_hDispThread;	//ͼ��ץȡ�̵߳ľ��
	CameraHandle    m_hCamera;		//��������������ͬʱʹ��ʱ���������������	
	BYTE*           m_pFrameBuffer; //���ڽ�ԭʼͼ������ת��ΪRGB�Ļ�����
	bool isCapturing = false;
};
