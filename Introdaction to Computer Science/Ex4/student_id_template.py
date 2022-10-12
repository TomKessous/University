import auv_interface
import numpy as np
import matplotlib.pyplot as plt


class HydroCamel(auv_interface.Auv):
    pass
    # write your code here


if __name__ == "__main__":

    # example 1
    map_size = (20, 15)
    mines = np.zeros(map_size).tolist()
    mines[16][6] = 1
    mines[12][4] = 1
    mines[14][10] = 1
    mines[17][11] = 1
    velocity = list()
    velocity.append([0, 1])
    sonar_range = 6
    sonar_angle = 60
    initial_position = (14, 1)
    duration = [7]

    game1 = HydroCamel(sonar_range, sonar_angle, map_size, initial_position, velocity, duration, mines)
    for i in range(0,7):
        game1.time_step()
    game1.display_map()
    print(game1.get_mines())
    print(game1.get_sonar_fov())

    # example 2
    sonar_range = 5
    sonar_angle = 30
    map_size = (25, 20)
    initial_position = (10, 10)
    velocity = list()
    velocity.append([2, 2])
    velocity.append([-2, -2])
    velocity.append([0, 2])
    velocity.append([2, 0])
    duration = [2, 2, 2, 2]
    mines = np.random.choice([1, 0], map_size, p=[0.05, 0.95]).tolist()

    game2 = HydroCamel(sonar_range, sonar_angle, map_size, initial_position, velocity, duration, mines)
    game2.start()
    print(game2.get_mines())
    print(game2.get_sonar_fov())
