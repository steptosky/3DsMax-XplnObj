
#ifndef UtilsExp_H
#define UtilsExp_H

#ifdef STS_UTILS_STATIC
#  define UtilsExp
#  define STSUTILS_NO_EXPORT
#else
#  ifndef UtilsExp
#    ifdef StsUtils_EXPORTS
        /* We are building this library */
#      define UtilsExp 
#    else
        /* We are using this library */
#      define UtilsExp 
#    endif
#  endif

#  ifndef STSUTILS_NO_EXPORT
#    define STSUTILS_NO_EXPORT 
#  endif
#endif

#ifndef UtilsDeprecated
#  define UtilsDeprecated __declspec(deprecated)
#endif

#ifndef UtilsDeprecated_EXPORT
#  define UtilsDeprecated_EXPORT UtilsExp UtilsDeprecated
#endif

#ifndef UtilsDeprecated_NO_EXPORT
#  define UtilsDeprecated_NO_EXPORT STSUTILS_NO_EXPORT UtilsDeprecated
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef STSUTILS_NO_DEPRECATED
#    define STSUTILS_NO_DEPRECATED
#  endif
#endif

#endif
