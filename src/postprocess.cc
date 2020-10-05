#include "../include/postprocess.h"
#include "Compare/compare.h"
#include "Tracker/tracker.h"
#include "Fitness/fitness.h"

PostProcess::PostProcess(QObject *parent)
  : QObject(parent)
{
}

PostProcess *PostProcess::make(QString model)
{
  //qInfo() << "Creating: " << model;
  Logger->trace("Filter type: {}", model.toStdString());

  if (model == "Tracker") {
    return new Tracker(nullptr);
  } else if (model == "Compare") {
    return new Compare(nullptr);
  } else if (model == "Fitness") {
    return new Fitness(nullptr);
  }
  return new Compare(nullptr);
}
