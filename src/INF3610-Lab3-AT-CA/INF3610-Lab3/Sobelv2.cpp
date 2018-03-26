///////////////////////////////////////////////////////////////////////////////
//
//	Sobelv2.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Sobelv2.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
Sobelv2::Sobelv2(sc_module_name name) : sc_module(name)
/* À compléter */
{
	SC_THREAD(thread);
	sensitive << clk.pos();
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
Sobelv2::~Sobelv2()
{
	/*

	À compléter

	*/
}


///////////////////////////////////////////////////////////////////////////////
//
//	thread
//
///////////////////////////////////////////////////////////////////////////////
void Sobelv2::thread(void)
{
	/*
	À compléter
	*/
	unsigned int width, height, data;
	unsigned int addr = 0;

	while (true) {
		width = Read(addr);
		addr += 4;
		height = Read(addr);
		size = width * height;
		uint8_t* cache = new uint8_t[4 * width];
		uint8_t* image = new uint8_t[size];

		CacheRead(addr, (unsigned int*)cache, 3 * width);
		addr += 3 * width;
		
		int index = 0;
		bool isCacheRead = false;
		for (unsigned int i = 0; i < height; i++) {
			fCacheRead = false;
			for (unsigned int j = 0; j < width; j++) {
				index = i * imgWidth + j;
				if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
					image[index] = 0;
					Write(8 + index, 0);
				}
				else {
					if(!fCacheRead) {
						CacheRead(addr, ((unsigned int*)cache) + ((addr - 8) % (4 * width) / 4), width);
						addr += width;
						fCacheRead = true;
					}
					image[index] = Sobelv2_operator((addr - 8 - 3 * width) % (width * 4) + j, width, cache);
					wait(clk->posedge_event());
				}
			}
		}

		for (unsigned int i = 0; i < height; i++) {
			for (unsigned int j = 0; j < width; j += 4) {
				index = i * imgWidth + j;
				data = (image[index] << 24 | image[index + 1] << 16| image[index+ 2] << 8 | image[index + 3])
				Write(8 + index, data));
			}
		}

		delete[] cache;
		delete[] image;
		sc_stop();
		wait();
	}

}



///////////////////////////////////////////////////////////////////////////////
//
//	Sobelv2_operator
//
///////////////////////////////////////////////////////////////////////////////
static inline uint8_t getVal(int index, int xDiff, int yDiff, int img_width, uint8_t * Y)
{
	int fullIndex = (index + (yDiff * img_width)) + xDiff;
	if (fullIndex < 0)
	{
		//Cas ou on doit chercher la derniere ligne
		fullIndex += img_width * 4;
	}
	else if (fullIndex >= img_width * 4)
	{
		//Cas ou on doit aller chercher la premiere ligne
		fullIndex -= img_width * 4;
	}

	return Y[fullIndex];
};

uint8_t Sobelv2::Sobelv2_operator(const int index, const int imgWidth, uint8_t * image)
{
	int x_weight = 0;
	int y_weight = 0;

	unsigned edge_weight;
	uint8_t edge_val;

	const char x_op[3][3] = { { -1,0,1 },
	{ -2,0,2 },
	{ -1,0,1 } };

	const char y_op[3][3] = { { 1,2,1 },
	{ 0,0,0 },
	{ -1,-2,-1 } };

	//Compute approximation of the gradients in the X-Y direction
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// X direction gradient
			x_weight = x_weight + (getVal(index, i - 1, j - 1, imgWidth, image) * x_op[i][j]);

			// Y direction gradient
			y_weight = y_weight + (getVal(index, i - 1, j - 1, imgWidth, image) * y_op[i][j]);
		}
	}

	edge_weight = std::abs(x_weight) + std::abs(y_weight);

	edge_val = (255 - (uint8_t)(edge_weight));

	//Edge thresholding
	if (edge_val > 200)
		edge_val = 255;
	else if (edge_val < 100)
		edge_val = 0;

	return edge_val;
}


unsigned int Sobelv2::Read(const unsigned int addr) {
	int readData = 0;
	address.write(addr);
	requestRead.write(true);
	do {
		wait(clk->posedge_event());
	} while (!ackReaderWriter.read());
	readData = dataRW.read();
	requestRead.write(false);
	return readData;
}

void Sobelv2::Write(const unsigned int addr, const unsigned int writeData) {
	address.write(addr);
	dataRW.write(writeData);
	requestWrite.write(true);
	do {
		wait(clk->posedge_event());
	} while (!ackReaderWriter.read());
	requestWrite.write(false);
}

void Sobelv2::CacheRead(const unsigned int addr, unsigned int* addressData, const unsigned int lgt) {
	address.write(addr);
	addressRes.write(addressData);
	length.write(lgt);
	requestCache.write(true);
	do {
		wait(clk->posedge_event());
	} while (!ackCache.read());
	requestCache.write(false);
}
