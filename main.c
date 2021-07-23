#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void read(unsigned char ***colorData,unsigned char **grayData,unsigned char *header,unsigned char *colorTable,int *height,int *width,int *bitDepth);
void imageWrite(unsigned char ***colorData,unsigned char **grayData,unsigned char *header,unsigned char *colorTable,int height,int width,int bitDepth);
void setPixelVal(int height,int bitDepth,unsigned char **graydata,unsigned char ***colorData);
void getImageDetails(int bitDepth,int height,int width,unsigned char **datag,unsigned char ***datac);
void getPixelVal(int height,int bitDepth,unsigned char **datag,unsigned char ***datac);
void colorToGray(int height,int width,int bitDepth,unsigned char ***datac);
void averageVal(int height,int width,int bitdepth,unsigned char **pixelG,unsigned char ***pixelC);
void grayToBinary(int height,int width,int bitDepth,unsigned char **datag,unsigned char ***datac);
void reflectImage(unsigned char **datag,unsigned char ***dataC,int height,int width,int bitDepth);
void enlarge(unsigned char **datag,unsigned char ***datac,int *height,int *width,unsigned char *header,int bitDepth);
void cropImage(unsigned char **datag,unsigned char ***datac,int *height,int *width,int bitDepth,unsigned char *header);
void negativaImage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth);
void translateIMage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth);
void rotateImage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth);
void sepiaFilter(unsigned char ***datac,int height,int width,int bitDepth);
void ImageMorphing(unsigned char ***datac,unsigned char**,int height,int width,int bitDepth);
void changeDetection(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth);
void meanFilter(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth);
void highPassFilter(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth);



int main()
{
    unsigned char ***ColorImagePixel;
    unsigned char **GrayImagePixel;       //Variable declaration
    unsigned char header[54];
    unsigned char colorTable[1024];
    int height,width,bitDepth,num;

    ColorImagePixel=malloc(3*sizeof(unsigned char*));
    GrayImagePixel=malloc(2800*sizeof(unsigned char*));
    for(int i=0;i<2800;i++)                                               //Dynamic allocation of 2d array matrix for gray image
    {                                                                     //Dynamic allocation of 3d array for color image
        GrayImagePixel[i]=malloc(2800*sizeof(unsigned char));
    }
    for(int i=0;i<3;i++)
    {

        ColorImagePixel[i]=malloc(2800*sizeof(unsigned char*));
        for(int j=0;j<2800;j++)
        {
            ColorImagePixel[i][j]=malloc(2800*sizeof(unsigned char));
        }
    }


//Switch case for selection of different option
    while(1)
    {
        printf("Enter\n1 To read an image\n2 To write image\n3 To get image details\n4 To get pixel value at location\n");
        printf("5 To set pixel value at location\n6 To convert color image to grayscale image\n");
        printf("7 To get average pixel value of image\n8 To convert to binary image\n");
        printf("9 To reflect image h-horizontal or v-vertical\n10 To enlarge/shorten image\n");
        printf("11 To crop an image\n12 To get negative of image\n13 To translate an image\n");
        printf("14 To rotate image\n15 To apply sepia filter\n16 To apply MEAN filter\n");
        printf("17 To apply High Pass Filter\n18 To do image morphing\n");
        printf("19 To do change detection\n");
        scanf("%d",&num);


        switch(num)
        {
        case 1:
            read(ColorImagePixel,GrayImagePixel,header,colorTable,&height,&width,&bitDepth);
            break;
        case 2:
            imageWrite(ColorImagePixel,GrayImagePixel,header,colorTable,height,width,bitDepth);
            break;
        case 3:
            getImageDetails(bitDepth,height,width,GrayImagePixel,ColorImagePixel);
            break;
        case 4:
            getPixelVal(height,bitDepth,GrayImagePixel,ColorImagePixel);
            break;
        case 5:
            setPixelVal(height,bitDepth,GrayImagePixel,ColorImagePixel);
            break;
        case 6:
            colorToGray(height,width,bitDepth,ColorImagePixel);
            break;
        case 7:
            averageVal(height,width,bitDepth,GrayImagePixel,ColorImagePixel);
            break;
        case 8:
            grayToBinary(height,width,bitDepth,GrayImagePixel,ColorImagePixel);
            break;
        case 9:
            reflectImage(GrayImagePixel,ColorImagePixel,height,width,bitDepth);
            break;
        case 10:
            enlarge(GrayImagePixel,ColorImagePixel,&height,&width,header,bitDepth);
            break;
        case 11:
            cropImage(GrayImagePixel,ColorImagePixel,&height,&width,bitDepth,header);
            break;
        case 12:
            negativaImage(GrayImagePixel,ColorImagePixel,height,width,bitDepth);
            break;
        case 13:
            translateIMage(GrayImagePixel,ColorImagePixel,height,width,bitDepth);
            break;
        case 14:
            rotateImage(GrayImagePixel,ColorImagePixel,height,width,bitDepth);
            break;
        case 15:
            sepiaFilter(ColorImagePixel,height,width,bitDepth);
            break;
        case 16:
            meanFilter(ColorImagePixel,GrayImagePixel,height,width,bitDepth);
            break;
        case 17:
            highPassFilter(ColorImagePixel,GrayImagePixel,height,width,bitDepth);
            break;
        case 18:
            ImageMorphing(ColorImagePixel,GrayImagePixel,height,width,bitDepth);
            break;
        case 19:
            changeDetection(ColorImagePixel,GrayImagePixel,height,width,bitDepth);
            break;


        default:

            free(GrayImagePixel);
            free(ColorImagePixel);
            exit(1);
        }
    }
    return 0;
}

void read(unsigned char ***colorData,unsigned char **grayData,unsigned char *header,unsigned char *colorTable,int *height,int *width,int *bitDepth)
{
    char name[60];
    printf("Enter image name to read: ");
    scanf("%s",name);
    FILE *imgIN;
    imgIN=fopen(name,"rb");
    if(imgIN==NULL)
    {
        printf("Image not open\n");
        free(colorData);
        free(grayData);
        exit(1);
    }

    for(int i=0;i<54;i++)
    {
        header[i]=getc(imgIN);      //read Header from image
    }
    *height=*(int*)&header[22];
    *width=*(int*)&header[18];           //Extract height,width and bitDepth from header
    *bitDepth=*(int*)&header[28];

    //For gray image
    if(*bitDepth<=8)
    {
        fread(colorTable,sizeof(unsigned char),1024,imgIN);        //Read colorTable from image
        for(int i=0;i<*height;i++)
        {
            for(int j=0;j<*width;j++)
            {
                grayData[i][j]=getc(imgIN);
                                                    //Read image pixel data from image
            }
        }
    }

    //for color image
    else if(*bitDepth>8)
    {
        for(int i=0;i<*height;i++)
        {
            for(int j=0;j<*width;j++)
            {
                colorData[0][i][j]=getc(imgIN);
                colorData[1][i][j]=getc(imgIN);      //read Image pixel data for blue green and red
                colorData[2][i][j]=getc(imgIN);
            }
        }

    }
    fclose(imgIN);
    printf("Image read successful\n\n");


}



void imageWrite(unsigned char ***colorData,unsigned char **grayData,unsigned char *header,unsigned char *colorTable,int height,int width,int bitDepth)
{
   FILE *imgOut;
   imgOut=fopen("copy.bmp","wb");
   if(imgOut==NULL)
   {
       printf("Image not created\n");
       free(colorData);
       free(grayData);
       exit(1);
   }

   fwrite(header,sizeof(unsigned char),54,imgOut);                     //Write header detail into output image

   //for gray image
   if(bitDepth<=8)
   {
       fwrite(colorTable,sizeof(unsigned char),1024,imgOut);           //write colorTable
       for(int i=0;i<height;i++)
       {
          for(int j=0;j<width;j++)
            {
                putc(grayData[i][j],imgOut);                //write image pixel data

            }
       }
   }

   //for color image
   else if(bitDepth>8)
   {
       for(int i=0;i<height;i++)
       {
           for(int j=0;j<width;j++)
           {
               putc(colorData[0][i][j],imgOut);                         //write image pixel data of blue green red simultaneously
               putc(colorData[1][i][j],imgOut);
               putc(colorData[2][i][j],imgOut);
           }
       }
   }
   fclose(imgOut);
   printf("Image write successfull\n\n");



}

void setPixelVal(int height,int bitDepth,unsigned char **graydata,unsigned char ***colorData)
{
    int row,col,pixVal;
    printf("Enter row no and column no from top\n");         //row and column input
    scanf("%d%d",&row,&col);
    row=height-row;                                          //subtracting row from height to get its location from bottom left corner

      //for gray image                                                        //pixel data in bitmap image are stored from bottom left corner
     if(bitDepth<=8)
    {
        printf("Enter pixel value at location: ");           //user input pixel value
        scanf("%d",&pixVal);
        graydata[row][col]=pixVal;                          //writing user input pixel value in image
        printf("Pixel set successful\n\n");
    }

    //for color image
    if(bitDepth>8)
    {
        int pixR,pixG,pixB;
        printf("Enter pixel value of RED GREEN BLUE\n");     //user input pixel value of red green and blue
        scanf("%d%d%d",&pixR,&pixG,&pixB);
        colorData[0][row][col]=pixB;
        colorData[1][row][col]=pixG;                          //writing red, green, blue pixel value to image
        colorData[2][row][col]=pixR;
        printf("Pixel set successful\n\n");
    }
    printf("Pixel set successfull\n\n");





}

void getImageDetails(int bitDepth,int height,int width,unsigned char **datag,unsigned char ***datac)
{
    printf("No of rows= %d\nNo of columns= %d\n",height,width);            //printing of no of rows and columns

     //for gray image
     //int16_t
    if(bitDepth<=8)
    {
        int maxVal=datag[0][0];
        for(int i=0;i<height;i++)
        {                                                     //finding maximum pixel value in gray image
            for(int j=0;j<width;j++)
            {
                if(datag[i][j]>maxVal)
                {
                    maxVal=datag[i][j];
                }

            }
        }
        printf("The max pixel value= %d\n",maxVal);             //printing of maximum pixel value
    }

     //for color image
    else if(bitDepth>8)
    {
        int maxValR=datac[2][0][0];
        int maxValG=datac[1][0][0];                            //intialize 1st pixel value of red, green, blue simultaneously to variable
        int maxValB=datac[0][0][0];
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(datac[2][i][j]>maxValR)
                {
                    maxValR=datac[2][i][j];
                }                                                //finding maximum pixel value of color image for red, blue and green
                if(datac[1][i][j]>maxValG)
                {
                    maxValG=datac[1][i][j];
                }
                if(datac[0][i][j]>maxValB)
                {
                    maxValB=datac[0][i][j];
                }

            }
        }
        printf("Max Red pixel value= %d\n",maxValR);
        printf("Max Green pixel value= %d\n",maxValG);                  //printing of max pixel value
        printf("Max Blue pixel value= %d\n",maxValB);
    }



}

void getPixelVal(int height,int bitDepth,unsigned char **datag,unsigned char ***datac)
{
    int row,col;
    printf("\nEnter row and column\n");                                 //user input of row and column from top
    scanf("%d%d",&row,&col);
    row=height-row;                                                     //changing location of row

    //for gray image
    if(bitDepth<=8)
    {
        int val;
        val=datag[row][col];
        printf("pixel value= %d\n",val);                               //printing of pixel value at location

    }

    //for color image
    else if(bitDepth>8)
    {
        int red,green,blue;
        red=datac[2][row][col];
        green=datac[1][row][col];
        blue=datac[0][row][col];
        printf("Pixel value\nRed= %d\nGreen= %d\nBlue= %d\n\n",red,green,blue);        //printing of pixel value o red green and blue
                                                                                        //at given location
    }




}

void colorToGray(int height,int width,int bitDepth,unsigned char ***datac)
{

     //for gray image
    if(bitDepth>8)
    {
      for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                int temp;
                temp=(datac[2][i][j]*0.3)+(datac[1][i][j]*0.59)+(datac[0][i][j]*0.11);        //computing gray value using this
                datac[0][i][j]=temp;
                datac[1][i][j]=temp;
                datac[2][i][j]=temp;
            }
        }
        printf("Image converted to grayscale\n\n");
    }
    else
    {
        printf("Image is already grayscale\n\n");
    }

}

void averageVal(int height,int width,int bitdepth,unsigned char **pixelG,unsigned char ***pixelC)
{
    //for gray image
    if(bitdepth<=8)
    {
        int sum=0,avg,sizee=height*width;

        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                sum=sum+pixelG[i][j];        //summation of pixel value for gray image
            }
        }
        avg=sum/sizee;
        printf("Average grayscale value= %d\n\n",avg);
    }
    else if(bitdepth>8)
    {
        int sum=0,avg,sizee=height*width;
        if(pixelC[0][0][0]==pixelC[1][0][0]&&pixelC[1][0][0]==pixelC[2][0][0])
        {
          for(int i=0;i<height;i++)
           {
            for(int j=0;j<width;j++)
            {
                sum=sum+pixelC[0][i][j];
            }
           }
           avg=sum/sizee;
           printf("Average grayscale value= %d\n\n",avg);

        }
        else
        {
            int sumR=0,sumG=0,sumB=0,avg1,avg2,avg3;
            for(int i=0;i<height;i++)
           {
            for(int j=0;j<width;j++)
            {
                sumR=sumR+pixelC[2][i][j];
                sumG=sumG+pixelC[1][i][j];
                sumB=sumB+pixelC[0][i][j];
            }
           }
           avg1=sumR/sizee;
           avg2=sumG/sizee;
           avg3=sumB/sizee;
           printf("Average Value\nRed= %d\nGreen= %d\nBlue= %d\n\n",avg1,avg2,avg3);
        }
    }



}

void grayToBinary(int height,int width,int bitDepth,unsigned char **datag,unsigned char ***datac)
{

    //printf("Enter threshold value: ");
    //scanf("%d",&thres);

    if(bitDepth<=8)
    {
        int thres=120;
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(datag[i][j]>thres)
                {
                    datag[i][j]=255;
                }
                else
                {
                    datag[i][j]=0;
                }
            }
        }
        printf("gray Image converted to binary successfully\n\n");
    }

    else if(bitDepth>8)
    {
        int thres=100;
        if(datac[0][0][0]==datac[1][0][0]&&datac[1][0][0]==datac[2][0][0])
        {
         for(int i=0;i<height;i++)
          {
            for(int j=0;j<width;j++)
            {
                if(datac[0][i][j]>=thres)
                {
                    datac[0][i][j]=255;
                    datac[1][i][j]=255;
                    datac[2][i][j]=255;
                }
                else
                {
                    datac[0][i][j]=0;
                    datac[1][i][j]=0;
                    datac[2][i][j]=0;
                }
             }
            }
            printf("Image converted to binary successfully\n\n");
        }
        else
        {
            //printf("color loop");
            colorToGray(height,width,bitDepth,datac);
            grayToBinary(height,width,bitDepth,datag,datac);
            //printf("Image converted to binary successfully\n\n");
        }
    }

}

void reflectImage(unsigned char **datag,unsigned char ***dataC,int height,int width,int bitDepth)
{
    unsigned char **gray;
    unsigned char ***color;


    gray=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
      gray[i]=malloc(width*sizeof(unsigned char*));
    }


    color=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        color[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            color[i][j]=malloc(width*sizeof(unsigned char));
        }
    }


    char flag;
    printf("Enter reflection axis h -horizontal v -vertical: ");
    scanf(" %c",&flag);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(flag=='v')
            {
                if(bitDepth<=8)
                {
                    gray[i][j]=datag[i][width-1-j];
                }
                else if(bitDepth>8)
                {
                    color[0][i][j]=dataC[0][i][width-1-j];
                    color[1][i][j]=dataC[1][i][width-1-j];
                    color[2][i][j]=dataC[2][i][width-1-j];
                }

            }
            else if(flag=='h')
            {
                if(bitDepth<=8)
                {
                    gray[i][j]=datag[height-1-i][j];
                }
                else if(bitDepth>8)
                {
                    color[0][i][j]=dataC[0][height-1-i][j];
                    color[1][i][j]=dataC[1][height-1-i][j];
                    color[2][i][j]=dataC[2][height-1-i][j];
                }

            }

        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=gray[i][j];
            }
            else if(bitDepth>8)
            {
                dataC[0][i][j]=color[0][i][j];
                dataC[1][i][j]=color[1][i][j];
                dataC[2][i][j]=color[2][i][j];
            }

        }
    }
    if(flag=='v')
    {
        printf("Image reflected in vertical direction\n\n");
    }
    else if(flag=='h')
    {
        printf("Image reflected in horizontal direction\n\n");
    }
    free(gray);
    free(color);



}

void enlarge(unsigned char **datag,unsigned char ***datac,int *height,int *width,unsigned char *header,int bitDepth)
{
    float fac1,fac2;
    int newheight,newwidth;

    printf("Enter enlarge/shrink factor in horizontal and vertical direction: ");
    scanf("%f%f",&fac1,&fac2);


    newheight=fac2 * (*height);
    newwidth=fac1 * (*width);
    *height=newheight;
    *width=newwidth;
    *(int *)&header[22]=*height;
    *(int *)&header[18]=*width;


    unsigned char **newDatag;
    unsigned char ***newDataC;
    newDatag=malloc(newheight*sizeof(unsigned char*));
    for(int i=0;i<newheight;i++)
    {
        newDatag[i]=(unsigned char*)malloc(newwidth*sizeof(unsigned char*));
    }

    newDataC=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {

        newDataC[i]=malloc(newheight*sizeof(unsigned char*));
        for(int j=0;j<newheight;j++)
        {
            newDataC[i][j]=malloc(newwidth*sizeof(unsigned char));
        }
    }

    //printf("Success\n");


    for(int i=0;i<newheight;i++)
    {
        for(int j=0;j<newwidth;j++)
        {
            if(bitDepth<=8)
            {
                newDatag[i][j]=datag[(int)(i/fac2)][(int)(j/fac1)];  //for grayscale image
            }
            else
            {
                newDataC[0][i][j]=datac[0][(int)(i/fac2)][(int)(j/fac1)];   //for color image
                newDataC[1][i][j]=datac[1][(int)(i/fac2)][(int)(j/fac1)];
                newDataC[2][i][j]=datac[2][(int)(i/fac2)][(int)(j/fac1)];
            }

        }

    }

    for(int i=0;i<newheight;i++)
    {
        for(int j=0;j<newwidth;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=newDatag[i][j];
            }
            else
            {
                datac[0][i][j]=newDataC[0][i][j];
                datac[1][i][j]=newDataC[1][i][j];
                datac[2][i][j]=newDataC[2][i][j];
            }

        }

    }
    free(newDatag);
    free(newDataC);

    printf("Image is enlarged\n\n");







}

void cropImage(unsigned char **datag,unsigned char ***datac,int *height,int *width,int bitDepth,unsigned char *header)
{
    int UpperCut,LowerCut,LeftCut,RightCut;
    int oldHeight=*height;
    int oldWidth=*width;

    printf("Width= %d Height= %d\n",oldWidth,oldHeight);
    printf("Enter Left cut,Right cut, Upper cut, Lower cut\n");
    scanf("%d %d %d %d",&LeftCut,&RightCut,&UpperCut,&LowerCut);

    *height=oldHeight-(UpperCut+LowerCut);
    *width=oldWidth-(LeftCut+RightCut);
    *(int *)&header[18]=*width;
    *(int *)&header[22]=*height;

    printf("1 gate\n");


    RightCut=oldWidth-RightCut;
    UpperCut=oldHeight-UpperCut;

    printf("%d\t%d\n",*(int *)&header[18],*(int *)&header[22]);

    unsigned char **dataNew;
    dataNew=malloc((*height)*sizeof(unsigned char*));
    for(int i=0;i<*height;i++)
     {
         dataNew[i]=malloc((*width)*sizeof(unsigned char*));
     }

     unsigned char ***dataNewC;
     dataNewC=malloc(3*sizeof(unsigned char*));
     for(int i=0;i<3;i++)
     {
         dataNewC[i]=(unsigned char**)malloc((*height)*sizeof(unsigned char*));
         for(int j=0;j<*height;j++)
         {
             dataNewC[i][j]=malloc((*width)*sizeof(unsigned char));
         }
     }



    for(int m=LowerCut,i=0;m<UpperCut;m++,i++)
    {
        for(int n=LeftCut,j=0;n<RightCut;n++,j++)
        {
            if(bitDepth<=8)
            {
                dataNew[i][j]=datag[m][n];

            }
            else if(bitDepth>8)
            {
                dataNewC[0][i][j]=datac[0][m][n];
                dataNewC[1][i][j]=datac[1][m][n];
                dataNewC[2][i][j]=datac[2][m][n];
            }
        }

    }
    printf("gate 2\n");

    for(int i=0;i<*height;i++)
    {
        for(int j=0;j<*width;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=dataNew[i][j];
            }
            else if(bitDepth>8)
            {
                datac[0][i][j]=dataNewC[0][i][j];
                datac[1][i][j]=dataNewC[1][i][j];
                datac[2][i][j]=dataNewC[2][i][j];
            }

        }
    }
    printf("Crop Successfull\n\n");
    free(dataNew);
    free(dataNewC);

}

void negativaImage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth)
{
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=255-datag[i][j];
            }
            else
            {
                datac[0][i][j]=255-datac[0][i][j];
                datac[1][i][j]=255-datac[1][i][j];
                datac[2][i][j]=255-datac[2][i][j];
            }

        }

    }
    printf("Nagative image complete\n\n");



}


void translateIMage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth)
{
    int x,y,a,b;
    unsigned char **newg,***newc;

    newg=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        newg[i]=malloc(width*sizeof(unsigned char*));
    }

     newc=malloc(3*sizeof(unsigned char*));
     for(int i=0;i<3;i++)
     {
         newc[i]=malloc(height*sizeof(unsigned char*));
         for(int j=0;j<height;j++)
         {
             newc[i][j]=malloc(width*sizeof(unsigned char));
         }
     }


    printf("Enter translation amount in x and y\n");
    scanf("%d%d",&x,&y);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            a=j-x;
            b=i-y;
            if((b>=0&&b<height)&&(a>=0&&a<width))
              {
                  if(bitDepth<=8)
                  {
                      newg[i][j]=datag[b][a];
                  }
                  else
                  {
                      newc[0][i][j]=datac[0][b][a];
                      newc[1][i][j]=datac[1][b][a];
                      newc[2][i][j]=datac[2][b][a];
                  }

              }
        }
    }

    printf("gate 1\n");
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=newg[i][j];
            }
            else
            {
                datac[0][i][j]=newc[0][i][j];
                datac[1][i][j]=newc[1][i][j];
                datac[2][i][j]=newc[2][i][j];
            }

        }
    }
    printf("Translation success\n\n");
    free(newg);
    free(newc);
}

void rotateImage(unsigned char **datag,unsigned char ***datac,int height,int width,int bitDepth)
{
    unsigned char **newDatag,***newDatac;
    double angle;
    int hWidth=width/2,hHeight=height/2,a,b,y,x;

    newDatag=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        newDatag[i]=malloc(height*sizeof(unsigned char));
    }

    newDatac=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        newDatac[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            newDatac[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    printf("Enter angle to rotate: ");
    scanf("%lf",&angle);
    angle=angle*3.141592654/180;

    for(int y1=0;y1<height;y1++)                               //a for x b for y
    {
        for(int x1=0;x1<width;x1++)
        {
            a=x1-hWidth;
            b=y1-hHeight;
            y=(b*cos(angle)+a*sin(angle))+hHeight;
            x=(-b*sin(angle)+a*cos(angle))+hWidth;
            if((y>0&&y<height)&&(x>0&&x<width))
               {
                   if(bitDepth<=8)
                {
                    newDatag[y1][x1]=datag[y][x];     //for grayscale image
                }

                //for color image
                else
                {
                    newDatac[0][y1][x1]=datac[0][y][x];
                    newDatac[1][y1][x1]=datac[1][y][x];
                    newDatac[2][y1][x1]=datac[2][y][x];
                }
               }
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(bitDepth<=8)
            {
                datag[i][j]=newDatag[i][j];
            }
            else
            {
                datac[0][i][j]=newDatac[0][i][j];
                datac[1][i][j]=newDatac[1][i][j];
                datac[2][i][j]=newDatac[2][i][j];
            }
        }
    }
    free(newDatac);
    free(newDatag);
    printf("Image rotated successfully\n\n");

}

void sepiaFilter(unsigned char ***datac,int height,int width,int bitDepth)
{
    int r,b,g;
    unsigned char ***newDatac;
    newDatac=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        newDatac[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            newDatac[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    if(bitDepth>8)
   {
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {

            r=(datac[2][i][j]*0.393)+(datac[1][i][j]*0.769)+(datac[0][i][j]*0.189);
            g=(datac[2][i][j]*0.349)+(datac[1][i][j]*0.686)+(datac[0][i][j]*0.168);
            b=(datac[2][i][j]*0.272)+(datac[1][i][j]*0.534)+(datac[0][i][j]*0.131);
            if(r>255)
                r=255;
            if(g>255)
                g=255;
            if(b>255)
                g=255;
            newDatac[0][i][j]=b;
            newDatac[1][i][j]=g;
            newDatac[2][i][j]=r;
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            datac[0][i][j]=newDatac[0][i][j];
            datac[1][i][j]=newDatac[1][i][j];
            datac[2][i][j]=newDatac[2][i][j];
        }
    }
    free(newDatac);
    printf("Sepia filter applied\n\n");

   }
}

void ImageMorphing(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth)
{

    unsigned char ***img2;
    img2=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        img2[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            img2[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    unsigned char **newData;
    newData=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        newData[i]=malloc(width*sizeof(unsigned char));

    }

    char name2[50];
    unsigned char header[54];
    unsigned char colorTable[1024];

    printf("Enter 2nd image name: ");
    scanf("%s",name2);
    FILE *image2;
    image2=fopen(name2,"rb");
    //fseek(image2,54,SEEK_CUR);
    for(int i=0;i<54;i++)
    {
        header[i]=getc(image2);
    }



    int hght=*(int*)&header[22];
    int wdth=*(int*)&header[18];
    int bitdepth=*(int*)&header[28];
    int r,g,b;

    if((hght==height)&&(wdth==width)&&(bitdepth==bitDepth))
    {
        if(bitDepth>8)
        {
           for(int i=0;i<54;i++)
           {
              header[i]=getc(image2);
           }
            //getting 2nd image pixel data
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    img2[0][i][j]=getc(image2);
                    img2[1][i][j]=getc(image2);
                    img2[2][i][j]=getc(image2);
                }
            }

            //both image pixel addition
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    r=g=b=0;
                    b=img2[0][i][j]+datac[0][i][j];
                    g=img2[1][i][j]+datac[1][i][j];
                    r=img2[2][i][j]+datac[2][i][j];
                    if(r>255)
                    {
                        r=255;
                    }
                    if(b>255)
                    {
                        b=255;
                    }
                    if(g>255)
                    {
                        g=255;
                    }

                    //pixel data return writing to original image
                    datac[0][i][j]=b;
                    datac[1][i][j]=g;
                    datac[2][i][j]=r;

                }
            }
        }
        else
        {
           int val;
           fread(colorTable,sizeof(unsigned char),1024,image2);
           for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    newData[i][j]=getc(image2);

                }
            }
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    val=0;
                    val=newData[i][j]+datag[i][j];
                    if(val>255)
                    {
                        val=255;
                    }
                    datag[i][j]=val;

                }
            }
        }
    }
    else
    {
        printf("Dimension of 2nd image not matching with 1st image\n\n");
    }
    free(img2);
    free(newData);
    fclose(image2);






}

void changeDetection(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth)
{
   unsigned char ***img2;
    img2=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        img2[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            img2[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    unsigned char **newData;
    newData=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        newData[i]=malloc(width*sizeof(unsigned char));

    }

    char name2[50];
    unsigned char header[54];
    unsigned char colorTable[1024];

    printf("Enter 2nd image name: ");
    scanf("%s",name2);
    FILE *image2;
    image2=fopen(name2,"rb");
    //fseek(image2,54,SEEK_CUR);
    for(int i=0;i<54;i++)
    {
        header[i]=getc(image2);
    }



    int hght=*(int*)&header[22];
    int wdth=*(int*)&header[18];
    int bitdepth=*(int*)&header[28];
    int r,g,b;

    if((hght==height)&&(wdth==width)&&(bitdepth==bitDepth))
    {
        if(bitDepth>8)
        {
           for(int i=0;i<54;i++)
           {
              header[i]=getc(image2);
           }
            //getting 2nd image pixel data
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    img2[0][i][j]=getc(image2);
                    img2[1][i][j]=getc(image2);
                    img2[2][i][j]=getc(image2);
                }
            }

            //both image pixel addition
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    r=g=b=0;
                    b=img2[0][i][j]-datac[0][i][j];
                    g=img2[1][i][j]-datac[1][i][j];
                    r=img2[2][i][j]-datac[2][i][j];
                    if(r<0)
                    {
                        r=0;
                    }
                    if(b<0)
                    {
                        b=0;
                    }
                    if(g<0)
                    {
                        g=0;
                    }

                    //pixel data return writing to original image
                    datac[0][i][j]=b;
                    datac[1][i][j]=g;
                    datac[2][i][j]=r;

                }
            }
        }
        else
        {
           int val;
           fread(colorTable,sizeof(unsigned char),1024,image2);
           for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    newData[i][j]=getc(image2);

                }
            }
            for(int i=0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    val=0;
                    val=newData[i][j]-datag[i][j];
                    if(val<0)
                    {
                        val=0;
                    }
                    datag[i][j]=val;

                }
            }
        }
    }
    else
    {
        printf("Dimension of 2nd image not matching with 1st image\n\n");
    }
    free(img2);
    free(newData);
    fclose(image2);



}

void meanFilter(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth)
{

    unsigned char ***colorData;
    colorData=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        colorData[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            colorData[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    unsigned char **grayData;
    grayData=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        grayData[i]=malloc(width*sizeof(unsigned char));

    }
    int y,x;
    //int half=3/2;
    float v=1.0/9.0;
    float mask[3][3]={ {v,v,v},
                     {v,v,v},
                     {v,v,v} };
     if(bitDepth<=8)
     {
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 for(int m=0;m<3;m++)
                 {
                     for(int n=0;n<3;n++)
                     {
                         y=i+(m-1);
                         x=j+(n-1);
                         if(y>=0&&y<height&&x>=0&&x<width)
                         {
                             grayData[i][j]+=mask[m][n]*datag[y][x];
                         }

                     }
                 }

             }
         }
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 datag[i][j]=grayData[i][j];
             }
         }
     }
     else
     {
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 for(int m=0;m<3;m++)
                 {
                     for(int n=0;n<3;n++)
                     {
                         y=i+(m-1);
                         x=j+(n-1);
                         if(y>=0&&x>=0)
                         {
                             colorData[0][i][j]+=mask[m][n]*datac[0][y][x];
                             colorData[1][i][j]+=mask[m][n]*datac[1][y][x];
                             colorData[2][i][j]+=mask[m][n]*datac[2][y][x];
                         }

                     }
                 }

             }
         }
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 datac[0][i][j]=colorData[0][i][j];
                 datac[1][i][j]=colorData[1][i][j];
                 datac[2][i][j]=colorData[2][i][j];
             }
         }
     }
     free(colorData);
     free(grayData);
     printf("Mean filter applied\n\n");


}

void highPassFilter(unsigned char ***datac,unsigned char **datag,int height,int width,int bitDepth)
{
     unsigned char ***colorData;
    colorData=malloc(3*sizeof(unsigned char*));
    for(int i=0;i<3;i++)
    {
        colorData[i]=malloc(height*sizeof(unsigned char*));
        for(int j=0;j<height;j++)
        {
            colorData[i][j]=malloc(width*sizeof(unsigned char));
        }
    }

    unsigned char **grayData;
    grayData=malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++)
    {
        grayData[i]=malloc(width*sizeof(unsigned char));

    }
    int y,x;
    int v=-1;
    float mask[3][3]={ {v,v,v},
                     {v,9,v},
                     {v,v,v}};


    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
           grayData[i][j]=0;
        }
    }



     int c,d;
     if(bitDepth<=8)
     {
         for(int i=0;i<height;++i)
         {
             for(int j=0;j<width;++j)
             {
                 for(int m=0;m<3;++m)
                 {
                     for(int n=0;n<3;++n)
                     {
                         y=i-m;
                         x=j-n;

                         if(y>=0&&y<height&&x>=0&&x<width)
                         {
                              grayData[i][j]+=(signed char)(mask[m][n])*datag[y][x];
                         }
                     }
                 }
             }
         }
         for(int i=0;i<height;++i)
         {
             for(int j=0;j<width;++j)
             {
                 datag[i][j]=grayData[i][j];
             }
         }
     }
     else
     {
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 for(int m=-1;m<2;m++)
                 {
                     for(int n=-1;n<2;n++)
                     {
                         y=i-m;
                         x=j-n;
                         if(y>=0&&x>=0)
                         {
                             colorData[0][i][j]+=mask[m][n]*datac[0][y][x];
                             colorData[1][i][j]+=mask[m][n]*datac[1][y][x];
                             colorData[2][i][j]+=mask[m][n]*datac[2][y][x];
                         }

                     }
                 }

             }
         }
         for(int i=0;i<height;i++)
         {
             for(int j=0;j<width;j++)
             {
                 datac[0][i][j]=colorData[0][i][j];
                 datac[1][i][j]=colorData[1][i][j];
                 datac[2][i][j]=colorData[2][i][j];
             }
         }
     }
     free(colorData);
     free(grayData);
     printf("High Pass filter applied\n\n");

}



