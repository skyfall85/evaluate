#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>

using namespace std;

#define f(i,a,b) for (int i=a;i<b;i++)
#define ABS(A) ((A)<0 ? (-1.0)*(A) : A)
#define angdiff(A,B) (ABS(A-B)<ABS(1.0-ABS(A-B)) ? A-B : ( (A-B)>0.0 ? (A-B)-1.0 : (ABS(A-B)<0.5 ? B-A : 1.0+(A-B)) )  )

int main()
{
	int tsteps[]={10,20,40,80,150,300,600,1200,2400,3200};
	int length=sizeof(tsteps)/sizeof(int);
	cout<<"length="<<length<<endl;

	double **field,**rotation;
	int *xp,*xm,*yp,*ym;
	
	const int L=4096;
	field=new double*[L];
	f(i,0,L) field[i]=new double[L];
	rotation=new double*[L];
	f(i,0,L) rotation[i]=new double[L];

	// set periodic boundary condition:
	xp=new int[L];
	xm=new int[L];
	yp=new int[L];
	ym=new int[L];
	f(i,0,L){xp[i]=i+1;xm[i]=i-1;yp[i]=i+1;ym[i]=i-1;}
	xp[L-1]=0;
	yp[L-1]=0;
	xm[0]=L-1;
	ym[0]=L-1;
    
    // ****************variables of the images output**************
	typedef unsigned char tRGBline[L][3];
  	tRGBline *myRGB;
  	TIFF *f;
  	myRGB = (tRGBline*)malloc(L*sizeof(tRGBline));
	// ****************variables of the images output**************
	
	double d_xpyp,d_xp,d_xpym,d_ym,d_xmym,d_xm,d_xmyp,d_yp;
	double sum_of_grad;
    
    int index;

    ofstream out_defnum;
	char out_defnum_file[50];
	sprintf(out_defnum_file,"number_of_def.dat");
	out_defnum.open(out_defnum_file);


	f(t,0,length)
	{
		// read in data file:
		ifstream in;
		char filename[50];
		sprintf(filename,"orientation_%d.txt",tsteps[t]);
		in.open(filename);
		cout<<filename<<endl;
		cout<<"Read in starts"<<endl;
		f(i,0,L){f(j,0,L){in>>field[i][j];}}
		f(i,250,256){f(j,250,256){cout<<field[i][j]<<"\t";}cout<<endl;}
		cout<<"Read in ends"<<endl;

		int num_of_left=0;
		int num_of_right=0;
		int zero=0;

		double total_sum=0.0;

		// write out the number of defects:

		f(i,0,L)
		{
			f(j,0,L)
			{
				
				sum_of_grad=0.0;
				d_xpyp=field[xp[i]][yp[j]];
				d_xpym=field[xp[i]][ym[j]];
				d_xmyp=field[xm[i]][yp[j]];
				d_xmym=field[xm[i]][ym[j]];
				d_xp=field[xp[i]][j];
				d_xm=field[xm[i]][j];
				d_ym=field[i][ym[j]];
				d_yp=field[i][yp[j]];
				
				sum_of_grad=angdiff(d_ym,d_xmym)+angdiff(d_xpym,d_ym)+angdiff(d_xp,d_xpym)+angdiff(d_xpyp,d_xp)+angdiff(d_yp,d_xpyp)+angdiff(d_xmyp,d_yp)+angdiff(d_xm,d_xmyp)+angdiff(d_xmym,d_xm);
				// this sum should be either -1.0 or 0.0 or +1.0
				total_sum+=sum_of_grad;
				rotation[i][j]=sum_of_grad;
				if (fabs(sum_of_grad+1.0)>1.0e-4 && fabs(sum_of_grad)>1.0e-4 && fabs(sum_of_grad-1.0)>1.0e-4) 
					{cout<<"Something is wrong at "<<i<<" : "<<j<<" coordinates the sum is: "<<sum_of_grad<<endl;
					 cout<<d_xmyp<<"\t"<<d_yp<<"\t"<<d_xpyp<<"\n"
					     <<d_xm<<"\t"<<field[i][j]<<"\t"<<d_xp<<"\n"
					     <<d_xmym<<"\t"<<d_ym<<"\t"<<d_xpym<<"\n"<<endl;
					}
				if (fabs(sum_of_grad+1.0)<1.0e-3) {sum_of_grad=-1.0;num_of_right++;}
				else if (fabs(sum_of_grad)<1.0e-3) {sum_of_grad=0.0;zero++;}
				else if (fabs(sum_of_grad-1.0)<1.0e-3) {sum_of_grad=1.0;num_of_left++;}
				// if (sum_of_grad!=0) cout<<"i:j:="<<i<<":"<<j<<" - sum_of_grad:"<<sum_of_grad<<endl;
				sum_of_grad!=0 ? index=int(sum_of_grad/fabs(sum_of_grad))+1 : index=1;
				myRGB[i][j][0]=0.0;
				myRGB[i][j][1]=0.0;
				myRGB[i][j][2]=0.0;
				
				myRGB[i][j][index]=255.0;
				

			}
		}	

	cout<<"total_sum: "<<total_sum<<endl;
	// **********write out the image********************
	char filename_out[50];
	sprintf(filename_out,"ori_defect_map_%03d.tiff",tsteps[t]);

	if((f=TIFFOpen(filename_out,"w")) == NULL)
    abort();
  
    TIFFSetField(f, TIFFTAG_IMAGEWIDTH, (uint32)L);
    TIFFSetField(f, TIFFTAG_IMAGELENGTH, (uint32)L);
    TIFFSetField(f, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(f, TIFFTAG_BITSPERSAMPLE, 8,8,8);
    TIFFSetField(f, TIFFTAG_COMPRESSION, 8);
    TIFFSetField(f, TIFFTAG_PHOTOMETRIC, 2);
    TIFFSetField(f, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(f, TIFFTAG_XRESOLUTION, 1.0);
    TIFFSetField(f, TIFFTAG_YRESOLUTION, 1.0);
    TIFFSetField(f, TIFFTAG_RESOLUTIONUNIT, 1);
    TIFFSetField(f, TIFFTAG_PLANARCONFIG, 1);	

    for(int x=0; x<L; x++){
	if(TIFFWriteScanline(f,myRGB[x],x,0)<0){
	  	TIFFClose(f);
		return 1;}}

	TIFFClose(f);
	

	cout<<" left: "<<num_of_left<<"\n right: "<<num_of_right<<"\n zero: "<<zero<<endl;
	

	char filename_out2[50];
	sprintf(filename_out2,"ori_defect_map_%03d.dat",tsteps[t]);
	ofstream out;
	out.open(filename_out2);
	f(i,0,L){f(j,0,L){out<<rotation[i][j]<<"\t";}out<<endl;}

	out_defnum<<tsteps[t]<<"\t"<<num_of_right<<"\t"<<num_of_left<<endl;

	} 
	out_defnum.close();

	free(myRGB);
	return 0;
}