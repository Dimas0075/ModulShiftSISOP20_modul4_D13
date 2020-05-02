# SoalShiftSISOP20_modul4_D13
# Anggota Kelompok
* Herri Purba                     05111840000056
* Nugroho Dimas Ardiyanto         05111840000075

# Soal 
Di suatu perusahaan, terdapat pekerja baru yang super jenius, ia bernama jasir. Jasir baru bekerja selama seminggu di perusahan itu, dalam waktu seminggu tersebut ia selalu terhantui oleh ketidak amanan dan ketidak efisienan file system yang digunakan perusahaan tersebut. Sehingga ia merancang sebuah file system yang sangat aman dan efisien. Pada segi keamanan, Jasir telah menemukan 2 buah metode enkripsi file. Pada mode enkripsi pertama, nama file-file pada direktori terenkripsi akan dienkripsi menggunakan metode substitusi. Sedangkan pada metode enkripsi yang kedua, file-file pada direktori terenkripsi akan di-split menjadi file-file kecil. Sehingga orang-orang yang tidak menggunakan filesystem rancangannya akan kebingungan saat mengakses direktori terenkripsi tersebut. Untuk segi efisiensi, dikarenakan pada perusahaan tersebut sering dilaksanakan sinkronisasi antara 2 direktori, maka jasir telah merumuskan sebuah metode agar filesystem-nya mampu mengsingkronkan kedua direktori tersebut secara otomatis. Agar integritas filesystem tersebut lebih terjamin, maka setiap command yang dilakukan akan dicatat kedalam sebuah file log.
(catatan: filesystem berfungsi normal layaknya linux pada umumnya)
(catatan: mount source (root) filesystem adalah direktori /home/[user]/Documents)

### 1.	Enkripsi versi 1:

a.	Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1. <br/>
b.	Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1. <br/>
c.	Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.<br/>
d.	Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.<br/>
e.	Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.<br/>

9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO <br/>

`Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10 <br/>
**“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg**
**Note** : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.<br/>

f.	Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

#### JAWABAN NO 1 :
```c
void encv_1(char *filename){
    if(!strcmp(filename, ".") || !strcmp(filename, "..")) return;
    for(int i=0; i< strlen(filename); i++){
        for(int j=0; j < 87; j++){
            if(filename[i]==key[j]){
                filename[i]=key[(j+10)%87];
                break;
            }
        }
    }
}

```
Untuk Mengencrip Nama file yang berada di file dengan kata encv1_ dengan metode caesar chiper

```c
void decrv_1(char *filename){
    if(!strcmp(filename, ".") || !strcmp(filename, "..")) return;
    for(int i=0; i< strlen(filename); i++){
        for(int j=0; j < 87; j++){
            if(filename[i]==key[j]){
                filename[i]=key[(j+77)%87];
                break;
            }
        }
    }
}
```
Untuk mendekcrip file yang sudah terdekrip jika terjadi rename dengan mengunakan key yang sama dan dengan metode caesar choper lagi

```c
static  int  xmp_getattr(const char *path, struct stat *stbuf)

{
    printf("GETATTR\n");

    int res;

    char fpath[1000], temp[1000];
    sprintf(temp, "%s", path);
    sprintf(fpath,"%s%s",dirpath,path);
    

    char dir[1000], *namafile;
    namafile=strrchr(temp,'/');
    dir[strlen(temp)-strlen(namafile)-1]='\0';
    printf("PATH : %s\n",path);
    printf("FPATH : %s\n",fpath);
    printf("nama and dir %s%s\n", namafile,dir);


    if(strcmp(dir,"encv1_")==0){
        res=1;
    }

    //  if (isDirectory(fpath))
    // {
    //     printf("MANA WOI %s\n",fpath);
    //     res = lstat(fpath,stbuf);
    // }else
    // {
    if(res==1){

        char *dot = get_filename_ext(temp);
        if(strcmp(dot,"")!=0){
            namafile[strlen(namafile)-strlen(dot)-1]='\0';
            decrv_1(namafile);
            sprintf(namafile,"%s%s",namafile,dot);
        }
        else
        {
            decrv_1(namafile);
        }

        sprintf(dir,"%s%s",dir,namafile);
        sprintf(fpath,"%s%s",dirpath,dir);
        res = lstat(fpath,stbuf);

    }else{

        res = lstat(fpath, stbuf);
    }
    
 //   }

    if (res == -1)
        return -errno;

    

    return 0;

}
```
Getattr untuk mengambil atribut dari directory dan pada getattr juga kami menggunakan fungsi decrip agar jika file di rename atau file ingin di lihat di directory aslinya maka akan kembali normal.

```c
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{
    printf("READDIR\n");

    char fpath[1000],temp2[1000];
    //char *dir;
    int check=0;

   //dir=strrchr(path,'.');

    if(strcmp(path,"/") == 0)
    {
        printf("=1=\n");
        path=dirpath;
        sprintf(fpath,"%s",path);
        printf("FPAthDir %s\n",fpath);
    }

    else {
        printf("=2=\n");
        if(strstr(path, "encv1")){
            check=1;
        }
        sprintf(temp2,"%s",path);
        sprintf(fpath, "%s%s",dirpath,path);
        printf("%s\n",path);
        printf("%s\n",fpath);
        
    }
    int res = 0;

    DIR *dp;

    struct dirent *de;

    (void) offset;

    (void) fi;
    //printf("@@\n");
    dp = opendir(fpath);
    //printf("$$\n");

    if (dp == NULL) return -errno;

    

    while ((de = readdir(dp)) != NULL) {

        char temp[1000];
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;

        st.st_mode = de->d_type << 12;
        printf("=3=");
        sprintf(temp,"%s",de->d_name);
        sprintf(fpath,"%s%s",fpath,temp);
        char *jenis = get_filename_ext(temp);
        printf("jenis : %s\n",jenis);
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
        {
            filler(buf, de->d_name, &st, 0); //menampilkan anma yang di inginkan ke file sistem
            continue;
        }
        if(check==1){
            printf("=4=");
            
                if(strcmp(jenis,"")!=0){
                    temp[strlen(temp)-strlen(jenis)-1]='\0';
                    encv_1(temp);
                    sprintf(temp,"%s%s",temp,jenis);
                }
                else{
                    encv_1(temp);
                }

            
        }
        printf("FIle %s%s\n",de->d_name,temp);
        res = (filler(buf, temp, &st, 0));

        if(res!=0) break;

    }

    

closedir(dp);

return 0;

}

```
Readdir untuk membaca file file dari sebuah directory yang di dituju damn di readdir kami menggunakan fungsi encv_1 dimana akan di lihat terlebih dahulu nama filenya apakan dengan awalan encv1_, jika ya maka file file yang ada di dalam directory tersebut akan terencripsi.

### 2.	Enkripsi versi 2:

a.	Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.<br/>
b.	Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.<br/>
c.	Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.<br/>
d.	Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.<br/>
e.	Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file  kecil yakni: File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004. <br/>
f.	Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif).<br/>

### 3.	Sinkronisasi direktori otomatis:

Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:<br/>
a.	Kedua directory memiliki parent directory yang sama.<br/>
b.	Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:<br/>
  * i.	Nama dari setiap berkas di dalamnya sama.
  * ii.	Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik. <br/>
c.	Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja. <br/>
d.	Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory. <br/>

Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.<br/>
Implementasi dilarang menggunakan symbolic links dan thread.<br/>

### 4.	Log system:

a.	Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.<br/>
b.	Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.<br/>
c.	Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.<br/>
d.	Sisanya, akan dicatat dengan level INFO.<br/>
e.	Format untuk logging yaitu:<br/>

[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]  |
---------------------------------------------------------|


LEVEL    : Level logging <br/>
yy   	 : Tahun dua digit <br/>
mm    	 : Bulan dua digit <br/>
dd    	 : Hari dua digit <br/>
HH    	 : Jam dua digit <br/>
MM    	 : Menit dua digit <br/>
SS    	 : Detik dua digit <br/>
CMD     	 : System call yang terpanggil <br/>
DESC      : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::) <br/>

Contoh format logging nantinya seperti: <br/>
-------------------------------------------------------------|
INFO::200419-18:29:28::MKDIR::/iz1                           |
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg                  |
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg |
-------------------------------------------------------------|

##### Jawaban No 4
```c
 void Warning(char * str){
	FILE * logFile = fopen("/home/dimas/fs.log", "a");
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(logFile, "WARNING::%d%d%d-%d:%d:%d::%s\n", timeinfo->tm_year+1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,  timeinfo->tm_min, timeinfo->tm_sec, str);
	fclose(logFile);
}

void Info(char * str){
	FILE * logFile = fopen("/home/dimas/fs.log", "a");
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(logFile, "INFO::%d%d%d-%d:%d:%d::%s\n", timeinfo->tm_year+1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, str);
	fclose(logFile);
}
```
Sesuai dengan perintah pada soal yaitu leveling saat melakukan syscall. Maka akan terdapat dua level yaitu Warning dan Info </br>
Untuk contohnya pada mkdir, pada fungsi tersebut akan memanggil Info(str) yang akan mengappend ke fs.log
```c
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	res = mkdir(fpath, mode);
	char str[100];
	sprintf(str, "MKDIR::%s", path);
	Info(str);
	if (res == -1)
		return -errno;

	return 0;
}
``` 
```c
fprintf(logFile, "WARNING::%d%d%d-%d:%d:%d::%s\n", timeinfo->tm_year+1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, str);
```
Lalu untuk format logging menggunakan localtime.</br>
Untuk syscall yang dipanggil dan deskripsi path atau filenya terdapat pada str.



