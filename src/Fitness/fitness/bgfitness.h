#ifndef FITNESS_BGFITNESS_H
#define FITNESS_BGFITNESS_H

#include "basefitness.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>

class QJsonObject;

namespace Fitnesses
{
class BGFitness : public BaseFitness
{
 public:
  BGFitness(QJsonObject const &a_config);

  void process(std::vector<_postData> &_data);
  void endProcess(std::vector<_postData> &_data);
  //struct fitness getFitness();
  //void addTime(double time);
  struct fitness Fitnesses::BGFitness::getFitness();

 private:
  imageErrors m_errors;
  fitnessFunction m_activeFitnessFunction;
};
} // namespace Fitnesses

#endif // FITNESS_BGFITNESS_H
