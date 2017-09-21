#include "imadjust_m.h"

void stretchlim_m(Mat &I, int row, int col, int tmin, int tmax, uchar *lowin, uchar *highin)
{
    unsigned int tol_low, tol_high;
    unsigned int rgbhist[256] = {0};
	int i, j;
	for (i = 0; i < row; i++)
	{
		uchar *p = I.ptr<uchar>(i);
		for (j = 0; j < col; j++)
		{
			rgbhist[*p]++;
			rgbhist[*(p+1)]++;
			rgbhist[*(p+2)]++;
			p += 3;
		}
	}
    unsigned int num = 0;
	for (i = 0; i < 256; i++)
	{
		num = num + rgbhist[i];
		rgbhist[i] = num / 3;
	}
	tol_low = num/300*tmin;
	tol_high = num/300*tmax;
	for (i = 0; i < 256; i++)
	{
		if (rgbhist[i] <= tol_low) *(lowin) = i + 1;
		if (rgbhist[i] >= tol_high)
		{
			*(highin) = i;
			break;
		}
	}
}

void makeLut_m(uchar * pLUT, uchar Min, uchar Max, unsigned int uiNrBins)
{
	int i;
	const uchar BinSize = (uchar) (1 + (Max - Min) / uiNrBins);

	for (i = Min; i <= Max; i++)  pLUT[i] = (i - Min) / BinSize;
}

void adjustArray_m(uchar * lut, uchar lin, uchar hin)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		if (lut[i] < lin) lut[i] = lin;
		if (lut[i] > hin) lut[i] = hin;
		lut[i] -= lin;
		lut[i] = (uchar)((float)lut[i]*255/(hin-lin) + 0.5f);
	}
}

void adjustWithLUT_m(uchar *lutb, uchar *lutg, uchar *lutr, uchar *lowin, uchar *highin)
{
	makeLut_m(lutb, 0, 255, 256);
	makeLut_m(lutg, 0, 255, 256);
	makeLut_m(lutr, 0, 255, 256);
	adjustArray_m(lutb, lowin[0], highin[0]);
	adjustArray_m(lutg, lowin[0], highin[0]);
	adjustArray_m(lutr, lowin[0], highin[0]);

}

void imadjust_m(Mat &I, int row, int col, int tmin, int tmax)
{
	int i, j;
	uchar lowin[3] = {0}, highin[3] = {0};
	uchar lutb[256] = {0};
	uchar lutg[256] = {0};
	uchar lutr[256] = {0};
	stretchlim_m(I, row, col, tmin, tmax, lowin, highin);
	adjustWithLUT_m(lutb, lutg, lutr, lowin, highin);
	for (i = 0; i < row; i++)
	{
		uchar *p = I.ptr<uchar>(i);
		for (j = 0; j < col; j++)
		{
			*p = lutb[*p];
			*(p+1) = lutg[*(p+1)];
			*(p+2) = lutr[*(p+2)];
			p += 3;
		}
	}
}
