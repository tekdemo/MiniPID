#ifndef MINIPID_H
#define MINIPID_H

class MiniPID{
public:
	MiniPID(double p, double i, double d);
	MiniPID(double p, double i, double d, double f);
	void setP(double p);
	void setI(double i);
	void setD(double d);
	void setF(double f);
	void setPID(double p, double i, double d);
	void setPID(double p, double i, double d,double f);
	void setMaxIOutput(double maximum);
	void setMaxOutput(double output);
	void setMaxOutput(double minimum,double maximum);
	void setDirection(bool reversed);
	void setSetpoint(double setpoint);
	void reset();
	void setOutputRampRate(double rate);
	void setSetpointRange(double range);
	void setOutputFilter(double strength);
	double getOutput();
	double getOutput(double actual);
	double getOutput(double actual, double setpoint);
};
#endif
