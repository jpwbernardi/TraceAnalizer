# TraceAnalyzer

After compiling the code, use the following command to make the tool calculates the mobility metrics:
```
./TraceAnalyzer &lt;NS2-TRACE-FILE&gt; &lt;NODE-PAUSE-TIME&gt; &lt;RANGE&gt; &lt;WIDTH&gt; &lt;LENGTH&gt;
```

Where:

* &lt;NS2-TRACE-FILE&gt; 	    : The ns-2 trace file format.
* &lt;NODE-PAUSE-TIME&gt; 	: The average node pause time. This value is employed for computing the IDSD metric.
* &lt;RANGE&gt;        		: The node transmission range.
* &lt;WIDTH&gt; 		        : The width of the simulation scenario (in meters). This info is employed only for computing the DNP metric.
* &lt;LENGTH&gt;       		: The length of the simulation scenario (in meters). This info is employed only for computing the DNP metric.
