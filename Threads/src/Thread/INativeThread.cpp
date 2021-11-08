
#include <Vriska/Threads/INativeThread.h>

#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
# include <Vriska/Threads/WindowsThread.h>
#else
# include <Vriska/Threads/LinuxThread.h>
#endif // !VRISKA_WINDOWS || VRISKA_MINGW

namespace Vriska
{
  INativeThread*	INativeThread::create()
  {
#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
    return (new WindowsThread());
#else
    return (new LinuxThread());
#endif // !VRISKA_WINDOWS || VRISKA_MINGW
  }
}
