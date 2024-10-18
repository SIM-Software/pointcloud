import cv2
import numpy as np
import open3d as o3d

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2 
from sensor_msgs_py import point_cloud2 as pc2
from std_msgs.msg import Header
from rclpy.clock import Clock

class PointCloudPublisher(Node):

    def __init__(self):
        super().__init__('pointcloud_publisher')

        self.PICTURE_WIDTH = 1920
        self.PICTURE_HEIGHT = 1080
        self.CAMERA_WIDTH = 36
        self.CAMERA_HEIGHT = 24
        self.FOCAL_LENGTH = 50

        self.pointcloud_publisher = self.create_publisher(PointCloud2, '/pointcloud', 1)

    def create_header(self, frame_id):
        """Creates a header object for the message

        Header:
            stamp: Time message which has the seconds and nanoseconds since the epoch
            frame_id: TF which the header is relevant to

        Args:
            frame_id (String): This is the transform which the message applies to

        Returns:
            Header: Header containing the timestamp and given frame_id
        """

        # Creates a timer for timestamps
        timer = Clock().now()
        header = Header()

        header.stamp = timer.to_msg()
        header.frame_id = frame_id

        return header
    
    def timer_callback(self):

        masks = []

        for i in range(7):
            mask = cv2.imread(f'photos/MaskImage{i}.png')
            mask = cv2.inRange(mask, np.array([255, 255, 255]), np.array([255, 255, 255]))
            masks.append(mask)

        coordinates = np.column_stack(np.where(masks[0] == 255))

        counter = 0

        points = []

        for coordinate in coordinates:
            y = coordinate[0] 
            x = coordinate[1] 

            binary_code = ''

            for i in range(1,7):
                value = masks[i][y, x]

                if value == 255:
                    binary_code += '1'
                else:
                    binary_code += '0'

            y = int((1080 / 2) - coordinate[0])
            x = int(coordinate[1] - (1920 / 2))

            x *= (self.CAMERA_WIDTH / self.PICTURE_WIDTH)
            y *= (self.CAMERA_HEIGHT / self.PICTURE_HEIGHT)

            x /= 1000
            y /= 1000
            z = -self.FOCAL_LENGTH / 1000

            binary_code = int(binary_code, 2)

            theta = 20 + ((52/63) * (binary_code - 32))
            a = -np.cos(np.radians(theta))
            b = 0
            c = np.sin(np.radians(theta))
            d = 20 * np.cos(np.radians(theta))

            t = -d / ((a * x) + (b * y) + (c * z))

            x *= t
            y *= t
            z *= t

            points.append((x, y, z))

            counter += 1

            progress = counter / len(coordinates)

            progress *= 100

            print(f'\r{progress:.2f}%')


        header = self.create_header('camera_link')

        msg = pc2.create_cloud_xyz32(header, points)

        while True:
            print('publishing')
            self.pointcloud_publisher.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    pointcloud_publisher = PointCloudPublisher()

    try:
        pointcloud_publisher.timer_callback()
    finally:
        pointcloud_publisher.destroy_node()


if __name__ == '__main__':
    main()