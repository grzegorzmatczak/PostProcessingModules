#include "opencv_trackers.h"
#include <QPolygonF>

constexpr auto TRACKERS{ "Trackers" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto INIT_FRAME{ "InitFrame" };
constexpr auto X{ "X" };
constexpr auto Y{ "Y" };
constexpr auto NAME{ "Name" };

Trackers::Trackers::Trackers(QJsonObject const &a_config)
    : m_configTracker(a_config[TRACKERS].toArray()), m_frameIteration(0) {

  for (int i = 0; i < m_configTracker.size(); i++) {
    QString _name = m_configTracker[i].toObject()[NAME].toString();
    if (_name == "KCF") {
      m_trackers.push_back(cv::TrackerKCF::create());
      // Logger->trace("Create KCF tracker");
    } else if (_name == "MIL") {
      m_trackers.push_back(cv::TrackerMIL::create());
      // Logger->trace("Create MIL tracker");
    } else if (_name == "MedianFlow") {
      m_trackers.push_back(cv::TrackerMedianFlow::create());
      // Logger->trace("Create MedianFlow tracker");
    } else if (_name == "CSRT") {
      m_trackers.push_back(cv::TrackerCSRT::create());
      // Logger->trace("Create CSRT tracker");
    } else if (_name == "BOOSTING") {
      m_trackers.push_back(cv::TrackerBoosting::create());
      // Logger->trace("Create BOOSTING tracker");
    } else if (_name == "TLD") {
      m_trackers.push_back(cv::TrackerTLD::create());
      // Logger->trace("Create TLD tracker");
    } else if (_name == "GOTURN") {
      m_trackers.push_back(cv::TrackerGOTURN::create());
      // Logger->trace("Create GOTURN tracker");
    } else if (_name == "MOSSE") {
      m_trackers.push_back(cv::TrackerMOSSE::create());
      // Logger->trace("Create MOSSE tracker");
    }

    m_activeTracking.push_back(false);
    m_frameTracking.push_back(
        m_configTracker[i].toObject()[INIT_FRAME].toInt());
  }
}

void Trackers::Trackers::process(std::vector<_postData> &_data) {
  // Logger->trace("Trackers::Tracker::process()");

  /*
  QPolygonF polygon;
  QPointF(0, 0);
  for (int i = 0; i < 8; i++) {
    polygon << QPointF(0, 0);
  }*/
  cv::Rect2d rect{0, 0, 0, 0};

  bool ok = false;
  for (int i = 0; i < m_configTracker.size(); i++) {
    if (m_activeTracking[i]) {
      // Logger->trace("Trackers::Tracker::process() update");
      ok = m_trackers[i]->update(_data[0].processing, rect);
      // Logger->trace("Trackers::Tracker::process() ok:{}", ok);
    }

    else if (m_frameIteration == m_frameTracking[i]) {
      // Logger->trace("Trackers::Tracker::process() init");
      cv::Rect2d rectCV =
          cv::Rect2d(m_configTracker[i].toObject()[X].toDouble(),
                     m_configTracker[i].toObject()[Y].toDouble(),
                     m_configTracker[i].toObject()[WIDTH].toDouble(),
                     m_configTracker[i].toObject()[HEIGHT].toDouble());

      // qDebug() << "rectCV.x:" << rectCV.x;

      ok = m_trackers[i]->init(_data[0].processing, rectCV);
      // Logger->trace("Trackers::Tracker::process() ok:{}", ok);

      m_activeTracking[i] = true;
    } else {
      // Logger->trace("else:{}", i);
    }
    QRectF rectOutput = QRectF(rect.x, rect.y, rect.width, rect.height);
    QPolygonF rectPolygonF = QPolygonF(rectOutput);

    _data[0].bounds.push_back(rectPolygonF);

    for (int i = 0; i < 3; i++) {
      cv::line(
          _data[0].processing,
          cv::Point2f(rectPolygonF.at(i).x(), rectPolygonF.at(i).y()),
          cv::Point2f(rectPolygonF.at(i + 1).x(), rectPolygonF.at(i + 1).y()),
          cv::Scalar(0, 255, 0), 1);
    }
    cv::line(_data[0].processing,
             cv::Point2f(rectPolygonF.at(0).x(), rectPolygonF.at(0).y()),
             cv::Point2f(rectPolygonF.at(3).x(), rectPolygonF.at(3).y()),
             cv::Scalar(0, 255, 0), 1);

    cv::Point point =
        cv::Point(rectPolygonF.at(0).x() + 3, rectPolygonF.at(0).y() + 10);

    QString data = m_configTracker[i].toObject()[NAME].toString();
    cv::putText(_data[0].processing, data.toStdString(), point,
                cv::FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0, 0, 0), 2);
    cv::putText(_data[0].processing, data.toStdString(), point,
                cv::FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(255, 255, 255), 0.7);
  }

  // Logger->trace("Trackers::Tracker::process() i:{}, ok:{}",
  // m_frameIteration, ok);

  m_frameIteration++;
}

void Trackers::Trackers::endProcess(std::vector<_postData> &_data) {}