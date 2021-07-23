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

	struct imageErrors m_errors2 { 0, 0, 0, 0, 0 };

	for (; itBinary != itEnd; ++itBinary, ++itGT)
	{
		if (*itGT == 255)
		{ // Model thinks pixel is foreground  
			if (*itBinary == 255)
			{
				// and it is
				m_errors2.tpError += 1;
			}
			else
			{
				// but it's not
				m_errors2.fpError += 1;
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
		cv::imshow("binary", _data[0].processing);
		cv::imshow("gt", _data[1].processing);
		cv::waitKey(1);
	#endif
	_data[0].ie = m_errors2;
}

void Compares::CodeStats2014::endProcess(std::vector<_postData> &_data) {}
