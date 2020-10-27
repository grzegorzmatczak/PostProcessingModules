#ifndef ENCODER_NONE_H
#define ENCODER_NONE_H

#include "baseencoder.h"

class QJsonObject;

namespace Encoders {
class None : public BaseEncoder {
 public:
	None();

	void process(std::vector<_postData> &_data);
	void endProcess(std::vector<_postData> &_data);

 private:
};
} // namespace Encoders

#endif // ENCODER_NONE_H
