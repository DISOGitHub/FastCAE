#ifndef vtk_netcdf_config_h
#define vtk_netcdf_config_h

/* Define if this is a shared build */
#define BUILD_SHARED_LIBS
#if defined( BUILD_SHARED_LIBS ) && defined( _WIN32 )
#  define DLL_NETCDF
#endif

#endif
