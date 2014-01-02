
#include <GLES2/gl2.h>
#include <exception>
#include <string>

///////////////////////////////////////////////////////////////////////////////

class ShaderException : public std::exception
{
public:
  ShaderException(const char* what)
  {
    mWhat = what;
  }

  ShaderException(GLuint shader);

  virtual ~ShaderException()
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

class Shader
{
public:
  Shader(GLenum shaderType, const char* srcFileName);
  virtual ~Shader();

  void compile();

  GLuint shader()
  {
    return mShaderInstance;
  }

private:
  void read(const char* srcFileName, std::string& outSrc);

  GLuint mShaderInstance;
  std::string mSrcFileName;
};

