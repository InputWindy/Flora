#include "OpenGLQuery.h"

XVerse::XOpenGLTimeQuery::~XOpenGLTimeQuery()
{
	ReleaseRHI();
}

bool XVerse::XOpenGLTimeQuery::IsValid()
{
	return glIsQuery(BeginHandle) == GL_TRUE && glIsQuery(EndHandle) == GL_TRUE;
}

bool XVerse::XOpenGLTimeQuery::InitRHI()
{
	glGenQueries(1, &BeginHandle);
	glGenQueries(1, &EndHandle);
	return IsValid();
}

bool XVerse::XOpenGLTimeQuery::ReleaseRHI()
{
	glDeleteQueries(1, &BeginHandle);
	glDeleteQueries(1, &EndHandle);
	return !IsValid();
}

bool XVerse::XOpenGLTimeQuery::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool XVerse::XOpenGLTimeQuery::UpdateRHI()
{
	return false;
}

void XVerse::XOpenGLTimeQuery::BeginQuery()
{
	glQueryCounter(BeginHandle, GL_TIMESTAMP);

	GLint resultAvailable = 0;

	// 等待结果可用
	do {
		glGetQueryObjectiv(BeginHandle, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
	} while (resultAvailable == 0);

	glGetQueryObjectui64v(BeginHandle, GL_QUERY_RESULT, &StartTime);
}

void XVerse::XOpenGLTimeQuery::EndQuery()
{
	glQueryCounter(EndHandle, GL_TIMESTAMP);

	GLint resultAvailable = 0;

	// 等待结果可用
	do {
		glGetQueryObjectiv(EndHandle, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
	} while (resultAvailable == 0);

	glGetQueryObjectui64v(EndHandle, GL_QUERY_RESULT, &EndTime);
}

XVerse::XOpenGLPrimitiveQuery::~XOpenGLPrimitiveQuery()
{
	ReleaseRHI();
}

bool XVerse::XOpenGLPrimitiveQuery::IsValid()
{
	return glIsQuery(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLPrimitiveQuery::InitRHI()
{
	glGenQueries(1, &Handle);
	return IsValid();
}

bool XVerse::XOpenGLPrimitiveQuery::ReleaseRHI()
{
	glDeleteQueries(1, &Handle);
	return !IsValid();
}

bool XVerse::XOpenGLPrimitiveQuery::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool XVerse::XOpenGLPrimitiveQuery::UpdateRHI()
{
	return false;
}

void XVerse::XOpenGLPrimitiveQuery::BeginQuery()
{
	glBeginQuery(GL_PRIMITIVES_GENERATED, Handle);
}

void XVerse::XOpenGLPrimitiveQuery::EndQuery()
{
	glEndQuery(GL_PRIMITIVES_GENERATED);
	glGetQueryObjectuiv(Handle, GL_QUERY_RESULT, &PrimitivesGenerated);
}
