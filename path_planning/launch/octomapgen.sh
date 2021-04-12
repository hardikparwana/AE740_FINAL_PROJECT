#!/bin/bash
rosservice call --wait /world/get_octomap '{bounding_box_origin: {x: 5, y: 0, z: 1.75}, bounding_box_lengths: {x: 20.8, y: 20.8, z: 4}, leaf_size: 0.1, publish_octomap: true}' &
exec "$@"