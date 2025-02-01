#ifndef MB_VERSION_H
#define MB_VERSION_H

/** @brief Stack version in the form XX.YY.ZZ where
 *          XX = Major version number (increase = API compatibility broken and/or major feature addition)
 *          YY = Minor version number (increase = Minor feature addition)
 *          ZZ = Fix version number (increase = Bug fix(es))
 *
 *          Stack version number is defined in the main CMakeLists.txt file of
 *          the build tree
 */
#define MB_VERSION LIBMB_MAJOR "." LIBMB_MINOR "." LIBMB_FIX

/** @brief Return the version string of the library */
extern "C" const char* MB_Version();

#endif // MB_VERSION_H
