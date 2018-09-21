// StitchImg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/stitching.hpp"
#include "omp.h"
#include <opencv/cv.h>

#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <ctype.h>
//#include <stitching/stitcher.hpp>
using namespace std;
using namespace cv;

#define SURF_MATCH_CONFIDENCE 0.65f
#define ORB_MATCH_CONFIDENCE  0.3f

 

#define DATESET_COUNT 8
#define METHOD_COUNT 5
using namespace cv;
using namespace std;
using namespace xfeatures2d;


static inline Ptr<detail::FeaturesFinder> getFeatureFinder(const std::string& name)
{
	if (name == "orb")
		return makePtr<detail::OrbFeaturesFinder>();
	else if (name == "surf")
		return makePtr<detail::SurfFeaturesFinder>();
	else if (name == "akaze")
		return makePtr<detail::AKAZEFeaturesFinder>();
	else
		return Ptr<detail::FeaturesFinder>();
}
static std::string GetParam()
{
	return "surf";
}
int main12()
{
	/*
	vector<Mat> imgs;
	char iname[256] = { '0' };
	int i = 1;
	while (true)
	{
		sprintf_s(iname, "%d.jpg", i);
		Mat img = imread(iname);
		if (img.empty())
		{
			cout << "read image " << i << endl;
			cout << "cannot read image" << endl;
			break;
		}
		else
		{
			sprintf_s(iname, "pimg%d", i);
			namedWindow(iname);
			imshow(iname, img);
			//cvShowImage(iname, (CvArr*)&img);
			//cv::waitKey(0);
			imgs.push_back(img);
		}

		i++;
	}

	Mat pano;
	Stitcher stic = Stitcher::createDefault();
	////��򵥵�ƴ��
	//Stitcher::Status status = stic.stitch(imgs, pano);
	//if (status != Stitcher::OK)
	//{
	//	cout << "cannot stitch images!" << endl;
	//}
	//else
	//{
	//	imshow("pinjie", pano);
	//	imwrite("pinjie.jpg", pano);
	//}

	//�Ƚϸ��ӵ�ƴ�ӣ�����ƴ�ӷ��������Ѱ�ҷ�ʽ��
	//ƴ�ӷ��
	//PlaneWarper* cw = new PlaneWarper();
	//SphericalWarper* cw = new SphericalWarper();

	Ptr<detail::FeaturesFinder> featuresFinder = getFeatureFinder(GetParam());

	Ptr<detail::FeaturesMatcher> featuresMatcher = GetParam() == "orb"
		? makePtr<detail::BestOf2NearestMatcher>(false, ORB_MATCH_CONFIDENCE)
		: makePtr<detail::BestOf2NearestMatcher>(false, SURF_MATCH_CONFIDENCE);

	


	StereographicWarper* cw = new StereographicWarper();

	//stic.setWarper(cw);//makePtr<SphericalWarper>()
	stic.setWarper(makePtr<SphericalWarper>());
	//Ѱ��������ķ�ʽ
	//detail::SurfFeaturesFinder* featureFinder = new detail::SurfFeaturesFinder();
	clock_t starts; clock_t ends;
	stic.setFeaturesFinder(featuresFinder);
	starts = clock();
	Stitcher::Status status = stic.stitch(imgs, pano);
	
#pragma omp parallel sections
	{

#pragma  omp section
	{

	//ƥ��ͼ�񼰹��������ת 
	Stitcher::Status status = stic.estimateTransform(imgs);
	ends = clock();
	cout << "estimateTransform  " << ends - starts << endl;
	if (status != Stitcher::OK)
	{
		cout << "cannot stitch images!" << endl;
	}
	

	
	starts = clock();
	//ƴ��ͼ��
	status = stic.composePanorama(pano);
	ends = clock();
	cout << "composePanorama  " << ends - starts << endl;
	
	if (status != Stitcher::OK)
	{
		cout << "cannot stitch images!" << endl;
	}
	else
	{
		imshow("pinjie", pano);
		imwrite("pinjie.jpg", pano);
	}
	}
	}


	waitKey(0);
    return 0;
	*/


	string strDateset[DATESET_COUNT];
	strDateset[0] = "bark"; strDateset[1] = "bikes"; strDateset[2] = "boat"; strDateset[3] = "graf"; strDateset[4] = "leuven";
	strDateset[5] = "trees"; strDateset[6] = "ubc"; strDateset[7] = "wall";
	string strMethod[METHOD_COUNT];
	strMethod[0] = "SIFT"; strMethod[1] = "SURF"; strMethod[2] = "BRISK"; strMethod[3] = "ORB"; strMethod[4] = "AKAZE";
	////�ݹ��ȡĿ¼��ȫ���ļ�
	vector<string> files;
	Mat descriptors1;
	std::vector<KeyPoint> keypoints1;
	Mat descriptors2;
	std::vector<KeyPoint> keypoints2;
	std::vector< DMatch > matches;
	std::vector< DMatch > good_matches;
	////����ģ������
	int innersize = 0;
	Mat img1;
	Mat imgn;
	int64 t = getTickCount();
	std::cout << "�ӣɣƣԡ��ӣգңơ��£ңɣӣˡ��ϣң¡�A K A Z E�㷨����ʵ�鿪ʼ" << endl;
	//��������������Ѱ�ҷ���
	for (int imethod = METHOD_COUNT - 1; imethod < METHOD_COUNT; imethod++)
	{

		string _strMethod = strMethod[imethod];
		std::cout << "��ʼ����" << _strMethod << "����" << endl;
		//��������·��
		for (int idateset = 0; idateset < DATESET_COUNT; idateset++)
		{
			//��ò���ͼƬ���Ե�ַ
			string path = "E:/template/dateset/" + strDateset[idateset];
			std::cout << "���ݼ�Ϊ" << strDateset[idateset];
			//��õ������ݼ��е�ͼƬ
			//GO::getFiles(path, files, "r");
			std::cout << " ��" << files.size() << "��ͼƬ" << endl;
			files.push_back("1.jpg");
			files.push_back("2.jpg");
			files.push_back("3.jpg");

			for (int iimage = 1; iimage < files.size(); iimage++)
			{
				//ʹ��img1�Ա����µ�ͼƬ���ó����    
				img1 = imread(files[0], 0);
				imgn = imread(files[iimage], 0);
				//�����������㷨����ƥ�䷽��
				Ptr<Feature2D>  extractor;
				BFMatcher matcher;
				switch (imethod)
				{
				case 0: //"SIFT"
					extractor = SIFT::create();
					matcher = BFMatcher(NORM_L2);
					break;
				case 1: //"SURF"
					extractor = SURF::create();
					matcher = BFMatcher(NORM_L2);
					break;
				case 2: //"BRISK"
					extractor = BRISK::create();
					matcher = BFMatcher(NORM_HAMMING);
					break;
				case 3: //"ORB"
					extractor = ORB::create();
					matcher = BFMatcher(NORM_HAMMING);
					break;
				case 4: //"AKAZE"
					extractor = AKAZE::create();
					matcher = BFMatcher(NORM_HAMMING);
					break;
				}
				try
				{
					extractor->detectAndCompute(img1, Mat(), keypoints1, descriptors1);
					extractor->detectAndCompute(imgn, Mat(), keypoints2, descriptors2);
					matcher.match(descriptors1, descriptors2, matches);
				}
				catch (...)
				{
					cout << " ��������ȡʱ�������� " << endl;
					continue;
				}

				//����������д�ƥ��
				double max_dist = 0;
				double min_dist = 100;
				for (int a = 0; a < matches.size(); a++)
				{
					double dist = matches[a].distance;
					if (dist < min_dist) min_dist = dist;
					if (dist > max_dist) max_dist = dist;
				}
				for (int a = 0; a < matches.size(); a++)
				{
					if (matches[a].distance <= max(2 * min_dist, 0.02))
						good_matches.push_back(matches[a]);
				}
				if (good_matches.size() < 4)
				{
					cout << " ��Ч��������ĿС��4������ƥ��ʧ�� " << endl;
					continue;
				}
				//ͨ��RANSAC�����������е�������Խ��С��ᴿ��
				std::vector<Point2f> obj;
				std::vector<Point2f> scene;
				for (int a = 0; a < (int)good_matches.size(); a++)
				{
					//�ֱ�������good_matches��Ӧ�ĵ��ѹ������,ֻ��Ҫѹ������Ϣ�Ϳ���
					obj.push_back(keypoints1[good_matches[a].queryIdx].pt);
					scene.push_back(keypoints2[good_matches[a].trainIdx].pt);
				}
				//���㵥Ӧ������calib3d��)
				Mat H;
				try
				{
					H = findHomography(obj, scene);
				}
				catch (...)
				{
					cout << " findHomographyʧ�� " << endl;
					continue;
				}
				if (H.rows < 3)
				{
					cout << " findHomographyʧ�� " << endl;
					continue;
				}
				//�����ڵ���Ŀ
				Mat matObj;
				Mat matScene;
				CvMat* pcvMat = &(CvMat)H;
				const double* Hmodel = pcvMat->data.db;
				double Htmp = Hmodel[6];
				for (int isize = 0; isize < obj.size(); isize++)
				{
					double ww = 1. / (Hmodel[6] * obj[isize].x + Hmodel[7] * obj[isize].y + 1.);
					double dx = (Hmodel[0] * obj[isize].x + Hmodel[1] * obj[isize].y + Hmodel[2])*ww - scene[isize].x;
					double dy = (Hmodel[3] * obj[isize].x + Hmodel[4] * obj[isize].y + Hmodel[5])*ww - scene[isize].y;
					float err = (float)(dx*dx + dy*dy); //3������֮����Ϊ��ͬһ����
					if (err < 9)
					{
						innersize = innersize + 1;
					}
				}
				//��ӡ�ڵ�ռȫ��������ı���
				float ff = (float)innersize / (float)good_matches.size();
				cout << ff;
				//��ӡʱ��
				cout << " " << ((getTickCount() - t) / getTickFrequency()) << endl;
				t = getTickCount();
				//���Ч���Ϻã����ӡ����
				Mat matTmp;
				if (ff == 1.0)
				{
					drawMatches(img1, keypoints1, imgn, keypoints2, good_matches, matTmp);
					char charJ[255]; sprintf_s(charJ, "_%d.jpg", iimage);
					string strResult = strDateset[idateset] + _strMethod + charJ;
					imwrite(strResult, matTmp);
				}
				ff = 0;
				innersize = 0;
				matches.clear();
				good_matches.clear();
			}
			files.clear();
		}
	}
	getchar();
	waitKey();
	return 0;
}

