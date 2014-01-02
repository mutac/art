
#include "Program.h"
#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////

ProgramException::ProgramException(GLuint program)
{
  int logLength = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

  char* logInfo = new char[logLength];
  if (logInfo != NULL)
  {
    int charsWritten = 0;
    glGetProgramInfoLog(program, logLength, &charsWritten, logInfo);

    mWhat = logInfo;
    delete [] logInfo;
  }
}

///////////////////////////////////////////////////////////////////////////////

Program::Program() :
  mProgramInstance(0)
{
  mProgramInstance = glCreateProgram();
}

Program::~Program()
{
  if (mProgramInstance > 0)
  {
    glDeleteProgram(mProgramInstance);
  }
}

void Program::attach(Shader& shader)
{
  glAttachShader(mProgramInstance, shader.shader());
}

void Program::activate()
{
  glLinkProgram(mProgramInstance);

  GLint linked = 0;
  glGetProgramiv(mProgramInstance, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    throw new ProgramException(mProgramInstance);
  }

  glUseProgram(mProgramInstance);
}

void Program::bindAttributeLocation(GLuint location, const char* attributeName)
{
  glBindAttribLocation(mProgramInstance, location, attributeName);
}

GLuint Program::getUniformLocation(const char* name)
{
  return glGetUniformLocation(mProgramInstance, name);
}