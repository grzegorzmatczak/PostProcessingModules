#include "encoder.h"
#include "encoderlist.h"

//#define DEBUG_ENCODER_MODULES

constexpr auto NAME{ "Name" };

Encoder::Encoder(QObject *parent)
	: PostProcess(parent)
{
	#ifdef DEBUG_ENCODER_MODULES
	Logger->debug("Encoder::Encoder()");
	#endif
	m_baseEncoder = new Encoders::None();
}

Encoder::~Encoder()
{
	delete m_baseEncoder;
}

void Encoder::configure(QJsonObject const &a_config)
{
	#ifdef DEBUG_ENCODER_MODULES
	Logger->debug("Encoder::configure()");
	#endif
	auto const _name{ a_config[NAME].toString() };
	delete m_baseEncoder;
	m_timer.reset();

	if (_name == "Preview")
	{
		m_baseEncoder = new Encoders::Preview{ a_config };
	} 
	else if (_name == "None")
	{
		m_baseEncoder = new Encoders::None();
	}
	else 
	{
		Logger->error("Unsupported encoder type: {}", _name.toStdString());
	}
}

void Encoder::process(std::vector<_postData> &_data)
{
	#ifdef DEBUG_ENCODER_MODULES
	Logger->debug("Encoder::process()");
	#endif
	m_timer.start();
	m_baseEncoder->process(_data);
	m_timer.stop();
}

double Encoder::getElapsedTime()
{
	return m_timer.getTimeMilli();
}

void Encoder::endProcess(std::vector<_postData> &_data)
{
	#ifdef DEBUG_ENCODER_MODULES
	Logger->debug("Encoder::endProcess()");
	#endif
	m_baseEncoder->endProcess(_data);
}

