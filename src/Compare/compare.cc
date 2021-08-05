#include "compare.h"
#include "comparelist.h"

//#define DEBUG

constexpr auto NAME{ "Name" };


Compare::Compare(QObject *parent) : PostProcess(parent)
{
	#ifdef DEBUG
	Logger->debug("Encoder::Encoder()");
	#endif
	m_baseCompare = new Compares::None{};
}

void Compare::configure(QJsonObject const &a_config)
{
	auto const _name{ a_config[NAME].toString() };
	#ifdef DEBUG
	Logger->debug("Encoder::configure() type:{}", _name.toStdString());
	#endif
	delete m_baseCompare;
	m_timer.reset();

	if (_name == "None")
	{
		m_baseCompare = new Compares::None{};
	} else if (_name == "VOT")
	{
		m_baseCompare = new Compares::VOT{a_config};
	} else if (_name == "CodeStats2014")
	{
		m_baseCompare = new Compares::CodeStats2014{ a_config };
	} else if (_name == "VOTCpp")
	{
		m_baseCompare = new Compares::VOTCpp{ a_config };
	} else if (_name == "DlibNetwork")
	{
		m_baseCompare = new Compares::DlibNetwork{ a_config };
	} else
	{
		Logger->error("Unsupported encoder type: {}", _name.toStdString());
	}
}


void Compare::process(std::vector<_postData> &_data)
{
	m_timer.start();
	m_baseCompare->process(_data);
	m_timer.stop();
}
double Compare::getElapsedTime() { return m_timer.getTimeMilli(); }

void Compare::endProcess(std::vector<_postData> &_data) {}
