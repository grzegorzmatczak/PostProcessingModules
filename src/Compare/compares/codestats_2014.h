#ifndef CODE_STATS_2014_H
#define CODE_STATS_2014_H

#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "basecompare.h"

const uchar BLACK = 0;
const uchar SHADOW = 50;
const uchar OUTOFSCOPE = 85;
const uchar UNKNOWN = 170;
const uchar WHITE = 255;

class QJsonObject;

namespace Compares {
class CodeStats2014 : public BaseCompare
{
 public:
  CodeStats2014(QJsonObject const &a_config);
  void process(std::vector<_postData> &_data);
  void alertBadImage(const cv::Mat_<uchar> &image, QString name);

 private:
  struct imageErrors m_errors;
  cv::Mat_<uchar> m_ROI;
  quint32 m_res;
};
} // namespace Compares

#endif // CODE_STATS_2014_H
