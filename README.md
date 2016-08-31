# TraceAnalizer

After compiling the code, use the following command to make the tool calculates the mobility metrics:

./TraceAnalyzer <NS2-TRACE-FILE> <NODE-PAUSE-TIME> <RANGE> <WIDTH> <LENGTH>

Where:

<NS2-TRACE-FILE> 	: The ns-2 trace file format.
<NODE-PAUSE-TIME> 	: The average node pause time. This value is employed for computing the IDSD metric.
<RANGE> 		: The node transmission range.
<WIDTH> 		: The width of the simulation scenario (in meters). This info is employed only for computing the DNP metric.
<LENGTH>		: The length of the simulation scenario (in meters). This info is employed only for computing the DNP metric.
