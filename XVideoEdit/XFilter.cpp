#include "XFilter.h"
#include <QThread>
#include "XImagePro.h"
#include <QMutex>
class CXFilter : public XFilter
{
public:
	std::vector<XTask> tasks;
	QMutex mutex;

	cv::Mat Filter(cv::Mat mat1, cv::Mat mat2)
	{
		mutex.lock();
		XImagePro pro;
		pro.Set(mat1, mat2);

		for (int i = 0; i < tasks.size(); i++)
		{
			switch (tasks[i].type)
			{
			case XTASK_GAIN:
				//亮度对比度调整
				pro.Gain(tasks[i].para[0], tasks[i].para[1]);
				break;
			case XTASK_ROTATE90:
				pro.Rotate90();
				break;
			case XTASK_ROTATE180:
				pro.Rotate180();
				break;
			case XTASK_ROTATE270:
				pro.Rotate270();
				break;
			case XTASK_FLIPX:
				pro.FlipX();
				break;
			case XTASK_FLIPY:
				pro.FlipY();
				break;
			case XTASK_FLIPXY:
				pro.FlipXY();
				break;
			case XTASK_RESIZE:
				pro.Resize(tasks[i].para[0], tasks[i].para[1]);
				break;
			case XTASK_PYDOWN:
				pro.PyDown(tasks[i].para[0]);
				break;
			case XTASK_PYUP:
				pro.PyUp(tasks[i].para[0]);
				break;
			case XTASK_CLIP:
				pro.Clip(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
				break;
			case XTASK_GRAY:
				pro.Gray();
				break;
			case XTASK_MARK:
				pro.Mark(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2]);
				break;
			case XTASK_BLEND:
				pro.Blend(tasks[i].para[0]);
				break;
			case XTASK_MERGE:
				pro.Merge();
				break;
			default:
				break;
			}
		}

		cv::Mat re = pro.Get();
		mutex.unlock();
		return re;
	}

	void Add(XTask task)
	{
		mutex.lock();
		tasks.push_back(task);
		mutex.unlock();
	}

	void Clear()
	{
		mutex.lock();
		tasks.clear();
		mutex.unlock();
	}
};

XFilter *XFilter::Get()
{
	static CXFilter cx;
	return &cx;
}
XFilter::XFilter()
{
}


XFilter::~XFilter()
{
}
