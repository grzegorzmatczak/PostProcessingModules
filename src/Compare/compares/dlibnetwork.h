#ifndef COMPARE_DLIB_NETWORK_H
#define COMPARE_DLIB_NETWORK_H

#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "basecompare.h"

class QJsonObject;

namespace Compares
{
	class DlibNetwork : public BaseCompare
	{
		public:
			DlibNetwork(QJsonObject const &a_config);
			void process(std::vector<_postData> &_data);
			void endProcess(std::vector<_postData> &_data);
			void alertBadImage(const cv::Mat_<uchar> &image, QString name);

		private:
			struct imageErrors m_errors;
			cv::Mat_<uchar> m_ROI;
			quint32 m_res;
	};

} // namespace Compares

#endif // COMPARE_DLIB_NETWORK_H
