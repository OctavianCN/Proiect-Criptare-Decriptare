#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
    unsigned char R,G,B;
}pixel;
typedef struct
{
    int x,y,dimh,dimw,cifra;
    float corr;
}fereastra;
typedef struct
{
    int x,y;
}coordonate;
void xorshift32(unsigned int *k);
int incarcareimagine(char *bin);
void criptareimagine(char *bmpin,char *bmpout,char *key);
void decriptarare(char *bmpin,char *bmpout,char *key);
void chisquaretest(char *bin);
int extragereimagine(char *Imagine);
void templatematching(char *Imagine,char *Sablon,double ps,unsigned char RGB[3],fereastra **f,int *ok);
void adaugarebord(pixel ***Mat,fereastra f,unsigned char RGB[3]);
void TransformareimaginiinGrayscale();
void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie);
double testarecorelatie(pixel **Fi,pixel **Matsb,unsigned int hsb,unsigned int wsb);
void apelaretemplatematching();
void eliminareaNonM(char *Imagine,fereastra *f,int n,unsigned int w,unsigned int h);
int comparator(const void *a,const void *b);
double suprapunere(fereastra f1,fereastra f2);
int main()
{
   //criptareimagine("imagine_5x5.bmp","imagine_5x5ENC.bmp","secret_key.txt");
  //decriptarare("imagine_5x5ENC.bmp","imagine_5x5DEC.bmp","secret_key.txt");
    /*criptareimagine("peppers.bmp","peppersENC.bmp","secret_key.txt");
    decriptarare("peppersENC.bmp","peppersDEC.bmp","secret_key.txt");
   chisquaretest("peppersENC.bmp");
   printf("\n");
   chisquaretest("peppers.bmp");*/
   char NumeImagineCriptata[256];
   printf("Imagine pe care doresti sa o criptezi este = ");
   scanf("%s",&NumeImagineCriptata);
   criptareimagine(NumeImagineCriptata,"ImagineENC.bmp","secret_key.txt");
   printf("Imaginea criptata ti-a fost salvata cu numele ImagineENC.bmp \n");
    decriptarare("ImagineENC.bmp","ImagineDEC.bmp","secret_key.txt");
     printf("Imaginea decriptata ti-a fost salvata cu numele ImagineDEC.bmp \n");
    printf("Valorile testului chi ale imaginii criptate sunt : \n");
   chisquaretest("ImagineENC.bmp");
   printf("\nValorile testului chi ale imaginii initiale sunt :\n");
   chisquaretest(NumeImagineCriptata);
   printf("\n");
   TransformareimaginiinGrayscale();
    apelaretemplatematching();
    return 0;
}
void xorshift32(unsigned int *k)
{
    unsigned int x;
    x=*k;
    x=x^x<<13;
    x=x^x>>17;
    x=x^x<<5;
    *k=x;
}
int incarcareimagine(char *bin)
{
    FILE *f;
    unsigned int w,h,r,z,t;
    pixel *l,*laux;
    unsigned char k,rgb[3];
    int i=0,j=0;
    f=fopen(bin,"rb");
    if(f==NULL)
       {
        printf("Nu am gasit imaginea");
        return ;
       }
    fseek(f,18,SEEK_SET);
    fread(&w,sizeof(h),1,f);
    fread(&h,sizeof(w),1,f);
    l=(pixel*)malloc(sizeof(pixel)*w*h);
    laux=(pixel*)malloc(sizeof(pixel)*w*h);
    fseek(f,54,SEEK_SET);
    int padding;
    if(w % 4 != 0)
        padding = 4 - (3 * w) % 4;
    else
        padding = 0;
    for(t=0;t<h;t++)
    {
     for(j=0;j<w;j++)
       {

        fread(rgb,3,1,f);
        l[i].R=rgb[2];
        l[i].G=rgb[1];
        l[i].B=rgb[0];
        laux[i].R=rgb[2];
        laux[i].G=rgb[1];
        laux[i].B=rgb[0];
        //  printf("%u %u %u\n",l[i].R,l[i].G,l[i].B);
        i++;
       }
       fseek(f,padding,SEEK_CUR);

    }
    r=w;
    z=h;
    t=0;
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            l[t].R=laux[z*r-r+j].R;
            l[t].G=laux[z*r-r+j].G;
            l[t].B=laux[z*r-r+j].B;
            t++;
        }
        z--;
    }
        t=0;
    free(laux);
    fclose(f);
    return l;
}
void salvareimagine(char *bin,pixel *l)
{
    FILE *f;
    unsigned int w,h,k,r,z,t;
    pixel *laux;
    int i=0,j;
    f=fopen(bin,"rb+");
    if(f==NULL)
       {
        printf("Nu am gasit imaginea");
        return ;
       }
    t=0;
    fseek(f,18,SEEK_SET);
    fread(&w,sizeof(h),1,f);
    fread(&h,sizeof(w),1,f);
    i=0;
    int padding;
    if(w % 4 != 0)
        padding = 4 - (3 * w) % 4;
    else
        padding = 0;
    fseek(f,54,SEEK_SET);
    k=0;
    r=w;
    z=h;
    laux=(pixel*)malloc(sizeof(pixel)*w*h);
    for(i=0;i<h;i++)
    {
        for(t=0;t<w;t++)
        {
            laux[k].R=l[z*r-r+t].R;
            laux[k].G=l[z*r-r+t].G;
            laux[k].B=l[z*r-r+t].B;
            k++;
        }
        z--;
    }
    for(i=0;i<w*h;i++)
    {
        l[i].R=laux[i].R;
        l[i].G=laux[i].G;
        l[i].B=laux[i].B;
    }
    i=0;
    for(int t=0;t<h;t++)
       {
        for(j=0;j<w;j++)
        {
            fwrite(&l[i].B,sizeof(unsigned char),1,f);
            fwrite(&l[i].G,sizeof(unsigned char),1,f);
            fwrite(&l[i].R,sizeof(unsigned char),1,f);
             //printf("%u %u %u \n",l[i].R,l[i].G,l[i].B);
            i++;
        }
        fseek(f,padding,SEEK_CUR);
       }
    fclose(f);
}
void criptareimagine(char *bmpin,char *bmpout,char *key)
{
    unsigned int w,h,k,*R,dim_R,*perm,dim_perm,j,i,r,aux,t=1,z=0;
    pixel *l,*lpr,*lc,*laux;
    unsigned char c;
    FILE *fin,*fout,*cheie;
    fin=fopen(bmpin,"rb");
    fout=fopen(bmpout,"wb");
    cheie=fopen(key,"r");
    if((fin==NULL)||(fout==NULL)||(cheie==NULL))
    printf("Eroare la deschiderea fisierelor");
    l=incarcareimagine(bmpin);
    fseek(fin,18,SEEK_SET);
    fread(&w,sizeof(h),1,fin);
    fread(&h,sizeof(w),1,fin);
    fscanf(cheie,"%u",&k);
    R=(unsigned int*)malloc(sizeof(unsigned int)*2*w*h+1);
    perm=(unsigned int*)malloc(sizeof(unsigned int)*(w*h+1));
    for(i=0;i<2*w*h;i++)
    {
        xorshift32(&k);
        R[i]=k;
    }
    dim_R=2*w*h-1;
    for(i=0;i<w*h;i++)
        perm[i]=i;
    dim_perm=w*h;
    j=0;
    for(i=dim_perm-1;i>0;i--)
    {
        r=R[j]%(i+1);
        aux=perm[i];
        perm[i]=perm[r];
        perm[r]=aux;
        j++;
    }
    lpr=(pixel*)malloc(sizeof(pixel)*w*h+1);
    for(i=0;i<w*h;i++)
    {
        lpr[perm[i]].R=l[i].R;
        lpr[perm[i]].G=l[i].G;
        lpr[perm[i]].B=l[i].B;
    }
   rewind(fin);
   rewind(fout);
   while(fread(&c,sizeof(c),1,fin))
    {
        fwrite(&c,sizeof(c),1,fout);
        fflush(fout);
    }
    fflush(fin);
    t=0;
    salvareimagine(bmpout,lpr);
    fclose(fout);
    fout=fopen(bmpout,"rb+");
    lpr=incarcareimagine(bmpout);
    lc=(pixel*)malloc(sizeof(pixel)*w*h);
    fscanf(cheie,"%u",&k);
    i=0;
    lc[i].B=(k&255)^(R[w*h]&255)^lpr[i].B;
    lc[i].G=((k>>8)&255)^((R[w*h]>>8)&255)^lpr[i].G;
    lc[i].R=((k>>16)&255)^((R[w*h]>>16)&255)^lpr[i].R;
    //printf("%u %u %u\n",k,R[w*h],lpr[i].R);
  //  printf("%u %u %u\n",R[w*h],R[w*h],R[w*h]);
   //printf("%u = %u %u %u\n",lc[i].R,k,R[w*h],lpr[i].R);
    for(i=1;i<w*h;i++)
    {
         lc[i].B=lc[i-1].B^(R[w*h+i]&255)^lpr[i].B;
         lc[i].G=lc[i-1].G^((R[w*h+i]>>8)&255)^lpr[i].G;
         lc[i].R=lc[i-1].R^((R[w*h+i]>>16)&255)^lpr[i].R;
         //printf("%u = %u %u %u\n",lc[i].R,lpr[i].R,R[w*h+i],lc[i-1].R);
    }
    rewind(fin);
    rewind(fout);
    while(fread(&c,sizeof(c),1,fin))
    {
        fwrite(&c,sizeof(c),1,fout);
        fflush(fout);
    }
    fflush(fin);
   salvareimagine(bmpout,lc);
   free(laux);
   free(l);
   free(lpr);
   free(R);
   free(perm);
   fclose(fin);
   fclose(fout);
   fclose(cheie);
}
void decriptarare(char *bmpin,char *bmpout,char *key)
{
    unsigned int w,h,k,*R,dim_R,*perm,dim_perm,j,i,r,aux,t=1,z=0,*perminv;
    pixel *l,*lcpr,*lc,*laux;
    unsigned char c;
    FILE *fin,*fout,*cheie;
    fin=fopen(bmpin,"rb");
    fout=fopen(bmpout,"wb+");
    cheie=fopen(key,"r");
    if((fin==NULL)||(fout==NULL)||(cheie==NULL))
    printf("Eroare la deschiderea fisierelor");
    lc=incarcareimagine(bmpin);
    fseek(fin,18,SEEK_SET);
    fread(&w,sizeof(h),1,fin);
    fread(&h,sizeof(w),1,fin);
    fscanf(cheie,"%u",&k);
    R=(unsigned int*)malloc(sizeof(unsigned int)*2*w*h+1);
    perm=(unsigned int*)malloc(sizeof(unsigned int)*(w*h+1));
    for(i=0;i<2*w*h;i++)
    {
        xorshift32(&k);
        R[i]=k;

    }

    dim_R=2*w*h-1;
    for(i=0;i<w*h;i++)
        perm[i]=i;
    dim_perm=w*h;
    j=0;
    perminv=(unsigned int*)malloc(sizeof(unsigned int)*(w*h+1));
    for(i=dim_perm-1;i>0;i--)
    {
        r=R[j]%(i+1);
        aux=perm[i];
        perm[i]=perm[r];
        perm[r]=aux;
        j++;
    }
    for(i=0;i<w*h;i++)
        perminv[perm[i]]=i;
    lcpr=(pixel*)malloc(sizeof(pixel)*(w*h+1));
    l=(pixel*)malloc(sizeof(pixel)*(w*h+1));
    fscanf(cheie,"%u",&k);
    i=0;
    for(i=0;i<w*h;i++)
    {
        lcpr[i].R=lc[i].R;
        lcpr[i].G=lc[i].G;
        lcpr[i].B=lc[i].B;
     //   printf("%u\n",lcpr[i].R);
    }
     i=0;
    lcpr[i].B=(k&255)^(R[w*h]&255)^lc[i].B;
    lcpr[i].G=((k>>8)&255)^((R[w*h]>>8)&255)^lc[i].G;
    lcpr[i].R=((k>>16)&255)^(((R[w*h]>>8)>>8)&255)^lc[i].R;
   // printf("%u = %u %u %u\n",lcpr[i].R,k,R[w*h],lc[i].R);
    t=w*h-1;
    for(i=1;i<w*h;i++)
   {
       lcpr[i].B=lc[i-1].B^(R[w*h+i]&255)^lc[i].B;
       lcpr[i].G=lc[i-1].G^((R[w*h+i]>>8)&255)^lc[i].G;
       lcpr[i].R=lc[i-1].R^((R[w*h+i]>>16)&255)^lc[i].R;
      // printf("%u = %u %u %u\n",lcpr[i].R,lc[i-1].R,R[w*h+i],lc[i].R);
       t--;
   }


  /*t=0;
   printf("\n");
   for(i=0;i<h;i++)
   {
       for(j=0;j<w;j++)
        {
            printf("%u %u %u ||",lcpr[t].R,lcpr[t].G,lcpr[t].B);
            t++;
        }
       printf("\n");
   }*/

    for(i=0;i<w*h;i++)
        {
            l[perminv[i]].R=lcpr[i].R;
            l[perminv[i]].G=lcpr[i].G;
            l[perminv[i]].B=lcpr[i].B;

        }
    rewind(fout);
    rewind(fin);
    while(fread(&c,sizeof(c),1,fin))
    fwrite(&c,sizeof(c),1,fout);
   salvareimagine(bmpout,l);
   free(l);
   free(lc);
   free(lcpr);
   free(R);
   free(perm);
   free(perminv);
   fclose(fin);
   fclose(fout);
   fclose(cheie);

}
void chisquaretest(char *bin)
{
    FILE *f;
    unsigned char c,*RGB;
    unsigned int h,w,*l;
    float fbar,chi;
    int i,x,*frR,*frG,*frB;
    f=fopen(bin,"rb");
    if(f==NULL)
        printf("Nu am gasit imaginea");
   l=incarcareimagine(bin);
   fseek(f,18,SEEK_SET);
   fread(&h,sizeof(h),1,f);
   fread(&w,sizeof(w),1,f);
   fseek(f,54,SEEK_SET);
   frR=(int*)calloc(sizeof(int),256);
   frG=(int*)calloc(sizeof(int),256);
   frB=(int*)calloc(sizeof(int),256);
   RGB=(unsigned char*)calloc(sizeof(unsigned char),3);
    while(fread(&c,sizeof(unsigned char),1,f))
       {

         frB[c]++;
         fread(&c,sizeof(unsigned char),1,f);
         frG[c]++;
         fread(&c,sizeof(unsigned char),1,f);
         frR[c]++;
       }
    fbar=w*h/256;
    x=2;
    chi=0;
    for(i=0;i<=255;i++)
      chi=chi+((frR[i]-fbar)*(frR[i]-fbar))/fbar;
     printf("%f ",chi);
    chi=0;
     for(i=0;i<=255;i++)
      chi=chi+((frG[i]-fbar)*(frG[i]-fbar))/fbar;
     printf("%f ",chi);
    chi=0;
    for(i=0;i<=255;i++)
      chi=chi+((frB[i]-fbar)*(frB[i]-fbar))/fbar;
     printf("%f ",chi);
    free(frR);
    free(frG);
    free(frB);
    free(RGB);
    fclose(f);

}
void apelaretemplatematching()
{
    float ps=0.5;
    unsigned char RGB[3];
    unsigned char c;
    fereastra *f;
    unsigned int w,h;
    int ok=0,u;
    FILE *f1,*f2;
    f1=fopen("test.bmp","rb");
    f2=fopen("test1.bmp","wb");
    if(f1==NULL)
        printf("Nu am putut deschide imaginea");
        fseek(f1,18,SEEK_SET);
        fread(&w,sizeof(unsigned int),1,f1);
        fread(&h,sizeof(unsigned int),1,f1);
        rewind(f1);
    while(fread(&c,1,1,f1))
        fwrite(&c,1,1,f2);
        fclose(f1);
    fclose(f2);
    f=(fereastra*)malloc(w*h*sizeof(fereastra));
    RGB[0]=255;
    RGB[1]=0;
    RGB[2]=0;
    templatematching("test1.bmp","cifra0_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=0;i<=ok;i++)
        f[i].cifra=0;
    u=ok;
    RGB[0]=255;
    RGB[1]=255;
    RGB[2]=0;
    templatematching("test1.bmp","cifra1_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=1;
    u=ok;
    RGB[0]=0;
    RGB[1]=255;
    RGB[2]=0;
    templatematching("test1.bmp","cifra2_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=2;
    u=ok;
    RGB[0]=0;
    RGB[1]=255;
    RGB[2]=255;
    templatematching("test1.bmp","cifra3_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=3;
    u=ok;
    RGB[0]=255;
    RGB[1]=0;
    RGB[2]=255;
    templatematching("test1.bmp","cifra4_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=4;
    u=ok;
    RGB[0]=0;
    RGB[1]=0;
    RGB[2]=255;
    templatematching("test1.bmp","cifra5_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=5;
    u=ok;
    RGB[0]=192;
    RGB[1]=192;
    RGB[2]=192;
    templatematching("test1.bmp","cifra6_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=6;
    u=ok;
    RGB[0]=255;
    RGB[1]=140;
    RGB[2]=0;
    templatematching("test1.bmp","cifra7_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=7;
    u=ok;
    RGB[0]=128;
    RGB[1]=0;
    RGB[2]=128;
    templatematching("test1.bmp","cifra8_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=8;
    u=ok;
    RGB[0]=128;
    RGB[1]=0;
    RGB[2]=0;
    templatematching("test1.bmp","cifra9_grayscale.bmp",ps,RGB,&f,&ok);
    for(int i=u+1;i<=ok;i++)
        f[i].cifra=9;
    u=ok;
   qsort(f,ok,sizeof(fereastra),comparator);
    eliminareaNonM("test.bmp",f,ok,w,h);
    free(f);
}
void eliminareaNonM(char *Imagine,fereastra *f,int n,unsigned int w,unsigned int h)
{
    int i,j,nr=0;
    double sup,ps=0.2;
    pixel **Mat=extragereimagine(Imagine);
    unsigned char RGB[3];
    RGB[0]=255;
    RGB[1]=0;
    RGB[2]=0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if((f[i].x!=-1)&&(f[j].x!=-1))
            {
            sup=suprapunere(f[i],f[j]);
            if(sup>ps)
            {
            f[j].x=-1;
            }

            }
        }
    }
    for(i=0;i<n;i++)
    {
        if(f[i].x!=-1)
        {
           if(f[i].cifra==0)
            {
                RGB[0]=255;
                RGB[1]=0;
                RGB[2]=0;
                adaugarebord(&Mat,f[i],RGB);
            }

            if(f[i].cifra==1)
            {
                RGB[0]=255;
                RGB[1]=255;
                RGB[2]=0;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==2)
            {
                RGB[0]=0;
                RGB[1]=255;
                 RGB[2]=0;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==3)
            {
                RGB[0]=0;
                RGB[1]=255;
                RGB[2]=255;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==4)
            {
               RGB[0]=255;
                RGB[1]=0;
                RGB[2]=255;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==5)
            {
                RGB[0]=0;
                RGB[1]=0;
                 RGB[2]=255;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==6)
            {
                RGB[0]=192;
                RGB[1]=192;
                RGB[2]=192;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==7)
            {
                RGB[0]=255;
                RGB[1]=140;
                RGB[2]=0;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==8)
            {
                RGB[0]=128;
                RGB[1]=0;
                RGB[2]=128;
                adaugarebord(&Mat,f[i],RGB);
            }
              if(f[i].cifra==9)
            {
                RGB[0]=128;
                RGB[1]=0;
                RGB[2]=0;
                adaugarebord(&Mat,f[i],RGB);
            }


        }
    }
    FILE *f1,*f2;
    f1=fopen(Imagine,"rb");
    f2=fopen("testcorect.bmp","wb");
    unsigned char c;
    rewind(f1);
    rewind(f2);
    while(fread(&c,1,1,f1))
        fwrite(&c,1,1,f2);
    int padding;
    if(w % 4 != 0)
        padding = 4 - (3 * w) % 4;
    else
        padding = 0;
    fseek(f2,54,SEEK_SET);
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            fwrite(&Mat[i][j].B,sizeof(Mat[i][j].B),1,f2);
            fwrite(&Mat[i][j].G,sizeof(Mat[i][j].G),1,f2);
            fwrite(&Mat[i][j].R,sizeof(Mat[i][j].R),1,f2);

        }
        fseek(f2,padding,SEEK_CUR);
    }
   // fclose(f1);
    //fclose(f2);
    for(i=0;i<w+h+100;i++)
        free(Mat[i]);
    free(Mat);
}
double suprapunere(fereastra f1,fereastra f2)
{
    double sup;
    coordonate f2js,f2sd,f2jd,f2ss,lsj,lss,lds,ldj;
    int i,j,m,n,l,k,ok1=0,ok2=0,ok3=0,u,L;
    float arie;
    if((f1.x==f2.x)&&(f1.y==f2.y))
        return 0;
    if((abs(f1.x-f2.x)>=f1.dimh)||(abs(f1.y-f2.y)>=f1.dimw))
        return 0;
    f2ss.x=f2.x;
    f2ss.y=f2.y;
    f2js.x=f2.x-f2.dimh;
    f2js.y=f2.y;
    f2jd.x=f2.x-f2.dimh;
    f2jd.y=f2.y+f2.dimw;
    f2sd.x=f2.x;
    f2sd.y=f2.y+f2.dimw;
    if((abs(f2jd.x-f1.x)>=2*f2.dimh)||(abs(f2jd.y-f1.y)>=2*f2.dimw))
    return 0;
    l=f2.dimw-abs(f2.x-f1.x);
    L=f2.dimh-abs(f2.y-f1.y);
    arie=l*L;
    sup=(arie)/(f1.dimh*f1.dimw+f2.dimh*f2.dimw-arie);
    return sup;


}
int comparator(const void *a,const void *b)
{
    fereastra *a1=(fereastra *)a;
    fereastra *a2=(fereastra *)b;
    if(a1->corr>a2->corr)
        return -1;
    if(a2->corr>a1->corr)
        return 1;
    return 0;
}
int extragereimagine(char *Imagine)
{

    unsigned int w,h;
    unsigned char RGB[3];
    int i,j,ind,jnd;
    FILE *Img;
    Img=fopen(Imagine,"rb");
    if(Img==NULL)
      {
        printf("Nu am putut deschide imaginea");
        return ;
      }

    fseek(Img,18,SEEK_SET);
    fread(&w,sizeof(unsigned int),1,Img);
    fread(&h,sizeof(unsigned int),1,Img);
    pixel **Mat=(pixel **)malloc(w*h*sizeof(pixel *));
    for(i=0;i<w+h+100;i++)
        Mat[i]=(pixel *)malloc(sizeof(pixel)*h*w);

    fseek(Img,54,SEEK_SET);
    int padding;
    if(w % 4 != 0)
        padding = 4 - (3 * w) % 4;
    else
        padding = 0;
    ind=0;
    jnd=0;
      int nr=0;
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {

            fread(RGB,3,1,Img);
            Mat[i][j].R=RGB[2];
            Mat[i][j].G=RGB[1];
            Mat[i][j].B=RGB[0];

        }

       fseek(Img,padding,SEEK_CUR);
    }
    fclose(Img);
    return Mat;
}
void templatematching(char *Imagine,char *Sablon,double ps,unsigned char RGB[3],fereastra **f,int *ok)
{
    FILE *Img,*Sb;
    pixel **Matimg,**Matsb,**Matimg2;
    double corr;
    unsigned int w,h,wsb,hsb,n;
   int i,j,k,l,x,y,u=ok;
    Matsb=extragereimagine(Sablon);
  Matimg=extragereimagine("test_grayscale.bmp");
  Matimg2=extragereimagine(Imagine);
    Img=fopen(Imagine,"rb+");
    Sb=fopen(Sablon,"rb");
    if((Img==NULL)||(Sb==NULL))
        printf("Nu am putut deschide imaginea");
    fseek(Img,18,SEEK_SET);
    fseek(Sb,18,SEEK_SET);
    fread(&w,sizeof(unsigned int),1,Img);
    fread(&h,sizeof(unsigned int),1,Img);
    fread(&wsb,sizeof(unsigned int),1,Sb);
    fread(&hsb,sizeof(unsigned int),1,Sb);
    //alocam dinamic fereastra
    pixel **Fi=(pixel **)malloc(wsb*hsb*sizeof(pixel *));
    for(i=0;i<wsb+hsb+100;i++)
        Fi[i]=(pixel *)malloc(sizeof(pixel)*hsb*wsb);
    n=wsb*hsb;
   for(i=0;i<h-hsb;i++)
   {
       for(j=0;j<w-wsb;j++)
       {
           x=i;
           y=j;
           for(k=0;k<hsb;k++)
           {
               for(l=0;l<wsb;l++)
               {
                  Fi[k][l].R=Matimg[k+i][l+j].R;
                  Fi[k][l].G=Matimg[k+i][l+j].G;
                  Fi[k][l].B=Matimg[k+i][l+j].B;
               }
           }

           corr=testarecorelatie(Fi,Matsb,hsb,wsb);
           if(corr>ps)
           {
               (*f)[(*ok)].dimh=hsb;
               (*f)[(*ok)].dimw=wsb;
               (*f)[(*ok)].corr=corr;
               (*f)[(*ok)].x=x;
               (*f)[(*ok)].y=y;
              adaugarebord(&Matimg2,(*f)[(*ok)],RGB);
              (*ok)++;
           }
       }
   }

   (*ok)=(*ok)-1;
   int padding;
    if(w % 4 != 0)
        padding = 4 - (3 * w) % 4;
    else
        padding = 0;
    fseek(Img,54,SEEK_SET);
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            fwrite(&Matimg2[i][j].B,sizeof(Matimg2[i][j].B),1,Img);
            fwrite(&Matimg2[i][j].G,sizeof(Matimg2[i][j].G),1,Img);
            fwrite(&Matimg2[i][j].R,sizeof(Matimg2[i][j].R),1,Img);

        }
        fseek(Img,padding,SEEK_CUR);
    }
  //Dezalocari si inchideri de fisiere
    for(i=0;i<wsb+hsb+100;i++)
        free(Fi[i]);
    free(Fi);
    for(i=0;i<wsb+hsb+100;i++)
        free(Matsb[i]);
    free(Matsb);
    for(i=0;i<w+h+100;i++)
        free(Matimg[i]);
    free(Matimg);
    for(i=0;i<w+h+100;i++)
        free(Matimg2[i]);
    free(Matimg2);
    fclose(Img);
    fclose(Sb);
}
double testarecorelatie(pixel **Fi,pixel **Matsb,unsigned int hsb,unsigned int wsb)
{
    int i,j;
    unsigned int n;
    double devstS,devstFi,sbar=0,fibar=0,rap,corr=0;
    n=hsb*wsb;
    for(i=0;i<hsb;i++)
    {
        for(j=0;j<wsb;j++)
        {
            sbar=sbar+Matsb[i][j].R;
            fibar=fibar+Fi[i][j].R;
        }
    }
    sbar=sbar/n;
    fibar=fibar/n;
    for(i=0;i<hsb;i++)
    {
        for(j=0;j<wsb;j++)
        {
            devstFi=devstFi+(Fi[i][j].R-fibar)*(Fi[i][j].R-fibar);
            devstS=devstS+(Matsb[i][j].R-sbar)*(Matsb[i][j].R-sbar);
        }
    }
    devstFi=devstFi/(n-1);
    devstS=devstS/(n-1);
    devstFi=sqrt(devstFi);
    devstS=sqrt(devstS);
    rap=1/(devstFi*devstS);
    for(i=0;i<hsb;i++)
    {
        for(j=0;j<wsb;j++)
        {
            corr=corr+rap*(Fi[i][j].R-fibar)*(Matsb[i][j].R-sbar);
        }
    }
    corr=corr/n;
    return corr;
}
void adaugarebord(pixel ***Mat,fereastra f,unsigned char RGB[3])
{

    unsigned int dim_borderh=f.dimh,dim_borderw=f.dimw;
    int i,j;
    j=f.y;
    for(i=f.x;i<f.x+dim_borderh;i++)
    {

            (*Mat)[i][j].R=RGB[0];
            (*Mat)[i][j].G=RGB[1];
            (*Mat)[i][j].B=RGB[2];

    }

    i=f.x;
    for(j=f.y;j<f.y+dim_borderw;j++)
    {


           (*Mat)[i][j].R=RGB[0];
            (*Mat)[i][j].G=RGB[1];
            (*Mat)[i][j].B=RGB[2];

    }
    j=f.y+dim_borderw;
    for(i=f.x;i<f.x+dim_borderh;i++)
    {
             (*Mat)[i][j].R=RGB[0];
            (*Mat)[i][j].G=RGB[1];
            (*Mat)[i][j].B=RGB[2];
    }
   i=f.x+dim_borderh;
   for(j=f.y;j<f.y+dim_borderw;j++)
   {
            (*Mat)[i][j].R=RGB[0];
            (*Mat)[i][j].G=RGB[1];
            (*Mat)[i][j].B=RGB[2];
   }
}
void TransformareimaginiinGrayscale()
{
    grayscale_image("test.bmp", "test_grayscale.bmp");
    grayscale_image("cifra0.bmp", "cifra0_grayscale.bmp");
	grayscale_image("cifra1.bmp", "cifra1_grayscale.bmp");
	grayscale_image("cifra2.bmp", "cifra2_grayscale.bmp");
	grayscale_image("cifra3.bmp", "cifra3_grayscale.bmp");
	grayscale_image("cifra4.bmp", "cifra4_grayscale.bmp");
	grayscale_image("cifra5.bmp", "cifra5_grayscale.bmp");
	grayscale_image("cifra6.bmp", "cifra6_grayscale.bmp");
	grayscale_image("cifra7.bmp", "cifra7_grayscale.bmp");
	grayscale_image("cifra8.bmp", "cifra8_grayscale.bmp");
	grayscale_image("cifra9.bmp", "cifra9_grayscale.bmp");
}
void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{
   FILE *fin, *fout;
   unsigned int dim_img, latime_img, inaltime_img;
   unsigned char pRGB[3], header[54], aux;

   printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

   fin = fopen(nume_fisier_sursa, "rb");
   if(fin == NULL)
   	{
   		printf("nu am gasit imaginea sursa din care citesc");
   		return;
   	}

   fout = fopen(nume_fisier_destinatie, "wb+");

   fseek(fin, 2, SEEK_SET);
   fread(&dim_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

   fseek(fin, 18, SEEK_SET);
   fread(&latime_img, sizeof(unsigned int), 1, fin);
   fread(&inaltime_img, sizeof(unsigned int), 1, fin);
  printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);

   //copiaza octet cu octet imaginea initiala in cea noua
	fseek(fin,0,SEEK_SET);
	unsigned char c;
	while(fread(&c,1,1,fin)==1)
	{

		fwrite(&c,1,1,fout);
		fflush(fout);
	}
	fclose(fin);

	//calculam padding-ul pentru o linie
	int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;

   printf("padding = %d \n",padding);

	fseek(fout, 54, SEEK_SET);
	int i,j;
	for(i = 0; i < inaltime_img; i++)
	{
		for(j = 0; j < latime_img; j++)
		{
			//citesc culorile pixelului
			fread(pRGB, 3, 1, fout);
			//fac conversia in pixel gri
			aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
			pRGB[0] = pRGB[1] = pRGB[2] = aux;
        	fseek(fout, -3, SEEK_CUR);
        	fwrite(pRGB, 3, 1, fout);
        	fflush(fout);
		}
		fseek(fout,padding,SEEK_CUR);
	}
	fclose(fout);
}
