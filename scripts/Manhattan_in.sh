#!/bin/bash
#Metadata: MAN_d_n_x_y_u_v_q_t_c_m_e_s_p_o_i

#Manhattan simulation parameter values
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

for di in ${d[@]}; do
    for ni in ${n[@]}; do
        for xi in ${x[@]}; do
                   for i in ${o[@]}; do
    echo $i
done
