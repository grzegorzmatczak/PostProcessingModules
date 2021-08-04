#include "preview.h"
#include <QJsonObject>

#ifdef __linux__
#include <unistd.h>
#endif // __linux__
#ifdef _WIN32
#endif // _WIN32
	
//#define DEBUG

Encoders::Preview::Preview(QJsonObject const &a_config)
: m_name("mp4")
, m_fps(30)
, m_iter(0)
, m_type("")
{
	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
	//std::string codecSTR = "avc1";
	//std::string codecSTR = "XVID";
	//std::string codecSTR = "PIM1";
	//std::string codecSTR = "MJPG";
	std::string codecSTR = "mp4v";
	m_code = cv::VideoWriter::fourcc(codecSTR[0], codecSTR[1], codecSTR[2], codecSTR[3]);
	//int stream = cv::CAP_GSTREAMER;
	int stream = cv::CAP_FFMPEG;
	
	m_name = a_config["Path"].toString();
	m_fps = (double)a_config["FPS"].toInt();
	m_iter = (a_config["Iter"].toInt());
	m_type = a_config["Type"].toString();
	m_width = a_config["Width"].toInt();
	m_height = a_config["Height"].toInt();

	m_videoShoal = new cv::VideoWriter((m_name + m_type).toStdString(),  
				 stream, m_code, m_fps, cv::Size(m_width, m_height), true);

	#ifdef DEBUG
	Logger->debug("name:{}", (m_name + m_type).toStdString());
	Logger->debug("m_fps:{}", m_fps);
	Logger->debug("m_iter:{}", m_iter);
	Logger->debug("m_type:{}", m_type.toStdString().c_str());
	Logger->debug("m_width:{}", m_width);
	Logger->debug("m_height:{}", m_height);
	#endif
}

void Encoders::Preview::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->debug("_data.size() == {}:", _data.size());
	Logger->debug("_data[0].processing.empty() == {}:",  _data[0].processing.empty());
	Logger->debug("_data[1].processing.empty()== {}:", _data[1].processing.empty());
	#endif

	cv::Mat m_image;
	cv::Mat m_gt;
	cv::Mat m_pre;
	cv::Mat m_post;

	cv::Mat Cat1;
	cv::Mat Cat2;
	cv::Mat Cat;

	if (_data.size() >= 2) 
	{
		m_image = Encoders::Preview::validBGR(_data[0].processing);
		m_gt = Encoders::Preview::validBGR(_data[1].processing);
	}
	if (_data.size() >= 3)
	{
		m_pre = Encoders::Preview::validBGR(_data[2].processing);
	}
	if (_data.size() >= 4)		
	{
		m_post = Encoders::Preview::validBGR(_data[3].processing);
	}
	
	if (_data.size() == 2) 
	{
		cv::hconcat(m_image, m_gt, Cat1);
		cv::resize(Cat1, Cat, cv::Size(static_cast<int>(m_width), static_cast<int>(m_height)), 0, 0, cv::INTER_NEAREST);

		#ifdef DEBUG
			cv::imshow("Cat", Cat);
			cv::waitKey(1);
		#endif

		m_videoShoal->write(Cat);
	}
	if (_data.size() == 3)
	{
		#ifdef DEBUG
		Logger->debug("_data.size() == {}:", _data.size());
		Logger->debug("_data[2].processing.empty() == {}:",  _data[2].processing.empty());
		#endif

		cv::hconcat(m_image, m_gt, Cat1);
		cv::hconcat(m_pre, Cat1, Cat);

		cv::resize(Cat, Cat, cv::Size(static_cast<int>(m_width), static_cast<int>(m_height)), 0, 0, cv::INTER_NEAREST);

		#ifdef DEBUG
			cv::imshow("Cat", Cat);
			cv::waitKey(1);
		#endif
		m_videoShoal->write(Cat);
	}
	if (_data.size() == 4)
	{
		#ifdef DEBUG
		Logger->debug("_data.size() == {}:", _data.size());
		Logger->debug("_data[2].processing.empty() == {}:",  _data[2].processing.empty());
		Logger->debug("_data[3].processing.empty()== {}:", _data[3].processing.empty());
		#endif

		cv::hconcat(m_image, m_gt, Cat1);
		cv::hconcat(m_pre, m_post, Cat2);
		cv::vconcat(Cat1, Cat2, Cat);

		cv::resize(Cat, Cat, cv::Size(static_cast<int>(m_width), static_cast<int>(m_height)), 0, 0, cv::INTER_NEAREST);

		#ifdef DEBUG
			cv::imshow("Cat", Cat);
			cv::waitKey(1);
		#endif
		m_videoShoal->write(Cat);
	}
}

cv::Mat Encoders::Preview::validBGR(cv::Mat & image)
{
	if(image.channels() == 1)
	{
		cv::Mat outputImage;
		cv::cvtColor(image, outputImage, cv::COLOR_GRAY2BGR);
		return outputImage;
	}
	else
	{
		return image.clone();
	}
}

void Encoders::Preview::endProcess(std::vector<_postData> &_data)
{
	m_iter++;
	#ifdef DEBUG
	Logger->debug("Encoders::Preview::endProcess(std::vector<_postData> &_data)");
	#endif

	m_videoShoal->release();
	delete m_videoShoal;
}
