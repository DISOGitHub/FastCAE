#ifndef _GMSH_API_H__
#define _GMSH_API_H__

#include <QtCore/QtGlobal>


#if defined(GMSH_API)
#define GMSHAPI Q_DECL_EXPORT
#else
#define  GMSHAPI Q_DECL_IMPORT
#endif

#endif
