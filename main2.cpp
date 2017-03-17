#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "..\include\PNPSolver.h"
#include"DetecRM.h"
using namespace std;
using namespace cv;

//������չʾһ��ʵʱ���λ�˹��Ƶ����̣����е�ԭ����ǰ�����Ѿ�˵���ˣ����á����λ�˹���1_1��OpenCV��solvePnP���η�װ�����ܲ��ԡ��й�������
//ʹ�ó�������Ӽ򵥡�����������HSV�ռ䣬���ٺ�ɫ�������㣬�����ٵ������������ڽ�PNP���⣬�õ����λ�ˣ���������������������������ת�ǣ���
//���ʹ��labview�е���άͼƬ�ؼ���������ϵͳ����3D�ؽ���

//@author:VShawn(singlex@foxmail.com)
//˵�����µ�ַ��http://www.cnblogs.com/singlex/p/pose_estimation_2.html


double fx = 1164.1; // / 1194.61;
double fy = 1163.8;///;// 1196.98;
double u0 = 631.2303;// 233.0957;// 302.9064;// 634.075;
double v0 = 373.7216;// 300.378;// 235.2960;// 504.842;
//��ͷ�������
double k1 = 0.0356;;
double k2 = -0.0737;
double p1 = -0.00099;
double p2 = -0.0030;
double k3 =0.00;

Mat patt;
 
int main()
{
	//��ʼ��������˴��滻Ϊ��������������
	
	cv::VideoCapture vcap;
	vcap.open(0);
	cv::Mat frame;
	cv::namedWindow("CamPos", 0);


		//cvSetMouseCallback("CamPos", on_mouse, NULL);//��������¼���ͨ����������׷�ٵ㡣

	patt = imread("D:/RM.jpg");
	cvtColor(patt, patt, CV_BGR2GRAY);
	resize(patt, patt, Size(200, 200));
	threshold(patt, patt, 100, 255, CV_THRESH_BINARY);
	imshow("Patt", patt);
		//��ʼ��λ�˹�����
		PNPSolver p4psolver;

		//��ʼ���������
		p4psolver.SetCameraMatrix(fx, fy, u0, v0);
		//���û������
		p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);

		//p4psolver.Points3D.push_back(cv::Point3f(100, 60, 0));		//P1��ά����ĵ�λ�Ǻ���
		//p4psolver.Points3D.push_back(cv::Point3f(83, 72, 0));	//P2
		//p4psolver.Points3D.push_back(cv::Point3f(116, 72, 0));	//P3
		//p4psolver.Points3D.push_back(cv::Point3f(90, 114, 0));	//P4
		//�������������������ӽ�ȥ
		p4psolver.Points3D.push_back(cv::Point3f(-60, -60, 0));		//P1��ά����ĵ�λ�Ǻ���
			
		p4psolver.Points3D.push_back(cv::Point3f(-60, 60, 0));	//
		p4psolver.Points3D.push_back(cv::Point3f(60, 60, 0));	//P3
	    p4psolver.Points3D.push_back(cv::Point3f(60, -60, 0));	//P2


		Point center;




		//������ͶӰ��ͼ�񣬼���ͶӰ���Ƿ���ȷ
		vector<cv::Point3f> r;
		r.push_back(cv::Point3f(-100, -100, 0));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(100, -100, 0));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(100, 100, 0));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(-100, 100, 0));//��ͶӰ���¼�����

		r.push_back(cv::Point3f(-100, -100, 200));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(100, -100, 200));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(100, 100, 200));//��ͶӰ���¼�����
		r.push_back(cv::Point3f(-100, 100, 200));//��ͶӰ���¼�����


		vcap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

		vcap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
		vcap.set(CV_CAP_PROP_FPS, 60);




	while(vcap.isOpened())
	{

	
		//p4psolver.Points3D.push_back(cv::Point3f(0, 100, 105));	//P5

		//std::cout << "������Ļ�ϵ��ȷ�������㣬������ĵ�ѡ˳��Ӧ��p4psolver.Points3D�Ĵ洢˳��һ�¡�" << endl;
 
		//��ȡ�����ǰ֡ͼ���޸�Ϊ����������
		//cv::Mat img = mvcam.Grub();
		//vcap.read(frame);
		vcap.read(frame);
		cv::Mat paintBoard = cv::Mat::zeros(frame.size(), CV_8UC3);//�½�һ��Mat�����ڴ洢���ƵĶ���

		cv::imshow("CamPos", (frame - paintBoard) + paintBoard + paintBoard);
		getFourVertexes(frame, p4psolver.Points2D, center);
		//cout << frame.cols << " "<<frame.rows<<endl;
		if (p4psolver.Points2D.size() == 4)
		{

			////��λ��
			p4psolver.Solve(PNPSolver::METHOD::CV_ITERATIVE);

			vector<cv::Point2f>	ps = p4psolver.WordFrame2ImageFrame(r);
 

			//Draw the reconstructed cube
			cv::line(frame, ps[4], ps[5], cv::Scalar(255, 255, 0), 2);
			cv::line(frame, ps[5], ps[6], cv::Scalar(255, 255, 0), 2);
			cv::line(frame, ps[6], ps[7], cv::Scalar(255, 255, 0), 2);
			cv::line(frame, ps[7], ps[4], cv::Scalar(255, 255, 0), 2);



			cv::line(frame, ps[0], ps[1], cv::Scalar(0, 255, 0), 3);
			cv::line(frame, ps[1], ps[2], cv::Scalar(0, 255, 0),3);
			cv::line(frame, ps[2], ps[3], cv::Scalar(0, 255, 0), 3);
			cv::line(frame, ps[3], ps[0], cv::Scalar(0, 255, 0),3);

			cv::line(frame, ps[0], ps[4], cv::Scalar(255, 0, 255), 3);
			cv::line(frame, ps[1], ps[5], cv::Scalar(255, 0, 255), 3);
			cv::line(frame, ps[2], ps[6], cv::Scalar(255, 0, 255), 3);
			cv::line(frame, ps[3], ps[7], cv::Scalar(255, 0, 255), 3);


			

			//���λ����Ϣ
			cout << "pitch:"<<p4psolver.Theta_W2C.x << " roll:" << p4psolver.Theta_W2C.y << " yaw:" << p4psolver.Theta_W2C.z << endl;
			//It semms the x,y,z results from solvPNP are not that trusty.
			//cout << "x;"<<p4psolver.Position_OcInW.x << " y:" << p4psolver.Position_OcInW.y << " z:"<< p4psolver.Position_OcInW.z << endl;

			//So calculate x,y directly.
			double dx = fx *(center.x - u0) * 0.001;// mm;
			double dy = fx * (center.y - v0) * 0.001;
			cout << "x: " << dx << " y:" << dy << " z:" << p4psolver.Position_OcInW.z <<endl;
		}
		p4psolver.Points2D.clear();
		cv::imshow("CamPos", (frame - paintBoard) + paintBoard + paintBoard);
		if( 27 == cv::waitKey(10))
			break;
	}
	vcap.release();//else
	//	std::cout << "�����ʼ��ʧ�ܣ�" << endl;
	return 0;
}