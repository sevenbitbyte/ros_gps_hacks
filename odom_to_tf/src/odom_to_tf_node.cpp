#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>

tf::TransformBroadcaster* odom_broadcasterPtr;
ros::Publisher* pathPubPtr;

nav_msgs::Path path;


void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{

  //first, we'll publish the transform over tf
  geometry_msgs::TransformStamped odom_trans;
  odom_trans.header.stamp = msg->header.stamp;
  odom_trans.header.frame_id = "odom";
  odom_trans.child_frame_id = "base_link";

  odom_trans.transform.translation.x = msg->pose.pose.position.x;
  odom_trans.transform.translation.y = msg->pose.pose.position.y;
  odom_trans.transform.translation.z = msg->pose.pose.position.z;
  odom_trans.transform.rotation.x = msg->pose.pose.orientation.x;
  odom_trans.transform.rotation.y = msg->pose.pose.orientation.y;
  odom_trans.transform.rotation.z = msg->pose.pose.orientation.z;
  odom_trans.transform.rotation.w = msg->pose.pose.orientation.w;

  geometry_msgs::PoseStamped poseStamped;

  poseStamped.header = msg->header;
  poseStamped.pose = msg->pose.pose;

  path.poses.push_back(poseStamped);

  //send the transform
  odom_broadcasterPtr->sendTransform(odom_trans);
  ROS_INFO("odom");
}

void timerCallback(const ros::TimerEvent& event){
  if(pathPubPtr->getNumSubscribers() > 0){
    pathPubPtr->publish(path);
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  path.header.frame_id = "odom";

  ros::NodeHandle n;
  ros::Subscriber odom_pub = n.subscribe("odom", 500, odomCallback);
  ros::Publisher pathPub = n.advertise<nav_msgs::Path>("path", 1);
  tf::TransformBroadcaster odom_broadcaster;

  odom_broadcasterPtr = &odom_broadcaster;
  pathPubPtr = &pathPub;

  ros::Timer timer = n.createTimer(ros::Duration(1), timerCallback);
  ros::spin();
}
