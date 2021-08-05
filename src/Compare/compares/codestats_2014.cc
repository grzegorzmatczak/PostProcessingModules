#include "codestats_2014.h"
#include <QJsonObject>

//#define DEBUG
//#define DEBUG_OPENCV

Compares::CodeStats2014::CodeStats2014(QJsonObject const &a_config)
{
	#ifdef DEBUG
	Logger->debug("Compare::CodeStats2014::CodeStats2014 m_dronSize:{}", m_dronSize);
	#endif
}

void Compares::CodeStats2014::alertBadImage(const cv::Mat_<uchar> &image, QString name)
{
	if (image.empty() || image.cols == 0 || image.rows == 0)
	{
		Logger->error("CodeStats2014::alertBadImage() image frame is null :{}\n", name.toStdString());
	}
}

void Compares::CodeStats2014::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->debug("Compares::CodeStats2014::process()");
	#endif

	int m_width = _data[0].processing.cols;
	int m_height = _data[0].processing.rows;

	const cv::Mat_<uchar> binary = _data[0].processing.clone();
	const cv::Mat_<uchar> gt = _data[1].processing.clone();
	
	
	cv::MatConstIterator_<uchar> itBinary = binary.begin();
	cv::MatConstIterator_<uchar> itGT = gt.begin();
	cv::MatConstIterator_<uchar> itEnd = binary.end();

	cv::Mat imgCompare = cv::Mat(_data[0].processing.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	cv::MatIterator_<cv::Vec3b> itCompare = imgCompare.begin<cv::Vec3b>();

	struct imageErrors m_errors2 { 0, 0, 0, 0, 0 };

	for (; itBinary != itEnd; ++itBinary, ++itGT, ++itCompare)
	{
		if (*itGT == 255)
		{ // Model thinks pixel is foreground  
			if (*itBinary == 255)
			{
				// and it is
				m_errors2.tpError += 1;
				(*itCompare)[0] = 0;
				(*itCompare)[1] = 255;//G
				(*itCompare)[2] = 0;
			}
			else
			{
				// but it's not
				m_errors2.fpError += 1;
				(*itCompare)[0] = 255;//R
				(*itCompare)[1] = 0;
				(*itCompare)[2] = 0;
			}
		}
		else if(*itGT == 0 )
		{ // Model thinks pixel is background
			if (*itBinary == 0)
			{
				m_errors2.tnError += 1; 
			} 
			else 
			{
				m_errors2.fnError += 1;
				(*itCompare)[0] = 255;//R
				(*itCompare)[1] = 0;
				(*itCompare)[2] = 0;
			}
		}
	}

	#ifdef DEBUG
		Logger->debug("CodeStats2014 tpError:{}", m_errors2.tpError);
		Logger->debug("CodeStats2014 fpError:{}", m_errors2.fpError);
		Logger->debug("CodeStats2014 fnError:{}", m_errors2.fnError);
		Logger->debug("CodeStats2014 tnError:{}", m_errors2.tnError);
	#endif
	#ifdef DEBUG_OPENCV
		if (_data.size() >= 2)		
		{
		cv::imshow("binary", _data[0].processing);
		cv::imshow("gt", _data[1].processing);
		}
		if (_data.size() >= 4)		
		{
		cv::imshow("clean", _data[2].processing);
		cv::imshow("post", _data[3].processing);
		}
		cv::waitKey(0);
	#endif

	_postData temp{imgCompare.clone()};
	_data.push_back(temp);

	_data[0].ie = m_errors2;
}

void Compares::CodeStats2014::endProcess(std::vector<_postData> &_data) {}
