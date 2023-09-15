#! /usr/bin/env python3

import rospy
from geometry_msgs.msg import PoseStamped
from mavros_msgs.msg import State
from mavros_msgs.srv import CommandBool, CommandBoolRequest, SetMode, SetModeRequest

import gol # for set global parameter

current_state = State()

current_local_pose = PoseStamped()


def state_cb(msg):
    global current_state
    current_state = msg

def local_pos_cb(msg):
    global current_local_pose
    current_local_pose = msg


# gol._init()

# def position_local_init():
#     x = gol.set_value("local_x", 0)
#     y = gol.set_value("local_y", 0)



def set_fly_point(x, y, z = None):

    goal_pose = PoseStamped()

    goal_pose.pose.position.x = current_local_pose.pose.position.x + x
    goal_pose.pose.position.y = current_local_pose.pose.position.y + y

    if z != None:
        goal_pose.pose.position.z = current_local_pose.pose.position.z + z

    return goal_pose




if __name__ == "__main__":

    rospy.init_node("offb_node_py")

    state_sub = rospy.Subscriber("mavros/state", State, callback = state_cb)

    local_pos_pub = rospy.Publisher("mavros/setpoint_position/local", PoseStamped, queue_size=10)

    local_pos_sub = rospy.Subscriber("mavros/local_position/pose", PoseStamped, callback = local_pos_cb)

    rospy.wait_for_service("/mavros/cmd/arming")
    arming_client = rospy.ServiceProxy("mavros/cmd/arming", CommandBool)

    rospy.wait_for_service("/mavros/set_mode")
    set_mode_client = rospy.ServiceProxy("mavros/set_mode", SetMode)


    # Setpoint publishing MUST be faster than 2Hz
    rate = rospy.Rate(20)

    # Wait for Flight Controller connection
    while(not rospy.is_shutdown() and not current_state.connected):
        rate.sleep()






#-----------init and loop function--------------------
    # target pose set up

    pose = PoseStamped()

    pose = set_fly_point(3,4,3)

    # pose.pose.position.x = 0
    # pose.pose.position.y = 0
    # pose.pose.position.z = 2

    # Send a few setpoints before starting
    for i in range(100):
        if(rospy.is_shutdown()):
            break

        local_pos_pub.publish(pose)
        rate.sleep()

    # offboard

    offb_set_mode = SetModeRequest()
    offb_set_mode.custom_mode = 'OFFBOARD'

    arm_cmd = CommandBoolRequest()
    arm_cmd.value = True

    last_req = rospy.Time.now()


    while(not rospy.is_shutdown()):
        if(current_state.mode != "OFFBOARD" and (rospy.Time.now() - last_req) > rospy.Duration(5.0)):
            if(set_mode_client.call(offb_set_mode).mode_sent == True):
                rospy.loginfo("OFFBOARD enabled")

            last_req = rospy.Time.now()
        else:
            if(not current_state.armed and (rospy.Time.now() - last_req) > rospy.Duration(5.0)):
                if(arming_client.call(arm_cmd).success == True):
                    rospy.loginfo("Vehicle armed")

                last_req = rospy.Time.now()

        

        local_pos_pub.publish(pose)

        rate.sleep()


