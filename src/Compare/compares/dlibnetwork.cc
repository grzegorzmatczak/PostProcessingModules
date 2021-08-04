#include "dlibnetwork.h"
#include <QJsonObject>

//#define DEBUG
//#define DEBUG_OPENCV

Compares::DlibNetwork::DlibNetwork(QJsonObject const &a_config)
{
	#ifdef DEBUG
	Logger->debug("Compares::DlibNetwork::DlibNetwork()");//
	#endif
}

void Compares::DlibNetwork::alertBadImage(const cv::Mat_<uchar> &image, QString name)
{
	if (image.empty() || image.cols == 0 || image.rows == 0)
	{
		Logger->error("image frame is null. Name:{}\n", name.toStdString());
	}
}

void Compares::DlibNetwork::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->debug("Compares::DlibNetwork::process()");
	#endif

	const cv::Mat_<uchar> binary = _data[0].processing.clone();
	const cv::Mat_<float> gt = _data[1].processing.clone();

	cv::MatConstIterator_<uchar> itBinary = binary.begin();
	cv::MatConstIterator_<float> itGT = gt.begin();
	cv::MatConstIterator_<uchar> itEnd = binary.end();

	//cv::Mat img_compare = cv::Mat::zeros(_data[0].processing.size(), CV_8UC1);
	cv::Mat imgCompare = cv::Mat::zeros(_data[0].processing.size(), CV_8UC3);
	cv::MatIterator_<cv::Vec3b> itCompare = imgCompare.begin<cv::Vec3b>();

	struct imageErrors m_errors2 { 0, 0, 0, 0, 0 };

	for (; itBinary != itEnd; ++itBinary, ++itGT, ++itCompare)
	{
		if (*itGT > 0)
		{ // Model thinks pixel is foreground  
			if (*itBinary > 0)
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
		else if(*itGT <= 0 )
		{ // Model thinks pixel is background
			if (*itBinary <= 0)
			{
				m_errors2.tnError += 1; 
			} 
			else 
			{
				m_errors2.fnError += 1;
				// but it's not
				m_errors2.fpError += 1;
				(*itCompare)[0] = 0;
				(*itCompare)[1] = 0;
				(*itCompare)[2] = 255;//B
			}
		}
	}

	#ifdef DEBUG
		Logger->debug("DlibNetwork tpError:{}", m_errors2.tpError);
		Logger->debug("DlibNetwork fpError:{}", m_errors2.fpError);
		Logger->debug("DlibNetwork fnError:{}", m_errors2.fnError);
		Logger->debug("DlibNetwork tnError:{}", m_errors2.tnError);
	#endif
	#ifdef DEBUG_OPENCV
		cv::imshow("binary", _data[0].processing);
		cv::imshow("gt", _data[1].processing);
		cv::imshow("clean", _data[2].processing);
		cv::waitKey(1);
	#endif
	_postData temp{imgCompare.clone()};
	_data.push_back(temp);

	_data[0].ie = m_errors2;
}

void Compares::DlibNetwork::endProcess(std::vector<_postData> &_data){}
