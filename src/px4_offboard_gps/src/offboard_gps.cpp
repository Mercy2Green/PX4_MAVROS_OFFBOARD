#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

#include <geographic_msgs/GeoPoseStamped.h>


mavros_msgs::State current_state;

void state_cb(const mavros_msgs::State::ConstPtr& msg){
      current_state = *msg;
}


int main(int argc, char* argv[])
{
      ros::init(argc, argv, "offboard_control_gps");
      ros::NodeHandle nh;

      ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);

      ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);

      ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");

      ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

      ros::Publisher global_pos_pub = nh.advertise<geographic_msgs::GeoPoseStamped>("mavros/setpoint_position/global", 10);


      ros::Rate rate(20.0);

      while(ros::ok() && !current_state.connected){
            ros::spinOnce();
            rate.sleep();
      }

      geographic_msgs::GeoPoseStamped global_pose;
      global_pose.pose.position.altitude = 537; // using rqt to see where the init position is.
      global_pose.pose.position.latitude = 48;
      global_pose.pose.position.longitude = 9;

      for(int i=100; ros::ok() && i>0; --i){

            global_pos_pub.publish(global_pose);
            ros::spinOnce();
            rate.sleep();
      }

      mavros_msgs::SetMode offb_set_mode;
      offb_set_mode.request.custom_mode = "OFFBOARD";

      mavros_msgs::CommandBool arm_cmd;
      arm_cmd.request.value = true;

      ros::Time last_request = ros::Time::now();

      while(ros::ok()){
            if(current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0))){
                  if(set_mode_client.call(offb_set_mode) && arm_cmd.response.success){
                        ROS_INFO("Vehicle armed");
                  }
                  last_request = ros::Time::now();
            } else{
                  if( !current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0))){
                        if(arming_client.call(arm_cmd) && arm_cmd.response.success){
                              ROS_INFO("Vehicle armed");
                        }
                        last_request = ros::Time::now();
                  }
            }

            global_pos_pub.publish(global_pose);

            ros::spinOnce();
            rate.sleep();
      }



      return 0;      
}



// def _find_gps_topic(args, op_name):
//     # XXX: since 0.13 global position always exists. need redo that.
//     global_fix = mavros.get_topic('global_position', 'global')
//     gps_fix = mavros.get_topic('global_position', 'raw', 'fix')

//     topics = rospy.get_published_topics()
//     # need find more elegant way
//     if len([topic for topic, type_ in topics if topic == global_fix]):
//         return global_fix
//     elif len([topic for topic, type_ in topics if topic == gps_fix]):
//         print_if(args.verbose, "Use GPS_RAW_INT data!")
//         return gps_fix
//     elif args.any_gps:
//         t = [topic for topic, type_ in topics if type_ == 'sensor_msgs/NavSatFix']
//         if len(t) > 0:
//             print("Use", t[0], "NavSatFix topic for", op_name)
//             return t[0]

//     return None