#include "mFileLoader.h"

int rav::FileLoader::Load(const string& path, string& output, bool binary_mode)
{
	//Open Input File Stream
	ifstream inputFileStream(path, binary_mode ? ifstream::binary : ifstream::in);

	if (inputFileStream.is_open())
	{
		//Seek the End of File flag
		inputFileStream.seekg(0, ios::end);

		//Use the number of chars to reserve proper string size
		size_t length = inputFileStream.tellg();
		output.reserve(length);

		//Go back to the beginning of the file
		inputFileStream.seekg(0, ios::beg);

		//Assign all data to the string
		output.assign(istreambuf_iterator<char>(inputFileStream), istreambuf_iterator<char>());

		//Return size
		return length;
	}

	//Debug error
	rvDebug.Log("Failed to read file: " + path, RV_ERROR_MESSAGE);

	//Return error code
	return 0;
}

int rav::FileLoader::Load(const string& path, char *& output, bool binary_mode)
{
	//Open Input File Stream
	ifstream inputFileStream(path, binary_mode ? ifstream::binary : ifstream::in);

	if (inputFileStream.is_open())
	{
		//Seek the End of File flag
		inputFileStream.seekg(0, std::ios::end);

		//Use the number of chars to allocate proper char array size
		size_t length = inputFileStream.tellg();
		output = new char[length+1];

		//Go back to the beginning of the file
		inputFileStream.seekg(0, std::ios::beg);

		//Assign the all data to the char array
		inputFileStream.read(output, length);
		output[length] = '\0';

		//Return size
		return length+1;
	}

	//Debug error
	rvDebug.Log("Failed to read file: " + path, RV_ERROR_MESSAGE);

	//Return error code
	return 0;
}
