#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>
#include <QObject>
#include <QPolygonF>
#include <QtCore>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"



typedef struct regionBounds {
  float top;
  float bottom;
  float left;
  float right;
} regionBounds;

typedef struct regionRectangle {
  float x;
  float y;
  float width;
  float height;
} regionRectangle;

typedef struct regionMask {
  float x;
  float y;
  float width;
  float height;
  char *data;
} regionMask;

typedef enum regionType { EMPTY, SPECIAL, RECTANGLE, POLYGON, MASK } regionType;
typedef struct regionContainer {
  enum regionType type;
  union {
    regionRectangle rectangle;
    regionMask mask;
  } data;

} regionContainer;

typedef struct regionOverlap {
  float overlap;
  float only1;
  float only2;
} regionOverlap;

struct imageErrors
{
    qint64 tpError;
    qint64 fpError;
    qint64 fnError;
    qint64 tnError;
    qint64 nbShadowError;
    double time;
};

struct _postData {
    cv::Mat processing;
    QString testStr;
    imageErrors ie;
    std::vector<QPolygonF> bounds;
    std::vector<cv::Rect> rects;
    QString nameOfTracker;
    QString nameOfFile;
};

class Tracker;
class Compare;

class PostProcess : public QObject {
  Q_OBJECT

public:
  enum Model { FILTER, ESTIMATOR };
  Q_ENUM(Model);

  explicit PostProcess(QObject *parent = nullptr);

  static PostProcess*make(QString model);
  virtual void configure(QJsonObject const &a_config) = 0;
  virtual void process(std::vector<_postData> &_data) = 0;
  virtual double getElapsedTime() = 0;
};

#endif // POST_PROCESS_H
