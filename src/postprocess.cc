#include "../include/postprocess.h"
#include "Compare/compare.h"
#include "Tracker/tracker.h"

PostProcess::PostProcess(QObject *parent)
  : QObject(parent)
{

spdlog::set_level(static_cast<spdlog::level::level_enum>(0));
  spdlog::set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
}

PostProcess *PostProcess::make(QString model)
{
  //qInfo() << "Creating: " << model;
  //spdlog->trace("Filter type: {}", model.toStdString());
  qInfo() << "Creating: " << model;
  if (model == "Tracker") {
    return new Tracker(nullptr);
  } else if (model == "Compare") {
    return new Compare(nullptr);
  }
  return new Compare(nullptr);
}
