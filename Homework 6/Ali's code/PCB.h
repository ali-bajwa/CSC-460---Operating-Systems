/*============================================================
DESCRIPTION:		This file contains the source code of the
					 PCB structure along with overloaded
					 comparison operators.
============================================================*/

struct PCB {
	int arrivalTime;
	int CPUburst;
	int waitingTime;
	
	bool operator() (const PCB& lhs, const PCB&rhs) const
	{
		return (lhs.CPUburst > rhs.CPUburst);
	}
	
	bool operator< (const PCB&rhs) const
	{
		return (this->CPUburst > rhs.CPUburst);
	}
	
	bool operator() (const PCB&rhs) const
	{
		return (this->CPUburst > rhs.CPUburst);
	}
};