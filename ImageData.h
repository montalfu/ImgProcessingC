#include <Graphics.hpp>
#include <Windows.hpp>
#include <stdio.h>
#include <mem.h>
#include <math.h>

class cImageData {
 private:
  unsigned char *Pixels;
  int Address;
  Vcl::Graphics::TPixelFormat PixelFormat;

  long FileSize(FILE *f)
  {
   long Current, End;
   Current=ftell(f);
   fseek(f, 0L, SEEK_END);
   End=ftell(f);
   fseek(f, Current, SEEK_SET);
   return End;
  }

 public:
  int Width, Height, BitPerPixel, BytePerPixel;
  
  cImageData()
  {
   BitPerPixel=24;
   BytePerPixel=3;
   Pixels=NULL;
   PixelFormat=pf24bit;
  }

  cImageData(int InitWidth, int InitHeight, int BPP)
  {
   BitPerPixel=BPP*8;
   BytePerPixel=BPP;
   Width=InitWidth;
   Height=InitHeight;
   Pixels=(unsigned char *)malloc(InitWidth*InitHeight*BPP);
   PixelFormat=pf24bit;
  }

  cImageData(int InitWidth, int InitHeight, int BPP, TPixelFormat tf)
  {
   BitPerPixel=BPP*8;
   BytePerPixel=BPP;
   Width=InitWidth;
   Height=InitHeight;
   Pixels=(unsigned char *)malloc(InitWidth*InitHeight*BPP);
   PixelFormat=tf;
  }

  void AllocateMemory(int InitWidth, int InitHeight)
  {
   Width=InitWidth;
   Height=InitHeight;
   if (Pixels!=NULL) free(Pixels);
   Pixels=(unsigned char *)malloc(Width*Height*BytePerPixel);
  }

  void AllocateMemory(int Size)
  {
   if (Pixels!=NULL) free(Pixels);
   Pixels=(unsigned char *)malloc(Size);
  }

  void AllocateMemory()
  {
   if (Pixels!=NULL) free(Pixels);
   Pixels=(unsigned char *)malloc(Width*Height*BytePerPixel);
  }

  void CopyFrom(cImageData *DataSrc)
  {
   int i, CopySize, BPP;
   void *Src, *Dest;

   AllocateMemory(DataSrc->Width, DataSrc->Height);
   CopySize=Width*BytePerPixel;

   for(i=0; i<Height; i++) {
    Src=&DataSrc->Pixels[i*CopySize];
    Dest=&Pixels[i*CopySize];
    memcpy(Dest, Src, CopySize);
   }
  }

  void CopyFromArrayBytes(unsigned char *DataSrc, int width, int height, int BytesPerPixel)
  {
   int i, CopySize;
   void *Src, *Dest;

   Width=width;
   Height=height;
   BytePerPixel=BytesPerPixel;
   BitPerPixel=BytesPerPixel*8;
   AllocateMemory();
   CopySize=Width*BytePerPixel;
   for(i=0; i<Height; i++) {
    Src=&DataSrc[i*CopySize];
    Dest=&Pixels[i*CopySize];
    memcpy(Dest, Src, CopySize);
   }
  }

  unsigned char *CopyToArrayBytes(unsigned char *DataDest, int *width, int *height, int *BytesPerPixel)
  {
   int i, CopySize;
   void *Src, *Dest;

   *width=Width;
   *height=Height;
   *BytesPerPixel=BytePerPixel;
   if (DataDest) free(DataDest);
   DataDest=(unsigned char *)malloc(Width*Height*BytePerPixel);
   CopySize=Width*BytePerPixel;
   for(i=0; i<Height; i++) {
    Src=&Pixels[i*CopySize];
    Dest=&DataDest[i*CopySize];
    memcpy(Dest, Src, CopySize);
   }
   return DataDest;
  }

  int CalculateAddress(int x, int y)     {   return (y*Width+x)*BytePerPixel;   }
  void ChangeDataAddress(int x, int y)   {   Address=CalculateAddress(x, y);    }

  int GetPixel(int x, int y)
  {
   int i, value, pow;

   Address=CalculateAddress(x,y);
   value=0;
   pow=1;
   for(i=1; i<=BytePerPixel; i++) {
    value=value+Pixels[Address]*pow;
    Address=Address+1;
    pow=pow*256;
   }
   return pow;
  }

  void GetARGB(int x, int y, unsigned char *A, unsigned char *R, unsigned char *G, unsigned char *B)
  {
   Address=CalculateAddress(x,y);
   *B=Pixels[Address];
   Address++;
   *G=Pixels[Address];
   Address++;
   *R=Pixels[Address];
   Address++;
   *A=Pixels[Address];
  }

  void GetRGB(int x, int y, unsigned char *R, unsigned char *G, unsigned char *B)
  {
   Address=CalculateAddress(x,y);
   *B=Pixels[Address];
   Address++;
   *G=Pixels[Address];
   Address++;
   *R=Pixels[Address];
  }

  unsigned char GetByte(int x, int y)
  {
   Address=CalculateAddress(x,y);
   return Pixels[Address];
  }

  unsigned char GetR(int x, int y)
  {
   Address=CalculateAddress(x,y)+2;
   return Pixels[Address];
  }

  unsigned char GetG(int x, int y)
  {
   Address=CalculateAddress(x,y)+1;
   return Pixels[Address];
  }

  unsigned char GetB(int x, int y)
  {
   return GetByte(x,y);
  }

  unsigned char GetA(int x, int y)
  {
   Address=CalculateAddress(x,y)+3;
   return Pixels[Address];
  }

  int GetWidth()          {  return Width;               }
  int GetHeight()         {  return Height;              }
  int GetBytePerPixel()   {  return BytePerPixel;        }
  int GetBitPerPixel()    {  return BitPerPixel;         }
  unsigned char GetCurrentByte()      {   return Pixels[Address];    }
  unsigned char GetCurrentR()         {   return Pixels[Address+2];  }
  unsigned char GetCurrentG()         {   return Pixels[Address+1];  }
  unsigned char GetCurrentB()         {   return Pixels[Address];    }
  unsigned char GetCurrentA()         {   return Pixels[Address+3];  }

  void SetPixel(int x, int y, unsigned char A, unsigned char R, unsigned char G, unsigned char B)
  {
   Address=CalculateAddress(x,y);
   Pixels[Address]=B;
   Address++;
   Pixels[Address]=G;
   Address++;
   Pixels[Address]=R;
   Address++;
   Pixels[Address]=A;
  }

  void SetPixel(int x, int y, unsigned char R, unsigned char G, unsigned char B)
  {
   Address=CalculateAddress(x,y);
   Pixels[Address]=B;
   Address++;
   Pixels[Address]=G;
   Address++;
   Pixels[Address]=R;
  }

  void SetPixel(int x1, int y1, cImageData *Src, int x2, int y2)
  {
   int Addr;
   Address=CalculateAddress(x1, y1);     //Address=(y1*Width+x1)*BytePerPixel;
   Addr=Src->CalculateAddress(x2, y2);   //Addr=(y2*Src->Width+x2)*Src->BytePerPixel;

   memcpy(&Pixels[Address], &Src->Pixels[Addr], BytePerPixel);
   /*Pixels[Address]=Src->Pixels[Addr];

   if (BytePerPixel>=2) {
    Address++;
    Addr++;
    Pixels[Address]=Src->Pixels[Addr];
   }

   if (BytePerPixel>=3) {
    Address++;
    Addr++;
    Pixels[Address]=Src->Pixels[Addr];
   }

   if (BytePerPixel>=4) {
    Address++;
    Addr++;
    Pixels[Address]=Src->Pixels[Addr];
   }*/
  }

  void SetARGB(int x, int y, unsigned char A, unsigned char R, unsigned char G, unsigned char B)
  {
   Address=CalculateAddress(x, y);
   Pixels[Address]=B;
   Address++;
   Pixels[Address]=G;
   Address++;
   Pixels[Address]=R;
   Address++;
   Pixels[Address]=A;
  }

  void SetRGB(int x, int y, unsigned char R, unsigned char G, unsigned char B)
  {
   Address=CalculateAddress(x, y);
   Pixels[Address]=B;
   Address++;
   Pixels[Address]=G;
   Address++;
   Pixels[Address]=R;
  }

  void SetByte(int x, int y, unsigned char ByteValue)
  {
   Address=CalculateAddress(x, y);
   Pixels[Address]=ByteValue;
  }

  void SetR(int x, int y, unsigned char Red)
  {
   Address=CalculateAddress(x, y)+2;
   Pixels[Address]=Red;
  }

  void SetG(int x, int y, unsigned char Green)
  {
   Address=CalculateAddress(x, y)+1;
   Pixels[Address]=Green;
  }

  void SetB(int x, int y, unsigned char Blue)
  {
   SetByte(x, y, Blue);
  }

  void SetA(int x, int y, unsigned char Alpha)
  {
   Address=CalculateAddress(x, y)+3;
   Pixels[Address]=Alpha;
  }

  void SetCurrentByte(unsigned char ByteValue)   {   Pixels[Address]=ByteValue;  }
  void SetCurrentR(unsigned char Red)            {   Pixels[Address+2]=Red;      }
  void SetCurrentG(unsigned char Green)          {   Pixels[Address+1]=Green;    }
  void SetCurrentB(unsigned char Blue)           {   SetCurrentByte(Blue);       }
  void SetCurrentA(unsigned char Alpha)          {   Pixels[Address+3]=Alpha;    }

  /*bool LoadFromBitmap(unsigned char FileName[])
  {
   bool Suceeded, loop;
   FILE *f;
   unsigned char buffer[3072], header[53];
   int Size, Count, BitmapWidth, BitmapHeight, DataPos, x, y, z, i, j, k, BuffSize, Rest, RowDataCount, RowBMPCount;
   //int BitPerPixel, CutSize, Position;

   Suceeded=false;
   BuffSize=3072;
   f=fopen(FileName, "rb");                    //Open Image BMP file
   if (f) {
     Size=FileSize(f);                         //Get the file size
     if (Size>54)  {                           //Not a valid BMP if size of header> 54
	  fread(header, 54, 1, f);
      if (header[0]=='B' && header[1]=='M') {  //first 2 bytes is BMP digital signature
       memcpy(&Count, &header[2], 4);          //Next 4 bytes is actual image data size
       if (Count==Size) {                 //If actual size is not equal to file size then it is a currupt BMP
        memcpy(&DataPos,&header[10], 4);       //Next 4 bytes is BMP image data position
        memcpy(&BitmapWidth,  &header[18], 4); //BMP image width is between bytes 18 to bytes 21
        memcpy(&BitmapHeight, &header[22], 4); //BMP image height is between bytes 22 to bytes 25

        BitPerPixel=header[28];              //get the depth of color in bit per pixel metric
		if (header[30]==0) {                 //If compression method at bytes 30 is zero then the BMP image is not compressed
         Suceeded=true;
         RowBMPCount=Size-DataPos;
         Size=Size-DataPos;
		 switch (BitPerPixel) {
          case 8: case 16: case 24: case 32: BytePerPixel=BitPerPixel / 8; break;
		  default: BytePerPixel=0;
         }

		 if (BytePerPixel>0) {
          if (DataPos>54) fseek(f, DataPos, SEEK_SET);
          j=BitmapWidth*BitmapHeight*BytePerPixel;
          Pixels=(unsigned char *)malloc(j);
          Width=BitmapWidth;
          Height=BitmapHeight;
          //Position=0;
          x=0;
          y=BitmapHeight-1;
          j=y*BitmapWidth*BytePerPixel;
          z=Size / BuffSize;
          Rest=Size % BuffSize;
          RowDataCount=BitmapWidth*BytePerPixel;
          RowBMPCount=RowBMPCount / BitmapHeight;
          for(i=1; i<=z; i++) {
           fread(buffer, BuffSize, 1, f);
           k=0;
           loop=true;
           while (k<BuffSize && loop) {
            Pixels[j]=buffer[k];
            j++;
            x++;
            if (x>=RowDataCount) {
             if (y==0) loop=false;
             k+=RowBMPCount-RowDataCount;
             x=0;
             y--;
             j=y*RowDataCount;
            }
            k++;
           }
          }

          if (Rest>0) {
           fread(buffer, Rest, 1, f);
           k=0;
           loop=true;
           while (k<Rest && loop) {
			Pixels[j]=buffer[k];
			j++;
            x++;
            if (x>=RowDataCount) {
             if (y==0) loop=false;
             k+=RowBMPCount-RowDataCount;
             x=0;
             if (y>0) {
              y--;
              j=y*RowDataCount;
             }
            }
            k++;
           }
          }
         }
        }         //Cannot process compressed BMP
       }          //Corrupt BMP file
      }           //Not a BMP image
     }            //Corrupt BMP file
     fclose(f);
    }            //cannot open file
   return Suceeded;
  }*/

  void CopyFromBitmap(Graphics::TBitmap *Bitmap)
  {
   int BytesPerLine, Row;
   void *PPixels, *PBytes;

   Width=Bitmap->Width;
   Height=Bitmap->Height;
   if (Pixels!=NULL) free(Pixels);
   Pixels=(unsigned char *)malloc(Width * Height * BytePerPixel);
   BytesPerLine = Bitmap->Width * BytePerPixel;
   for(Row=0; Row<Bitmap->Height; Row++)
   {
    PBytes = &Pixels[Row * BytesPerLine];
    PPixels = Bitmap->ScanLine[Row];
    memcpy(PBytes, PPixels, BytesPerLine);
   }
  }

  void CopyToBitmap(Graphics::TBitmap *Bitmap)
  {
   int BytesPerLine, Row;
   void *PPixels, *PBytes;

   Bitmap->Width = Width;
   Bitmap->Height = Height;
   Bitmap->PixelFormat = PixelFormat;
   BytesPerLine = Bitmap->Width * BytePerPixel;
   for(Row=0; Row<Bitmap->Height; Row++)
   {
    PBytes = &Pixels[Row * BytesPerLine];
    PPixels = Bitmap->ScanLine[Row];
    memcpy(PPixels, PBytes, BytesPerLine);
   }
  }

  void CopyToBitmap(Graphics::TBitmap *Bitmap, TPixelFormat tf)
  {
   int BytesPerLine, Row;
   void *PPixels, *PBytes;

   Bitmap->Width = Width;
   Bitmap->Height = Height;
   Bitmap->PixelFormat = tf;
   BytesPerLine = Bitmap->Width * BytePerPixel;
   for(Row=0; Row<Bitmap->Height; Row++)
   {
    PBytes = &Pixels[Row * BytesPerLine];
    PPixels = Bitmap->ScanLine[Row];
    memcpy(PPixels, PBytes, BytesPerLine);
   }
  }

  bool Convert8BitTo24Bit(cImageData *ImageResult)
  {
   int i, Addr;
   bool ReturnValue;

   if (BytePerPixel==1) {
    ImageResult->BytePerPixel=3;
    ImageResult->BitPerPixel=24;
    ImageResult->AllocateMemory(Width, Height);

    Addr=0;
    for(i=0; i<Width*Height; i++) {
     memset(&ImageResult->Pixels[Addr], Pixels[i], 3);
     Addr+=3;
     Addr++;
    }
    ReturnValue=true;
   } else ReturnValue=false;
   return ReturnValue;
  }

  bool Convert24BitTo8Bit (cImageData *ImageResult)
  {
   int x , y, Addr, Total, Average;
   bool ReturnValue;

   if (BytePerPixel==3) {
    ImageResult->BytePerPixel=1;
    ImageResult->BitPerPixel=8;
    ImageResult->AllocateMemory(Width, Height);

    Address=0;
    Addr=0;
    for(y=0; y<Height; y++)
     for(x=0; x<Width; x++) {
      Total=(int)Pixels[Address];
      Address++;
      Total+=(int)Pixels[Address];
      Address++;
      Total+=(int)Pixels[Address];
      Address++;
      Average=(unsigned char)(Total / 3);

      ImageResult->Pixels[Addr]=Average;
      Addr++;
     }
    ReturnValue=true;
   } else ReturnValue=false;
   return ReturnValue;
  }

  bool Convert24BitTo32Bit(cImageData *ImageResult)
  {
   int x , y, Addr;
   bool ReturnValue;

   if (BytePerPixel==3) {
    ImageResult->BytePerPixel=4;
    ImageResult->BitPerPixel=32;
    ImageResult->AllocateMemory(Width, Height);

    Address=0;
    Addr=0;
    for(y=0; y<Height; y++)
     for(x=0; x<Width; x++) {
      memcpy(&ImageResult->Pixels[Addr], &Pixels[Address], 3);
      Addr+=3;
      Address+=3;
      ImageResult->Pixels[Addr]=255;
      Addr++;
     }
    ReturnValue=true;
   } else ReturnValue=false;
   return ReturnValue;
  }

  bool Convert32BitTo8Bit (cImageData *ImageResult)
  {
   int x , y, Addr, Total, Average;
   bool ReturnValue;

   if (BytePerPixel==4) {
    ImageResult->BytePerPixel=1;
    ImageResult->BitPerPixel=8;
    ImageResult->AllocateMemory(Width, Height);

    Address=0;
    Addr=0;
    for(y=0; y<Height; y++)
     for(x=0; x<Width; x++) {
      Total=(int)Pixels[Address];
      Address++;
      Total+=(int)Pixels[Address];
      Address++;
      Total+=(int)Pixels[Address];
      Address+=2;
      Average=(unsigned char)(Total / 3);

      ImageResult->Pixels[Addr]=Average;
      Addr++;
     }
    ReturnValue=true;
   } else ReturnValue=false;
   return ReturnValue;
  }

  bool Convert32BitTo24Bit(cImageData *ImageResult)
  {
   int x , y, Addr;
   bool ReturnValue;

   if (BytePerPixel==4) {
    ImageResult->BytePerPixel=3;
    ImageResult->BitPerPixel=24;
    ImageResult->AllocateMemory(Width, Height);

    Address=0;
    Addr=0;
    for(y=0; y<Height; y++)
     for(x=0; x<Width; x++) {
      memcpy(&ImageResult->Pixels[Addr], &Pixels[Address], 3);
      Address+=4;
      Addr+=3;
     }
    ReturnValue=true;
   } else ReturnValue=false;
   return ReturnValue;
  }

  void HorizontalLine(int StartX, int EndX, int y, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   int x;
   for(x=StartX; x<=EndX; x++)
    SetPixel(x, y, Red, Green, Blue);
  }

  void VerticalLine(int x, int StartY, int EndY, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   int y;
   for(y=StartY; y<=EndY; y++)
    SetPixel(x, y, Red, Green, Blue);
  }

  void CalculateLineGradient(int x1, int y1, int x2, int y2, double *rx, double *ry, int *Count)
  {
   int dx, dy, xLength, yLength;

   dx=x2-x1;
   dy=y2-y1;
   xLength=abs(dx);
   yLength=abs(dy);

   if (xLength>yLength) {
    *rx=1;
    *ry=fabs((double)yLength/xLength);
    *Count=xLength;
   } else {
    *rx=fabs((double)xLength/yLength);
    *ry=1;
    *Count=yLength;
   }

   if (dx<0) *rx=-*rx;
   if (dy<0) *ry=-*ry;
  }

  void Line(int x1, int y1, int x2, int y2, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   int i, Count;
   double x, y, rx, ry;

   CalculateLineGradient(x1, y1, x2, y2, &rx, &ry, &Count);
   x=x1;
   y=y1;

   for(i = 1; i<=Count; i++) {
    SetPixel((int)x, (int)y, Red, Green, Blue);
    x=x+rx;
    y=y+ry;
   }
  }

  void Circle(int CenterX, int CenterY, int Radius, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   double AngleDeg, AngleRad;
   int xC, yC, xS, yS;

   AngleDeg=0;
   while (AngleDeg<=360) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( Radius * cos(AngleRad) );
    yC = (int) ( Radius * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);

    if (Radius>0)
     AngleDeg+=0.1;
      else
     AngleDeg++;
   }
  }

  void Arc(int CenterX, int CenterY, int Radius, int StartAngle, int SweepAngle, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   double AngleDeg, AngleRad;
   int xC, yC, xS, yS, EndAngle;

   EndAngle=StartAngle+SweepAngle;
   AngleDeg=StartAngle;
   while (AngleDeg<=EndAngle) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( Radius * cos(AngleRad) );
    yC = (int) ( Radius * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);

    if (Radius>0)
     AngleDeg+=0.1;
    else
     AngleDeg++;
   }
  }

  void Ellipse(int CenterX, int CenterY, int RadiusX, int RadiusY, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   double AngleDeg, AngleRad;
   int xC, yC, xS, yS;

   AngleDeg=0;
   while (AngleDeg<=360) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( RadiusX * cos(AngleRad) );
    yC = (int) ( RadiusY * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);

    if (RadiusX>0 || RadiusY>0)
     AngleDeg+=0.1;
    else
     AngleDeg++;
   }
  }

  void DrawPolygon(TPoint Area[], int n, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   int i, LastX, LastY;

   for(i=1; i<n; i++) {
    Line(Area[i-1].x, Area[i-1].y, Area[i].x, Area[i].y, Red, Green, Blue);
    LastX=Area[i].x;
    LastY=Area[i].y;
   }
   Line(LastX, LastY, Area[0].x, Area[0].y, Red, Green, Blue);
  }

  void Rectangle(int x1, int y1, int x2, int y2, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   int x,y;
   for(y=y1; y<=y2; y++) {
    SetPixel(x1, y, Red, Green, Blue);
    SetPixel(x2, y, Red, Green, Blue);
   }
   for(x=x1; x<=x2; x++) {
    SetPixel(x, y1, Red, Green, Blue);
    SetPixel(x, y2, Red, Green, Blue);
   }
  }

  void Fill(int x, int y, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   bool loopY, loopX;
   int x1, y1, x2, y2;
   unsigned char R, G, B, BGR, BGG, BGB;

   GetRGB(x, y, &BGR, &BGG, &BGB);
   loopY=true;
   y1=y;
   while (y1>0 && loopY) {
    loopX=true;
    x1=x;
    while (x1>0 && loopX) {
     GetRGB(x1, y1, &R, &G, &B);
     if (R!=BGR && G!=BGG && B!=BGB)
      loopX=false;
     else
      SetPixel(x1, y1, Red, Green, Blue);
     x1--;
    }

    loopX=true;
    x2=x+1;
    while (x2<Width && loopX) {
     GetRGB(x2, y1, &R, &G, &B);
     if (R!=BGR && G!=BGG && B!=BGB)
      loopX=false;
     else
      SetPixel(x2, y1, Red, Green, Blue);
     x2++;
    }

    y1--;
    GetRGB(x, y1, &R, &G, &B);
    if (R!=BGR && G!=BGG && B!=BGB) loopY=false;
   }

   loopY=true;
   y2=y+1;
   while (y2<Height && loopY) {
    loopX=true;
    x1=x;
    while (x1>0 && loopX) {
     GetRGB(x1, y2, &R, &G, &B);
     if (R!=BGR && G!=BGG && B!=BGB)
      loopX=false;
     else
      SetPixel(x1, y2, Red, Green, Blue);
     x1--;
    }

    loopX=true;
    x2=x+1;
    while (x2<Width && loopX) {
     GetRGB(x2, y2, &R, &G, &B);
     if (R!=BGR && G!=BGG && B!=BGB)
      loopX=false;
     else
      SetPixel(x2, y2, Red, Green, Blue);
     x2++;
    }

    y2++;
    GetRGB(x, y2, &R, &G, &B);
    if (R!=BGR && G!=BGG && B!=BGB!=BGB) loopY=false;
   }
  }

  void Pie(int CenterX, int CenterY, int Radius, int StartAngle, int SweepAngle, unsigned char Red, unsigned char Green, unsigned char  Blue)
  {
   double AngleDeg, AngleRad, MiddleAngle, DA, x, y, rx, ry;
   int xC, yC, xS, yS, xR, yR, EndAngle, r, Left, Right, Top, Bottom, X, Y, xStart, xEnd;
   unsigned char BGR, BGG, BGB, R, G, B;

   Left=Right=CenterX;
   Top=Bottom=CenterY;

   EndAngle=StartAngle+SweepAngle;
   if (Radius==0) DA=1; else DA=10/(double)Radius;
   r=Radius/2;
   AngleRad = (StartAngle+SweepAngle/2) * M_PI / 180;
   xC = (int) ( r * cos(AngleRad) );
   yC = (int) ( r * sin(AngleRad) );
   xS = CenterX + xC;
   yS = CenterY - yC;
   GetRGB(xS, yS, &BGR, &BGG, &BGB);

   AngleRad = StartAngle * M_PI / 180;
   xC = (int) ( Radius * cos(AngleRad) );
   yC = (int) ( Radius * sin(AngleRad) );
   xS = CenterX + xC;
   yS = CenterY - yC;
   Line(CenterX, CenterY, xS, yS, Red, Green, Blue);
   if (xS<Left)   Left=xS;
   if (xS>Right)  Right=xS;
   if (yS<Top)    Top=yS;
   if (yS>Bottom) Bottom=yS;

   AngleRad = EndAngle * M_PI / 180;
   xC = (int) ( Radius * cos(AngleRad) );
   yC = (int) ( Radius * sin(AngleRad) );
   xS = CenterX + xC;
   yS = CenterY - yC;
   Line(CenterX, CenterY, xS, yS, Red, Green, Blue);
   if (xS<Left)   Left=xS;
   if (xS>Right)  Right=xS;
   if (yS<Top)    Top=yS;
   if (yS>Bottom) Bottom=yS;

   AngleDeg=StartAngle;
   while (AngleDeg<=EndAngle) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( Radius * cos(AngleRad) );
    yC = (int) ( Radius * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);

    if (xS<Left)   Left=xS;
    if (xS>Right)  Right=xS;
    if (yS<Top)    Top=yS;
    if (yS>Bottom) Bottom=yS;
    AngleDeg+=DA;
   }

   for(Y=Top; Y<=Bottom; Y++) {
    X=Left;
    GetRGB(X, Y, &R, &G, &B);
    while (R==BGR && G==BGG && B==BGB) {
     X++;
     GetRGB(X, Y, &R, &G, &B);
    }
    if (X<CenterX+Radius) {
     xStart=X;
     while (R!=BGR || G!=BGG || B!=BGB) {
      X++;
      GetRGB(X, Y, &R, &G, &B);
     }
     xStart=X;
     while (R==BGR && G==BGG && B==BGB) {
      X++;
      GetRGB(X, Y, &R, &G, &B);
     }
     if (X<CenterX+Radius && X>xStart) {
      xEnd=X;
      HorizontalLine(xStart, xEnd, Y, Red, Green, Blue);
     }
    }
   }
  }

  void FillRectangle(int x1, int y1, int x2, int y2, unsigned char Red, unsigned char Green, unsigned char Blue)
  {
   int x,y;
   for(y=y1; y<=y2; y++)
    for(x=x1; x<=x2; x++)
     SetPixel(x, y, Red, Green, Blue);
  }

  void FillCircle(int CenterX, int CenterY, int Radius, unsigned char Red, unsigned char Green, unsigned char Blue)
  {
   double AngleDeg, AngleRad;
   int xC, yC, xS, yS;

   AngleDeg=0;
   while (AngleDeg<=360) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( Radius * cos(AngleRad) );
    yC = (int) ( Radius * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);

    if (Radius>0)
     AngleDeg+=0.1;
    else
     AngleDeg++;
   }
   Fill(CenterX, CenterY, Red, Green, Blue);
  }

  void FillEllipse(int CenterX, int CenterY, int RadiusX, int RadiusY, unsigned char Red, unsigned char Green, unsigned char Blue)
  {
   double AngleDeg, AngleRad;
   int xC, yC, xS, yS;

   AngleDeg=0;
   while (AngleDeg<=360) {
    AngleRad = AngleDeg * M_PI / 180;
    xC = (int) ( RadiusX * cos(AngleRad) );
    yC = (int) ( RadiusY * sin(AngleRad) );
    xS = CenterX + xC;
    yS = CenterY - yC;

    if ((xS>=0 && xS<Width) && (yS>=0 && yS<Height))
     SetPixel(xS, yS, Red, Green, Blue);;

    if (RadiusX>0 || RadiusY>0)
     AngleDeg+=0.1;
    else
     AngleDeg++;
   }
   Fill(CenterX, CenterY, Red, Green, Blue);
  }

  ~cImageData()
  {
   //if (Pixels!=NULL) free(Pixels);
  }
};

