/*
 * LinkAnalysis.h
 *
 *  Created on: 04/02/2010
 *  Author: Elmano RC
 *  Last modified: Feb 13 2013
 */

#include "misc.h"
#include <cmath>

#define TIME_SLOT 900 //constant
#define NODE_NUM 100 //constant
#define NIL -999 //constant

#define MAX_NUMBER_WAYPOINTS 50 //waypoints = visiting points
#define NODE_PAUSE_TIME_SLOT 50 //array that keeps all pause time durations of a node
#define NODES_PAUSE_TIME_SLOT NODE_NUM*NODE_PAUSE_TIME_SLOT //array that keeps all pause time durations of all nodes
#define NODE_TRIP_LENGTH_SLOT 50
#define NODE_PATH_LENGTH_SLOT NODE_TRIP_LENGTH_SLOT //same size
#define NODES_TRIP_LENGTH_SLOT NODE_NUM * NODE_TRIP_LENGTH_SLOT
#define NODES_PATH_LENGTH_SLOT NODES_TRIP_LENGTH_SLOT //same size
#define NODE_SPEED_SLOT TIME_SLOT //the speed values of a node (/10 otherwise the array would be to large)
#define NODES_SPEED_SLOT NODE_NUM * NODE_SPEED_SLOT
#define NODE_ANGLE_SLOT TIME_SLOT //the velocity angle values of a node (upper bound = 1, i.e. angle changes at every time stamp)
#define NODES_ANGLE_SLOT NODE_NUM * NODE_ANGLE_SLOT

//#define RADIUS 100

float mean(int* pdf) {
	int i;
	int sum1 = 0, sum2 = 0;
	float avg;

	for (i = 0; i < TIME_SLOT + 1; i++){
		sum1 = sum1 + pdf[i];
		sum2 = sum2 + i * pdf[i];
	}

	avg = (float)sum2 / (float)sum1;
	return avg;
}



//-----------------------------------------------------------------//

struct data { //Dados do nodo(nó)
	int id;
	double time, start;
	double x, y;
	double next_x, next_y;
	double angle, speed;
	int timetodestiny;
};

//raw data
struct data** trace;
double * ini_x = new double [NODE_NUM];
double * ini_y = new double [NODE_NUM];

struct point {
	double x, y;
	double time;
};

struct point** waypoints;

class Trace {
	private:
		//used for link calculation
		int link_status[NODE_NUM][NODE_NUM];
		int link_up_num[NODE_NUM][NODE_NUM];
		int link_down_num[NODE_NUM][NODE_NUM];
		int last_up_time[NODE_NUM][NODE_NUM];
		int last_down_time[NODE_NUM][NODE_NUM];

	public:

		float DSD[NODE_NUM][NODE_NUM][TIME_SLOT];
		float estimatedIDSD[NODE_NUM][NODE_NUM][TIME_SLOT]; //to keep all the historic of all spatial correlations, by Elmano
		float estimatedHIDSD[NODE_NUM][NODE_NUM][TIME_SLOT];
		float temporalCorrelations[NODE_NUM][TIME_SLOT]; //to keep all the historic of all temporal correlations, by Elmano
		float distances[NODE_NUM][NODE_NUM][TIME_SLOT]; //to keep all the distances between nodes at all time steps, by Elmano
		float average_distances[NODE_NUM][NODE_NUM]; //to keep the average of the distances between nodes, by Elmano
		float std_distances[NODE_NUM][NODE_NUM]; //to keep the std of the distances between nodes, by Elmano

		float nodePathLengths[NODE_NUM][NODE_PAUSE_TIME_SLOT]; //to keep the trip DISTANCE of all nodes
		float nodesPathLengths[NODES_TRIP_LENGTH_SLOT];
		float nodeTripLengths[NODE_NUM][NODE_PAUSE_TIME_SLOT]; //to keep the trip DISPLACEMENT of all nodes
		float nodesTripLengths[NODES_TRIP_LENGTH_SLOT];
		float nodePauseTimes[NODE_NUM][NODE_PAUSE_TIME_SLOT]; //to keep the values of pause duration of all nodes (i.e., the duration of time where the node was stationary)
		float nodesPauseTimes[NODES_PAUSE_TIME_SLOT];
		float nodeSpeeds[NODE_NUM][NODE_SPEED_SLOT]; //the values of speed of each node (in the upper bound each node has TIME_SLOT values, i.e., one
		float nodesSpeeds[NODES_SPEED_SLOT]; //the values of speed of all nodes
		float nodeAngles[NODE_NUM][NODE_ANGLE_SLOT]; //the values of velocity angle of each node (in the upper bound each node has TIME_SLOT values)
		float nodesAngles[NODES_ANGLE_SLOT]; //the values of velocity angle of all nodes
		float degreeSpatialDistribution;

		//int cells[TIME_SLOT][NODE_NUM+1][NODE_NUM+1]; //last column and line will contains the summarize
		int cells[NODE_NUM+1][NODE_NUM+1]; //store the total amount of nodes in each cell during the simulation

		int SCENARIO_WIDTH;
		int SCENARIO_LENGTH;
		int RADIUS;
		int TIMEPAUSE; //AVERAGE TIME PAUSE

		int GRID_WIDTH;
		int GRID_LENGTH;
		int CELL_WIDTH;
		int CELL_LENGTH;

		int total_link_change;
		float avg_link_duration;
		int ld_pdf[TIME_SLOT+1];

		float node_degree;
		float nd_pdf[TIME_SLOT+1];
		float static_degree;
		int static_pdf[TIME_SLOT+1];

		float cluster_coefficient;

		void initiate();
		void setAngleSpeedArrays();
		void show_trace();
		void show_trace(int start, int end);
		void show_trace(int node, int start, int end);
		void read_trace(char *filename);
		void set_data();

		void cal_link(char* filename);
		void cal_node_degree(char* filename);
		void cal_static_degree(char* filename);

		//new functions (version 2.0) by Elmano
		float degree_of_spatial_dependence(); //DSD
		float degree_of_temporal_dependence(); //DTD
		float improved_degree_of_spatial_dependence(); //IDSD
		float high_improved_degree_of_spatial_dependence(); //HIDSD
		float improved_degree_of_temporal_dependence(); //IDTD
		float average_relative_speed();
		//void is_there_correlation_between_nodes_i_j(int i, int j);
		void print_spatial_dependence_statistics(int i, int j);
		void print_DSD(int i, int j, int start, int end);
		void print_IDSD(int i, int j, int start, int end);
		void print_HIDSD(int i, int j, int start, int end);
		void print_SpatialMetrics(int i, int j, int start, int end);
		bool is_stopped(int i, int t);
		bool stop_trip(int i, int t);
		bool velocity_has_changed(int i, int t);
		bool equal_or_almost_equal(float x, float y);
		void average_distance_and_coverage();
		float average_distance();
		float average_distance(int x, int y);
		float average_coverage();
		float average_coverage_i_j(int x, int y);
		float distance_i_j(int i, int j, int t);
		float SMAverage(int i, int j, int t, int periods);
		float WMAverage(int i, int j, int t, int periods);
		float EWMAverage(int i, int j, int t);
		float compute_mean_basic_temporal_correlation(int i, int t);
		void test_is_stopped(int x, int y);
		void test_velocity_has_changed(int x, int y);
		void print_distance_between_nodes();
		void average_std_distances();
		float degree_of_node_proximity();
		float improved_degree_of_node_proximity();
		float maximum_average_distance();
		float average_tripLength();
		float average_pathLength();
		bool node_is_stationary(int id, int t);
		bool is_at_previous_position(int id, int t);
		bool is_at_destination(int id, int t);
		bool no_next_xy(int id, int t);
		bool same_destination(int id, int t);
		void update_stationary_state(int id, int t);
		int update_moving_state(int id, int t);
		int start_new_movement(int id, int t);
		void printSingleTrace(int i, int t);
		void printPauseTimes(int i);
		void printPauseTimes();
		void printAngles(int i);
		void printAngles();
		void printSpeeds(int i);
		void printSpeeds();
		void printTripLengths(int i);
		void printTripLengths();
		void printPathLengths(int i);
		void printPathLengths();
		void fillNodesPauseTimes();
		void fillNodesSpeeds();
		void fillNodesAngles();
		void fillDistances();
		void fillNodesTripLengths();
		void fillNodesPathLengths();
		float getPositiveAngle(float angle);
		float speed_angle_rate(int i);
		float speed_angle_rate();
		float angle_variation_coefficient();
		float speed_variation_coefficient();
		float pause_variation_coefficient();
		float degreeOfSpatialDistribution();
		float positionDensityVariance();
		float emptyCells();
		int getLength(float array[], int max);
		void createIdentityVector(int size);
		void xy_axis_interval();
		float distanceTraveled(int id, int t1, int t2);
};


//Membership Function

void Trace::initiate() {  //inicializa vetores com valores -1 e 0
	int i, j, t;

	total_link_change = 0;

	for (i = 0; i < NODE_NUM; i++)
		for (j = 0; j < NODE_NUM; j++) {
			link_status[i][j] = -1;
			link_up_num[i][j] = 0; link_down_num[i][j] = 0;
			last_up_time[i][j] = -1; last_down_time[i][j] = -1;

			//average and std of distances between nodes
			average_distances[i][j] = 0.0;
			std_distances[i][j] = 0.0;
			//correlations and distances historic
			for (t = 0; t < TIME_SLOT; t++) {
				DSD[i][j][t] = 0.0;
				estimatedIDSD[i][j][t] = 0.0;
				estimatedHIDSD[i][j][t] = 0.0;
				temporalCorrelations[i][t] = 0.0;
				distances[i][j][t] = 0.0;
			}
		}
	for (i = 0; i < TIME_SLOT + 1; i++)
		ld_pdf[i] = 0;

	setAngleSpeedArrays();
}


void Trace::setAngleSpeedArrays() { //altera valor dos vetores para NIL = -999
	int id, k;
	//change the default array value (0 to NIL), since 0 has a meaning for both speed and angle.
	for (id = 0; id < NODE_NUM; ++id) {
		for (k = 0; k < NODE_ANGLE_SLOT; ++k)
			nodeAngles[id][k] = NIL;
		for (k = 0; k < NODE_SPEED_SLOT; ++k)
			nodeSpeeds[id][k] = NIL;
		for (k = 0; k < NODE_TRIP_LENGTH_SLOT; ++k)
			nodeTripLengths[id][k] = NIL;
		for (k = 0; k < NODE_PATH_LENGTH_SLOT; ++k)
			nodePathLengths[id][k] = NIL;
	}

	for (k = 0; k < NODES_ANGLE_SLOT; ++k)
		nodesAngles[k] = NIL;
	for (k = 0; k < NODES_SPEED_SLOT; ++k)
		nodesSpeeds[k] = NIL;
	for (k = 0; k < NODES_TRIP_LENGTH_SLOT; ++k)
		nodesTripLengths[k] = NIL;
	for (k = 0; k < NODES_PATH_LENGTH_SLOT; ++k)
		nodesPathLengths[k] = NIL;
}


void Trace::read_trace(char *filename) { //lê arquivo com traços gerados no formato NS-2
	FILE *trace_fp;
	int id = 0, t = 0, i = 0;

	char word[200], sentence[200], temp, c0, c1;
	double time, x, y, speed;
	double x0, y0, z0;

	trace_fp = fopen(filename,"r");
	if (trace_fp == NULL){
		printf("CANNOT find TRACE file: %s!\n",filename);
		exit(0);
	}

	while (!feof(trace_fp)) {
		temp = fgetc(trace_fp);
		if (temp=='#') {	//if the comment is '#', skip it
			if (fgets(sentence,200,trace_fp) == NULL)
				printf("Wrong format for # comment!\n");
		} else if (temp=='$') { //if not comment, analyze it
			//Check the two initial letters
			c0 = fgetc(trace_fp); c1 = fgetc(trace_fp);
			// $node_(i) set X_ 26.523097872900
			if (c0 == 'n' && c1 == 'o')	{
				for (i = 0; i < 4; i++)
					temp = fgetc(trace_fp);
				fscanf(trace_fp, "%d", &id); //id = id do nodo
				temp = fgetc(trace_fp);
				fscanf(trace_fp, "%s", word);
				fscanf(trace_fp, "%s", word);

				if (word[0] == 'X') {
					fscanf(trace_fp, "%lf", &x0); //x0 = posição do nodo no ponto x
					ini_x[id] = x0;
					trace[id][t].x = x0;
				}	else if(word[0]=='Y')	{
					fscanf(trace_fp, "%lf", &y0); //y0 = posição do nodo no ponto y0
					ini_y[id] = y0;
					trace[id][t].y = y0;
				}	else fscanf(trace_fp,"%lf",&z0);
			// $god_ set-dist 0 1 1677215
			} else if (c0=='g' && c1=='o') {
				if (fgets(sentence,200,trace_fp) == NULL)
					printf("Wrong format for $god_ argument!\n");
			// $ns_ at 30.000000234323 "$node_(1) setdest 534.67642310 435.43899348 43.367834743"
			// or $ns_ at 344.442322520850 "$god_ set-dist 0 1 7215"
			} else if (c0 == 'n' && c1 == 's') {
				temp = fgetc(trace_fp);
				fscanf(trace_fp, "%s" ,word);
				fscanf(trace_fp, "%lf" ,&time);

				t = (int)time;

				temp = fgetc(trace_fp); temp = fgetc(trace_fp); temp = fgetc(trace_fp); //< "$>
				c0 = fgetc(trace_fp);
				if (c0=='n')	{
					for (i = 0; i < 5; i++)
						temp = fgetc(trace_fp);
					fscanf(trace_fp, "%d", &id);
					temp = fgetc(trace_fp);

					fscanf(trace_fp, "%s", word);
					fscanf(trace_fp, "%lf", &x);
					fscanf(trace_fp, "%lf", &y);
					fscanf(trace_fp, "%lf", &speed);
					temp = fgetc(trace_fp);

					trace[id][t].id = id;
					trace[id][t].time = time;
					trace[id][t].next_x = x; //x aqui eh o x de destino (prox parada, q o no atingira apos t segundos) nao necessariamente o proximo x!
					trace[id][t].next_y = y;
					trace[id][t].speed = speed;
					trace[id][t].angle = 0;

					if (t==0) { //find the velocity angle and the duration of the first trip
						trace[id][t].x = ini_x[id];
						trace[id][t].y = ini_y[id];
						trace[id][t].angle = get_angle(trace[id][t].x, trace[id][t].y,
																					 trace[id][t].next_x, trace[id][t].next_y);
						trace[id][t].timetodestiny = (int) dist(trace[id][t].x, trace[id][t].y,
																										trace[id][t].next_x, trace[id][t].next_y) / trace[id][t].speed;
					}
				}	else if(c0=='g') {
						if(fgets(sentence,200,trace_fp) == NULL)
							printf("Wrong format for $ns $god_ argument!\n");
				}
			}// for "ns","node","god"
		}//for "# or $"
	}//for while
	fclose(trace_fp);
}



void Trace::set_data() {

	int id, t, t_aux, ttd = 0;
	//float distance; Not used

	//waypoints: a bidimensional array that stores the visiting points of a node
	waypoints = new struct point* [NODE_NUM];
	for (int i = 0; i < NODE_NUM; i++)
		waypoints[i] = new struct point[MAX_NUMBER_WAYPOINTS];

	for (id = 0; id < NODE_NUM; id++){
		//add the first waypoint (at t=0)
		waypoints[id][0].x = trace[id][0].x;
		waypoints[id][0].y = trace[id][0].y;
		waypoints[id][0].time = 0;

		t = t_aux = 0;

		while (node_is_stationary(id, t_aux) && t_aux < TIME_SLOT) {
			update_stationary_state(id, t_aux);
			t_aux++;
			//printSingleTrace(id,t_aux);
		}

		t = t_aux;

		int pindex = 0; //pause time array index
		int aindex = 0; //velocity angle array index
		int sindex  = 0; //speed array index
		int tripindex = 0;	//trip length array index
		int windex = 1; //waypoint index
		//int wtindex = 0; //waypoint time index (the time steps when nodes start to pause) Not used

		//if t>0 implies that node has been paused for 't' seconds
		if (t > 0) nodePauseTimes[id][pindex++] = t;

		int paux = 0;

		while (t < TIME_SLOT) {

			ttd = start_new_movement(id, t); //ttd is the Time To arrive at the next Destination

			if (aindex == 0) { //the first value must be stored
				nodeAngles[id][aindex++] = getPositiveAngle(trace[id][t].angle);
				//only store node's velocity angle iff it's different than the previous value
			} else if (t > 0 && !equal_or_almost_equal(trace[id][t].angle, trace[id][t - 1].angle)){
				nodeAngles[id][aindex++] = getPositiveAngle(trace[id][t].angle); //1..360 degrees (instead of using radians)
			}

			nodeSpeeds[id][sindex++] = trace[id][t].speed;

			for (t_aux = t + 1; ttd > 0 && t_aux < TIME_SLOT; t_aux++)
				ttd = update_moving_state(id, t_aux);

			if (t_aux >= TIME_SLOT)	break;

			t_aux--; //in order to check if node was previously stationary
			paux = t_aux;

			if (node_is_stationary(id, t_aux)) {
				//add one more waypoint
				waypoints[id][windex].x = trace[id][t_aux].x;
				waypoints[id][windex].y = trace[id][t_aux].y;
				waypoints[id][windex].time = t_aux;

				//store the distance between this and the previous waypoint
				nodeTripLengths[id][tripindex] = dist(waypoints[id][windex].x, waypoints[id][windex].y,
																							waypoints[id][windex - 1].x, waypoints[id][windex - 1].y);

				//now compute all the distance traveled between this and the previous waypoint
				nodePathLengths[id][tripindex++] = distanceTraveled(id, waypoints[id][windex - 1].time, waypoints[id][windex].time);
				windex++;
			}

			while (node_is_stationary(id, t_aux) && t_aux < TIME_SLOT) {
				update_stationary_state(id, t_aux);
				t_aux++;
			}

			//set the pause time duration
			if (trace[id][t_aux - 1].speed == 0 && t_aux > paux) //check if node was paused one time step before
				nodePauseTimes[id][pindex++] = t_aux - paux;

			if (t_aux >= TIME_SLOT)	break;
			t = t_aux;
		}
	}
	fillNodesPauseTimes();
	fillNodesSpeeds();
	fillNodesAngles();
	fillDistances();
	fillNodesTripLengths();
	fillNodesPathLengths();
}

//all the distance traveled between two time steps (measuring step by step)
float Trace::distanceTraveled(int id, int t1, int t2) { //t2>t1

	float x1, y1;
	float x2, y2;
	float distx, disty;
	float alldistance = 0;

	for (int t = t1 + 1; t <= t2; t++) {
		x1 = trace[id][t - 1].x;
		y1 = trace[id][t - 1].y;
		x2 = trace[id][t].x;
		y2 = trace[id][t].y;
		distx = x2 - x1;
		disty = y2 - y1;
		alldistance += sqrt(distx * distx + disty * disty);
	}
	return alldistance;
}

//If angle is 0, should this function return 360?
float Trace::getPositiveAngle(float a) {
	float angle = (180 * a) / PI;
	return angle > 0 ? angle : 360 - (-1) * angle; //?? 360 + angle?
}

void Trace::fillNodesPauseTimes() {
	int aux = 0, i, k;
	for (i = 0; i < NODE_NUM; ++i)
		for (k = 0; nodePauseTimes[i][k] > 0 && aux < NODES_PAUSE_TIME_SLOT; k++)
			nodesPauseTimes[aux++] = nodePauseTimes[i][k];
}

void Trace::fillNodesSpeeds() {
	int aux = 0, i, k;
	for (i = 0; i < NODE_NUM; ++i)
		for (k = 0; nodeSpeeds[i][k] != NIL && aux < NODES_SPEED_SLOT; k++)
			nodesSpeeds[aux++] = nodeSpeeds[i][k];
}

void Trace::fillNodesAngles() {
	int aux = 0, i, k;
	for (i = 0; i < NODE_NUM; ++i)
		for (k = 0; nodeAngles[i][k] != NIL && aux < NODES_ANGLE_SLOT; k++)
			nodesAngles[aux++] = nodeAngles[i][k];
}

void Trace::fillNodesTripLengths() {
	int aux = 0, i, k;
	for (i = 0; i < NODE_NUM; ++i)
		for (k = 0; nodeTripLengths[i][k] != NIL && aux < NODES_TRIP_LENGTH_SLOT; k++)
			nodesTripLengths[aux++] = nodeTripLengths[i][k];
}

void Trace::fillNodesPathLengths() {
	int aux = 0, i, k;
	for (i = 0; i < NODE_NUM; ++i)
		for (k = 0; nodePathLengths[i][k] != NIL && aux < NODES_PATH_LENGTH_SLOT; k++)
			nodesPathLengths[aux++] = nodePathLengths[i][k];
}

void Trace::fillDistances() {
	int i, j, t;
	for (i = 0; i < NODE_NUM; i++)
		for (j = i + 1; j < NODE_NUM; j++)
			for (t = 0; t < TIME_SLOT; t++)
				distances[i][j][t] = distance_i_j(i, j, t);
}

float Trace::speed_angle_rate(int i) {
	int numberOfSpeedValues = getLength(nodeSpeeds[i], NODE_SPEED_SLOT);
	int numberOfAngleValues = getLength(nodeAngles[i], NODE_ANGLE_SLOT);

	//MAX is used somewhere?
	// int MAX = NODE_SPEED_SLOT;
	// if (NODE_ANGLE_SLOT > NODE_SPEED_SLOT) MAX = NODE_ANGLE_SLOT;

	//numberOfAngleValues should never be <= 0
	return numberOfAngleValues > 0 ? numberOfSpeedValues / (float) numberOfAngleValues : 0;
}

float Trace::speed_angle_rate() {
	int numberOfSpeedValues = getLength(nodesSpeeds, NODES_SPEED_SLOT);
	int numberOfAngleValues = getLength(nodesAngles, NODES_ANGLE_SLOT);

	//MAX is used somewhere?
	// int MAX = NODES_SPEED_SLOT;
	// if (NODES_ANGLE_SLOT > NODES_SPEED_SLOT) MAX = NODES_ANGLE_SLOT;

	//numberOfAngleValues should never be <= 0
	return numberOfAngleValues > 0 ? numberOfSpeedValues / (float) numberOfAngleValues : 0;
}

float Trace::angle_variation_coefficient() {
	float mean = getAverageNotZero(nodesAngles, NODES_ANGLE_SLOT);
	float std = getStdNotZero(nodesAngles, mean, NODES_ANGLE_SLOT);
	return std / mean;
}

float Trace::speed_variation_coefficient() {
	float mean = getAverageNotZero(nodesSpeeds, NODES_SPEED_SLOT);
	float std = getStdNotZero(nodesSpeeds, mean, NODES_SPEED_SLOT);
	return std / mean;
}

float Trace::pause_variation_coefficient() {
	float mean = getAverageNotZero(nodesPauseTimes, NODES_PAUSE_TIME_SLOT);
	float std = getStdNotZero(nodesPauseTimes, mean, NODES_PAUSE_TIME_SLOT);

	return std / mean;
}

int Trace::getLength(float array[], int size) {
	int count = 0, k;
	for (k = 0; (k < size) && (array[k] != NIL); ++k)
		count++;
	return count;
}

void Trace::printPauseTimes(int i) {
	printf("Pause times of node %d:\n", i);
	for (int k = 0; k < NODE_PAUSE_TIME_SLOT; k++)
		printf("%f \t", nodePauseTimes[i][k]);
}

void Trace::printPauseTimes() {
	printf("Pause times of all nodes:\n");
	for (int k = 0; k < NODES_PAUSE_TIME_SLOT; k++)
		printf("%f \t", nodesPauseTimes[k]);
}

void Trace::printAngles(int i) {
	printf("Velocity angles of node %d:\n",i);
	for (int k = 0; nodeAngles[i][k] != NIL && k < NODE_ANGLE_SLOT; k++)
		printf("%f \n", nodeAngles[i][k]);
}

void Trace::printAngles() {
	printf("Velocity angles of all node:\n");
	for (int k = 0; nodesAngles[k] != NIL && k < NODES_ANGLE_SLOT; k++)
		printf("%f \n", nodesAngles[k]);
}

void Trace::printSpeeds(int i) {
	printf("Speeds of node %d:\n", i);
	for (int k = 0; nodeSpeeds[i][k] != NIL && k < NODE_SPEED_SLOT; k++)
		printf("%f \n", nodeSpeeds[i][k]);
}

void Trace::printSpeeds() {
	printf("Speeds of all nodes:\n");
	for (int k = 0; nodesSpeeds[k] != NIL && k < NODES_SPEED_SLOT; k++)
		printf("%f \n", nodesSpeeds[k]);
}

void Trace::printTripLengths(int i) {
	printf("Trip lengths for node %d:\n", i);
	for (int k = 0; nodeTripLengths[i][k] != NIL && k < NODE_TRIP_LENGTH_SLOT; k++)
		printf("%f \n", nodeTripLengths[i][k]);
}

void Trace::printTripLengths() {
	printf("Trip lengths for all nodes:\n");
	for (int k = 0; nodesTripLengths[k] != NIL && k < NODES_TRIP_LENGTH_SLOT; k++)
		printf("%f \n", nodesTripLengths[k]);
}

void Trace::printPathLengths(int i) {
	printf("Path lengths for node %d:\n", i);
	for (int k = 0; nodeTripLengths[i][k] != NIL && k < NODE_TRIP_LENGTH_SLOT; k++)
		printf("Path[%d] %f \t Trip[%d] % f \n", k, nodePathLengths[i][k], k, nodeTripLengths[i][k]);
}

void Trace::printPathLengths() {
	printf("Path lengths for all nodes:\n");
	for (int k = 0; nodesPathLengths[k] != NIL && k < NODES_TRIP_LENGTH_SLOT; k++)
		printf("%f \n", nodesPathLengths[k]);
		//printf("Path[%d] %f \t Trip[%d] % f \n", k, nodesPathLengths[k], k, nodesTripLengths[k]);
}

void Trace::printSingleTrace(int i, int t) {
	printf("trace[%d][%d] (%f,%f) (%f %f) %f %f \n", i, t, trace[i][t].x, trace[i][t].y,
			trace[i][t].next_x, trace[i][t].next_y,
			trace[i][t].speed, trace[i][t].angle / 3.14159265359 * 180);
}

int Trace::start_new_movement(int id, int t) {
	//starting a new trip: actual x,y should be the previous next_x, next_y
	if (t > 0) {
		trace[id][t].x = trace[id][t - 1].next_x;
		trace[id][t].y = trace[id][t - 1].next_y;
	}

	float distance = dist(trace[id][t].x, trace[id][t].y, trace[id][t].next_x, trace[id][t].next_y);

	trace[id][t].id = id;
	trace[id][t].time = t;
	trace[id][t].angle = get_angle(trace[id][t].x, trace[id][t].y, trace[id][t].next_x, trace[id][t].next_y);
	trace[id][t].timetodestiny = (int) distance / trace[id][t].speed + 1; //due the int is truncated during float conversion
	return trace[id][t].timetodestiny;
}

void Trace::update_stationary_state(int id, int t) {
	//t == 0 does nothing, because the same is done after the if statment
	if (t == 0) { //there is x,y but not next_x,y
		trace[id][t].next_x = trace[id][t].x;
		trace[id][t].next_y = trace[id][t].y;
	} else { //there is next_x,y but not x,y
		trace[id][t].x = trace[id][t - 1].next_x;
		trace[id][t].y = trace[id][t - 1].next_y;
	}

	trace[id][t].id = id;
	trace[id][t].time = t;
	trace[id][t].next_x = trace[id][t].x;
	trace[id][t].next_y = trace[id][t].y;
	trace[id][t].speed = 0.0; //correction if the trace was wrong (like in the SLAW code, where it is 1.0)
	trace[id][t].angle = 0.0;
	trace[id][t].timetodestiny = 0.0; //of course, node is not moving!
}

int Trace::update_moving_state(int id, int t) {

	//update new x,y values
	trace[id][t].id = id;
	trace[id][t].time = t;
	trace[id][t].x = trace[id][t - 1].x + trace[id][t - 1].speed * cos(trace[id][t - 1].angle);
	//printf("trace[id][t].x: %d \n",trace[id][t].x);
	trace[id][t].y = trace[id][t - 1].y + trace[id][t - 1].speed * sin(trace[id][t - 1].angle);
	trace[id][t].x = floor(trace[id][t].x * 100 +  0.5) / 100; //to avoid very small decimal values
	//printf("trace[id][t].x: %d \n",trace[id][t].x);
	trace[id][t].y = floor(trace[id][t].y * 100 +  0.5) / 100; //to avoid very small decimal values

	//BUG AQUI: pode acontecer de o no ja ter chegado quando ttd=2! ex: no 0 em t=68,95 (RPGM_d900_n10_x500_y500_a5_s0_r100_c0.0_l1_h10_p0_0)
	if(trace[id][t - 1].timetodestiny == 1 || (trace[id][t - 1].timetodestiny == 2 && trace[id][t].next_x != 0)) {
		//node arrived at destination at time t, then actual position should be the same as next position at t-1.
		trace[id][t].x = trace[id][t - 1].next_x;
		trace[id][t].y = trace[id][t - 1].next_y;
		trace[id][t].timetodestiny = 0;

		//if next_x,y is not zero than it should be equal to x,y (node is stationary now)
		if (trace[id][t].next_x == 0) {
			trace[id][t].next_x = trace[id][t].x;
			trace[id][t].next_y = trace[id][t].y;
		}
	} else { //node is still moving towards destination, then we should update the speed, angle and timetodestiny variables
		trace[id][t].speed = trace[id][t - 1].speed;
		trace[id][t].angle = trace[id][t - 1].angle;
		trace[id][t].timetodestiny = trace[id][t - 1].timetodestiny - 1;

		if (trace[id][t].next_x == 0 && trace[id][t].next_y == 0) { //i.e., there is no data about node id at time t in the trace file
			trace[id][t].next_x = trace[id][t-1].next_x;
			trace[id][t].next_y = trace[id][t-1].next_y;
		}

		/*if(is_at_destination(id,t)){ //node finally arrives at destination!
			trace[id][t].speed = 0.0; //SLAW FIX
			trace[id][t].angle = 0.0;
			trace[id][t].timetodestiny = 0; //maybe this is redundant
		}*/
	}
	return trace[id][t].timetodestiny;
}

bool Trace::node_is_stationary(int id, int t) {
	//initial stationary condition:
	if (t == 0 and trace[id][t].speed == 0){
		trace[id][t].next_x = trace[id][t].x;
		trace[id][t].next_y = trace[id][t].y;
		return true;
	}
	//node is stationary if previous speed is 0 and next_x,y is equal as previous x,y or next_x,y is 0

	return (is_at_destination(id, t) || trace[id][t].speed==0 ||
			(equal_or_almost_equal(trace[id][t - 1].x, trace[id][t].next_x) && equal_or_almost_equal(trace[id][t - 1].y, trace[id][t].next_y)));

	/*bool previous_paused = trace[id][t-1].speed == 0;
	return (   (previous_paused && (no_next_xy(id,t) || same_destination(id,t)) )
			 || is_at_previous_position(id,t) || is_at_destination(id,t)        );*/
}

bool Trace::is_at_previous_position(int id, int t) { //TO CHECK: IS THIS EVER USED ??
	return equal_or_almost_equal(trace[id][t].x, trace[id][t - 1].x) &&
			equal_or_almost_equal(trace[id][t].y, trace[id][t - 1].y);
}

bool Trace::is_at_destination(int id, int t) { //TO CHECK: IS THIS EVER USED ??
	return equal_or_almost_equal(trace[id][t].x, trace[id][t].next_x) &&
			equal_or_almost_equal(trace[id][t].y, trace[id][t].next_y);
}

bool Trace::same_destination(int id, int t) {
	return equal_or_almost_equal(trace[id][t].next_x, trace[id][t - 1].next_x) &&
			equal_or_almost_equal(trace[id][t].next_y, trace[id][t - 1].next_y);
}

bool Trace::no_next_xy(int id, int t) {
	return trace[id][t].next_x == 0 && trace[id][t].next_y == 0;
}

void Trace::show_trace() {
	int i, t;
	for(i = 0; i < NODE_NUM; i++)
		for(t = 0; t < TIME_SLOT; t++)
			printf("trace[%d][%d] (%f,%f) (%f %f) %f %f \n", i, t, trace[i][t].x, trace[i][t].y,
								trace[i][t].next_x, trace[i][t].next_y,
								trace[i][t].speed, trace[i][t].angle / 3.14159265359 * 180);
}

//shows the trace info from t:0 to lastTimestep
void Trace::show_trace(int start, int end) {
	int i, t;
	for (i = 0; i < NODE_NUM; i++)
		for(t=start;t<end;t++)
			printf("trace[%d][%d] (%f,%f) (%f %f) %f %f \n", i, t, trace[i][t].x, trace[i][t].y,
								trace[i][t].next_x, trace[i][t].next_y,
								trace[i][t].speed, trace[i][t].angle / 3.14159265359 * 180);
}

//shows the trace info of node i from t:start to t:end
void Trace::show_trace(int i, int start, int end) {
	for (int t = start; t < end; t++)
		printf("trace[%d][%d] (%f,%f) (%f %f) %f %f \n", i, t, trace[i][t].x, trace[i][t].y,
							trace[i][t].next_x, trace[i][t].next_y,
							trace[i][t].speed, trace[i][t].angle / 3.14159265359 * 180);
}

void Trace::cal_link(char *filename) {
	int t;
	int i, j;
	int current_status;
	FILE* fp;

	//t=0, initiate the link status;
	for (i = 0; i < NODE_NUM; i++)
		for (j = i + 1; j < NODE_NUM; j++)
			if(dist(trace[i][0].x, trace[i][0].y, trace[j][0].x, trace[j][0].y) <= RADIUS) {
				link_status[i][j] = 1; link_status[j][i] = 1;
				link_up_num[i][j] = 1; link_up_num[j][i] = 1;
				last_up_time[i][j] = 0; last_up_time[j][i] = 0;
			} else {	link_status[i][j]=0; link_status[j][i]=0;	}

	for (t = 1; t < TIME_SLOT; t++)
		for (i = 0; i < NODE_NUM; i++)
			for (j = i + 1; j < NODE_NUM; j++) {
				if (dist(trace[i][t].x, trace[i][t].y, trace[j][t].x, trace[j][t].y) <= RADIUS)
					current_status = 1;
				else current_status = 0;

				// link: 0 ---> 1  link comes up
				if (link_status[i][j] == 0 && current_status == 1) {
					link_up_num[i][j]++; link_up_num[j][i]++;
					last_up_time[i][j] = t; last_up_time[j][i] = t;
					link_status[i][j] = current_status;	link_status[j][i] = current_status;
				}
				// link: 1 ---> 0 link comes down
				if(link_status[i][j] == 1 && current_status == 0) {
					link_down_num[i][j]++; link_down_num[j][i]++;
					last_down_time[i][j] = t; last_down_time[j][i] = t;
					link_status[i][j] = current_status;	link_status[j][i] = current_status;
					ld_pdf[last_down_time[i][j] - last_up_time[i][j]]++;
				}
				//at the end of simulation, count the duration
				if(t == TIME_SLOT-1 && link_status[i][j] == 1 && current_status == 1)
					ld_pdf[(TIME_SLOT - last_up_time[i][j])]++;
 			}//for (j)

	for(i = 0; i < NODE_NUM; i++)
		for(j = i + 1; j < NODE_NUM; j++) {
			total_link_change += link_up_num[i][j];
			total_link_change += link_down_num[i][j];
		}

	avg_link_duration = mean(ld_pdf);
	fp = fopen(filename,"w");
	if(fp == NULL)
		{ printf("can not create the file\n"); return; }
	for(i = 0; i < TIME_SLOT; i++)
		fprintf(fp, "%d\n", ld_pdf[i]);
	fclose(fp);
}


void Trace::cal_node_degree(char* filename) {
	int i, j, k;
	int neighbor_all = 0;
	int neighbor_slot = 0;
	FILE *fp;

	for (k = 0; k < TIME_SLOT; k++) {
		neighbor_slot = 0;
		for (i = 0; i < NODE_NUM; i++)
			for (j = 0; j < NODE_NUM; j++)
				if (i!=j && dist(trace[i][k].x, trace[i][k].y,trace[j][k].x, trace[j][k].y) <= RADIUS)
					{	neighbor_all++; neighbor_slot++;}
		nd_pdf[k] = neighbor_slot / (float)NODE_NUM;
	}
	node_degree = neighbor_all / (float)(TIME_SLOT * NODE_NUM);

	fp = fopen(filename,"w");
	if (fp==NULL)
		{	printf("can not create the file\n"); return; }
	for (i = 0; i < TIME_SLOT; i++)
		fprintf(fp, "%f\n", nd_pdf[i]);
	fclose(fp);
}

void Trace::cal_static_degree(char* filename) {
	int i, k;
	int static_all = 0;
	int static_slot;
	FILE *fp;

	for (k = 0;k < TIME_SLOT; k++) {
		static_slot = 0;
		for (i = 0; i < NODE_NUM; i++)
			if (trace[i][k].speed == 0)
				{	static_all++; static_slot++; }
		static_pdf[k] = static_slot;
	}
	static_degree = static_all / (float)(TIME_SLOT);

	fp = fopen(filename,"w");
	if (fp == NULL)
		{	printf("can not create the file\n"); return; }
	for (i = 0; i < TIME_SLOT; i++)
		fprintf(fp, "%d\n", static_pdf[i]);
	fclose(fp);
}


//REMOTENESS function = STEP function (<2R,1; >2R,0)
float Trace::average_relative_speed() {
	float v1 = 0.0,v2 = 0.0,v3 = 0.0;
	int t, i, j;
	int count = 0;

	v3 = 0.0;
	for (t = 0; t < TIME_SLOT; t++) {
		v2 = 0.0; count = 0;
		for (i = 0; i < NODE_NUM; i++)
			for (j = i + 1; j < NODE_NUM; j++)
				if (distance_i_j(i, j, t) <= 2 * RADIUS) {
					v1 = relative_speed(trace[i][t].speed, trace[i][t].angle,
															trace[j][t].speed, trace[j][t].angle);
					v2 += v1;
					count++;
				}
		if(count!=0)	v3 = v3 + v2 / (float)count;
	}
	return v3 / (float)TIME_SLOT;
}

//Degree of Temporal Dependence - DTD
float Trace::degree_of_temporal_dependence() {
	float cor1_old = 0.0, cor2_old = 0.0, cor3_old = 0.0;
	int id, t, k;
	int count_old = 0;
	float DTD = 0.0;

	for (id = 0; id < NODE_NUM; id++) {
		cor2_old = 0.0; count_old = 0;
		for (t = 0; t < TIME_SLOT - 50; t++)
			for (k = t + 1; k < t + 50; k++)
				if (k - t < 50) { //ALWAYS TRUE!!!
					cor1_old = DSDijt(trace[id][t].speed, trace[id][t].angle,
														trace[id][k].speed, trace[id][k].angle);
					cor2_old += cor1_old;
					count_old++;
				}
		if (count_old!=0)	cor3_old = cor3_old + cor2_old / (float)count_old;
	}
	DTD = cor3_old / (float)NODE_NUM;
	return DTD;
}

//Improved Degree of Temporal Dependence - IDTD
float Trace::improved_degree_of_temporal_dependence() {
	float cor1 = 0.0, cor2 = 0.0, cor3 = 0.0;
	int i, t;
	int count = 0;
	float IDTD = 0.0;

	for (i = 0; i < NODE_NUM; i++) {
		cor2 = 0.0; count = 0;
		for (t = 1; t < TIME_SLOT; t++){
			if (velocity_has_changed(i, t)) {
				cor1 = basic_correlation_positive(trace[i][t].speed, trace[i][t].angle,
																					trace[i][t - 1].speed, trace[i][t - 1].angle);
				cor2 += cor1;
				temporalCorrelations[i][t] = cor1; //this is not used yet
				count++;
			}
			//IMPROVEMENT: 12/05/2011
			//else{ //node has uniform movement or is stationary. Then, should consider the last k movements
			//	cor1 = compute_mean_basic_temporal_correlation(i,t-1);
			//}
		}
		if (count != 0)	cor3 = cor3 + cor2 / (float)count;
	}
	IDTD = cor3 / (float)NODE_NUM;
	return IDTD;
}

//Degree of Spatial Dependence - DSD
float Trace::degree_of_spatial_dependence() {
	float cor1 = 0.0, cor2 = 0.0, cor3 = 0.0;
	int t, i, j;
	int count_old = 0;

	for (t = 0; t < TIME_SLOT; t++) {
		cor2 = 0.0; count_old = 0;
		for (i = 0; i < NODE_NUM; i++)
			for (j = i + 1; j < NODE_NUM; j++)
				if (distance_i_j(i,j,t) <= 2*RADIUS) {
					cor1 = DSDijt(trace[i][t].speed, trace[i][t].angle,
												trace[j][t].speed, trace[j][t].angle);
					DSD[i][j][t] = cor1;
					cor2 += cor1;
					count_old++;
				}
		if (count_old != 0)	cor3 = cor3 + cor2 / (float)count_old;
	}
	return cor3 / (float)TIME_SLOT;
}

//Improved Degree of Spatial Dependence - IDSD
float Trace::improved_degree_of_spatial_dependence() {
	float cor1 = 0.0, cor2 = 0.0, cor3 = 0.0;
	int t, i, j;
	int count = 0;
	float IDSD = 0.0;

	//clean array estimatedIDSD
	for (i = 0; i < NODE_NUM; i++)
		for (j = i + 1; j < NODE_NUM; j++)
			for (t = 0; t < TIME_SLOT; t++)
				estimatedIDSD[i][j][t] = 0.0;

	for (t = 0; t < TIME_SLOT; t++) {
		cor2 = 0.0; count = 0;
		for (i = 0; i < NODE_NUM; i++)
			for (j = i + 1; j < NODE_NUM; j++)
				if (distance_i_j(i, j, t) <= 2 * RADIUS) {
					if (is_stopped(i, t) || is_stopped(j, t)) {
						if (TIMEPAUSE > 0) cor1 = SMAverage(i, j, t, TIMEPAUSE / 10);
						else cor1 = DSDijt(trace[i][t].speed, trace[i][t].angle,
															 trace[j][t].speed, trace[j][t].angle);
					} else {
						cor1 = DSDijt(trace[i][t].speed, trace[i][t].angle,
													trace[j][t].speed, trace[j][t].angle);
					}
					estimatedIDSD[i][j][t] = cor1;
					cor2 += cor1;
					count++;
				} else { //if distance > 2R
					estimatedIDSD[i][j][t] = 0.0;
				}
		if (count!=0) cor3 = cor3 + cor2/(float)count;
	}

	IDSD = cor3 / (float)TIME_SLOT;
	return IDSD;
}

//High Improved Degree of Spatial Dependence - HIDSD
float Trace::high_improved_degree_of_spatial_dependence() {
	float cor1 = 0.0, cor2 = 0.0, cor3 = 0.0;
	int t, i, j;
	int count = 0;
	float IDSD = 0.0;

	//clean array estimatedHIDSD
	for (i = 0; i < NODE_NUM; i++)
		for (j = i + 1; j < NODE_NUM; j++)
			for (t = 0; t < TIME_SLOT; t++)
				estimatedHIDSD[i][j][t] = 0.0;

	//set initial pause time period correlations (undetermined value)
	t = 0;
	for (i = 0; i < NODE_NUM; i++)
		for (j = i + 1; j <NODE_NUM; j++)
			while (distance_i_j(i, j, t) <= RADIUS && is_stopped(i, t) && is_stopped(j, t))
				{ estimatedHIDSD[i][j][t] = NIL; t++; }

	for (t = 0; t < TIME_SLOT; t++) {
		cor2 = 0.0; count = 0;
		for (i = 0; i < NODE_NUM; i++)
			for (j = i + 1; j < NODE_NUM; j++)
				if (distance_i_j(i, j, t) <= 2 * RADIUS && estimatedIDSD[i][j][t] != NIL) {
					//We should remove the initial stationary period since it is impossible to ensure whether there is or no correlation
					if (!is_stopped(i, t) && !is_stopped(j, t)) {
						//CASE 1: both nodes are moving
						cor1 = EWMAverage(i, j, t); //we have the actual SD value and past values
					}	else if (TIMEPAUSE <= 0) { //results the same as DSD
						cor1 = DSDijt(trace[i][t].speed, trace[i][t].angle,
									  			trace[j][t].speed, trace[j][t].angle);
					}	else if ((is_stopped(i, t) && !is_stopped(j, t)) || (!is_stopped(i, t) && is_stopped(j, t)) ) {
						//CASE 2: one node is moving and the other is stationary
						cor1 = WMAverage(i, j, t, TIMEPAUSE / 10); //we only have past SD values

						//O TIMEPAUSE sera obtido por cada dispositivo e compartilhado, sempre que o veiculo parar o novo valor eh incrementado
						//para calcular o ATP basta dividir o tempo total de pausa pelo tempo em que o sistema foi ligado.
						//TODO: considerar se o no se aproxima ou se afasta do outro? Caso se aproxime aumenta a correlacao, caso se afaste diminui?
					}	else {
						//CASE 3: both nodes are stationary
						cor1 = WMAverage(i, j, t, TIMEPAUSE / 10);  //we only have past SD values
					}
					estimatedHIDSD[i][j][t] = cor1; //keeps the last correlations between the nodes i,j
					if (estimatedHIDSD[i][j][t] != NIL) { cor2 += cor1; count++; } //considera apenas os valores validos
				}	else estimatedHIDSD[i][j][t] = 0.0; //if distance > 2R
		if (count != 0) cor3 = cor3 + cor2 / (float)count;
	}
	//int numberofpairs = NODE_NUM * (NODE_NUM -1) / 2;
	IDSD = cor3 / (float)TIME_SLOT;
	return IDSD;
}

//METHOD 1: Simple Moving Average (MSS) (the same as arithmetic mean)
//This function is only called by IDSD()
float Trace::SMAverage(int i, int j, int t, int periods) {
	float cor = 0.0;
	for (int k = 1; k <= periods && t - k >= 0 && estimatedIDSD[i][j][t] != NIL; k++)
		cor += estimatedIDSD[i][j][t-k];
	return cor/periods;
}

//METHOD 2: Weighted Moving Average (WMA)
//This function is only called by HIDSD()
float Trace::WMAverage(int i, int j, int t, int periods) {
	float cor = 0.0;
	int count = 0;
	//check if nodes are stationary since simulation start; if yes, correlation should be undefined (-99)
	if (is_stopped(i, t) && is_stopped(j, t) && (t == 0 || estimatedHIDSD[i][j][t - 1] == NIL))
		return NIL;

	for (int k = 0; k < periods && t - k > 0; k++) //vai ao passado ate onde havia correlacao valida
		if (estimatedHIDSD[i][j][t - k - 1] != NIL) {
			cor += (periods - k) * estimatedHIDSD[i][j][t - k - 1]; //peso decrescente (PA com razao = 1)
			count += periods - k; // N(N+1)/2 soma dos termos de uma PA 1..periods
		}	else break;
	return count == 0 ? 0.0 : cor / (float)count;
}

//METHOD 3: Exponential Weighted Moving Average (EWMA)
//This function is only called by HIDSD()
float Trace::EWMAverage(int i, int j, int t) {
	// variante da EWMA, onde alfa pode ser igual a 1/N ou 2/(N+1) (N pode ser o tempo de pausa medio)
	float alfa = 0.125;
	if (t == 0 || (estimatedHIDSD[i][j][t - 1] == NIL)) //there is no past values
		return DSDijt(trace[i][t].speed, trace[i][t].angle, trace[j][t].speed, trace[j][t].angle);
	else return (1 - alfa) * estimatedHIDSD[i][j][t - 1] + alfa *
							DSDijt(trace[i][t].speed, trace[i][t].angle, trace[j][t].speed, trace[j][t].angle);//[Roberts (1959)]
		/* Uma variante eh usar DSD(i,j,t-1) [Hunter (1986)] ao inves de DSD(i,j,t) [Roberts (1959)] como valor atual da correlacao.
		 * => http://www.itl.nist.gov/div898/handbook/pmc/section4/pmc431.htm
		 */
		//return (1-alfa)*estimatedHIDSD[i][j][t-1] + alfa*DSDijt(trace[i][t-1].speed,trace[i][t-1].angle,trace[j][t-1].speed,trace[j][t-1].angle);//[Hunter (1986)]
}


float Trace::compute_mean_basic_temporal_correlation(int i, int t) {
	float cor = 0.0;
	int iterations = 10; //number of time steps to go back in the history of past movements
	int count = 0;

	for (int k = 0; k < iterations && t - k >= 0; k++)
		if (velocity_has_changed(i,t-k)) {
			cor += temporalCorrelations[i][t-k]; //ja foi preenchido esse array???
			count++;
		}

	return count==0 ? 0.0 : cor/(float)count;
}

void Trace::print_spatial_dependence_statistics(int i, int j) {
	//based on average_distance() and within_coverage() functions
	float distance = 0.0;
	float all_distance = 0.0;
	int count_coverages = 0;
	int count_both_moving = 0;
	int count_one_moving = 0;
	int count_both_stationary = 0;
	int count_distance_greater2R = 0;

	for (int t = 0; t < TIME_SLOT; t++){
		distance = dist(trace[i][t].x, trace[i][t].y,
						 				trace[j][t].x, trace[j][t].y);
		all_distance += distance;
		if (distance_i_j(i, j, t) <= 2 * RADIUS) {
			if (distance <= RADIUS)	count_coverages += 1;
			if (!is_stopped(i, t) && !is_stopped(j, t)) count_both_moving++; //CASE 1: both nodes are moving
			else if ((is_stopped(i,t) && !is_stopped(j,t)) ||
							 (!is_stopped(i,t) && is_stopped(j,t)))	count_one_moving++; //CASE 2: one node is moving and the other is stationary
			else count_both_stationary++; //CASE 3: both nodes are stationary
		} else count_distance_greater2R++;
	}
	// float average_coverage = count_coverages / (float)TIME_SLOT; Not used
	// float average_distance = all_distance / (float)TIME_SLOT; Not used

	//magic formula
	printf("count_distance_greater2R = %d\n", count_distance_greater2R);
	printf("count_both_moving = %d\n", count_both_moving);
	printf("count_one_moving = %d\n", count_one_moving);
	printf("count_both_stationary = %d\n", count_both_stationary);
}


void Trace::average_std_distances() {
	//average measurement
	float sum = 0.0;
	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			sum = 0.0;
			for (int t = 0; t < TIME_SLOT; t++) sum += distances[i][j][t];
			average_distances[i][j] = sum / TIME_SLOT;
			if (i == 0 && j == 1)
				printf("average_distances[%d][%d]= %f\n", i, j, average_distances[i][j]);
		}
	//std measurement
	float variation = 0.0;
	sum = 0.0;
	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			for (int t = 0; t < TIME_SLOT; t++){
				variation = distances[i][j][t] - average_distances[i][j];
				variation = variation * variation;
				sum += variation;
			}
			std_distances[i][j] = sqrt(sum / TIME_SLOT);
			if (i == 0 && j == 1)
			printf("std_distances[%d][%d]= %f\n", i, j, std_distances[i][j]);
		}
}

float Trace::degree_of_node_proximity() {
	float sum_distances_all = 0.0;
	float average_distance_all = 0.0;
	int count = 0;

	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			sum_distances_all += average_distance(i, j) / RADIUS; //distances[i][j] is filled
			count += 1;
		}
	average_distance_all = sum_distances_all / (float)count;
	float DNP = 1 - average_distance_all / maximum_average_distance();
	return DNP;
}

float Trace::improved_degree_of_node_proximity() {
	float next_CV = 0.0;
	float sum_CVs = 0.0;
	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			next_CV = coefficient_of_variation(average_distances[i][j], std_distances[i][j]);
			sum_CVs += next_CV;
			if (i==2) printf("CV[%d][%d]=%f \n",i,j,next_CV);
		}
	return sum_CVs;
}

/*void Trace::printPauseTimes(){

	//joint all pause times durations of all nodes
	float allPauseTimes[NODES_PAUSE_TIME_SLOT]; //lembrar de desconsiderar os valores 0, reduzir para array que contenha so valores validos
	int aux=0, j=0;

	for (int i = 0; i < NODE_NUM; ++i) {
		j=0;
		while (nodePauseTimes[i][j] > 0 && aux<NODES_PAUSE_TIME_SLOT){
			allPauseTimes[aux++] = nodePauseTimes[i][j];
			j++;
		}
	}
	printf("All pause time durations of all nodes:\n");
	for (int k=0; (k<1000 && allPauseTimes[k]>0); ++k) {
		printf("%f \n", allPauseTimes[k]);
	}

}*/

float Trace::maximum_average_distance() {
	return sqrt(SCENARIO_LENGTH * SCENARIO_LENGTH + SCENARIO_WIDTH * SCENARIO_WIDTH) / (2 * RADIUS);
}

//show the x-axis and y-axis interval (min and max)
void Trace::xy_axis_interval() {
	float xmin = trace[0][0].x;
	float ymin = trace[0][0].y;
	float xmax = trace[0][0].x;
	float ymax = trace[0][0].y;
	for (int t = 0; t < TIME_SLOT; t++)
		for (int i = 0; i < NODE_NUM; i++) {
			if (trace[i][t].x < xmin)	xmin = trace[i][t].x;
			else if (trace[i][t].x > xmax) xmax = trace[i][t].x;
			if (trace[i][t].y < ymin)	ymin = trace[i][t].y;
			else if (trace[i][t].y > ymax) ymax = trace[i][t].y;
		}
	printf("x-axis interval=[%f, %f]\n",xmin,xmax);
	printf("y-axis interval=[%f, %f]\n",ymin,ymax);
}

float Trace::average_tripLength() {
	float avg = getAverageNotZero(nodesTripLengths,NODES_TRIP_LENGTH_SLOT);
	//float std = getStdNotZero(nodesTripLengths,avg,NODE_NUM);
	return avg / RADIUS;
}

float Trace::average_pathLength() {
	float avg = getAverageNotZero(nodesPathLengths,NODES_PATH_LENGTH_SLOT);
	//float std = getStdNotZero(nodesTripLengths,avg,NODE_NUM);
	return avg / RADIUS;
}

float Trace::degreeOfSpatialDistribution() {
	int t,i,x,y;

	GRID_WIDTH = NODE_NUM;
	GRID_LENGTH = NODE_NUM;
	CELL_WIDTH = (int) SCENARIO_WIDTH / (GRID_WIDTH);
	CELL_LENGTH = (int) SCENARIO_LENGTH / (GRID_LENGTH);

	//1) initialize the cells matrix
	for (x = 0; x < GRID_WIDTH; x++)
		for (y = 0; y < GRID_LENGTH; y++)
			cells[x][y] = 0; //means no node inside this cell

	//2) fill the cells matrix
	for (t = 0; t < TIME_SLOT; t++)
		for (i = 0; i < NODE_NUM; i++) {
			x = trace[i][t].x / CELL_WIDTH;
			y = trace[i][t].y / CELL_LENGTH;
			cells[x][y]++;
		}

	/*printf("BEFORE PROCESSING: \n");
		printf("LOCATION MATRIX AT TIME %d: \n", t);
		for (x=0; x < GRID_WIDTH; x++){
			for (y=0; y < GRID_LENGTH; y++){
				//printf("cells[%d,%d,%d]=%d",t,x,y,cells[t][x][y]);
				printf("%d	",cells[x][y]);
			}
			printf("\n");
		}
		*/

	int sumX, sumY;

	//3) calculates the sum of all lines and columns of the grid (inserting the values into the last line and column of the grid)

	//filling the last line with the sum of all values
	for (x = 0; x < GRID_WIDTH; x++) {
		for (sumY = y = 0; y < GRID_LENGTH; y++)
			sumY += cells[x][y];
		cells[x][GRID_LENGTH] = sumY;
		//if (t>0) cells[t][x][GRID_LENGTH] = cells[t-1][x][GRID_LENGTH] + sumY;
	}
	//filling the last column with the sum of all values
	for (y = 0; y < GRID_LENGTH; y++) {
		for (sumX = x = 0; x < GRID_WIDTH; x++)
			sumX += cells[x][y];
		cells[GRID_WIDTH][y] = sumX;
		//if (t>0) cells[t][GRID_WIDTH][y] = cells[t-1][GRID_WIDTH][y] + sumX;
	}

	/*printf("AFTER PROCESSING: \n");
	printf("LOCATION MATRIX AFTER COMPUTATION AT TIME %d: \n", t);
	for (x=0; x <= GRID_WIDTH; x++){
		for (y=0; y <= GRID_LENGTH; y++){
			//printf("cells[%d,%d,%d]=%d",t,x,y,cells[t][x][y]);
			printf("%d	",cells[x][y]);
		}
		printf("\n");
	}
	*/

	//4) calculates the deviation from uniform distribution
	float deviation = 0;
	float deviationSum = 0;

	for (x = 0; x < GRID_WIDTH; x++) {
		deviation = abs(cells[x][GRID_LENGTH] - TIME_SLOT);
		deviationSum += deviation;
	}
	for (y = 0; y < GRID_LENGTH; y++) {
		deviation = abs (cells[GRID_WIDTH][y] - TIME_SLOT);
		deviationSum += deviation;
	}
	float MAX_DEVIATION = 4 * (NODE_NUM - 1) * TIME_SLOT;
	degreeSpatialDistribution = 1 - log(deviationSum) / log(MAX_DEVIATION);
	return degreeSpatialDistribution;
}

float Trace::emptyCells() {
	float emptyCells = 0;
	for (int x = 0; x < GRID_WIDTH; x++)
		for (int y = 0; y < GRID_LENGTH; y++)
			if (cells[x][y] == 0)	emptyCells++;
	return emptyCells / (GRID_WIDTH * GRID_LENGTH);
}

//Position Density Variance metric
//Authors of the paper: 'Feature selection for user motion pattern recognition in mobile networks'
float Trace::positionDensityVariance() {
	float average = 0;
	// float PDV; Not used
	for (int x = 0; x < GRID_WIDTH; x++)
		for (int y = 0; y < GRID_LENGTH; y++)
			average += cells[x][y];
	average = average / (GRID_WIDTH * GRID_LENGTH);
	//PDV
	//What shoud it return?
	return average;
}

// void Trace::createIdentityVector(int size) {
// 	int vector[NODE_NUM];
// 	for (int i = 0; i < NODE_NUM; ++i)
// 		vector[i] = 1;
// }

void Trace::print_DSD(int i, int j, int start, int end) {
	float media = 0.0;
	int count = 0;
	for (int t = start; t < end; t++) {
		printf("%f\n", DSD[i][j][t]);
		media += DSD[i][j][t];
		count++;
	}
	printf("DSD = %f\n", media / (float)count);
}

void Trace::print_IDSD(int i, int j, int start, int end) {
	float media = 0.0;
	int count = 0;
	for (int t = start; t < end; t++) {
		printf("%f\n", estimatedIDSD[i][j][t]);
		media += estimatedIDSD[i][j][t];
		count++;
	}
	printf("IDSD = %f\n", media / (float)count);
}

void Trace::print_HIDSD(int i, int j, int start, int end) {
	float media = 0.0;
	int count = 0;
	//printf("%f\n", 0.11111111111111111);
	for (int t = start; t < end; t++) {
		printf("%f\n", estimatedHIDSD[i][j][t]);
		if (estimatedHIDSD[i][j][t] != NIL) { //desconsidera os valores iniciais de pausa (correlation undefined)
			media += estimatedHIDSD[i][j][t];
			count++;
		}
	}
	printf("HIDSD = %f\n", media/(float)count);
}

void Trace::print_SpatialMetrics(int i, int j, int start, int end) {
	printf("DSD   IDSD   HIDSD\n");
	for (int t = start; t < end; t++)
		printf("%d %f   %f   %f\n", t, DSD[i][j][t], estimatedIDSD[i][j][t], estimatedHIDSD[i][j][t]);
}

void Trace::print_distance_between_nodes() {
	for(int i = 0; i < NODE_NUM; i++)
		for(int j = i + 1; j < NODE_NUM; j++)
			for(int t = 0; t < TIME_SLOT; t++)
				printf("distances[%d][%d][%d]%% = %f\n", i, j, t, distances[i][j][t]);
}

float Trace::average_distance() {
	float average_distance_all = 0.0;
	int count = 0;
	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			average_distance_all += average_distance(i, j) / RADIUS;
			count += 1;
		}
	return average_distance_all / (float)count;
}

float Trace::average_distance(int i, int j) {
	float distance = 0.0;
	float all_distances = 0.0;
	for (int t = 0; t < TIME_SLOT; t++) {
		distance = dist(trace[i][t].x, trace[i][t].y,
						 				trace[j][t].x, trace[j][t].y);
		all_distances += distance;
		distances[i][j][t] = distance / RADIUS;
	}
	return all_distances / (float)TIME_SLOT;
}

float Trace::average_coverage() {
	int count = 0;
	float average_coverage_all = 0.0;
	for (int i = 0; i < NODE_NUM; i++)
		for (int j = i + 1; j < NODE_NUM; j++) {
			average_coverage_all += average_coverage_i_j(i, j);
			count += 1;
		}
	return average_coverage_all / (float)count;
}

float Trace::average_coverage_i_j(int i, int j) {
	int count = 0;
	for(int t = 0; t < TIME_SLOT; t++){
		if (dist(trace[i][t].x, trace[i][t].y,
				 		 trace[j][t].x, trace[j][t].y) <= RADIUS)
			count += 1;
	}
	return (float)count / (float)TIME_SLOT;
}

float Trace::distance_i_j(int i, int j, int t){
	float x0 = trace[i][t].x;
	float y0 = trace[i][t].y;
	float x1 = trace[j][t].x;
	float y1 = trace[j][t].y;
	return dist(x0,y0,x1,y1);
}

bool Trace::stop_trip(int i, int t) {//node,time
	if (t == 0) return true;
	//node i is stopped at time t iff at time t-1 it were at the same location
	float x0 = trace[i][t - 1].x;
	float x1 = trace[i][t].x;
	float y0 = trace[i][t - 1].y;
	float y1 = trace[i][t].y;

	float nx1 = trace[i][t].next_x;
	float ny1 = trace[i][t].next_y;
	return dist(x0, y0, x1, y1) == 0.0 || dist(x1, y1, nx1, ny1) == 0.0 ? true : false;
}

bool Trace::is_stopped(int i, int t) {//node,time
	return (trace[i][t].speed == 0) ? true : false;
}

bool Trace::velocity_has_changed(int i, int t) {
	if (t == 0) return false;
	float speed0 = trace[i][t - 1].speed;
	float speed1 = trace[i][t].speed;
	float angle0 = trace[i][t - 1].angle;
	float angle1 = trace[i][t].angle;

	if (equal_or_almost_equal(speed0,speed1) &&
		equal_or_almost_equal(angle0,angle1))
		return false;
	else return true;
}

bool Trace::equal_or_almost_equal(float x, float y) {
	if (x == y)	return true;
	else if (x > y) return x - y < 0.01 ? true : false;
	else return y - x < 0.01 ? true : false;
}

void Trace::test_is_stopped(int x, int y) {
	if (is_stopped(x, y)) printf("Node %d is stopped at time %d\n", x, y);
}

void Trace::test_velocity_has_changed(int x, int y) {
	if (velocity_has_changed(x,y))
		printf("Node'velocity %d has changed at time %d\n",x, y);
}
