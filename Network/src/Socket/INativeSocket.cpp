
#include <Vriska/Network/INativeSocket.h>

#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
# include <Vriska/Network/WindowsSocket.h>
#else
# include <Vriska/Network/LinuxSocket.h>
#endif // !VRISKA_WINDOWS

namespace Vriska
{
  VRISKA_ACCESSIBLE
  INativeSocket*	INativeSocket::create()
  {
#if defined(VRISKA_WINDOWS) && !defined(VRISKA_MINGW)
    return (new WindowsSocket());
#else
    return (new LinuxSocket());
#endif // !VRISKA_WINDOWS || VRISKA_MINGW
  }
}
