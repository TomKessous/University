import numpy as np
import matplotlib.pyplot as plt


class HydroCamel:
    def __init__(self, _sonar_range, _sonar_angle, _map_size, _initial_position, _velocity, _duration, _mines_map):
        ''' init method for class Auv.
        Input _sonar_range - An integer with values between 3-9, denote the range of the sonar
        _sonar_angle - An integer with values between 15-90 in degrees, denote half of the field of view angle
        _map_size - a tuple (Height, Width) of the map
        _initial_position - a tuple (Py, Px). The starting point of the AUV.
        _velocity - a tuple of lists. each donates a velocity ([Vy1, Vx1], [Vy2, Vx2]...)
        _duration - a lists of integers. each denote the time to run the simulation
                                         with the matching velocity [t1, t2,...]
        _mines_map - a list of lists holding the location of all the mines.
        Output None.
        '''
        if _sonar_angle == 45:
            _sonar_angle += 10**(-3)
            _sonar_range += 10**(-3)
        self._initial_position = _initial_position
        self._sonar_range = _sonar_range
        self._sonar_angle = _sonar_angle
        self.find_first_abc()
        if isinstance(_velocity[0], int):
            self._velocity = [list(_velocity)]
        else:
            self._velocity = list(_velocity)
        self._duration = _duration
        self.mine_coordinates = []
        self.board = np.matrix(_mines_map)
        self.found_mines = []
        self.turn_angle = 0
        self.get_sonar_fov()
        self.flag = len(_duration)

    def get_mines(self):
        ''' Returns the position of all the mines that the AUV has found.
        Input None.
        Output A list of tuples. Each tuple holds the coordinates (Yi , Xi) of found mines. The list should be sorted.
        '''

        for cordinate in self.get_sonar_fov():
            if self.board[cordinate] == 1:
                if cordinate not in self.found_mines:
                    self.found_mines.append(cordinate)
        return self.sort(self.found_mines)

    def sort_y(self, equal_1):
        less = []
        equal = []
        greater = []

        if len(equal_1) > 1:
            pivot = equal_1[(len(equal_1) // 2)]
            for x in equal_1:
                if x[0] < pivot[0]:
                    less.append(x)
                if x[0] == pivot[0]:
                    equal.append(x)
                if x[0] > pivot[0]:
                    greater.append(x)
            return self.sort_y(less) + equal + self.sort_y(greater)
        else:
            return equal_1

    def sort(self, found_mine):
        less = []
        equal = []
        greater = []

        if len(found_mine) > 1:
            pivot = found_mine[(len(found_mine)//2)]
            for x in found_mine:
                if x[1] < pivot[1]:
                    less.append(x)
                if x[1] == pivot[1]:
                    equal.append(x)
                if x[1] > pivot[1]:
                    greater.append(x)
            return self.sort(less) + self.sort_y(equal) + self.sort(greater)
        else:
            return found_mine

    def get_sonar_fov(self):
        ''' Returns all the current  (Yi , Xi) coordinates of the map which are in range for the sonar
        Input None.
        Output A dictionary. The keys of the dictionary are tuples of the (Yi , Xi) coordinates
        and the value should be Boolean True
        '''
        fov_dict = {}
        y_max = int(max(self.a[0], self.b[0], self.c[0]))
        y_min = int(min(self.a[0], self.b[0], self.c[0]))
        x_max = int(max(self.a[1], self.b[1], self.c[1]))
        x_min = int(min(self.a[1], self.b[1], self.c[1]))
        if self.a[0] == self.c[0]:
            tmp = self.b
            self.b = self.c
            self.c = tmp

        a = self.a
        b = self.b
        c = self.c
        for y in range(y_min, y_max + 1):
            for x in range(x_min, x_max + 1):
                w1 = (a[1]*(c[0]-a[0]) + (y-a[0])*(c[1]-a[1]) - x*(c[0] - a[0]))/((b[0]-a[0])*(c[1] - a[1]) - (b[1] - a[1])*(c[0]-a[0]))
                w2 = (y-a[0] - w1*(b[0]-a[0]))/(c[0]-a[0])

                if w1 >= 0 and w2 >= 0 and (w1 + w2) <= 1:
                    fov_dict[(y, x)] = True
        return fov_dict

    def find_first_abc(self):
        self.a = self._initial_position
        distance_x = self._sonar_range * np.cos(np.deg2rad(self._sonar_angle))
        distance_y = self._sonar_range * np.sin(np.deg2rad(self._sonar_angle))
        self.b = (self.a[0] + distance_y, self.a[1] + distance_x)
        self.c = (self.a[0] - distance_y, self.a[1] + distance_x)

    def display_map(self):
        ''' Display the current map.
        Input None.
        Output None
        '''
        self.board[self._initial_position] += 2
        for cordinate in self.get_sonar_fov():
            self.board[cordinate] += 7
        plt.matshow(self.board)
        plt.show()
        self.board[self._initial_position] -= 2
        for cordinate in self.get_sonar_fov():
            self.board[cordinate] -= 7

    def get_heading(self):
        ''' Returns the Direction of movement of the AUV in Degrees. The heading will be between 0-360.
                    With respect to the x and y axes of the map.
        Input None.
        Output the Direction of movement of the AUV in Degrees.
        '''

        if np.degrees(self.turn_angle) < 0:
            return np.degrees(self.turn_angle) + 360
        else:
            return np.degrees(self.turn_angle)

    def set_course(self, _velocity, _duration):
        ''' Receive new values for the velocity and duration properties. Append the new values to the current ones
        Input- Velocity as tuple of lists.
        Duration as list of integers
        Output None.
        '''
        for element in range(len(_velocity)):
            self._velocity.append(_velocity[element])
            self._duration.append(_duration[element])

    def time_step(self):
        ''' Propagate the simulation by one step (one second) if duration >0
        Input None.
        Output None.
        '''
        if len(self._duration) > 0:
            if self._duration[0] > 0:
                self._duration[0] = self._duration[0] - 1
                if self.flag == len(self._duration):
                    self.turn_sonar(self._velocity[0])
                    self.flag -= 1
                self._initial_position = (self._initial_position[0] + self._velocity[0][0], self._initial_position[1] + self._velocity[0][1])
                self.a = (self.a[0] + self._velocity[0][0], self.a[1] + self._velocity[0][1])
                self.b = (self.b[0] + self._velocity[0][0], self.b[1] + self._velocity[0][1])
                self.c = (self.c[0] + self._velocity[0][0], self.c[1] + self._velocity[0][1])
                if self._duration[0] == 0:
                    self._duration.pop(0)
                    self._velocity.pop(0)

            else:
                self._duration.pop(0)
                self._velocity.pop(0)
            self.get_mines()

    def turn_sonar(self, velocity):
        self.turn_angle = np.arctan2(velocity[0], velocity[1]) - self.turn_angle
        c, s = np.cos(self.turn_angle), np.sin(self.turn_angle)
        xb = c * (self.b[1] - self.a[1]) - s * (self.b[0] - self.a[0]) + self.a[1]
        yb = s * (self.b[1] - self.a[1]) + c * (self.b[0] - self.a[0]) + self.a[0]
        self.b = (yb, xb)
        xc = c * (self.c[1] - self.a[1]) - s * (self.c[0] - self.a[0]) + self.a[1]
        yc = s * (self.c[1] - self.a[1]) + c * (self.c[0] - self.a[0]) + self.a[0]
        self.c = (yc, xc)
        self.turn_angle = np.arctan2(velocity[0], velocity[1])

    def start(self):
        ''' Activate the simulation and run until duration has ended
        Input None.
        Output None.
        '''
        while len(self._velocity) > 0:
            self.time_step()


if __name__ == "__main__":
    pass
