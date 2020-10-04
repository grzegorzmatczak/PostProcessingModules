#ifndef COMPARE_H
#define COMPARE_H

#include <QDebug>
#include <QObject>

#include <opencv2/imgproc.hpp>

#include "../../include/postprocess.h"

class BaseCompare;
class QJsonObject;

class Compare : public PostProcess {
  Q_OBJECT
public:
  explicit Compare(QObject *parent = nullptr);
  void configure(QJsonObject const &a_config);

  void process(std::vector<_postData> &_data);
  double getElapsedTime();

private:
  BaseCompare *m_baseCompare{};
  cv::TickMeter m_timer;
};

#endif // COMPARE_H
