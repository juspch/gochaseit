#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void move_forward()
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Moving Forward...");

    // Request centered joint angles [1.57, 1.57]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = 0.5;
    srv.request.angular_z = 0.0;

    // Call the command_robot service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");

}

void turn_right()
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Turning Right...");

    // Request centered joint angles [1.57, 1.57]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = 0.0;
    srv.request.angular_z = -0.5;

    // Call the command_robot service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");

}

void turn_left()
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Turning Left...");

    // Request centered joint angles [1.57, 1.57]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = 0.0;
    srv.request.angular_z = 0.5;

    // Call the command_robot service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");

}

void stop()
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Stopping...");

    // Request centered joint angles [1.57, 1.57]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = 0.0;
    srv.request.angular_z = 0.0;

    // Call the command_robot service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    int left = 0, middle = 0, right = 0;

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    for (int i = 0; i < img.height * img.step; i++) {
        if (img.data[i] == white_pixel) {
            if (i % img.step < img.step / 3 && i % img.step >= 0 ) {
              left++;
            }

            else if (i % img.step < 2 * img.step / 3 && i % img.step >= img.step / 3) {
              middle++;
            }
            else if (i % img.step >= 2 * img.step / 3 ) {
              right++;
            }
        }
    }

    if (middle > left && middle > right) {
      move_forward();
    }

    else if (left > middle && left > right) {
      turn_left();
    }

    else if (right > left && right > middle) {
      turn_right();
    }

    else {
      stop();
    }

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
