#include <stdio.h>
#include <math.h>

int main(int argc, char** argv){
    FILE* saida, *Itof, *Ttof, *plot, *areaabs, *arearel,*arearelmenos;
    saida = fopen("saida.log","w");
    Itof = fopen("Intervalo_TOF.txt","r");
    Ttof = fopen("Table_TOF.txt","r");
    plot = fopen("plot.log", "w");
    areaabs = fopen("areaabsoluta.log","w");
    arearel = fopen("arearelativa.log","w");
    arearelmenos = fopen("arearelativamenos.log","w");

    int istart[30],istop[30],imq[30], iconts[5000];
    int l = 0;
    int c;
    do{
        fscanf(Itof,"%d",&imq[l]);
        fscanf(Itof,"%d",&istart[l]);
        fscanf(Itof,"%d",&istop[l]);
        l++;
    }while((c = fgetc(Itof)) != EOF);
    fprintf(saida,"Numero de fragmentos %d\n",l);
    int nFrag = l;
    int i = 0;
    c = 0;
    do{
        fscanf(Ttof,"%d", &iconts[i]);
        i++;
    }while((c = fgetc(Ttof)) != EOF);
    
    fprintf(saida,"tempo de voo maximo %d\n", i-1);
    int tvoo = i-1;

    double aa = 0, bb = 0;
    int ix1 = 121, ix2 = tvoo - 121;
    for(int c = 1; c <= 41; c++){
        
        aa+=iconts[100+c];
        
        bb+=iconts[tvoo-100-c];
    }
    
    aa/=41.0;
    bb/=41.0;

    double bcoef=(aa-bb)/(ix1-ix2), acoef=aa-bcoef*ix1;

    fprintf(saida,"dados de ajuste\n");
    fprintf(saida,"ix1, aa, ix2, bb\n");
    fprintf(saida,"%d %.15f %d %.15f\n",ix1,aa,ix2,bb);
    fprintf(saida,"acoef, bcoef\n");
    fprintf(saida,"%.15f %.18f\n",acoef, bcoef);

    for(int cont=0;cont<=tvoo;cont++){
        double temp = acoef+bcoef*cont;
        fprintf(plot,"%d %d %.15f %.15f\n",cont,iconts[cont],temp,iconts[cont]-temp);
    }

    double areatot = 0, acortot = 0, corr;
    double area[30],areacor[30];
    for(int i=0;i<nFrag;i++){
        corr=0;
        area[i]=0;
        for(int j = istart[i];j<=istop[i];j++){
            area[i] += iconts[j];
            corr += acoef+bcoef*j;
        }
        areacor[i] = area[i]-corr;
        areatot += area[i];
        acortot += areacor[i];
        fprintf(saida,"pico area \t%d\t%d\t%.15f\t%.15f\n", i+1,imq[i],area[i],areacor[i]);
        fprintf(areaabs,"%d %d %.15f %.15f", i+1, imq[i], area[i], areacor[i]);
    }
    fprintf(saida,"\narea total %.15f %.15f\n",areatot,acortot);
    fprintf(saida,"area relativa pico pela area total\n");

    double er = 4,erro;
    for(int i=0;i<nFrag;i++){
        erro = sqrt(areacor[i])+areacor[i]*er/100;
        erro = erro/acortot*100;
        fprintf(saida,"%3d\t%3d\t%8.3f\t%8.3f\n",i+1,imq[i],areacor[i]/acortot*100,erro);
        fprintf(arearel,"%3d\t%3d\t%8.3f\t%8.3f\n",i+1,imq[i],areacor[i]/acortot*100,erro);
    }

    fprintf(saida,"\narea relativa pico pela area total menos pico\n");
    
    for(int i=0;i<nFrag;i++){
        erro = sqrt(areacor[i])+areacor[i]*er/100.;
        erro=erro/(acortot-areacor[nFrag-1])*100;
        fprintf(saida,"%3d\t%3d\t%8.3f\t%8.3f\n",i+1,imq[i],areacor[i]/(acortot-areacor[nFrag-1])*100,erro);
        fprintf(arearelmenos,"%3d\t%3d\t%8.3f\t%8.3f\n",i+1,imq[i],areacor[i]/(acortot-areacor[nFrag-1])*100,erro);
    }

    fclose(arearel);
    fclose(arearelmenos);
    fclose(areaabs);
    fclose(plot);
    fclose(Ttof);   
    fclose(saida);
    fclose(Itof);
    return 0;
}
