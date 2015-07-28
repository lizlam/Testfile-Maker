// -*- mode: C++; tab-width: 4; -*-
// vi:ts=4 noexpandtab autoindent

/*
 * Matthew Birky
 * 10/22/2014
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <sstream>
using namespace std;

const char *gProgname = "<unknown program>";

/*
* convert a string to any compatible value
*/
template<typename T>
T ValOf(string s)
{
	stringstream	 os;
	T	 val;

	os << s;
	os >> val;

	return val;
}

/*
 * Thank you mwittenberg for the code
 * return the filename (strip off the preceding path, if any)
 */
const char *
GetProgname(const char *argp)
{
	const char	*p = strrchr(argp, '/');

	return p ? p+1 : argp;
}

/*
 * Show usage synopsis
 */
void usage()
{
	cout << gProgname << " [file name] [-c val] [-e [extension name]] [-h] [-l] [-s val[k|m|g]]" << endl;
}

/*
 * Show optional error message (if desired)
 * before showing usage synopsis
 */
void help(const char ch = '\0')
{
	if(ch)
		cout << gProgname << ": " << "Unknown option '" << ch << "'" << endl;
	usage();

	exit(1);
}

/*
 * Show arbitrary messag (if desired)
 * before showing usage synopsis
 */
void help(const char *cp)
{
	if(cp)
		cout << gProgname << ": " << cp << endl;
	usage();

	exit(1);
}

int main(int argc, const char * argv[]) {
	unsigned int fileCount	= 1;
	unsigned long long fileSize	= 10;	//bytes
	const char * fileName	= "test";
	bool seperate_lines		= false;
	string fileExtension;

	gProgname = GetProgname(*argv);

	while(--argc > 0)	{
		const char *arg = *++argv;

		if(*arg == '-')	{
			while(*++arg)	{
				switch(*arg)	{
				case 'c':
					if( (argc > 1) && isdigit(*argv[1]) ) {
						argc--;
						fileCount = ValOf<unsigned int>(*++argv);
					}
					else {
						help("-c requires a number");
					}
					break;
				case 'e':
					if(argc > 1)	{
						argc--;
						fileExtension = *++argv;
					}
					else {
						fileExtension = ".txt";
					}
					break;
				case 'h':
					help();
					break;
				case 'l':
					seperate_lines=true;
					break;
				case 's':
					if( (argc > 1) && isdigit(*argv[1]) )	{
						argc--;
						string arg_string = *++argv;
						fileSize = ValOf<unsigned int>(arg_string);
						unsigned int arg_length = arg_string.length();

						switch(arg_string[arg_length - 1])	{
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							break;
						case 'k':
						case 'K':
							fileSize *= 1024;
							break;
						case 'm':
						case 'M':
							fileSize *= 1024 * 1024;
							break;
						case 'g':
						case 'G':
							fileSize *= 1024 * 1024 * 1024;
							break;
						default:
							help(*arg);
							break;
						}
					}
					else {
						help("-s requires a number");
					}
					break;
				default:
					help(*arg);
					break;
				}
			}
		}
		else {
			fileName = arg;
		}
	}

	// Create number of files as called for
	for(int fileNum(0); fileNum < fileCount; fileNum++) {
		stringstream outputFileName;
		outputFileName << fileName;

		if(fileCount != 1) {
			outputFileName << fileNum;
		}
		/*
		* Tack on the extension only if the user provided an extension,
		* but first interpose a period unless the user-providided extension
		* begins with a period.
		*/
		if( fileExtension.length() ) {
				if( fileExtension[0]!= '.' )
						outputFileName << ".";
				outputFileName << fileExtension;
		}

		std::ofstream outputfile;
		outputfile.open(outputFileName.str().c_str());
		unsigned int output_char=0;
		for(unsigned int size(0); size < fileSize; ++size)	{
			outputfile << output_char++ % 10;
			if( seperate_lines && size + 1 < fileSize)	{
				outputfile << '\n';
				++size;
			}
		}
		outputfile.close();
	}

	return 0;
}