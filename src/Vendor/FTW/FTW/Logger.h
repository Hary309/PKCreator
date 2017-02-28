/*
*********************************************************************
* File          : Logger.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>
#include <iostream>

namespace FTW
{
	extern "C"
	{
		class DECLSPEC Logger
		{
		private:
			FILE *m_file;
			char m_filePath[_MAX_PATH];

			unsigned m_tmStart;
			static Logger *s_pInst;

		private:
			void Log(const char *prefix, const char *txt);

		public:
			Logger();
			~Logger();

			bool Init(const char *logPath);
			void Shutdown();

			static void Msg(const char *msg, ...);
			static void Warning(const char *msg, ...);
			static void Error(const char *msg, ...);

			static Logger *Get();
		};
	}
}