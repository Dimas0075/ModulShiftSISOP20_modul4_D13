#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>

  
static const char *dirpath = "/home/herri/Documents";
char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

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

int isDirectory(char *path)
{
    struct stat sb;
    stat(path, &sb);
    return S_ISDIR(sb.st_mode);
}
char *get_filename_ext(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

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

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,

struct fuse_file_info *fi)

{

    char fpath[1000];

    if(strcmp(path,"/") == 0)

    {

        path=dirpath;

        sprintf(fpath,"%s",path);

    }

    else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;

    int fd = 0 ;

    

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)

    return -errno;

    

    res = pread(fd, buf, size, offset);

    if (res == -1)
      res = -errno;

    close(fd);
     return res;

}

  

static struct fuse_operations xmp_oper = {

.getattr = xmp_getattr,

.readdir = xmp_readdir,

.read = xmp_read,

};

  

int  main(int  argc, char *argv[])

{

umask(0);

return fuse_main(argc, argv, &xmp_oper, NULL);

}