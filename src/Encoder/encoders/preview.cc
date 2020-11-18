#include "preview.h"
#include <QJsonObject>

#define DEBUG FALSE

Encoders::Preview::Preview(QJsonObject const &a_config)
{
  std::string codecSTR = "avc1";
  m_code = cv::VideoWriter::fourcc(codecSTR[0], codecSTR[1], codecSTR[2], codecSTR[3]);

  m_name = a_config["Path"].toString();
  m_fps = (double)a_config["FPS"].toInt();
  m_iter = QString::number(a_config["Iter"].toInt());
  m_type = a_config["Type"].toString();
  m_width = a_config["Width"].toInt();
  m_height = a_config["Height"].toInt();
  m_videoShoal = {
    (m_name + m_iter + m_type).toStdString(), cv::CAP_FFMPEG, m_code, m_fps, cv::Size(m_width, m_height), true
  };
  Logger->trace("name:{}", (m_name + m_iter + m_type).toStdString().c_str());
 Logger->trace("m_fps:{}", m_fps);
  Logger->trace("m_iter:{}", m_iter.toStdString());
  Logger->trace("m_type:{}", m_type.toStdString().c_str());
  Logger->trace("m_width:{}", m_width);
  Logger->trace("m_height:{}", m_height);
  iter = 0;
}

void Encoders::Preview::process(std::vector<_postData> &_data)
{
#if (DEBUG)
  Logger->info("_data.size() == {}:", _data.size());


#endif

  if (_data.size() == 2) {

    cv::Mat a_image = _data[0].processing.clone();
    cv::Mat a_gt = _data[1].processing.clone();
    cv::Mat m_image;
    cv::Mat m_gt;
    cv::cvtColor(a_image, m_image, cv::COLOR_GRAY2BGR);
    cv::cvtColor(a_gt, m_gt, cv::COLOR_GRAY2BGR);
    cv::Mat Cat1;
    cv::hconcat(m_image, m_gt, Cat1);
    cv::Mat Cat;
    cv::resize(Cat1, Cat, cv::Size(static_cast<int>(m_width), static_cast<int>(m_height)), 0, 0, cv::INTER_NEAREST);
    m_videoShoal.write(Cat);
  }
  if (_data.size() == 4) {
    cv::Mat a_image = _data[0].processing.clone();
    cv::Mat a_gt = _data[1].processing.clone();
    cv::Mat a_pre = _data[2].processing.clone();
    cv::Mat a_post = _data[3].processing.clone();

    cv::Mat m_image;
    cv::Mat m_gt;
    cv::Mat m_pre;
    cv::Mat m_post;

    cv::cvtColor(a_image, m_image, cv::COLOR_GRAY2BGR);
    cv::cvtColor(a_gt, m_gt, cv::COLOR_GRAY2BGR);
    cv::cvtColor(a_pre, m_pre, cv::COLOR_GRAY2BGR);
    cv::cvtColor(a_post, m_post, cv::COLOR_GRAY2BGR);

    cv::Mat Cat1;
    cv::Mat Cat2;
    cv::Mat Cat;

    cv::hconcat(m_image, m_gt, Cat1);
    cv::hconcat(m_pre, m_post, Cat2);
    cv::vconcat(Cat1, Cat2, Cat);

    cv::resize(Cat, Cat, cv::Size(static_cast<int>(m_width), static_cast<int>(m_height)), 0, 0, cv::INTER_NEAREST);
    m_videoShoal.write(Cat);


  }


}
void Encoders::Preview::endProcess(std::vector<_postData> &_data)
{
  iter++;
#if (DEBUG)
  Logger->info("Encoders::Preview::endProcess(std::vector<_postData> &_data)");
 #endif
  m_videoShoal.release();
  m_videoShoal = {
    (m_name + m_iter +"_" + QString::number(iter) + m_type ).toStdString(), cv::CAP_FFMPEG, m_code, m_fps, cv::Size(m_width, m_height), true
  };
}
