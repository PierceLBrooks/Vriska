
#ifndef VRISKA_LIB_NETWORK_CLIENT_H_
# define VRISKA_LIB_NETWORK_CLIENT_H_

# include <Vriska/Core/Time.h>
# include <Vriska/Network/SimpleClient.h>
# include <Vriska/Network/ICallable.h>
 
namespace Vriska
{
  class VRISKA_EXPORT Client : public SimpleClient
  {
  public:
    typedef bool	(*Function)(Client&);
    
  public:
    Client(Socket::Protocol protocol = Socket::TCP);
    virtual ~Client();
    
  private:
    Client(Socket const & other);
    Socket const & operator=(Socket const & other);
    
  public:
    Error::Code		connect(std::string const & host, unsigned int port, bool force = false);
    Error::Code		disconnect();
    Error::Code		reconnect();
    
    void			setBlocking(bool val);
    Error::Code		launch();
    Error::Code		waitUntilSent(bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilDelim(std::string const & delim, bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilSize(size_t size, bool callBack = true, bool timeOut = true);
    Error::Code		waitLine(bool callBack = true, bool timeOut = true);
    Error::Code		waitFor(Time const & t, bool callBack = true, bool timeOut = false);
    
    void			registerOnReceive(Function func);
    void			registerOnReceive(IClientCallable *call);
    void			unregisterOnReceive();
    void			registerOnSend(Function func);
    void			registerOnSend(IClientCallable *call);
    void			unregisterOnSend();
    void			registerOnStdin(Function func);
    void			registerOnStdin(IClientCallable *call);
    void			unregisterOnStdin();
    
    void			setTimeout(Time const & t, Function func, bool exact = true);
    void			setTimeout(Time const & t, IClientCallable *call, bool exact = true);
    void			unsetTimeout();
    Time const &	getElapsedTime() const;

  public:
    int				read(void *buffer, size_t size);
    int 			read(std::string& buffer);
    int 			read(std::string& buffer, size_t size);
    int				peek(std::string& buffer, size_t size, unsigned int offset = 0);
    int				peek(void *buffer, size_t size, unsigned int offset = 0);
    void			seek(unsigned int size);
    int 			readUntil(std::string& buffer, std::string const & delim);
    int 			readLine(std::string& buffer);
    unsigned int 	sizeToRead() const;
    unsigned int 	sizeCanRead() const;
    
  public:
    int 			write(char const *buffer);
    int 			write(void const *buffer, size_t size);
    int 			write(std::string const & buffer);
    int 			write(std::string const & buffer, size_t size);
    unsigned int 	sizeToWrite() const;
    unsigned int 	sizeCanWrite() const;
    
  private:
    bool			hasTimeout() const;
    Time			getTimeout() const;
    bool			watchStdin() const;

    void			addTimeout(Time const & elapsed);
    void			addTime(Time const & elapsed);
    bool			callbackTimeout();
    
    bool			callbackReceive();
    bool			callbackSend();
    bool			callbackStdin();
    
    Error::Code			manageIO(bool callBack, bool timeOut);
    
  private:
    bool			_isBlocking;
    
    bool			_tried;
    std::string			_host;
    unsigned int		_port;
    
    Time			_timeTarget;
    Time			_timeElapsed;
    bool			_timeExact;
    
    Time			_waitTarget;
    Time			_waitElapsed;
    
    IClientCallable	*_callReceive;
    Function		_funcReceive;
    IClientCallable	*_callSend;
    Function		_funcSend;
    IClientCallable	*_callStdin;
    Function		_funcStdin;
    IClientCallable	*_callTime;
    Function		_funcTime;
  };
}

#endif // !VRISKA_LIB_NETWORK_CLIENT_H_
