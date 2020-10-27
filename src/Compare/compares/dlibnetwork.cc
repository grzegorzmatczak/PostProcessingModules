#include "dlibnetwork.h"
#include <QJsonObject>

#define DEBUG FALSE

Compares::DlibNetwork::DlibNetwork(QJsonObject const &a_config)
{
}

void Compares::DlibNetwork::alertBadImage(const cv::Mat_<uchar> &image, QString name)
{
  if (image.empty() || image.cols == 0 || image.rows == 0) {
    spdlog::error("image frame is null. Name:{}\n", name.toStdString());
  }
}

void Compares::DlibNetwork::process(std::vector<_postData> &_data)
{
  Logger->trace("DlibNetwork");
  const cv::Mat_<uchar> binary = _data[0].processing.clone();
  const cv::Mat_<uchar> gt = _data[1].processing.clone();

  #if (DEBUG)
      cv::imshow("binary", _data[0].processing);
      cv::imshow("gt", _data[1].processing);
      cv::waitKey(1);
  #endif
  /*
  cv::imshow("0", binary);
  cv::imshow("1", gt);
  cv::waitKey(0);*/

  cv::MatConstIterator_<uchar> itBinary = binary.begin();
  cv::MatConstIterator_<uchar> itGT = gt.begin();
 // cv::MatConstIterator_<uchar> itROI = m_ROI.begin();
  struct imageErrors m_errors2;
  m_errors2.fnError = 0;
  m_errors2.fpError = 0;
  m_errors2.tnError = 0;
  m_errors2.tpError = 0;
  m_errors2.nbShadowError = 0;
  //Logger->trace("imageErrors Compare::CodeStats2014 Main loop: m_res:{}", m_res);
  cv::MatConstIterator_<uchar> itEnd = binary.end();
  for (; itBinary != itEnd; ++itBinary, ++itGT) {
    // Current pixel needs to be in the ROI && it must not be an unknown color

      if (*itBinary >0 ) { // Model thinks pixel is foreground  
          //Logger->trace("itBinary>1 input:{}  gt:{}", *itBinary, *itGT);
        if (*itGT > 0) {
          //++m_errors2.tpError; // and it is
          m_errors2.tpError+=1;
        } else {
          //++m_errors2.fpError; // but it's not
          m_errors2.fpError += 1;
        }
      } else { // Model thinks pixel is background
        //Logger->trace("itBinary=0 input:{}  gt:{}", *itBinary, *itGT);
        if (*itGT > 0) {
          m_errors2.fnError += 1; // but it's not
        } else {
          m_errors2.tnError += 1; // and it is
        }
      }
  }
#if (DEBUG)
  Logger->info("DlibNetwork tpError:{}", m_errors2.tpError);
  Logger->info("DlibNetwork fpError:{}", m_errors2.fpError);
  Logger->info("DlibNetwork fnError:{}", m_errors2.fnError);
  Logger->info("DlibNetwork tnError:{}", m_errors2.tnError);
#endif
  _data[0].ie = m_errors2;
 /// Logger->trace("imageErrors Compare::CodeStats2014 done");
  //return m_errors2;
}
void Compares::DlibNetwork::endProcess(std::vector<_postData> &_data) {}