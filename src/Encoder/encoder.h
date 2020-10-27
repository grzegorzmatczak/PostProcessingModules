#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include "../../include/postprocessing.h"

#include <opencv2/imgproc.hpp>
#include <QVector>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

class BaseEncoder;
class QJsonArray;

namespace cv
{
	class Mat;
	class TickMeter;
} // namespace cv

class Encoder : public PostProcess {
  
  Q_OBJECT
 public:
  explicit Encoder(QObject *parent = nullptr);
  void configure(QJsonObject const &a_config);
  ~Encoder();

  void process(std::vector<_postData> & _data);
  void endProcess(std::vector<_postData> &_data);
  double getElapsedTime();

 private:
  BaseEncoder * m_baseEncoder;
  cv::TickMeter m_timer;
};

#endif // VIDEO_ENCODER_H
