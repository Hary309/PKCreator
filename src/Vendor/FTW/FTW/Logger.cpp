#include "Logger.h"

#include <Common.h>

#include <cstdio>
#include <stdarg.h>
#include <Windows.h>
#include <ctime>

#include <stdexcept>

FTW::Logger *FTW::Logger::s_pInst;

FTW::Logger::Logger()
{
	m_file = nullptr;
	s_pInst = this;
}

FTW::Logger::~Logger()
{
	Shutdown();
}

bool FTW::Logger::Init(const char *logPath)
{
#ifdef ALLOC_CONSOLE
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	if (strlen(logPath) > MAX_PATH)
	{
#ifdef THROW_EXCEPTION
		throw std::runtime_error("Log file path is too long");
#else
		return false;
#endif
	}

	unsigned fileSize = 0;

	strcpy_s(m_filePath, logPath);

	m_tmStart = GetTickCount();

	m_file = _fsopen(m_filePath, "a+", _SH_DENYNO);

	if (!m_file)
	{

#ifdef THROW_EXCEPTION
		char error[256];

		sprintf_s(error, "Cannot open \"%s\"", logPath);

		throw std::runtime_error(error);
#else
		return false;
#endif
	}

	char header[1024];

	time_t rawtime;
	tm timeinfo;
	char strTime[32];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(strTime, 32, "%F @ %T", &timeinfo);

	sprintf_s(header, "========== Created on: %s ==========\r", strTime);
	fputs(header, m_file);
	Logger::Msg("Logger initialized!");

	return true;
}

void FTW::Logger::Shutdown()
{
	Logger::Msg("Shutting down Logger...\r");

	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

void FTW::Logger::Log(const char *prefix, const char *str)
{
	char dest[256] = { 0 };

	sprintf_s(dest, "%8d ms [%s] %s\r",  GetTickCount() - m_tmStart, prefix, str);

	printf(dest);
	printf("\n");

	fputs(dest, m_file);
	fflush(m_file);
}

void FTW::Logger::Msg(const char *format, ...)
{
	char dest[256] = { 0 };

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(dest, format, argptr);
	va_end(argptr);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (FTW::Logger::Get())
		FTW::Logger::Get()->Log("Msg", dest);
}

void FTW::Logger::Warning(const char *format, ...)
{
	char dest[256] = { 0 };

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(dest, format, argptr);
	va_end(argptr);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	if (FTW::Logger::Get())
		FTW::Logger::Get()->Log("Warning", dest);
}

void FTW::Logger::Error(const char *format, ...)
{
	char dest[256] = { 0 };

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(dest, format, argptr);
	va_end(argptr);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (FTW::Logger::Get())
		FTW::Logger::Get()->Log("Error", dest);
}

FTW::Logger * FTW::Logger::Get()
{
	return s_pInst;
}
