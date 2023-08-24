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