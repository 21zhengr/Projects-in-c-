#include "RleData.h"
#include <vector>

void RleData::Compress(const char* input, size_t inSize)
{
	//clean prior data
	delete[] mData;
	
	mSize = 0; // okay to set it to value without reseting to 0?

	//allocate new memory for mData and set mSize to 2* inSize
	//mData = new char[(2 * inSize)];

	//create temp char array
	char* temp = new char[(2 * inSize)];

	//int debug_variable;

	//first pass, consider all as positive runs --> don't forget edge case with only 1 letter
	int count = 1;
	char data;
	for (size_t i = 0; i < inSize; ++i) {
		if (input[i] == input[i+1]) { //positive run condition
			//++count;
			data = input[i];
			i += 1; //move along the sequence
			while (input[i] == data && count < MaxRunSize() && (i < inSize)) {
				++count;
				++i; //move the pointer along
			} //exits when the char are no longer repeating
			
			temp[mSize] = count;
			//debug_variable = static_cast<int>(temp[mSize]);
			mSize += 1;
			temp[mSize] = data;
			mSize += 1;
			count = 1;	//reset count back to 1
			--i;
		} 
		else { // this block will only be reached if input[i] != input[i+1]
			//perhaps i need to check for negative run that exceeds the 127
			temp[mSize] = 1;
			//debug_variable = static_cast<int>(temp[mSize]);
			mSize += 1;
			temp[mSize] = input[i];
			mSize += 1;
		}
	}
	//msize needs to be decremented by 1 since line 42 or line 33 increases it on the last iteration
	//--mSize;
	int mSizzle = 0;
	//second pass
	int i = 0;
	int insert = 0;
	
	while (i < mSize) {
		if (temp[i] == 1 && temp[i + 2] == 1) {
			int negativeRunSize = 0; //!!!!! possible error hither
			int tempInsert = insert;
			while (temp[i] == 1 && abs(negativeRunSize) < MaxRunSize()) {
				--negativeRunSize;
				char c = temp[i + 1]; 
				temp[++tempInsert] = c;
				++mSizzle;
				i += 2; //move indice to next number
			}
			temp[insert] = static_cast<char>(negativeRunSize);
			//debug_variable = static_cast<int>(temp[insert]);

			++mSizzle;
			insert += abs(negativeRunSize) + 1;
			continue;
		}
		//else {
			temp[insert] = temp[i];
			++mSizzle;
			insert += 1;
			temp[insert] = temp[i + 1];
			++mSizzle;
			insert += 1;
		//}
		i += 2;
	}

	//you still have left over data from previous pass here ---> i suggest cerating new dynamic array for mData and since you have mSize, only loop through mSize and copy;
	mData = new char[(2 * inSize)]; //allocate 2x insize for mData --> specified in write up

	for (int i = 0; i < mSizzle; ++i) {
		mData[i] = temp[i];
	}
	mSize = mSizzle;

	delete[] temp;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// TODO
	delete []mData;
	mData = new char[outSize]; //since we are given the exact size of the output
	int mDataInsert = 0,
		inputCounter = 0;
	char data;
	while(inputCounter < inSize) { //to loop through input
		int runSize = static_cast<int>(input[inputCounter]);
		inputCounter += 1;
		if (runSize > 0) { //positive run
			data = input[inputCounter];
			for (int i = 0; i < runSize; ++i) {
				mData[mDataInsert] = data;
				mDataInsert += 1;
			}
			inputCounter += 1; //move the counter along
			continue;
		}
		//else {
			for (int i = 0; i < abs(runSize); ++i) {
				mData[mDataInsert] = input[inputCounter];
				inputCounter += 1;
				mDataInsert += 1;
			}
			//continue; //travis said to do this
		//}
		
	}
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
