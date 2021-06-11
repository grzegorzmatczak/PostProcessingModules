#include "postprocessing.h"
#include "Compare/compare.h"
#include "Tracker/tracker.h"
#include "Fitness/fitness.h"
#include "Encoder/encoder.h"

//#define DEBUG_POSTPROCESSING_MODULES

PostProcess::PostProcess(QObject *parent)
	: QObject(parent)
{
}

PostProcess *PostProcess::make(QString model)
{
	#ifdef DEBUG_POSTPROCESSING_MODULES
		Logger->debug("PostProcessing::make type: {}", model.toStdString());
	#endif

	if (model == "Tracker")
	{
		return new Tracker(nullptr);
	} 
	else if (model == "Compare")
	{
		return new Compare(nullptr);
	} 
	else if (model == "Fitness")
	{
		return new Fitness(nullptr);
	} 
	else if (model == "Encoder")
	{
		return new Encoder(nullptr);
	}
	return new Compare(nullptr);
}
