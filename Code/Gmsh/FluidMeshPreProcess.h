#ifndef  FLUIDMESHPREPROCESS_H__
#define  FLUIDMESHPREPROCESS_H__

#include <QList>
#include <QMultiHash>

class  TopoDS_Compound;
class  Bnd_Box;

namespace Gmsh
{
	class  FluidMeshPreProcess
	{
	public:
		FluidMeshPreProcess() = default;
		~FluidMeshPreProcess() = default;
		//设置流体域范围
		void setFluidField(QList<double*>  c);
		//生成流体域形状
		void mergeFluidField(TopoDS_Compound* c,  QMultiHash<int, int> solids);
		//获取内部边或者面的索引  type 1-边 2-面
		QList<int> getInerMember(int type);

	private:
		bool isIner(Bnd_Box* outer, Bnd_Box* iner, double tol);

	private:
		QList<double*> _fluidField{};

		TopoDS_Compound* _compand{};
		QMultiHash<int, int> _solidHash{};
	};


}


#endif