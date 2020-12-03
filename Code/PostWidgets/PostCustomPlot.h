#ifndef _POSTCUSTOMPLOT_H_

#define _POSTCUSTOMPLOT_H_

#include "PostWidgetsAPI.h"
#include "CurveAnalyse/qcustomplot.h"

namespace Post
{
	class POSTAPI PostCustomPlot : public QCustomPlot
	{
	public:
		PostCustomPlot(QWidget* w);
		~PostCustomPlot();

	protected:


	private:


	};


}

#endif