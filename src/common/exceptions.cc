#include <exception>

class Exception: public std::exception{
    public:
        Exception(const char* msg): msg(msg){}
        const char* msg;
        virtual const char* what() const throw()
        {
          return msg;
        }
};
