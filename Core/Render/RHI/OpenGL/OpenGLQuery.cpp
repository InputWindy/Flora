#include "OpenGLQuery.h"

flora::XOpenGLTimeQuery::~XOpenGLTimeQuery()
{
	ReleaseRHI();
}

bool flora::XOpenGLTimeQuery::IsValid()
{
	return glIsQuery(BeginHandle) == GL_TRUE && glIsQuery(EndHandle) == GL_TRUE;
}

bool flora::XOpenGLTimeQuery::InitRHI()
{
	glGenQueries(1, &BeginHandle);
	glGenQueries(1, &EndHandle);
	return IsValid();
}

bool flora::XOpenGLTimeQuery::ReleaseRHI()
{
	glDeleteQueries(1, &BeginHandle);
	glDeleteQueries(1, &EndHandle);
	return !IsValid();
}

bool flora::XOpenGLTimeQuery::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool flora::XOpenGLTimeQuery::UpdateRHI()
{
	return false;
}

void flora::XOpenGLTimeQuery::BeginQuery()
{
	glQueryCounter(BeginHandle, GL_TIMESTAMP);

	GLint resultAvailable = 0;

	// 等待结果可用
	do {
		glGetQueryObjectiv(BeginHandle, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
	} while (resultAvailable == 0);

	glGetQueryObjectui64v(BeginHandle, GL_QUERY_RESULT, &StartTime);
}

void flora::XOpenGLTimeQuery::EndQuery()
{
	glQueryCounter(EndHandle, GL_TIMESTAMP);

	GLint resultAvailable = 0;

	// 等待结果可用
	do {
		glGetQueryObjectiv(EndHandle, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
	} while (resultAvailable == 0);

	glGetQueryObjectui64v(EndHandle, GL_QUERY_RESULT, &EndTime);
}

flora::XOpenGLPrimitiveQuery::~XOpenGLPrimitiveQuery()
{
	ReleaseRHI();
}

bool flora::XOpenGLPrimitiveQuery::IsValid()
{
	return glIsQuery(Handle) == GL_TRUE;
}

bool flora::XOpenGLPrimitiveQuery::InitRHI()
{
	glGenQueries(1, &Handle);
	return IsValid();
}

bool flora::XOpenGLPrimitiveQuery::ReleaseRHI()
{
	glDeleteQueries(1, &Handle);
	return !IsValid();
}

bool flora::XOpenGLPrimitiveQuery::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool flora::XOpenGLPrimitiveQuery::UpdateRHI()
{
	return false;
}

void flora::XOpenGLPrimitiveQuery::BeginQuery()
{
	glBeginQuery(GL_PRIMITIVES_GENERATED, Handle);
}

void flora::XOpenGLPrimitiveQuery::EndQuery()
{
	glEndQuery(GL_PRIMITIVES_GENERATED);
	glGetQueryObjectuiv(Handle, GL_QUERY_RESULT, &PrimitivesGenerated);
}
