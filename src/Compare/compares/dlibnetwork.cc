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
  int m_width = _data[0].processing.cols;
  int m_height = _data[0].processing.rows;

  //m_res = (m_width * m_height - m_dronSize) / m_dronSize;


  const cv::Mat_<uchar> binary = _data[0].processing.clone();
  const cv::Mat_<uchar> gt = _data[1].processing.clone();

#if (DEBUG)
  //Logger->critical("DlibNetwork  m_width:{}", m_width);
  //Logger->critical("DlibNetwork  m_height:{}", m_height);
  cv::imshow("binary", _data[0].processing);
  cv::imshow("gt", _data[1].processing);
  cv::waitKey(1);
#endif
  struct imageErrors m_errors2 { 0, 0, 0, 0, 0 };



  for (int i = 0; i < m_width; i++) {
#if (DEBUG)
   // Logger->trace("DlibNetwork:{}", i);
#endif
    for (int j = 0; j < m_height; j++) {

      if (_data[0].processing.at<unsigned char>(cv::Point(i, j)) > 0)
      {   // Binary   - true
        if (_data[1].processing.at<unsigned char>(cv::Point(i, j)) > 0)
        { // Gt       - true
          m_errors2.tpError += 1;
        }
        else
        { // Gt       - false
          m_errors2.fpError += 1;
        }
      }
      else
      {                                                            // Binary   - false
        if (_data[1].processing.at<unsigned char>(cv::Point(i, j)) > 0)
        { // Gt       - true
          m_errors2.fnError += 1;
        }
        else { // Gt       - false
          m_errors2.tnError += 1;
        }
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
}

/*
void Compares::DlibNetwork::process2(std::vector<_postData> &_data)
{
  Logger->trace("DlibNetwork");
  const cv::Mat_<uchar> binary = _data[0].processing.clone();
  const cv::Mat_<uchar> gt = _data[1].processing.clone();

  #if (DEBUG)
      cv::imshow("binary", _data[0].processing);
      cv::imshow("gt", _data[1].processing);
      cv::waitKey(0);
  #endif


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
}
*/





void Compares::DlibNetwork::endProcess(std::vector<_postData> &_data) {}