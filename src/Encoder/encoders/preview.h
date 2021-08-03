#ifndef ENCODER_PREVIEW_H
#define ENCODER_PREVIEW_H

#include "baseencoder.h"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <QtCore>

class QJsonObject;

namespace Encoders 
{
	class Preview : public BaseEncoder 
	{
	 public:
	  Preview(QJsonObject const &a_config);

	  void process(std::vector<_postData> &_data);
	  void endProcess(std::vector<_postData> &_data);
	  void endVideo();

	 private:
		QString m_name;
		QString m_type;
		int m_code;
		double m_fps;
		int m_iter;
		int m_width;
		int m_height;
		cv::VideoWriter* m_videoShoal;
   	 	int iter;
		QString m_split;
	};
} // namespace Encoders

#endif // ENCODER_PREVIEW_H
