#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>
#include <QObject>
#include <QtCore>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/includespdlog.h"
#include "utils/configreader.h"


typedef struct regionBounds
{
	float top;
	float bottom;
	float left;
	float right;
} regionBounds;

typedef struct regionRectangle
{
	float x;
	float y;
	float width;
	float height;
} regionRectangle;

typedef struct regionMask
{
	float x;
	float y;
	float width;
	float height;
	char *data;
} regionMask;

typedef enum regionType { EMPTY, SPECIAL, RECTANGLE, POLYGON, MASK } regionType;
typedef struct regionContainer
{
	enum regionType type;
	union {
		regionRectangle rectangle;
		regionMask mask;
	} data;

} regionContainer;

typedef struct regionOverlap
{
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

struct fitnessFunction
{
	bool Accuracy;
	bool Recall;
	bool Specificity;
	bool FPR;
	bool FNR;
	bool PWC;
	bool Precision;
	bool NegativePrecision;
	bool FMeasure;
	bool fitnessTime;
};

struct fitness
{
	qint64 fn;
	qint64 fp;
	qint64 tn;
	qint64 tp;
	qint64 nb;
	double Accuracy;
	double Recall;
	double Specificity;
	double FPR;
	double FNR;
	double PWC;
	double Precision;
	double NegativePrecision;
	double FMeasure;
	double fitness;
	double rfitness;
	double cfitness;
	double time;
	double fitnessTime;
};

struct _postData
{
	cv::Mat processing;
	QString testStr;
	imageErrors ie;
	struct fitness fs;
	//std::vector<QPolygonF> bounds;
	std::vector<cv::Rect> rects;
	QString nameOfTracker;
	QString nameOfFile;
};

class Tracker;
class Compare;
class Fitness;

class PostProcess : public QObject
{
	Q_OBJECT

	public:

		explicit PostProcess(QObject *parent = nullptr);
		static PostProcess* make(QString model);
		virtual void configure(QJsonObject const &a_config) = 0;
		virtual void process(std::vector<_postData> &_data) = 0;
		virtual void endProcess(std::vector<_postData>& _data) = 0;
		virtual double getElapsedTime() = 0;
		
};

#endif // POST_PROCESS_H
