#include "../include/postprocessing.h"
#include "Compare/compare.h"
#include "Tracker/tracker.h"
#include "Fitness/fitness.h"
#include "Encoder/encoder.h"

PostProcess::PostProcess(QObject *parent)
  : QObject(parent)
{
}

PostProcess *PostProcess::make(QString model)
{
  //qInfo() << "Creating: " << model;
  Logger->debug("PostProcessing type: {}", model.toStdString());

  if (model == "Tracker") {
    return new Tracker(nullptr);
  } else if (model == "Compare") {
    return new Compare(nullptr);
  } else if (model == "Fitness") {
    return new Fitness(nullptr);
  } else if (model == "Encoder") {
    return new Encoder(nullptr);
  }
  return new Compare(nullptr);
}
