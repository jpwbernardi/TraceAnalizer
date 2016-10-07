#!/bin/bash
#Metadata: MAN_d_n_x_y_u_v_q_t_c_m_e_s_p_o_i

#IN_NS2="/home/elmano/ns-allinone-2.34/ns-2.34"
IN_BONNMOTION="/home/uffs/Documents/Research/MobilityModels/bonnmotion-2.1a/bin" #"/home/elmano/Programas/bonnmotion-1.4/bin"
IN_TRACEANALYZER="/home/uffs/Documents/Research/Joao/TraceAnalyzer/" #"/home/elmano/workspace-cpp/TraceAnalyzer/Debug"

ROOT="/home/uffs/Documents/Research/Joao/TraceAnalyzer/scripts/output" #"/home/elmano/output"
#HOME=$ROOT/MAN
OUT_BONNMOTION="/home/uffs/Documents/Research/Joao/TraceAnalyzer/scripts/output/MAN/BonnMotion" #"$HOME/BonnMotion
OUT_TRACEANALYZER="/home/uffs/Documents/Research/Joao/TraceAnalyzer/scripts/output/MAN/Statistics" #$HOME/Statistics
OUT_NS2="/home/uffs/Documents/Research/Joao/TraceAnalyzer/scripts/output/MAN/NS2" #$HOME/NS2
STATISTICS="/home/uffs/Documents/Research/Joao/TraceAnalyzer/scripts/output/MAN/Statistics" #$HOME/Statistics
#RAPIDMINER=$HOME/RapidMiner

#All Manhattan simulation parameters values (Total of 90 scenarios)
d=(900)		  #simulation time
n=(100)		  #number of nodes
x=(1000) 	  #x-axis
y=(1000) 	  #y-axis
u=(10) 		  #<no. of blocks along x-axis>
v=(10)		  #<no. of blocks along y-axis>
#q=(m)		  #<update distance>
t=(0.5)		  #<turn probability>
c=(0.1) 	  #<speed change probability>
m=(6 11 16)	  #<mean speed>
e=(0 2 4)	  #<min. speed>
#s=(~m,e) 	  #<speed standard deviation>
p=(0.1)		  #<pause probability> 0.0 (5min) 0.1 (30s)!!!
o=(0 100 200 300 400 500 600 700 800 900)	  #<max. pause>
range=(100 150 200)

D=1	#DEFAULT =1
N=1	#DEFAULT =2
X=1	#DEFAULT =2
Y=1	#DEFAULT =2
U=1	#DEFAULT =2
V=1	#DEFAULT =2
#Q=1	#DEFAULT =1
T=1	#DEFAULT =1
C=1	#DEFAULT =2
M=1	#DEFAULT =3
E=2	#DEFAULT =3
#S=2	#DEFAULT =2
P=1	#DEFAULT =1
O=1	#DEFAULT =3
R=1	#DEFAULT =3
I=1	#DEFAULT =10

#NEXT:
di=0
ni=0
xi=0
yi=0
ui=0
vi=0
#qi=0
ti=0
mi=0
ei=0
#si=0
pi=0
oi=0
i=0

#2. ALL
for ((di; di<D; di++)); do
  for ((ni; ni<N; ni++)); do
    for ((xi; xi<X; xi++)); do
      for ((yi; yi<Y; yi++)); do
        for ((ui; ui<U; ui++)); do
          for ((vi; vi<V; vi++)); do
            for ((ti; ti<T; ti++)); do
              for ((ci; ci<C; ci++)); do
                for ((mi; mi<M; mi++)); do
                  for ((ei; ei<E; ei++)); do
                    for ((pi; pi<P; pi++)); do
                      for ((oi; oi<O; oi++)); do
                        for ((i; i<I; i++)); do #change the seed 10 times for each set of parameters
                        #CALCULATING THE VALUE FOR SPEED STANDARD DESVIATION
                        #speed = (randomNextGaussian() * speedStdDev) + meanSpeed; //Manhattan code
                        #randomNextGaussian(): 99,9936% estao entre [-4.0@,4.0@] :: @ eh o desvio padrao
                        Vmean=${m[mi]}
                        Vmin=${e[ei]}
                        if (($Vmean==6 && $Vmin==0)); then
                          s=1.565;
                        elif (($Vmean==6 && $Vmin==2)); then
                          s=1.045;
                        elif (($Vmean==6 && $Vmin==4)); then
                          s=0.522;
                        elif (($Vmean==11 && $Vmin==0)); then
                          s=2.85;
                        elif (($Vmean==11 && $Vmin==2)); then
                          s=2.35;
                        elif (($Vmean==11 && $Vmin==4)); then
                          s=1.83;
                        elif (($Vmean==16 && $Vmin==0)); then
                          s=4.17;
                        elif (($Vmean==16 && $Vmin==2)); then
                          s=3.65;
                        else s=3.13; #Vmean==16 && $Vmin==4
                        fi

                        SCENARIO=MAN_d"${d[di]}"_n"${n[ni]}"_x"${x[xi]}"_y"${y[yi]}"_u"${u[ui]}"_v"${v[vi]}"_q"${m[mi]}"_t"${t[ti]}"_c"${c[ci]}"_m"${m[mi]}"_e"${e[ei]}"_s"$s"_p"${p[pi]}"_o"${o[oi]}"_"$i"	#q=m[mi] ok!
                        PARAMS="-d ${d[di]} -n ${n[ni]} -x ${x[xi]} -y ${y[yi]} -u ${u[ui]} -v ${v[vi]} -q ${m[mi]} -t ${t[ti]} -c ${c[ci]} -m ${m[mi]} -e ${e[ei]} -s $s -p ${p[pi]} -o ${o[oi]}"

                        echo "FILE: "$SCENARIO
                        cd $IN_BONNMOTION

                        if [ -f $OUT_NS2/$SCENARIO.ns_movements.gz ];  then a=a;
                        else
                          ./bm -f $OUT_BONNMOTION/${SCENARIO} ManhattanGrid ${PARAMS}
                          echo "FILE: "$SCENARIO
                          echo "  1/5: BonnMotion file created."
                          ./bm NSFile -f $OUT_BONNMOTION/${SCENARIO}
                          echo "  2/5: BonnMotion ns-2 file created."
                          NS2FILE=$OUT_BONNMOTION/${SCENARIO}
                          mv $NS2FILE.ns_movements $NS2FILE.ns_movements_old
                          ./bm NSFile -f $NS2FILE
                          rm $NS2FILE.ns_movements_old
                          gzip $NS2FILE.ns_movements #original
                          mv $NS2FILE.ns_movements.gz $OUT_NS2
                          echo "FILE: "$SCENARIO
                          echo "  3/5: Original ns-2 file created."
                        fi
                        if [ -f $STATISTICS/${SCENARIO}.stats ]; then a=a;
                        else
                          PARAMS="-r 100.1,150.1,200.1"
                          ./bm Statistics -f $OUT_BONNMOTION/${SCENARIO} ${PARAMS}
                          mv $OUT_BONNMOTION/${SCENARIO}.stats $STATISTICS/${SCENARIO}.stats
                          echo "FILE: "$SCENARIO
                          echo "  4/5: BonnMotion statistics file created."
                        fi
                        if [ -f ${STATISTICS}/${SCENARIO}_R${range[0]}.analyzer ]; then a=a;
                        else
                          NS2FILE=${OUT_NS2}/${SCENARIO}.ns_movements
                          gunzip ${NS2FILE}.gz
                          cd $IN_TRACEANALYZER
                          TmP=$(echo "${o[oi]}/2" | bc) #TmP=TMP/2
                          for ((k=0; k<R; k++)); do
                            echo "./TraceAnalyzer ${NS2FILE} ${range[k]} ${TmP} $x $y  > ${STATISTICS}/${SCENARIO}_R${range[k]}.analyzer"
                            ./TraceAnalyzer ${NS2FILE} ${range[k]} ${TmP} $x $y  > ${STATISTICS}/${SCENARIO}_R${range[k]}.analyzer
                            echo "       TraceAnalyzer with Range=${range[k]} created"
                          done
                          gzip ${NS2FILE}
                          echo "FILE: "$SCENARIO
                          echo "  5/5: TraceAnalyzer statistics file created."j
                        fi
                        done
                        i=0
                      done
                      oi=0
                    done
                    pi=0
                  done
                  ei=0
                done
                mi=0
              done
              ci=0
            done
            ti=0
          done
          vi=0
        done
        ui=0
      done
      yi=0
    done
    xi=0
  done
  ni=0
done
