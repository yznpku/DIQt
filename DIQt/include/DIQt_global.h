#ifndef DIQT_GLOBAL_H
#define DIQT_GLOBAL_H

#if defined(DIQT_INCLUDE)
#define DIQT_EXPORT
#elif defined(DIQT_LIB)
#define DIQT_EXPORT Q_DECL_EXPORT
#else
#define DIQT_EXPORT Q_DECL_IMPORT
#endif

#endif // DIQT_GLOBAL_H
