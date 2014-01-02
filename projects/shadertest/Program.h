
#include <GLES2/gl2.h>
#include <exception>
#include <string>

class Shader;

///////////////////////////////////////////////////////////////////////////////

class ProgramException : public std::exception
{
public:
  ProgramException(const char* what)
  {
    mWhat = what;
  }

  ProgramException(GLuint program);

  virtual ~ProgramException()
  {
  }

  virtual const char* what() const throw()
  {
    return mWhat.c_str();
  }

private:
  std::string mWhat;
};

///////////////////////////////////////////////////////////////////////////////

class Program 
{
public:
  Program();

  virtual ~Program();

  void activate();
  void attach(Shader& shader);

  void bindAttributeLocation(GLuint location, const char* attributeName);
  GLuint getUniformLocation(const char* name);

private:
  GLuint mProgramInstance;
};
