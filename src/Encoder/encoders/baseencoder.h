#ifndef BASE_ENCODER_H
#define BASE_ENCODER_H

#include "../encoder.h"

#include <opencv2/imgproc.hpp>

class BaseEncoder
{
 public:
  BaseEncoder();
  virtual ~BaseEncoder();

  virtual void process(std::vector<_postData> &_data) = 0;
  virtual void endProcess(std::vector<_postData> &_data) = 0;
};
#endif // BASE_ENCODER_H
