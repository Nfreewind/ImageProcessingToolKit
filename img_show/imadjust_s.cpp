#include "imadjust_s.h"

void stretchlim_s(Mat &I, int row, int col, int tmin, int tmax, uchar *lowin, uchar *highin)
{
    unsigned int tol_low, tol_high;
    unsigned int rhist[256] = {0};
    unsigned int ghist[256] = {0};
    unsigned int bhist[256] = {0};
	int i, j;
	for (i = 0; i < row; i++)
	{
		uchar *p = I.ptr<uchar>(i);
		for (j = 0; j < col; j++)
		{
			bhist[*p]++;
			ghist[*(p+1)]++;
			rhist[*(p+2)]++;
			p += 3;
		}
	}
    unsigned int numr = 0, numg = 0, numb = 0;
	for (i = 0; i < 256; i++)
	{
		numb = numb + bhist[i];
		bhist[i] = numb;
		numg = numg + ghist[i];
		ghist[i] = numg;
		numr = numr + rhist[i];
		rhist[i] = numr;
	}
	tol_low = numb/100*tmin;
	tol_high = numb/100*tmax;
	for (i = 0; i < 256; i++)
	{
		if (bhist[i] <= tol_low) *(lowin) = i + 1;
		if (bhist[i] >= tol_high)
		{
			*(highin) = i;
			break;
		}
	}
	for (i = 0; i < 256; i++)
	{
		if (ghist[i] <= tol_low) *(lowin+1) = i + 1;
		if (ghist[i] >= tol_high)
		{
			*(highin+1) = i;
			break;
		}
	}
	for (i = 0; i < 256; i++)
	{
		if (rhist[i] <= tol_low) *(lowin+2) = i + 1;
		if (rhist[i] >= tol_high)
		{
			*(highin+2) = i;
			break;
		}
	}
}

void makeLut_s(uchar * pLUT, uchar Min, uchar Max, unsigned int uiNrBins)
{
    int i;
    const uchar BinSize = (uchar) (1 + (Max - Min) / uiNrBins);

    for (i = Min; i <= Max; i++)  pLUT[i] = (i - Min) / BinSize;
}

void adjustArray_s(uchar * lut, uchar lin, uchar hin)
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

void adjustWithLUT_s(uchar *lutb, uchar *lutg, uchar *lutr, uchar *lowin, uchar *highin)
{
	makeLut_s(lutb, 0, 255, 256);
	makeLut_s(lutg, 0, 255, 256);
	makeLut_s(lutr, 0, 255, 256);
	adjustArray_s(lutb, lowin[0], highin[0]);
	adjustArray_s(lutg, lowin[1], highin[1]);
	adjustArray_s(lutr, lowin[2], highin[2]);

}

void imadjust_s(Mat &I, int row, int col, int tmin, int tmax)
{
	int i, j;
	uchar lowin[3] = {0}, highin[3] = {0};
	uchar lutb[256] = {0};
	uchar lutg[256] = {0};
	uchar lutr[256] = {0};
	stretchlim_s(I, row, col, tmin, tmax, lowin, highin);
	adjustWithLUT_s(lutb, lutg, lutr, lowin, highin);
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
