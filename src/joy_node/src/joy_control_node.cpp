#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float32.h>


class JoyControl
{
public:
  JoyControl();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_;
  double steer;
  double accel;
  double l_scale_, a_scale_;
  ros::Subscriber joy_sub_;
  ros::Publisher vel_pub_;

};


JoyControl::JoyControl():
  linear_(1),
  angular_(2)
{

  nh_.param("axis_linear", linear_, linear_);
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);


  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel",1);


  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &JoyControl::joyCallback, this);

}

void JoyControl::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;
  twist.angular.z = 128*joy->axes[0];
  twist.linear.x = 255*abs(joy->axes[1]);
  vel_pub_.publish(twist);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "joy_control_node");
  JoyControl teleop_turtle;

  ros::spin();
}

