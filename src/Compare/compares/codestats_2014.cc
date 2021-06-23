#include "codestats_2014.h"
#include <QJsonObject>

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto DRONSIZE{ "DronSize" };

//#define DEBUG
//#define DEBUG_OPENCV

Compares::CodeStats2014::CodeStats2014(QJsonObject const &a_config)
{
	#ifdef DEBUG
	Logger->trace("Compare::CodeStats2014::CodeStats2014 m_dronSize:{}", m_dronSize);
	#endif

	m_dronSize = a_config[DRONSIZE].toInt();
}

void Compares::CodeStats2014::alertBadImage(const cv::Mat_<uchar> &image, QString name)
{
	if (image.empty() || image.cols == 0 || image.rows == 0)
	{
		spdlog::error("image frame is null. Name:{}\n", name.toStdString());
	}
}

void Compares::CodeStats2014::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->trace("imageErrors Compare::CodeStats2014 Main loop: m_res:{}", m_res);
	#endif

	int m_width = _data[0].processing.cols;
	int m_height = _data[0].processing.rows;
	m_res = (m_width * m_height - m_dronSize) / m_dronSize;

	//m_ROI = cv::Mat(m_height, m_width, CV_8UC1, cv::Scalar(255));

	const cv::Mat_<uchar> binary = _data[0].processing.clone();
	const cv::Mat_<uchar> gt = _data[1].processing.clone();
	
	
	cv::MatConstIterator_<uchar> itBinary = binary.begin();
	cv::MatConstIterator_<uchar> itGT = gt.begin();
	//cv::MatConstIterator_<uchar> itROI = m_ROI.begin();

	struct imageErrors m_errors2;
	m_errors2.fnError = 0;
	m_errors2.fpError = 0;
	m_errors2.tnError = 0;
	m_errors2.tpError = 0;
	m_errors2.nbShadowError = 0;

	cv::MatConstIterator_<uchar> itEnd = binary.end();
	for (; itBinary != itEnd; ++itBinary, ++itGT)
	{
		if (*itGT == 255)
		{ // Model thinks pixel is foreground  
			if (*itBinary == 255)
			{
				//++m_errors2.tpError; // and it is
				m_errors2.tpError+=1;
			}
			else
			{
				//++m_errors2.fpError; // but it's not
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

		if (*itGT == SHADOW) {
			if (*itBinary == WHITE) {
				++m_errors2.nbShadowError;
			}
		}
	}

	#ifdef DEBUG
	Logger->debug("imageErrors Compare::CodeStats2014 Main loop: m_res:{}", m_res);
	Logger->debug("CodeStats2014 tpError:{}", m_errors2.tpError);
	Logger->debug("CodeStats2014 fpError:{}", m_errors2.fpError);
	Logger->debug("CodeStats2014 fnError:{}", m_errors2.fnError);
	Logger->debug("CodeStats2014 tnError:{}", m_errors2.tnError);
	#endif
	#ifdef DEBUG_OPENCV
	cv::imshow("0", binary);
	cv::imshow("1", gt);


	#endif
	_data[0].ie = m_errors2;
}

void Compares::CodeStats2014::endProcess(std::vector<_postData> &_data) {}