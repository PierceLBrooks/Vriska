
#include <Vriska/Threads/LinuxCondVar.h>

namespace Vriska
{
  LinuxCondVar::LinuxCondVar(INativeMutex* mutex) : INativeCondVar(mutex)
  {
    pthread_cond_init(&_condvar, NULL);
  }

  LinuxCondVar::~LinuxCondVar()
  {
    pthread_cond_destroy(&_condvar);
  }
  
# ifdef VRISKA_MINGW
  // https://stackoverflow.com/a/5404467

  LARGE_INTEGER			LinuxCondVar::getFileTime()
  {
      SYSTEMTIME s;
      FILETIME f;
      LARGE_INTEGER t;

      s.wYear = 1970;
      s.wMonth = 1;
      s.wDay = 1;
      s.wHour = 0;
      s.wMinute = 0;
      s.wSecond = 0;
      s.wMilliseconds = 0;
      SystemTimeToFileTime(&s, &f);
      t.QuadPart = f.dwHighDateTime;
      t.QuadPart <<= 32;
      t.QuadPart |= f.dwLowDateTime;
      return (t);
  }

  void			LinuxCondVar::getTime(timespec * ts)
  {
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFileTime();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    (ts)->tv_sec = t.QuadPart / 1000000;
    (ts)->tv_nsec = ( t.QuadPart % 1000000 ) * 1000;
  }
#endif // !VRISKA_MINGW

  bool			LinuxCondVar::notify(bool lockMutex)
  {
    bool		res;

    if (lockMutex)
      ownLock();
    res = (pthread_cond_signal(&_condvar) == 0);
    if (lockMutex)
      ownUnlock();
    return (res);
  }

  bool			LinuxCondVar::notifyAll(bool lockMutex)
  {
    bool		res;

    if (lockMutex)
      ownLock();
    res = (pthread_cond_broadcast(&_condvar) == 0);
    if (lockMutex)
      ownUnlock();
    return (res);
  }

  INativeCondVar::Result	LinuxCondVar::wait(Time const & timeout, bool lockMutex)
  {
    pthread_mutex_t*		mutex = reinterpret_cast<pthread_mutex_t *>(_mutex.getNative());
    int				ret;
    timespec			ts;

    if (lockMutex)
      ownLock();
    if (timeout != Time::Zero)
      {
  	get_clocktime(&ts);
  	ts.tv_nsec += timeout.getNano();
  	ts.tv_sec += timeout.getSeconds();
  	ret = pthread_cond_timedwait(&_condvar, mutex, &ts);
  	if (ret == ETIMEDOUT)
  	  return (INativeCondVar::Timeout);
      }
    else
      ret = pthread_cond_wait(&_condvar, mutex);
    if (lockMutex)
      ownUnlock();
    if (ret == 0)
      return (INativeCondVar::NoError);
    return (INativeCondVar::Failed);
  }
}
