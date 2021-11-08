
#include <Vriska/Threads/INativeMutex.h>
#include <Vriska/Core/Utils.hpp>

#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
# include <Vriska/Threads/WindowsMutex.h>
#else
# include <Vriska/Threads/LinuxMutex.h>
#endif // !VRISKA_WINDOWS || VRISKA_MINGW

namespace Vriska
{
  INativeMutex*	INativeMutex::create(bool recursive)
  {
#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
    Utils::ignore(recursive);
    return (new WindowsMutex());
#else
    return (new LinuxMutex(recursive));
#endif // !VRISKA_WINDOWS || VRISKA_MINGW
  }
}
