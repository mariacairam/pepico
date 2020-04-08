import numpy as np

saida = open('saida.log', 'w')
plot = open('plot.log', 'w')
areaabs = open('areaabsoluta.log', 'w')
arearel = open('arearelativa.log','w')
arearelmenos = open('arearelativamenos.log','w')
imq = np.empty(30, dtype=object)
istart = np.empty(30, dtype=object)
istop = np.empty(30, dtype=object)
icounts = np.empty(5000, dtype=object)

IT = "Intervalo_TOF.txt"
imq, istart, istop = np.loadtxt(IT, delimiter=None, unpack=True)
print(imq, istart, istop)
i = 0
with open(IT, 'r') as f:
    for line in f:
        i += 1
        nFrag = i
saida.write('Numero de fragmentos   '+ str(nFrag))


TT = "Table_TOF.txt"
icounts = np.loadtxt(TT)
print(icounts)
i = 0
with open (TT, 'r') as g:
    for line in g:
        i += 1
        tvoo = i-1
saida.write('\nTempo de voo maximo  ' +str(tvoo))

aa= 0
bb= 0
ix1= 121
ix2= tvoo-121
c = 1
while (c<=41):
    aa += icounts[100+c]
    bb += icounts[tvoo-100-c]
    c +=1
aa/=41
bb/=41
print(aa, bb)
bcoef=(aa-bb)/(ix1-ix2)
acoef=aa-bcoef*ix1
saida.write('\n\nDados do ajuste:\nix1, aa, ix2, bb\n' +''+str(ix1) +'   '+str(aa) +'   '+str(ix2)
            +'   '+str(bb) +'\nacoef, bcoef\n' +''+str(acoef) +'   '+str(bcoef) +'\n')

c=0
while (c<=tvoo):
    temp= acoef + bcoef*i
    plot.write('\n'+str(c) +' '+str(icounts[c]) +' '+str(temp) +' '+str(icounts[c]-temp))
    c +=1

areatot= 0
acortot= 0
areacor = np.empty(30, dtype=object)
i=0
while (i<nFrag):
    istart = istart.astype(int)
    istop = istop.astype(int)
    j=istart[i]
    corr= 0
    area= np.zeros(30)
    while (j<=istop[i]):
        area[i] += icounts[j]
        corr += acoef+bcoef*j
        j +=1
    ar= area[i]
    areacor[i]= ar-corr
    areatot += area[i]
    acortot += areacor[i]
    saida.write('\nPico area    '+str(i+1) +'   ' + str(imq[i]) +'   ' + str(area[i]) +'   ' + str(areacor[i]))
    areaabs.write('\nPico area    ' + str(i + 1) + '   ' + str(imq[i]) + '   ' + str(area[i]) + '   ' + str(areacor[i]))
    i +=1
saida.write('\n\nArea total    \n'+str(areatot) +'   ' + str(acortot) +'\n\nArea relativa pico pela area total\n')

er=4
i=0
while(i<nFrag):
    erro1= np.sqrt(areacor[i])+areacor[i]*er/100
    erro1 = erro1/acortot*100
    saida.write('\n' +str(i+1) +'   ' + str(imq[i]) +'   ' + str(areacor[i]/acortot*100) +'   ' +str(erro1))
    arearel.write('\n' +str(i+1) +'   ' + str(imq[i]) +'   ' + str(areacor[i]/acortot*100) +'   ' +str(erro1))
    i += 1

saida.write('\n\nArea relativa pico pela area total menos pico')

er=4
i=0
while(i<nFrag):
    erro2= np.sqrt(areacor[i])+areacor[i]*er/100
    erro2 = erro2/(acortot-areacor[nFrag-1])*100
    saida.write('\n' + str(i + 1) + '   ' + str(imq[i]) + '   ' + str(areacor[i]/(acortot-areacor[nFrag-1])*100)
                  + '   ' + str(erro2))
    arearelmenos.write('\n' + str(i+1) + '   ' + str(imq[i]) + '   ' + str(areacor[i]/(acortot-areacor[nFrag-1]) *100)
                  + '   ' + str(erro2))
    i += 1

saida.close()
plot.close()
areaabs.close()
arearel.close()
arearelmenos.close()
