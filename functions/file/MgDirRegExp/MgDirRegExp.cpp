/////////////////////////////////////////////////////////////////////////////////////////////
// This function will return a cell array contains all matched file names in a given folder,
// regular expression should be used to filter the file names.
/////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
// Author: Mango Feng
// Date: 2019-09-02
//-------------------------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <regex>
#include <filesystem>
#include "mex.h"

// acquire the list of file names in the dir that matches filter
std::vector<std::string> GetFileNamesInDir(const char* dir, const char* filter)
{
    namespace fs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	if (!fs::is_directory(dir))
	{
		return filenames;
	}

	fs::directory_iterator end_iter;
	std::regex e(filter);

	for (auto&& fe : fs::directory_iterator(dir))
	{
		std::string file = fe.path().filename().string();

		if (std::regex_match(file, e))
		{
			filenames.push_back(file);
		}
	}
	return filenames;
}

/* The gateway function */
// nlhs: Number of expected mxArray output arguments, specified as an integer.
// plhs: Array of pointers to the expected mxArray output arguments.
// nrhs: Number of input mxArrays, specified as an integer.
// prhs: Array of pointers to the mxArray input arguments. 
void mexFunction(int nlhs, mxArray *plhs[],	int nrhs, const mxArray *prhs[])
{
	/* check for proper number of arguments */
	if (nrhs != 2) {
		mexErrMsgTxt("Two inputs required.");
	}
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments");
	}
	/* inputs must be string */
	if (!mxIsChar(prhs[0]) || !mxIsChar(prhs[1])) {
		mexErrMsgTxt("All inputs must be string.");
	}

    /* copy the string data from prhs[0] and prhs[1] into a C string inputString.    */
    char* dir = mxArrayToString(prhs[0]);
    char* regexp = mxArrayToString(prhs[1]);

    auto filenames = GetFileNamesInDir(dir, regexp);

    plhs[0] = mxCreateCellMatrix(filenames.size(), 1);

    for (size_t i =0; i < filenames.size(); i++) 
    {
        mxSetCell(plhs[0], i, mxCreateString(filenames[i].c_str()));
    }

    mxFree(dir);
    mxFree(regexp);
    
	return;
}