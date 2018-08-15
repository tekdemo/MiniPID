# miniPID
This is a small, fully self contained PID class designed to help provide simple, efficient tuning, and simple integration wherever any level of PID control might be needed. 

All code is contained in a single C++ class, and doesn't depend on any specific compiler or toolchain. However, example Arduino sketches are included.

## Design Goals
- Provide all expected features of a quality PID loop. 
- Allow for stability without extensive tuning. 
- Be simple to integrate into projects without code restructuring
- Be easy to "chain", sending the output of one PID to the input of another. 
- Be flexible enough that any provided functions can be used in isolation. 
- Be simple enough to be used in "transient" or one-off control sequences 

## Features
##### PID Functions
True to the name, the main purpose of this class is PID control.

##### Feed Forward rate setting. 
Provides an "Expected output", helpful when doing velocity control systems. 

##### Setpoint Range
Force the PID system to cap the setpoint to a range near the current input values. This allows you to tune the PID in a smaller range, and have it perform sanely during large setpoint changes. 

##### Output ramping
Allows a maximum rate of change on the output, preventing jumps in output during setpoint changes

##### Output Range
Adjustable min and maximum range, so the output can directly drive a variety of systems. 

##### Output Filtering
Helps smooth the output, preventing high-frequency oscillations.

##### I term restriction
Allows you to specify a maximum output value the I term will generate. This allows active error correction, with minimal risk of I-term windup. 

##### Smart I term buildup
The I term and summed error will never increase if the system is already doing everything permitted to correct the system error. 

##### Simple API. 
No need for lots of convoluted calculation functions, or asyncronous calculation modes. After configuration, `getOutput()` is probably the only function you need. 

## Usage
A bare bones PID system could look like this. 

``` cpp
MiniPID pid=MiniPID(1,0,0);
//set any other PID configuration options here. 

while(true){
  //get some sort of sensor value
  //set some sort of target value
  double output=pid.getOutput(sensor,target);
  //do something with the output
  delay(50);
}
```
That's it. No fuss, no muss. A few lines of code and some basic tuning, and your PID system is in place. 

### Getting and setting outputs
There's several ways. In simple systems, the fastest is  `output=pid.getOutput(sensor,target);`. This does all the calculations with the current values, applying any configuration, and returns the output. 

For more event-driven systems, you can split up the setting and getting, using `output=pid.getOutput(sensor);` and `pid.setSetpoint(target)`.

If your outputs are to be disabled or driven by a different system, then you may want to use the `pid.reset()` method. That will set the PID controller to re-initialize the next time  `getOutput()` is used. Because of this, `reset()` can be used when disabling PID control, when reenabling it, or at any point between.

### Configuring the PID. 
The most complex part of PID systems is the configuration. Tuning a PID process properly typically requires either significant calculation, significant trial and error, or both. 

This library is designed to produce "decent" PID results with minimal effort and time investment, by providing more extensive configuration options than most controllers.

Note, PID systems work best when the calculations are performed at constant time intervals. This PID implimentation does not handle this, and assumes the primary loop or framework handles the precise timing details.

#### `MiniPID(p,i,d)`
#### `MiniPID(p,i,d,f)`

These create the basic PID, allowing for further configuration. Generally, you initialize the PID values here, but you can re-configure on the fly using the `setP(double P)`,`setI(double I)`,`setD(double D)`, and `setF(double F)` methods. 

Tuning PID systems is out of scope of this readme, but a good examples can be found all over the internet. 

#### `SetF(double F)`
Feed-Forward is a 4th system variable that is very helpful on systems with a target velocity, or other systems where an on-target system results in continous motion. Feed forward is not helpful on positional control systems, or other systems where being on target results in halted (or small cyclic) motion.

Conceptually, Feed-forward defines a "best guess" as to what the system output should be for a given setpoint value. Feed forward does not consider what the system is _actually_ doing, and a system driven solely by feed-forward is actually an open-loop system. Mathematically, a feed-forward only system is equivilent to `output=setpoint*F`. 

In most cases, the F term can be calculated very simply by running an output at full speed, and measuring your sensor rate. The F term is then `max_output_value/max_sensor_rate`.

For this class of systems, it's helpful to consider the F term the primary variable, and configured first. Using F in this way will result in a shorter time-to-target since you don't wait for error buildup (to aquire the I term). It's also simpler to tune and more stable since you don't have large P and D terms which will cause oscillation. The P, I, and D  values then will then add minor corrections, operating on a much smaller system error. In this setup, P and I will generally correct for non-linearities in the system such as such as drag, inertia, and friction. D is helpful for providing recovery on sudden loading of the system, or quickly switching to a new setpoint.

#### `setOutputLimits(double minimum,double maximum)`
#### `setOutputLimits(double output)`
Optional, but highly recommended to set. The set the output limits, and ensure the controller behaves when reaching the maximum output capabilities of your physical system. 

#### `setMaxIOutput(double maximum)`
Sets the maximum output generated by the I term inside the controller. This is independent of the `setOutputLimits` values. This can assist in reducing windup over large setpoint changes or stall conditions. 

#### `setDirection(boolean reversed)`
Reverses the output. Use this if the controller attempts to go the wrong way during operation. 

#### `reset()`
Resets the PID controller. This primarily clears the I and D terms clears the previous sensor state, and sets the target setpoint the the current position. 

This is useful if the output system's state may have changed since the last time the PID system output was applied. This is generally the case when the output system is in a manual control mode (such as a joystick), or was disabled and may have been physically moved.

#### `setOutputRampRate(double rate)`
Set the maximum rate of change in a single calculation cycle. This is particularly useful for adding "inertial" to the system, preventing jerks during setpoint changes.

#### `setOutputFilter(double strength)`
The output filter prevents sharp changes in the output, adding inertia and minimizing the effect of high frequency oscillations. This adds significant stability to systems with poor tunings, but at the cost of slower setpoint changes, disturbance rejection, and increased overshoot.
