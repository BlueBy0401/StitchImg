// StitchImg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/stitching.hpp"
//#include <stitching/stitcher.hpp>
using namespace std;
using namespace cv;

#define SURF_MATCH_CONFIDENCE 0.65f
#define ORB_MATCH_CONFIDENCE  0.3f

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
int main()
{
	vector<Mat> imgs;
	char iname[256] = { '0' };
	int i = 1;
	while (true)
	{
		sprintf_s(iname, "%d.jpg", i);
		Mat img = imread(iname);
		if (img.empty())
		{
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
	//	Stitcher::Status status = stic.stitch(imgs, pano);
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

	stic.setFeaturesFinder(featuresFinder);

	Stitcher::Status status = stic.stitch(imgs, pano);

	////ƥ��ͼ�񼰹��������ת 
	//Stitcher::Status status = stic.estimateTransform(imgs);
	//if (status != Stitcher::OK)
	//{
	//	cout << "cannot stitch images!" << endl;
	//}
	////ƴ��ͼ��
	//status = stic.composePanorama(pano);
	if (status != Stitcher::OK)
	{
		cout << "cannot stitch images!" << endl;
	}
	else
	{
		imshow("pinjie", pano);
		imwrite("pinjie.jpg", pano);
	}
	

	waitKey(0);
    return 0;
}

