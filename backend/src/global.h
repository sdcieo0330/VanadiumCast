#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(VCL_LIB)
#  define VCL_EXPORT Q_DECL_EXPORT
#else
#  define VCL_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOBAL_H
