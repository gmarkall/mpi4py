/* Author:  Lisandro Dalcin   */
/* Contact: dalcinl@gmail.com */

#define pympiAtoI(p, i)                \
  do {                              \
    i = 0;                          \
    while (*p >= '0' && *p <= '9')  \
      { i *= 10; i += *p++ - '0'; } \
  } while(0)

#define pympiVersionParser(S, a, b, c)      \
  do {                                   \
    const char *s = S;                   \
    a = b = c = 0;                       \
    pympiAtoI(s, a); if(*s++ != '.') break; \
    pympiAtoI(s, b); if(*s++ != '.') break; \
    pympiAtoI(s, c); if(*s++ != '.') break; \
  } while(0)

static int PyMPI_Get_vendor(const char **vendor_name,
                            int         *version_major,
                            int         *version_minor,
                            int         *version_micro)
{
  const char* name="unknown";
  int major=0, minor=0, micro=0;

  /* MPICH3 */
#if defined(MPICH3)
  #if defined(MPICH_NUMVERSION)
  {int version = MPICH_NUMVERSION/1000;
  major = version/10000; version -= major*10000;
  minor = version/100;   version -= minor*100;
  micro = version/1;     version -= micro*1;}
  #elif defined(MPICH_VERSION)
  pympiVersionParser(MPICH_VERSION,major,minor,micro);
  #endif
  name = "MPICH";
  #if defined(DEINO_MPI)
  name = "DeinoMPI";
  #elif defined(MS_MPI)
  name = "Microsoft MPI";
  #endif
#endif

  /* MPICH2 */
#if defined(MPICH2)
  #if defined(MPICH2_NUMVERSION)
  {int version = MPICH2_NUMVERSION/1000;
  major = version/10000; version -= major*10000;
  minor = version/100;   version -= minor*100;
  micro = version/1;     version -= micro*1;}
  #elif defined(MPICH2_VERSION)
  pympiVersionParser(MPICH2_VERSION,major,minor,micro);
  #endif
  name = "MPICH2";
  #if defined(DEINO_MPI)
  name = "DeinoMPI";
  #elif defined(__SICORTEX__)
  name = "SiCortex MPI";
  #endif
#endif

  /* Microsoft MPI */
#if defined(MS_MPI)
  name = "Microsoft MPI";
#endif

  /* Open MPI */
#if defined(OPEN_MPI)
  name = "Open MPI";
  #if defined(OMPI_MAJOR_VERSION)
  major = OMPI_MAJOR_VERSION;
  #endif
  #if defined(OMPI_MINOR_VERSION)
  minor = OMPI_MINOR_VERSION;
  #endif
  #if defined(OMPI_RELEASE_VERSION)
  micro = OMPI_RELEASE_VERSION;
  #endif
#endif

  /* Platform MPI */
#if defined(PLATFORM_MPI)
  name = "Platform MPI";
  major = (PLATFORM_MPI>>24)&0xff;
  minor = (PLATFORM_MPI>>16)&0xff;
  micro = (PLATFORM_MPI>> 8)&0xff;
  major = (major/16)*10+(major%16);
#endif

  /* HP MPI */
#if defined(HP_MPI) && !defined(PLATFORM_MPI)
  name = "HP MPI";
  major = HP_MPI/100;
  minor = HP_MPI%100;
  #if defined(HP_MPI_MINOR)
  micro = HP_MPI_MINOR;
  #endif
#endif

  /* MPICH1 */
#if defined(MPICH_NAME) && MPICH_NAME==1
  name = "MPICH1";
  #if defined(MPICH_VERSION)
  pympiVersionParser(MPICH_VERSION,major,minor,micro);
  #endif
#endif

  /* LAM/MPI */
#if defined(LAM_MPI)
  name = "LAM/MPI";
  #if defined(LAM_MAJOR_VERSION)
  major = LAM_MAJOR_VERSION;
  #endif
  #if defined(LAM_MINOR_VERSION)
  minor = LAM_MINOR_VERSION;
  #endif
  #if defined(LAM_RELEASE_VERSION)
  micro = LAM_RELEASE_VERSION;
  #endif
#endif

  /* SGI */
#if defined(SGI_MPI)
  name = "SGI";
#endif

  if (vendor_name)   *vendor_name   = name;
  if (version_major) *version_major = major;
  if (version_minor) *version_minor = minor;
  if (version_micro) *version_micro = micro;

  return 0;
}

#undef pympiAtoI
#undef pympiVersionParser

/*
   Local variables:
   c-basic-offset: 2
   indent-tabs-mode: nil
   End:
*/
