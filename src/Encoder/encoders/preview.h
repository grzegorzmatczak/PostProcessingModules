#ifndef ENCODER_PREVIEW_H
#define ENCODER_PREVIEW_H

#include "baseencoder.h"

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
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
		qint32 m_code;
		double m_fps;
		QString m_iter;
		qint32 m_width;
		qint32 m_height;
		cv::VideoWriter m_videoShoal;
	};
} // namespace Encoders

#endif // ENCODER_PREVIEW_H
