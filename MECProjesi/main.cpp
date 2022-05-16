#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <graphics.h>

/*Iliskili degerleri tek isim altinda toplamak icin cember yapisini olusturdum. 
Oluþturduðum yapýnýn bir deðiþken türü olarak tanýmlanýp yapýnýn çoðaltýlmasý ve yapiya 
yeni deðiþken tanýmlarken struct ifadesini tekrardan kullanmamak icin typedef kullanimi yaptim.
*/

typedef struct{ 
  double x,y;
  double r;
} cember;

/*3 farkli sinir noktasi olan cemberin merkezini ve yaricapini bulmak için kullandigimiz denklemleri ve formulleri icerir.
 matematiksel kullanimlarin formulleri icin http://www.ambrsoft.com/trigocalc/circle3d.htm 
 ve wikipedia: Circumscribed circle adreslerine bakabilirsiniz.
*/

cember uc_Nokta_Hesaplama(double x1, double y1, double x2, double y2, double x3, double y3) {
	//Dairenin denklemi olan x² + y² + 2GX + 2fy + C = 0 ve
	//yaricap demklemi olan (x - h) ² + (y -k) ² = r² formullerinden merkez (h, k) ve yaricap (r) bulunmasi amaclanmistir.
	cember olusacakCember;
	//2f = ((x1^2–x3^2)(x1–x2)+(y1^2–y3^2)(x1–x2)+(x2^2–x1^2)(x1–x3)+(y2^2–y1^2)(x1–x3))/(y3–y1)(x1–x2)–(y2–y1)(x1–x3)
    double f = ((pow(x1, 2) - pow(x3, 2)) * (x1 - x2)
             + (pow(y1, 2) - pow(y3, 2)) * (x1 - x2)
             + (pow(x2, 2) - pow(x1, 2)) * (x1 - x3)
             + (pow(y2, 2) - pow(y1, 2)) * (x1 - x3))
            / (2 * ((y3 - y1) * (x1 - x2) - (y2 - y1) * (x1 - x3)));
    //2g=((x1^2–x3^2).(y1–x2)+(y1^2–y3^2).(y1–y2)+(x2^2–x1^2).(y1-y3)+(y2^2–y1^2).(y1–y3))/(y3-y1).(y1–y2)-(x2–x1).(y1–y3)
    double g = ((pow(x1, 2) - pow(x3, 2)) * (y1 - y2)
             + (pow(y1, 2) - pow(y3, 2)) * (y1 - y2)
             + (pow(x2, 2) - pow(x1, 2)) * (y1 - y3)
             + (pow(y2, 2) - pow(y1, 2)) * (y1 - y3))
            / (2*((x3 - x1)*(y1 - y2)-(x2 - x1)*(y1 - y3)));
	//bulunan g ve f degerlerini yerine koydugumuzda c degerini buluruz. (bkz:x²+y²+2gx+2fy+C=0)
	
    double c=-pow(x1, 2)-pow(y1, 2)-2*g*x1-2*f*y1;
    // Daire denklemi x²+y²+2gx+2fy+C=0
    // Merkez (h=-g,k=-f) ve yaricap r^2= h^2+k^2-c
    double h=-g;
    double k=-f;
    double r_kare =h*h+k*k-c;

    double r=sqrt(r_kare);

    olusacakCember.x = h;
    olusacakCember.y = k;
    olusacakCember.r = r;
    return olusacakCember;
}

//Bu kod satýrý koordinatlar arasý mesafe hesaplama formuluyle mesafe hesaplar.
double mesafeHesapla(double x1, double y1, double x2, double y2){ 
  double mesafe;
  mesafe=sqrt(pow(x1-x2,2)+pow(y1-y2,2)); 
  return mesafe;
}

cember Minimum_Cevreleyen_Cember_diskoordinat2(double koordinat[][2], int n, int disnokta1, int disnokta2){	
  cember c;
  float icindeMi;
  //dista kalan yeni nokta ve diger dis nokta sinir kabul edilen yeni cember cizilir.  
  c.x = (koordinat[disnokta1][0] + koordinat[disnokta2][0]/2);
  c.y = (koordinat[disnokta1][1] + koordinat[disnokta2][1]/2);
  c.r = (mesafeHesapla(koordinat[disnokta1][0], koordinat[disnokta1][1], koordinat[disnokta2][0], koordinat[disnokta2][1])/2);

//yeni cembere gore eski noktalara bakilir, dista nokta var mi yok mu kontrol edilir. Eger yaricaptan buyuk olan (dista kalan) nokta varsa
//bizim yeni dis noktamizla birlikte toplam 3 farkli sinir noktamiz var demektir.   
  for(int i=0; i<n; i++){
  	
    icindeMi = pow(koordinat[i][0]-c.x,2)+pow(koordinat[i][1]-c.y,2)-pow(c.r,2);
    
	//3 sinir noktasi icin yapilacak merkez ve yaricap olcumleri farkli oldugundan yaricap ve merkez islemleri icin fonksiyon cagrilir.  
    if(icindeMi > 0) {
      c = uc_Nokta_Hesaplama(koordinat[disnokta1][0], koordinat[disnokta1][1], koordinat[disnokta2][0], koordinat[disnokta2][1], koordinat[i][0], koordinat[i][1]);
    }
  }

  return c;
}


cember Minimum_Cevreleyen_Cember_diskoordinat1(double koordinat[][2], int disnokta1, int n){
  cember c;
  int i, j;
  double icindeMi;

//cember disarida kalan noktaya gore duzenlenir
  c.x = (koordinat[disnokta1][0]+koordinat[0][0])/2;
  c.y = (koordinat[disnokta1][1]+koordinat[0][1])/2;
  c.r = (mesafeHesapla(koordinat[disnokta1][0], koordinat[disnokta1][1], koordinat[0][0], koordinat[0][1])/2);

//öncesinde iceride olan tum noktalar yeni cemberin de icinde mi kontrol edilir. 
//yeni cemberin disinda kalan nokta varsa ayni islemler tekrarlanir ve cember duzenlenmeye gonderilir 
  for(j=1; j<disnokta1; j++){
    
    icindeMi = pow(koordinat[j][0]-c.x,2)+pow(koordinat[j][1]-c.y,2)-pow(c.r,2);
    if(icindeMi>0){
    	
      c=Minimum_Cevreleyen_Cember_diskoordinat2(koordinat,j,j,disnokta1);
    }
  }

  return c;

}
//bu kod girilen koordinatlardan bir cember olusturmayi ve cemberin 
//disinda kalacak her noktaya göre cemberi yeniden sekillendirmeyi amaclar.
cember Minimum_Cevreleyen_Cember(double koordinat[][2], int n){
  cember c;
  float x;
  double icindeMi;
  //iki noktaya gore orta nokta ve yaricap bulunur
  c.x = (koordinat[1][0]+koordinat[0][0])/2; 
  c.y = (koordinat[1][1]+koordinat[0][1])/2;
  c.r = (mesafeHesapla(koordinat[0][0], koordinat[0][1], koordinat[1][0], koordinat[1][1])/2);

  for(int i=2; i<n; i++){
  	//nokta olusan cemberin icinde mi yoksa disinda mi bulunur
  	//icindemi=0 ise uzerinde, icindemi<0 ise icinde, icindemi>0 ise disindadir ve cember duzenlenmeye gonderilir. 
  	//Noktalar iceride veya sinirda kalacak sekilde bu islem devam eder.
    icindeMi = pow(koordinat[i][0]-c.x,2)+pow(koordinat[i][1]-c.y,2)-pow(c.r, 2);
    if(icindeMi>0){
      c=Minimum_Cevreleyen_Cember_diskoordinat1(koordinat,i,n);
    }
  }

  return c;
}
/*//B SPLÝNE EGRÝSÝNÝN MATEMATÝK KODUNU COZEMEDÝGÝM ÝCÝN SONUCU VERMIYOR
double b_Spline_Hesapla(double kontrol[][2],int n){
		
	double x[n],f[n];
	for (int i=0;i<n;i++){
	x[i]	=kontrol[i][0];//x degerlerimizi cekiyoruz
	f[i]	= kontrol[i][1];//f(x) yani y degerimizi cekiyoruz
	}
	
	 
	double u;
	double a[n],b[n],c[n],h[n],su[n];
	c[0]=0;//matematiksel formuldeki turev adimi* 
	for(int i=0;i<n;i++){
	    a[i]=f[i];//a degerleri y degerlerine esittir.
		h[i]=x[i+1]-x[i];
		b[i+1]=b[i]+2*c[i]*h[i];
		for (u=x[i];u<x[i+1];u+=0.05)
		{
				su[i]=a[i]+b[i]*(u-x[i])+c[i]*(u-x[i])*(u-x[i]);
					if(f[i]<su[i]<f[i+1]){
			
	           putpixel(u,su[i],RED);
			   		}
		}}
	}*/
//Bu fonksiyon dosyadan okunan n tane koordinatýn sayýsýný(kac deger girildigini) hesaplar.
int satirSayisiHesaplama(FILE *f){
	int i=0,k=0;
	char ch;
	f=fopen("koordinat.txt","r");
	while(feof(f)==NULL){ // dosyayi bastan sona okuyoruz
   		ch=getc(f);
    	if(ch=='\n'){ //her bosluk bitiste k degiskeni artiriliyor
       		k++;
		}
   }
	printf("%d koordinat girildi, isleniyor...\n ",k+1);
	return k+1;
}

int main() {

//Dosyadan deger okuma kodu
  FILE *f; // f isminde dosya tanýmlandý
  f=fopen("koordinat.txt","r"); // koordinat.txt dosyasý READING modunda açýldý
  int n;
  int i,j;
  cember c; // structtan c isminde bi yapý türetildi
 
  //kac deger girildigini bulan fonksiyonu cagýrýyoruz.
 n=satirSayisiHesaplama(f); 

  double koordinat[n+1][2]; //x ve y koordinatlari

  for(i=0;i<n;i++){
    fscanf(f, "%lf %lf", &koordinat[i][0], &koordinat[i][1]); //dosyamýzdaki koordinatlarý diziye dolduruyoruz
  }
  double splinekoordinat[n+1][2]; 
   for(i=0;i<n;i++){
     splinekoordinat[i][0]=koordinat[i][0];
     splinekoordinat[i][1]=koordinat[i][1];
  }
  // b_Spline_Hesapla(splinekoordinat,n);
   
   
  c=Minimum_Cevreleyen_Cember(koordinat,n); // MEC Hesaplamasý
  printf("Merkez= {%.2lf, %.2lf}\n Yaricap= %.4lf\n", c.x, c.y, c.r); 
  
    initwindow(1500, 700, "Grafik"); //1500x700 Pencere aciliyor.
 		setcolor(1); // renk 1 e ayarlandý
        line(10,350,690,350);//x icin
        line(350,10,350,690);//y icin
        //koordinat duzlemi icin sayi cizgileri eklenir.
        setcolor(7);
        
		 // KOORDINAT DUZLEMI CIZIMI_BASLANGIC
		for (int i=10;i<=690;i+=17){//y ekseninin sayi kesiklerini atmak için
        	int deger=(350-i)/17;//sayýnýn kac olacagini belirlemek için
        	char sayi[3];//deger bir sayidir ve bunu ekrana bastirabilmek icin
			sprintf(sayi,"%d",deger);//chara çevirmeliyiz.
			settextstyle(2,HORIZ_DIR,5);//yazý boyutu ve fontu ayarlýyoruz
			if(deger!=0){//0 yazmamasi icin
			outtextxy(354,i-7,sayi);//konum ve yazacagimiz sey
			line(348,i,352,i);//sayi kesigi
			
		}}
		 for (int j=10;j<=690;j+=17){//x ekseni sayi kesigi
        	int deger=-(350-j)/17;
        	char sayi[3];
			sprintf(sayi,"%d",deger);//int--> char donusumu
			settextstyle(2,HORIZ_DIR,5);
			if (deger!=0){
			outtextxy(j,358,sayi);
			line(j,348,j,352);		
		}}
		
		 // KOORDINAT DUZLEMI CIZIMI_SON
		 
		 
		setcolor(2);
		for(i=0;i<n;i++){ //koordinatlarý tek tek ekrana basýyoruz
			fillellipse(koordinat[i][0]*17+350,-koordinat[i][1]*17+350,2,2);//17, kesikler arasý mesafe
	  			}//// {0,0} noktasý için hangi koordinatý belirlediysek o kadar ekleme yapýyoruz x ve y icin
	  			
	  			
		setcolor(WHITE);
		char cemberX[10], cemberY[10], cemberYariCap[10]; //cember bilgilerinin char degiskenleri 
		sprintf(cemberX,"Merkez={%.1f , ",c.x); // c.x'in char karsiligi
		sprintf(cemberY,"%.1f}",c.y); // c.y'in char karsiligi
		sprintf(cemberYariCap," R= %.1f",c.r); //c.r'in char karsiligi
		
		outtextxy(c.x*17+350,-c.y*17+350,cemberX); // X, Y ve Çemberin R deðerlerini ekrana basýyoruz
		outtextxy(c.x*17+420,-c.y*17+350,cemberY);
		outtextxy(c.x*17+350,-c.y*17+370,cemberYariCap);
				setcolor(RED); 
		circle(c.x*17+350,-c.y*17+350,c.r*17); //Cemberin Graphics.h > circle() ile cizimi.
	//spline icin koordinat dogrusu ve kesikler
		setcolor(1);
        line(710,350,1390,350);//x icin
        line(1050,10,1050,690);//y icin
        //koordinat duzlemi icin sayi cizgileri eklenir.
        setcolor(7);
        for (int i=10;i<=690;i+=17){
        	int deger=(350-i)/17;
        	char sayi[3];
			sprintf(sayi,"%d",deger);
			settextstyle(2,HORIZ_DIR,5);
			if(deger!=0){
			outtextxy(1054,i-7,sayi);
			line(1048,i,1052,i);
			
		}}
		 for (int j=710;j<=1390;j+=17){
        	int deger=-(1050-j)/17;
        	char sayi[3];
			sprintf(sayi,"%d",deger);
			settextstyle(2,HORIZ_DIR,5);
			if (deger!=0){
			outtextxy(j,358,sayi);
			line(j,348,j,352);		
		}}
			setcolor(2);
		for(i=0;i<n;i++){ //koordinatlarý tek tek ekrana basýyoruz
			fillellipse(koordinat[i][0]*17+1050,-koordinat[i][1]*17+350,2,2);//17, kesikler arasý mesafe
	  			}//// {0,0} noktasý için hangi koordinatý belirlediysek o kadar ekleme yapýyoruz x ve y icin
	
    getch();
    closegraph();
  return 0;
}
