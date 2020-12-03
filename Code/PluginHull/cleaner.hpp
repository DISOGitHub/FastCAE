#ifndef CLEANER_H__
#define CLEANER_H__

#include <TopoDS_Shape.hxx>
#include <list>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>

int getCount(TopoDS_Shape* shape, TopAbs_ShapeEnum type)
{
	TopExp_Explorer ptExp(*shape, type);
	int index = 0;
	for (; ptExp.More(); ptExp.Next(), ++index){}
	return index;
}

bool isSameBox(const Bnd_Box& box1, const Bnd_Box & box2)
{
	gp_Pnt max1 = box1.CornerMax();
	gp_Pnt max2 = box2.CornerMax();

	gp_Pnt min1 = box1.CornerMin();
	gp_Pnt min2 = box2.CornerMin(); 

	double dmax = max1.Distance(max2);
	double dmin = min1.Distance(min2);
	
	return (dmin < 1e-5 && dmax < 1e-5);
}

void cleanEdge(TopoDS_Shape& shape)
{
//	const int n = getCount(&shape, TopAbs_EDGE);
	
	TopExp_Explorer ptExp(shape, TopAbs_EDGE);
	std::list<TopoDS_Shape> shapeList;
	for (int index = 0; ptExp.More(); ptExp.Next(), ++index)
	{
		TopoDS_Shape current = ptExp.Current();
		Bnd_Box cbox;
		BRepBndLib::Add(current, cbox);
		cbox.SetGap(0.00);
		bool append = true;
		for (TopoDS_Shape s : shapeList)
		{
			Bnd_Box sbox;
			BRepBndLib::Add(current, sbox);
			cbox.SetGap(0.00);
			if (isSameBox(cbox,sbox))
			{
				append = false;
				break;
			}
		}
		if (append)
			shapeList.push_back(current);
	}

	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aRes);

	for (TopoDS_Shape s : shapeList)
	{
		aBuilder.Add(aRes, s);
	}
	shape = aRes;
//	int ne = getCount( &shape, TopAbs_EDGE);
	
}



#endif