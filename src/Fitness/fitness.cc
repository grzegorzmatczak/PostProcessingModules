#include "fitness.h"
#include "fitnesslist.h"

//#define DEBUG

constexpr auto NAME{ "Name" };


Fitness::Fitness(QObject *parent)
	: PostProcess(parent)
{
	#ifdef DEBUG
	Logger->debug("Fitness::Fitness()");
	#endif
	m_baseFitness = new Fitnesses::None();
}

void Fitness::configure(QJsonObject const &a_config)
{

	#ifdef DEBUG
	Logger->debug("Fitness::configure()");
	#endif
	auto const _name{ a_config[NAME].toString() };
	delete m_baseFitness;
	m_timer.reset();

	if (_name == "BGFitness")
	{
		m_baseFitness = new Fitnesses::BGFitness{ a_config };
	}
	else if (_name == "None")
	{
		m_baseFitness = new Fitnesses::None();
	}
	else
	{
		Logger->error("Unsupported fitness type: {}", _name.toStdString());
	}

}

void Fitness::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG
	Logger->debug("Fitness::process(a_image)");
	#endif
	m_timer.start();
	m_baseFitness->process(_data);
	m_timer.stop();
}
double Fitness::getElapsedTime()
{
	return m_timer.getTimeMilli();
}
void Fitness::endProcess(std::vector<_postData> &_data)
{
	m_baseFitness->endProcess(_data);
}