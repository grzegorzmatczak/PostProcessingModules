#include "fitness.h"
#include "fitnesslist.h"


constexpr auto NAME{ "Name" };

Fitness::Fitness(QObject *parent)
  : PostProcess(parent)
{
  //Logger->trace("Fitness::Fitness()");
  m_baseFitness = new Fitnesses::None();
}

void Fitness::configure(QJsonObject const &a_config)
{
 Logger->trace("Fitness::configure()");
  auto const NAME_STRING{ a_config[NAME].toString() };
  delete m_baseFitness;
  m_timer.reset();

  if (NAME_STRING == "BGFitness") {
    m_baseFitness = new Fitnesses::BGFitness{ a_config };
  } else if (NAME_STRING == "None") {
    m_baseFitness = new Fitnesses::None();
  } else {
   Logger->error("Unsupported encoder type: {}", NAME_STRING.toStdString());
  }

}

void Fitness::process(std::vector<_postData> &_data)
{
  //Logger->trace("Fitness::process(a_image)");
  m_timer.start();
  m_baseFitness->process(_data);
  m_timer.stop();
}
double Fitness::getElapsedTime()
{
  return m_timer.getTimeMilli();
}
void Fitness::endProcess(std::vector<_postData> &_data) {
  m_baseFitness->endProcess(_data);
}