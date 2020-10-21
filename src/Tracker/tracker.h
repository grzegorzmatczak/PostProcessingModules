#ifndef TRACKER_H
#define TRACKER_H

#include <QDebug>
#include <QObject>

#include <opencv2/imgproc.hpp>

#include "../../include/postprocessing.h"

class BaseTracker;
class QJsonObject;

class Tracker : public PostProcess {
  Q_OBJECT
public:
  explicit Tracker(QObject *parent = nullptr);
  void configure(QJsonObject const &a_config);

  void process(std::vector<_postData> &_data);
  void endProcess(std::vector<_postData> &_data);
  double getElapsedTime();

private:
  BaseTracker *m_baseTracker{};
  cv::TickMeter m_timer;
};

#endif // TRACKER_H
