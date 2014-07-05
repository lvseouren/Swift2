#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>

namespace swift
{
	class Logger
	{
		public:
			explicit Logger(const std::string& header, const std::string& logFile = "./data/swift.log")
			{
				warnings = 0;
				errors = 0;

				fout.open(logFile);

				// Write the first lines
				if(fout.is_open())
				{
					fout << header << "\n\n";
				}
			}

			~Logger()
			{
				if(fout.is_open())
				{
					fout << std::endl << std::endl;

					// Report number of errors and warnings
					fout << warnings << " warnings" << std::endl;
					fout << errors << " errors" << std::endl;

					fout.close();
				}
			}

			// Make it Non Copyable
			Logger(const Logger&) = delete;
			Logger& operator=(const Logger&) = delete;

			Logger& operator<<(char c)
			{
				fout << c;
				return *this;
			}

			Logger& operator<<(const std::string& text)
			{
				fout << text;
				return *this;
			}

			Logger& operator<<(int n)
			{
				fout << n;
				return *this;
			}

			Logger& operator<<(unsigned n)
			{
				fout << n;
				return *this;
			}
			
			Logger& operator<<(std::size_t n)
			{
				fout << n;
				return *this;
			}

			Logger& operator<<(float n)
			{
				fout << n;
				return *this;
			}

			Logger& operator<<(double n)
			{
				fout << n;
				return *this;
			}

		private:
			std::ofstream fout;

			unsigned warnings;
			unsigned errors;
	};
	
	extern Logger log;
}

#endif // LOGGER_HPP
