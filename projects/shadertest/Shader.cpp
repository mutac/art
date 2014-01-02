
#include "Shader.h"
#include <fstream>
#include <streambuf>

///////////////////////////////////////////////////////////////////////////////

ShaderException::ShaderException(GLuint shader)
{
  int logLength = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

  char* logInfo = new char[logLength];
  if (logInfo != NULL)
  {
    int charsWritten = 0;
    glGetShaderInfoLog(shader, logLength, &charsWritten, logInfo);

    mWhat = logInfo;
    delete [] logInfo;
  }
}

///////////////////////////////////////////////////////////////////////////////

Shader::Shader(GLenum shaderType, const char* srcFileName) :
  mShaderInstance(0)
{
  mShaderInstance = glCreateShader(shaderType);
  mSrcFileName = srcFileName;
}
 
Shader::~Shader()
{
  if(mShaderInstance > 0)
  {
    glDeleteShader(mShaderInstance);
    mShaderInstance = 0;
  }
}

void Shader::compile()
{
  std::string src;

  read(mSrcFileName.c_str(), src);
  
  const char* pSrc = src.c_str();
  glShaderSource(mShaderInstance, 1, &pSrc, NULL);

  glCompileShader(mShaderInstance);

  GLint compiled = 0;
  glGetShaderiv(mShaderInstance, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    throw new ShaderException(mShaderInstance);
  }
}

void Shader::read(const char* srcFileName, std::string& outSrc)
{
  outSrc.clear();

  try
  {
    std::ifstream t(srcFileName);

    // Preallocate string
    t.seekg(0, std::ios::end);
    outSrc.reserve((unsigned int)t.tellg());
    t.seekg(0, std::ios::beg);

    // Read file
    outSrc.assign(std::istreambuf_iterator<char>(t),
      std::istreambuf_iterator<char>());
  }
  catch (std::exception&)
  {
    throw new ShaderException("Error reading source file");
  }
}
