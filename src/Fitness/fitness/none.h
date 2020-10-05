#ifndef FITNESS_NONE_H
#define FITNESS_NONE_H

#include "basefitness.h"

class QJsonObject;

namespace Fitnesses {
class None : public BaseFitness {
 public:
  None();

  void process(std::vector<_postData> &_data);
  void endProcess(std::vector<_postData> &_data);

 private:
};
} // namespace Fitnesses

#endif // FITNESS_NONE_H