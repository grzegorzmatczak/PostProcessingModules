#include "dlibnetwork.h"
#include <QJsonObject>

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto DRONSIZE{ "DronSize" };

Compares::DlibNetwork::DlibNetwork(QJsonObject const &a_config)
{
  quint32 m_width = a_config[WIDTH].toInt();
  quint32 m_height = a_config[HEIGHT].toInt();
  m_ROI = cv::Mat(m_height, m_width, CV_8UC1, cv::Scalar(255));
  quint32 m_dronSize = a_config[DRONSIZE].toInt();
  m_res = (m_width * m_height - m_dronSize) / m_dronSize;
  Logger->trace("Compare::CodeStats2014::CodeStats2014 m_dronSize:{}", m_dronSize);
}

void Compares::DlibNetwork::alertBadImage(const cv::Mat_<uchar> &image, QString name)
{
  if (image.empty() || image.cols == 0 || image.rows == 0) {
    spdlog::error("image frame is null. Name:{}\n", name.toStdString());
  }
}

void Compares::DlibNetwork::process(std::vector<_postData> &_data)
{
  Logger->trace("DlibNetwork m_res:{}", m_res);
  const cv::Mat_<uchar> binary = _data[0].processing.clone();
  const cv::Mat_<uchar> gt = _data[1].processing.clone();
  /*
  cv::imshow("0", binary);
  cv::imshow("1", gt);
  cv::waitKey(0);*/

  cv::MatConstIterator_<uchar> itBinary = binary.begin();
  cv::MatConstIterator_<uchar> itGT = gt.begin();
  cv::MatConstIterator_<uchar> itROI = m_ROI.begin();
  struct imageErrors m_errors2;
  m_errors2.fnError = 0;
  m_errors2.fpError = 0;
  m_errors2.tnError = 0;
  m_errors2.tpError = 0;
  m_errors2.nbShadowError = 0;
  //Logger->trace("imageErrors Compare::CodeStats2014 Main loop: m_res:{}", m_res);
  cv::MatConstIterator_<uchar> itEnd = binary.end();
  for (; itBinary != itEnd; ++itBinary, ++itGT, ++itROI) {
    // Current pixel needs to be in the ROI && it must not be an unknown color

      if (*itBinary > 0) { // Model thinks pixel is foreground  
        if (*itGT >0) {
          //++m_errors2.tpError; // and it is
          m_errors2.tpError+=1;
        } else {
          //++m_errors2.fpError; // but it's not
          m_errors2.fpError += 1;
        }
      } else { // Model thinks pixel is background
        if (*itGT > 0) {
          m_errors2.fnError += 1; // but it's not
        } else {
          m_errors2.tnError += 1; // and it is
        }
      }
  }
  Logger->trace("DlibNetwork done");
  _data[0].ie = m_errors2;
 /// Logger->trace("imageErrors Compare::CodeStats2014 done");
  //return m_errors2;
}
void Compares::DlibNetwork::endProcess(std::vector<_postData> &_data) {}