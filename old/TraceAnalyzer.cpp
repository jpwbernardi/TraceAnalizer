//============================================================================
// Name        : TraceAnalyzer.cpp
// Author      : Elmano R. Cavalcanti
// Version     : 2.1 (May 13 2011)
// Copyright   : Your copyright notice
// Description : Mobility Trace Analyzer (NS-2 compatible)
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LinkAnalysis.h"

#define length(a) ( sizeof ( a ) / sizeof ( *a ) )

using namespace std;

class Trace mytrace;

int main(int argc, char* argv[]) {

/*
	char name[150];
	int i;

	//strcpy(name,"/home/elmano/output/ComCom2013/RWP/NS2/RWP_d900_n10_x500_y500_l1_h10_p100_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RandomWalk/NS2/RWalk_d900_n10_x100_y100_l1_h20_p0_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RandomDirection/NS2/RD_d900_n10_x500_y500_l1_h5_p0_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/Gauss-Markov/NS2/GM_d900_n10_x100_y100_a6_m10_q1_s1.82_w0.5_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RPGM/NS2/RPGM_d900_n20_x100_y100_a4_s0_r10_c0.0_l1_h10_p0_0.ns_movements");


	//strcpy(name,"/home/elmano/output/ComCom2013/RWP/NS2/RWP_d900_n100_x1000_y1000_l1_h10_p100_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RandomWalk/NS2/RWalk_d900_n100_x1000_y1000_l1_h10_p0_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RandomDirection/NS2/RD_d900_n100_x1000_y1000_l1_h10_p0_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/Gauss-Markov/NS2/GM_d900_n100_x1000_y1000_a6_m10_q1_s1.82_w0.5_0.ns_movements");
	//strcpy(name,"/home/elmano/Dropbox/Doutorado/software/mobility-models/smooth/smooth.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/CMM/NS2/CMM_t900_n100_X1000_Y1000_r900_s1_S10_p0.1_R5_C5_w0.1_G10_aon_don_T200_0.ns_movements");
	strcpy(name,"/home/uffs/Documents/Research/TraceAnalyzer/TraceAnalyzer-2.2/src/MAN_d900_n100_x1000_y1000_u10_v10_q6_t0.5_c0.1_m6_e0_s1.565_p0.1_o0_0.ns_movements");
	//strcpy(name,"/home/elmano/Dropbox/Doutorado/output/CUSTOM-MOBILITY/QUADRADO_d40_n2_h10_p0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RPGM/NS2/RPGM_d900_n100_x1000_y1000_a10_s0_r10_c0.0_l1_h10_p100_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/Manhattan/NS2/MAN_d900_n100_x1000_y1000_u5_v5_q10_t0.5_c0.1_m10_e1_s3.13_p0.1_o0_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/SLAW/NS2/SLAW_t900_a3_n100_xy500_w50_H0.75_R50_b1_p1_P900_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/VANET/N200.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/SMS/NS2/SMS_t900_n100_x1000_y500_m0_M10_p10_b1_c8_u5_z0.5_s1_0.ns_movements");
	//strcpy(name,"/home/elmano/output/ComCom2013/RWPSteadyState/NS2/RWPSS_d900_n100_x1000_y1000_o10_p5_k0_l0_0.ns_movements");

	mytrace.RADIUS = atoi("200");
	mytrace.TIMEPAUSE = atoi("50"); //=APT
	mytrace.SCENARIO_WIDTH = atoi("1000");
	mytrace.SCENARIO_LENGTH = atoi("1000");

	trace = new struct data* [NODE_NUM];
	for(i=0;i<NODE_NUM;i++)
		trace[i]=new struct data[TIME_SLOT];

	mytrace.initiate();
	mytrace.read_trace(name);
	//mytrace.show_trace(0,0,100);
	mytrace.set_data();
	//mytrace.show_trace(0,0,100); //to see the trace after adjustments
	//mytrace.show_trace(1,0,100);

	//mytrace.printAngles();
	//mytrace.printSpeeds();
	mytrace.printPauseTimes();
	//mytrace.printTripLengths();
	//mytrace.printPathLengths();
	mytrace.xy_axis_interval();

	printf("DNSD \t REC \t LC \t LD \t ND \t DNP \t ATL \t APL \t AC \t RS \t DSD \t IDSD \t HIDSD \t DTD \t IDTD \t SAR \t AVC \t SVC \t PVC \t SCENARIO\n");
 	//POSITION-BASED METRICS
 	printf("%f\t",mytrace.degreeOfSpatialDistribution()); //DNSD
 	printf("%f\t", mytrace.emptyCells()); //Rate of Empty Cells

	//LINK-BASED METRICS
	char link_distribution[] = "link_distribution.txt";
	mytrace.cal_link(link_distribution);
	printf("%d\t",mytrace.total_link_change); //Number of Link Changes
	printf("%f\t",mytrace.avg_link_duration); //Link Duration

	//GRAPH-BASED METRICS
	char node_degree_dist[] = "node_degree_dist.txt";
	mytrace.cal_node_degree(node_degree_dist);
	printf("%f\t",mytrace.node_degree); //Node Degree
	//printf("CC=%f\t",mytrace.cluster_coefficient); //Cluster Coefficient

	//DISTANCE-BASED METRICS
	printf("%f\t",mytrace.degree_of_node_proximity()); //Degree of Node Proximity (R units)
	printf("%f\t",mytrace.average_tripLength()); //Average Trip Length (displacement) (R units)
	printf("%f\t",mytrace.average_pathLength()); //Average Path Length (distance) (R units)
	printf("%f\t",mytrace.average_coverage());

	//VELOCITY-BASED METRICS
	mytrace.degree_of_spatial_dependence();
	mytrace.improved_degree_of_spatial_dependence();
	mytrace.high_improved_degree_of_spatial_dependence();

	printf("%f\t",mytrace.average_relative_speed()); //Relative Speed
	printf("%f\t",mytrace.degree_of_spatial_dependence()); //Degree of Spatial Dependence
	printf("%f\t",mytrace.improved_degree_of_spatial_dependence()); //Improved Degree of Spatial Dependence
	printf("%f\t",mytrace.high_improved_degree_of_spatial_dependence()); //High Improved Degree of Spatial Dependence
	printf("%f\t",mytrace.degree_of_temporal_dependence()); //Degree of Temporal Dependence
	printf("%f\t",mytrace.improved_degree_of_temporal_dependence()); //Improved Degree of Temporal Dependence
	printf("%f\t",mytrace.speed_angle_rate()); //Speed Angle Rate
	printf("%f\t",mytrace.angle_variation_coefficient()); //Angle Variation Coefficient
	printf("%f\t",mytrace.speed_variation_coefficient()); //Speed Variation Coefficient

	//TIME-BASED METRICS
	printf("%f\t",mytrace.pause_variation_coefficient()); //Pause Variation Coefficient

	printf("%s", name);



	//LD is both link and time-based
	//PVC is only time-based */

	/*mytrace.print_spatial_dependence_statistics(0,1);
	mytrace.print_DSD(0,1,0,100);
	mytrace.print_IDSD(0,1,0,100);
	mytrace.print_HIDSD(0,1,0,100);*/
	//mytrace.print_SpatialMetrics(0,1,0,50);

	//return 0;

	/*
	 * ABAIXO, O CODIGO ORIGINAL, PARA EXECUCAO EM LOTE
	 * BASTA COMENTAR O CODIGO ACIMA E DESCOMENTAR O DEBAIXO
	 */



	char name[150];
	char mobilityModel[150];
	int i;

	if(argc < 6) {
		printf("Missing arguments!\t");
		exit(0);
	}

	// strcpy(mobilityModel,argv[1]);
	strcpy(name,argv[1]);
	mytrace.RADIUS = atoi(argv[2]);
	mytrace.TIMEPAUSE = atoi(argv[3]);
	mytrace.SCENARIO_WIDTH = atoi(argv[4]);
	mytrace.SCENARIO_LENGTH = atoi(argv[5]);


	trace = new struct data * [NODE_NUM];
	for(i = 0; i < NODE_NUM; i++)
		trace[i] = new struct data[TIME_SLOT];

	mytrace.initiate();
	mytrace.read_trace(name);
	mytrace.set_data();

	//LINK-BASED METRICS
	char link_distribution[] = "link_distribution.txt";
	mytrace.cal_link(link_distribution);
	printf("LC=%d\t",mytrace.total_link_change); //Number of link changes
	printf("LD=%f\t",mytrace.avg_link_duration); //Link duration

	//GRAPH-BASED METRICS
	char node_degree_dist[] = "node_degree_dist.txt";
	mytrace.cal_node_degree(node_degree_dist);
	printf("ND=%f\t",mytrace.node_degree); //Node Degree

	//Why is doing it again?
	mytrace.cal_link(link_distribution);

	//Calling it twice. Once here and other in prints. There may be an depencence
	mytrace.degree_of_spatial_dependence();
	mytrace.improved_degree_of_spatial_dependence();
	mytrace.high_improved_degree_of_spatial_dependence();

	//DISTANCE-BASED METRICS
	printf("RS=%f\t",mytrace.average_relative_speed()); //Relative Speed
	printf("DSD=%f\t",mytrace.degree_of_spatial_dependence()); //Degree of Spatial Dependence
	printf("DTD=%f\t",mytrace.degree_of_temporal_dependence()); //Degree of Temporal Dependence
	printf("IDSD=%f\t",mytrace.improved_degree_of_spatial_dependence()); //Degree of Spatial Dependence
	printf("IDTD=%f\t",mytrace.improved_degree_of_temporal_dependence()); //Improved Degree of Temporal Dependence
	//printf("HIDSD=%f\t",mytrace.high_improved_degree_of_spatial_dependence()); //High Improved Degree of Spatial Dependence
	printf("DNP=%f\t",mytrace.degree_of_node_proximity()); //Degree of Node Proximity
	//Standard Deviation of Average Node Distance
	printf("AC=%f\t",mytrace.average_coverage()); //Average Coverage
	return 0;
 }
