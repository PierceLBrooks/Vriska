
#include <Vriska/Threads/CondVar.h>

#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
# include <Vriska/Threads/WindowsCondVar.h>
#else
# include <Vriska/Threads/LinuxCondVar.h>
#endif // !VRISKA_WINDOWS || VRISKA_MINGW

namespace Vriska
{
  INativeCondVar::INativeCondVar(INativeMutex* mutex, bool recursive) : _mutex(mutex == NULL ? *INativeMutex::create(recursive) : *mutex), _own(mutex == NULL)
  {
  }

  INativeCondVar::~INativeCondVar()
  {
    if (_own)
      delete &_mutex;
  }

  INativeCondVar*	INativeCondVar::create(INativeMutex* mutex)
  {
#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
    return (new WindowsCondVar(mutex));
#else
    return (new LinuxCondVar(mutex));
#endif // !VRISKA_WINDOWS || VRISKA_MINGW
  }

  bool	INativeCondVar::lock()
  {
    return (_mutex.lock());
  }

  bool	INativeCondVar::tryLock()
  {
    return (_mutex.tryLock());
  }

  bool	INativeCondVar::unlock()
  {
    return (_mutex.unlock());
  }

  bool	INativeCondVar::ownLock()
  {
    if (_own)
      return (_mutex.lock());
    return (true);
  }

  bool	INativeCondVar::ownUnlock()
  {
    if (_own)
      return (_mutex.unlock());
    return (true);
  }

  bool  INativeCondVar::isRecursive() const
  {
      return (_mutex.isRecursive());
  }
}
