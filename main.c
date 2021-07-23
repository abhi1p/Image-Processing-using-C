#include <stdio.h>
#include <stdlib.h>
#define MAXV 512*512
#define SIZEH 54
#define SIZEC 1024

void read(unsigned char header[],unsigned char colortable[],unsigned char pixelvalue[],unsigned char colorpixel[][MAXV],int *height,int *width,int *bitdepth)
{
    int i;
    FILE *img;
    char name1[50];
    printf("Enter image name:");
    scanf("%s",name1);
    img=fopen(name1,"rb");
    if(img==NULL)
    {
        printf("Error\n");
        exit(1);
    }
    fread(header,sizeof(unsigned char),54,img);
    *height=*(int*)&header[22];
    *width=*(int*)&header[18];
    *bitdepth=*(int*)&header[28];
    if(*bitdepth<=8)
    {
        fread(colortable,sizeof(unsigned char),1024,img);
        /*for(i=0;i<height*width;i++)
        {
           pixelvalue[i]=getc(img);
        }*/
        fread(pixelvalue,sizeof(unsigned char),(*height)*(*width),img);
    }

    else
    {
        for(i=0;i<(*height)*(*width);i++)
        {
            colorpixel[0][i] = getc(img);
            colorpixel[1][i] = getc(img);
            colorpixel[2][i] = getc(img);
        }

    }




    printf("Image read\n");
    fclose(img);

}

void write(unsigned char header[],unsigned char colortable[],unsigned char pixelvalue[],unsigned char colorpixel[][MAXV],int height,int width,int bitdepth)
{
    int i;
    FILE *img;
    img=fopen("newImage.bmp","wb");
    fwrite(header,sizeof(unsigned char),54,img);
    if(bitdepth<=8)
    {
        fwrite(colortable,sizeof(unsigned char),1024,img);
        for(i=0;i<height*width;i++)
        {
             putc(pixelvalue[i],img);
        }

    }

    else
    {
        for(i=0;i<height*width;i++)
        {
             putc(colorpixel[0][i],img);
             putc(colorpixel[1][i],img);
             putc(colorpixel[2][i],img);
        }
    }
    fclose(img);

}

void pixval(unsigned char pixelvalue[],int width)
{
    int row,column,index;
    printf("Enter row and column to get details\n");
    scanf("%d%d",&row,&column);
    row=row-1;
    column=column-1;
    index=row*width+column;
    printf("Pixel value= %d\n",(int)(pixelvalue[index]));

}

void details(unsigned char pixelvalue[],int height,int width)
{
    int max=pixelvalue[0],i;
    for(i=0;i<(height*width);i++)
    {
        if(pixelvalue[i]>max)
        {
            max=pixelvalue[i];
        }
    }
    printf("Height= %d\nWidth= %d\n",height,width);
    printf("Max pixel value= %d\n",max);
}

void setval(unsigned char pixelvalue[],int width)
{
    int row,column,index,val;
    printf("Enter row and column to set pixel value\n");
    scanf("%d%d",&row,&column);
    printf("Enter pixel value to set\n");
    scanf("%d",&val);
    row=row-1;
    column=column-1;
    index=row*width+column;
    pixelvalue[index]=val;
}

void colortogray(unsigned char pixelvalue[][MAXV],int height,int width)
{
    int i,val;
    for(i=0;i<(height*width);i++)
    {
        val=0;
        val=(pixelvalue[2][i]*0.3)+(pixelvalue[1][i]*0.59)+(pixelvalue[0][i]*0.11);
        pixelvalue[0][i]=val;
        pixelvalue[1][i]=val;
        pixelvalue[2][i]=val;
    }
}


void meanGray(unsigned char pixelvalue[],int height,int width)
{
    int m=0,avg,i;
    for(i=0;i<(height*width);i++)
    {
        m+=pixelvalue[i];
    }
    avg=m/(height*width);
    printf("Average pixel value= %d\n",avg);
}


void grayToBinary(unsigned char pixelvalue[],int height,int width)
{
    int thresh=100,i;
    for(i=0;i<(height*width);i++)
    {
        if(pixelvalue[i]>thresh)
        {
            pixelvalue[i]=255;
        }
        else
        {
            pixelvalue[i]=0;
        }
    }
}

void reflectImage(unsigned char pixelvalue[],int height,int width)
{
    int i,j,index1,index2;
    char flag;
    printf("Enter h-horizontal reflection and v-vertical reflection\n");
    scanf(" %c",&flag);
    unsigned char arr[height*width];
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {
            if(flag=='h')
        {
            index1=i*width+j;
            index2=(height-1-i)*width+j;
            arr[index1]=pixelvalue[index2];
        }
        else if(flag=='v')
        {
            index1=i*width+j;
            index2=i*width+(width-1-j);
            arr[index1]=pixelvalue[index2];
        }
        }
     for(i=0;i<height*width;i++)
     {
         pixelvalue[i]=arr[i];
     }

}

void negative(unsigned char pixelvalue[],int height,int width)
{
    int i;
    for(i=0;i<height*width;i++)
    {

        pixelvalue[i]=255-pixelvalue[i];
    }
}

void brightnes(unsigned char pixelvalue[],int height,int width)
{
    int val,i,num;
    printf("Enter +ve value to increase brightness/-ve value to decrease brightness\n");
    scanf("%d",&val);
    for(i=0;i<height*width;i++)
    {
        num=pixelvalue[i]+val;
        if(num>255)
            num=255;
        if(num<0)
            num=0;
        pixelvalue[i]=num;
    }
}


void translateImage(unsigned char pixelvalue[],int height,int width)
{
    int x,y,i,j;
    int index1,index2;
    unsigned char arr[height*width];
    printf("Enter value to translate in x: ");
    scanf("%d",&x);
    printf("Enter value to translate in y: ");
    scanf("%d",&y);
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
    {
        index1=i*width+j;
        index2=(i-y)*width+(j-x);
        arr[index1]=pixelvalue[index2];

    }
    for(i=0;i<height*width;i++)
    {
        pixelvalue[i]=arr[i];
    }
}


void sepiafilter(unsigned char pixelvalue[][MAXV],int height,int width)
{
    int i,r,g,b;
    for(i=0;i<height*width;i++)
    {
        r=(pixelvalue[2][i]*0.393)+(pixelvalue[1][i]*0.769)+(pixelvalue[0][i]*0.189);
        g=(pixelvalue[2][i]*0.349)+(pixelvalue[1][i]*0.686)+(pixelvalue[0][i]*0.168);
        b=(pixelvalue[2][i]*0.272)+(pixelvalue[1][i]*0.534)+(pixelvalue[0][i]*0.131);
        if(r>255)
            r=255;
        if(g>255)
            g=255;
        if(b>255)
            b=255;
        pixelvalue[0][i]=b;
        pixelvalue[1][i]=g;
        pixelvalue[2][i]=r;
    }
}


int main()
{
    unsigned char header[SIZEH];
    unsigned char colortable[SIZEC];
    unsigned char pixelvalueg[MAXV];
    unsigned char pixelValuec[3][MAXV];
    int height,width,bitdepth;

    int num;
    while(1)
    {
        printf("Enter 1 to read image\nEnter 2 to write image\nEnter 3 to get image details\nEnter 4 to get image pixel value\n");
        printf("Enter 5 to set pixel value at location\nEnter 6 to convert color image to grayscale\nEnter 7 to get average pixel value\n");
        printf("Enter 8 to convert image to binary image\nEnter 9 to reflect image in horizontal- h  or vertical- v\n");
        printf("Enter 10 get negative of image\nEnter 11 to increase or decrease brightness of image\nEnter 12 to translate image in horizontal and vertical directions\n");
        printf("Enter 13 to apply sepia filter to image\nEnter 0 to exit\n");
        scanf("%d",&num);
        switch(num)
        {
        case 1:
            read(header,colortable,pixelvalueg,pixelValuec,&height,&width,&bitdepth);
            break;
        case 2:
            write(header,colortable,pixelvalueg,pixelValuec,height,width,bitdepth);
            break;
        case 3:
            details(pixelvalueg,height,width);
            break;
        case 4:
            pixval(pixelvalueg,width);
            break;
        case 5:
            setval(pixelvalueg,width);
            break;
        case 6:
            colortogray(pixelValuec,height,width);
            break;
        case 7:
            meanGray(pixelvalueg,height,width);
            break;

        case 8:
            grayToBinary(pixelvalueg,height,width);
            break;
        case 9:
            reflectImage(pixelvalueg,height,width);
            break;
        case 10:
            negative(pixelvalueg,height,width);
            break;
        case 11:
            brightnes(pixelvalueg,height,width);
            break;
        case 12:
            translateImage(pixelvalueg,height,width);
            break;
        case 13:
            sepiafilter(pixelValuec,height,width);
            break;
        default:
            exit(1);

        }
    }
    return 0;
}







