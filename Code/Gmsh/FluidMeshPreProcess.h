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
		//����������Χ
		void setFluidField(QList<double*>  c);
		//������������״
		void mergeFluidField(TopoDS_Compound* c,  QMultiHash<int, int> solids);
		//��ȡ�ڲ��߻����������  type 1-�� 2-��
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