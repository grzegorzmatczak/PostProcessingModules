#ifndef FITNESS_CALCULATION_H
#define FITNESS_CALCULATION_H

#include <QDebug>
#include <QObject>

#include <opencv2/imgproc.hpp>

#include "../../include/postprocessing.h"

class BaseFitness;
class QJsonObject;

class Fitness : public PostProcess {
  
    Q_OBJECT
 public:
  explicit Fitness(QObject *parent = nullptr);
  void configure(QJsonObject const &a_config);

  void process(std::vector<_postData> & _data);
  void endProcess(std::vector<_postData> &_data);
  double getElapsedTime();

 private:
  BaseFitness* m_baseFitness;
  cv::TickMeter m_timer;
};

#endif // FITNESS_CALCULATION_H