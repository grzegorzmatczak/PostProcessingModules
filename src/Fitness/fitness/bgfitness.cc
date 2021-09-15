#include "bgfitness.h"
#include <QJsonObject>

//#define DEBUG 

constexpr auto FITENSS_FUNCTION{ "FitnessFunction" };
constexpr auto ACCURACY{ "Accuracy" };
constexpr auto RECALL{ "Recall" };
constexpr auto SPECIFICITY{ "Specificity" };
constexpr auto PRECISION{ "Precision" };
constexpr auto NEGATIVE_PRECISION{ "NegativePrecision" };
constexpr auto F_MEASURE{ "FMeasure" };
constexpr auto FITNESS_TIME{ "FitnessTime" };


Fitnesses::BGFitness::BGFitness(QJsonObject const &a_config)
{
	QString activeFitnessString = a_config[FITENSS_FUNCTION].toString();
	QStringList activeFitnessList = activeFitnessString.split("_");
	m_activeFitnessFunction.Accuracy = false;
	m_activeFitnessFunction.Recall = false;
	m_activeFitnessFunction.Specificity = false;
	m_activeFitnessFunction.FPR = false;
	m_activeFitnessFunction.FNR = false;
	m_activeFitnessFunction.PWC = false;
	m_activeFitnessFunction.Precision = false;
	m_activeFitnessFunction.NegativePrecision = false;
	m_activeFitnessFunction.FMeasure = false;
	m_activeFitnessFunction.fitnessTime = false;

	for (qint32 i = 0; i < activeFitnessList.size(); i++)
	{
		if (activeFitnessList[i] == QString::fromStdString(ACCURACY))
		{
			m_activeFitnessFunction.Accuracy = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(RECALL))
		{
			m_activeFitnessFunction.Recall = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(SPECIFICITY))
		{
			m_activeFitnessFunction.Specificity = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(PRECISION))
		{
			m_activeFitnessFunction.Precision = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(NEGATIVE_PRECISION))
		{
			m_activeFitnessFunction.NegativePrecision = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(F_MEASURE))
		{
			m_activeFitnessFunction.FMeasure = true;
		}
		if (activeFitnessList[i] == QString::fromStdString(FITNESS_TIME))
		{
			m_activeFitnessFunction.fitnessTime = true;
		}
	}

	m_errors.fnError = 0;
	m_errors.fpError = 0;
	m_errors.tnError = 0;
	m_errors.tpError = 0;
}

void Fitnesses::BGFitness::process(std::vector<_postData> &_data)
{
	
	m_errors.fnError += _data[0].ie.fnError;
	m_errors.fpError += _data[0].ie.fpError;
	m_errors.tnError += _data[0].ie.tnError;
	m_errors.tpError += _data[0].ie.tpError;

	#ifdef DEBUG
	Logger->debug("_data.size() == {}:", _data.size());	
	Logger->debug("_data[0].ie.tpError:{}",_data[0].ie.tpError);
	Logger->debug("_data[0].ie.fpError:{}", _data[0].ie.fpError);
	Logger->debug("_data[0].ie.tnError:{}", _data[0].ie.tnError);
	Logger->debug("_data[0].ie.tpError:{}", _data[0].ie.tpError);
	#endif
}

void Fitnesses::BGFitness::endProcess(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->debug("Fitnesses::BGFitness::endProcess");
	Logger->debug("m_errors.fnError:{}", m_errors.fnError);
	Logger->debug("m_errors.fpError:{}", m_errors.fpError);
	Logger->debug("m_errors.tnError:{}", m_errors.tnError);
	Logger->debug("m_errors.tpError:{}", m_errors.tpError);
	#endif

	double fn = m_errors.fnError ? m_errors.fnError : 0.1;
	double fp = m_errors.fpError ? m_errors.fpError : 0.1;
	double tn = m_errors.tnError ? m_errors.tnError : 0.1;
	double tp = m_errors.tpError ? m_errors.tpError : 0.1;
	double time = m_errors.time ? m_errors.time : 0.1;


	struct fitness fs;
	fs.fn = fn;
	fs.fp = fp;
	fs.tn = tn;
	fs.tp = tp;
	fs.time = time;
	fs.fitness = 0;
	//fs.fitnessTime = static_cast<double>((1 / fs.time) * 200);
	fs.fitnessTime = 0;

	fs.Accuracy = static_cast<double>((tp + tn) / (tp + tn + fp + fn));
	fs.Recall = (tp / (tp + fn));
	fs.Specificity = (tn / (tn + fp));
	fs.FPR = (fp / (fp + tn));
	fs.FNR = (fn / (tp + fn));
	fs.PWC = (100 * (fn + fp) / (tp + fn + fp + tn));
	fs.Precision = (tp / (tp + fp));
	fs.FMeasure = (2 * fs.Precision * fs.Recall) / (fs.Precision + fs.Recall);
	fs.NegativePrecision = (tn / (tn + fn));

	if (m_activeFitnessFunction.Accuracy == true)
	{
		fs.fitness = fs.fitness + fs.Accuracy;
	}
	if (m_activeFitnessFunction.Specificity == true)
	{
		fs.fitness = fs.fitness + fs.Specificity;
	}
	if (m_activeFitnessFunction.Recall == true)
	{
		fs.fitness = fs.fitness + fs.Recall;
	}
	if (m_activeFitnessFunction.Precision == true)
	{
		fs.fitness = fs.fitness + fs.Precision;
	}
	if (m_activeFitnessFunction.NegativePrecision == true)
	{
		fs.fitness = fs.fitness + fs.NegativePrecision;
	}
	if (m_activeFitnessFunction.FMeasure == true)
	{
		fs.fitness = fs.fitness + fs.FMeasure;
	}
	if (m_activeFitnessFunction.fitnessTime == true)
	{
		fs.fitness = fs.fitness + fs.fitnessTime;
	}
	_data[0].fs = fs;
	
	#ifdef DEBUG
	Logger->debug("_data[0].fs.Accuracy:{}", _data[0].fs.Accuracy);
	Logger->debug("_data[0].fs.Recall:{}", _data[0].fs.Recall);
	Logger->debug("_data[0].fs.FMeasure:{}", _data[0].fs.FMeasure);
	Logger->debug("_data[0].fs.Precision:{}", _data[0].fs.Precision);
	Logger->debug("_data[0].fs.fitness:{}", _data[0].fs.fitness);
	#endif

	//clean:
	m_errors.fnError = 0;
	m_errors.fpError = 0;
	m_errors.tnError = 0;
	m_errors.tpError = 0;
}

